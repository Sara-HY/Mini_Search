#pragma once
#pragma execution_character_set("UTF-8")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "inverted_index.h"
#include "Chinese_word_segment.h"
#include "htmlparser.h"


//djb2 is one of the best string hash function
uint inverted_index_hash(const void* ptr, int lenth)
{
	uint hash = 5381;
	int i;
	uchar* p = (uchar*)ptr;
	for (i=0; i < lenth; ++i)
		hash += ((hash << 5) +hash+ p[i]);
	return hash;
}

//find a prime not less than bound for words' hashtable
uint find_prime(uint bound)
{
	uint val;
	uint sr, i;
	if(bound <= 2)
		return 2;
	for(val=bound; ;++val)
	{
		sr = sqrt(val*1.0);
		for(i=2; i<=sr; ++i)
			if(val%i == 0)
				continue;
		return val;
	}
}

//make a new word node with a given word
WORDNODE* inverted_index_new_wordnode(const char* src)
{
	char* pword=NULL;
	WORDNODE* pwordnode=NULL;
	if(src==NULL || !*src)
		return NULL;
	pword = (char*)calloc(strlen(src)+1, sizeof(char));
	pwordnode = (WORDNODE*)calloc(1, sizeof(WORDNODE));
	if(pword==NULL || pwordnode==NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		free(pword);
		free(pwordnode);
		return NULL;
	}

	strcpy(pword, src);
	pwordnode->pword = pword;
	pwordnode->phead = NULL;
	pwordnode->pnext = NULL;
	pwordnode->idf = 0;
	return pwordnode;
}

//make a new document node with given url
DOCNODE* inverted_index_new_docnode(int doc_id)
{
	DOCNODE* pdocnode = NULL;
	pdocnode = (DOCNODE*)calloc(1,sizeof(DOCNODE));
	if(pdocnode==NULL)
	{
		fprintf(stderr, " Error allocating memory\n");
		return NULL;
	}
	pdocnode->docid = doc_id;
	pdocnode->pnext = NULL;
	pdocnode->tf = 0;
	return pdocnode;
}

INDEXTABLE* inverted_index_initialize(const DICTIONARY* pdict)
{
	INDEXTABLE* ptable;
	INDEXITEM *ptr;
	uint word_num=0;
	if(pdict==NULL || !pdict->dic_capacity)
		return NULL;
	ptable = (INDEXTABLE*)calloc(1, sizeof(INDEXTABLE));
	if(ptable==NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		return NULL;
	}
    
	word_num = pdict->dic_capacity;//number of words in dictionary
	ptable->prime_num = find_prime(word_num);
	ptr = (INDEXITEM*)calloc(ptable->prime_num, sizeof(INDEXITEM));
	if(ptr==NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		free(ptable);
		return NULL;
	}
    
	ptable->pindex = ptr;
	return ptable;
}

//insert a word into the inverted index
void inverted_index_insert_word(INDEXTABLE* ptable, const char* pword)
{
	uint hashcode=0;
	const char* ptr = pword;
	WORDNODE* pwordnode=NULL;
	WORDNODE *pwordpre=NULL, *pwordcur=NULL;
	uint pos=0;
	if(ptable==NULL || pword==NULL || !*pword)
		return;
	while(*ptr)
        ++ptr;
    //compute the hash code for this word
	hashcode = inverted_index_hash(pword, ptr-pword);
    //find the place to insert the word node
	pos = hashcode%(ptable->prime_num);
    //no word has been inserted in this position
	if(ptable->pindex[pos].phead == NULL)
	{
		//make a new word node
		pwordnode = inverted_index_new_wordnode(pword);
		if(pwordnode==NULL)
			return;
		ptable->pindex[pos].phead = pwordnode;
		return;
	}
	//find the place to insert the word
	pwordcur = ptable->pindex[pos].phead;
	if(!strcmp(pwordcur->pword, pword))//the word is identical with the first node
		return;
	while(pwordcur && strcmp(pwordcur->pword,pword)<0)
	{
		pwordpre = pwordcur;
		pwordcur = pwordcur->pnext;
	}
	//the word doesn't exists
	if(pwordcur==NULL || strcmp(pwordcur->pword,pword))
	{
		//make a new word and insert it
		pwordnode = inverted_index_new_wordnode(pword);
		if(pwordnode==NULL)
			return;
		pwordnode->pnext = pwordcur;
		pwordpre->pnext = pwordnode;
	}
}

//insert multiple words stored in a trie-tree
void inverted_index_insert_words(INDEXTABLE* ptable, const TRIETREE root, char* buf, int depth)
{
	uchar i;
	if(root==NULL)
		return;
	buf[depth] = root->keyword;
	if(root->stat == STAT_WORD)
	{
		buf[depth+1] = 0;
		inverted_index_insert_word(ptable, buf);
	}
	for(i=0; i<root->numchild; ++i)
		inverted_index_insert_words(ptable, root->childptr+i, buf, depth+1);
}

//insert the whole words in dictionary into inverted index
void inverted_index_insert_dictionary(INDEXTABLE* ptable, const DICTIONARY* pdict)
{
	uchar i;
	const int MAX_DEPTH=100;
	char* buf;
	if(ptable==NULL || pdict==NULL)
		return;
	buf = (char*)malloc(sizeof(char)*MAX_DEPTH);
	if(buf==NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		return;
	}
    
	memset(buf, 0, sizeof(char)*MAX_DEPTH);
	for(i=0; i<0xff; ++i)
		if(pdict->ptrees[i])
			inverted_index_insert_words(ptable, pdict->ptrees[i], buf, 0);
	free(buf);
}

void inverted_index_save(const INDEXTABLE* ptable, const char* outfilename)
{
	WORDNODE* pwordnode=NULL;
	DOCNODE* pdocnode=NULL;
	unsigned int i=0, cnt=0;
	FILE* fp=NULL;
	fp = fopen(outfilename,"wt");
	if(fp==NULL)
	{
		fprintf(stderr, "Can't write to file %s", outfilename);
		return;
	}
    
	fprintf(fp, "%d\n", ptable->prime_num);
	for(i=0; i<ptable->prime_num; ++i)
	{
		pwordnode = ptable->pindex[i].phead;
		while(pwordnode)
		{
			fprintf(fp, "%u %s %f", i, pwordnode->pword, pwordnode->idf);
			//write the number of documents containing this word into file
			cnt=0;
			pdocnode = pwordnode->phead;
			while(pdocnode)
			{
				++cnt;
				pdocnode = pdocnode->pnext;
			}
			fprintf(fp," %u", cnt);
			//write the id of documents
			pdocnode = pwordnode->phead;
			while(pdocnode)
			{
				fprintf(fp, " %u %f", pdocnode->docid, pdocnode->tf);
				pdocnode = pdocnode->pnext;
			}
			fprintf(fp, "\n");
			pwordnode = pwordnode->pnext;
		}
	}
	fclose(fp);
}

//FUNCTION:load inverted index from file
INDEXTABLE* inverted_index_load(const char* infilename)
{
	INDEXTABLE* ptable=NULL;
    INDEXITEM* pitem=NULL;
	WORDNODE* pwordnode=NULL, *pwordlast=NULL;
	DOCNODE* pdocnode=NULL, *pdoclast=NULL;
	unsigned int primenum=0, cnt=0, i=0, doc_id;
	unsigned int hashcode=0;
	char pword[150]={0};
	double idf, tf;
	FILE* fp=NULL;
    
	fp = fopen(infilename, "rt");
	if(fp==NULL)
	{
		fprintf(stderr, "Cant't read file %s\n", infilename);
		return NULL;
	}
    
	fscanf(fp, "%u", &primenum);
	ptable = (INDEXTABLE*)calloc(1, sizeof(INDEXTABLE));
	pitem = (INDEXITEM*)calloc(primenum, sizeof(INDEXITEM));
	if(ptable==NULL || pitem==NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		fclose(fp);
		free(ptable);
		free(pitem);
		return NULL;
	}
    
	while(4 == fscanf(fp, "%u %s %lf %u", &hashcode, pword, &idf, &cnt))
	{
		pwordnode = inverted_index_new_wordnode(pword);
		pwordnode->idf = idf;
		if(pitem[hashcode].phead == NULL)
		{
			pitem[hashcode].hashcode = hashcode;
			pitem[hashcode].phead = pwordnode;
			pwordlast = pwordnode;
		}
		else
		{
			pwordlast->pnext = pwordnode;
			pwordlast = pwordnode;
		}
		for(i=0; i<cnt; ++i)
		{
			fscanf(fp, "%u %lf", &doc_id, &tf);
			pdocnode = inverted_index_new_docnode(doc_id);
			pdocnode->tf = tf;
			if(pwordnode->phead == NULL)
			{
				pwordnode->phead = pdocnode;
				pdoclast = pdocnode;
			}
			else
			{
				pdoclast->pnext = pdocnode;
				pdoclast = pdocnode;
			}
		}
	}
	fclose(fp);
	ptable->prime_num = primenum;
	ptable->pindex = pitem;
	return ptable;
}