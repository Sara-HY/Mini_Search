#pragma once
#pragma execution_character_set("UTF-8")

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dictionary.h"
#include "Chinese_word_segment.h"
#include "search.h"

DOCSINFO* search_new_docinfo(const char* url, const char* title)
{
	char* purl = NULL;
	char* ptitle = NULL;
	DOCSINFO* pdocinfo = NULL;
	if (url == NULL || title == NULL)
		return NULL;

	purl = (char*)calloc(strlen(url) + 1, sizeof(char));
	ptitle = (char*)calloc(strlen(title) + 1, sizeof(char));
	pdocinfo = (DOCSINFO*)calloc(1, sizeof(DOCSINFO));

	if (purl == NULL || ptitle == NULL || pdocinfo == NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		free(purl);
		free(ptitle);
		free(pdocinfo);
		return NULL;
	}

	strcpy(purl, url);
	strcpy(ptitle, title);
	pdocinfo->url = purl;
	pdocinfo->title = ptitle;
	return pdocinfo;
}

DOCSET* search_new_docset(const char* pmapfile)
{
	unsigned int max_doc_id = 0, doc_id = 0;
	char purl[1024] = { 0 };
	FILE* fpmap = NULL;
	DOCSET* pdocset = NULL;
	DOCSINFO* pdocinfo;
	unsigned int cnt = 0;

	pdocset = (DOCSET*)calloc(1, sizeof(DOCSET));
	if (pdocset == NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		return NULL;
	}

	fpmap = fopen(pmapfile, "rt");
	if (fpmap == NULL)
	{
		fprintf(stderr, "Can not open file %s\n", pmapfile);
		free(pdocset);
		return NULL;
	}

	//get the maximum of docid from map file
	while (2 == fscanf(fpmap, "%u %s", &doc_id, purl))
	{
		if (max_doc_id < doc_id)
			max_doc_id = doc_id;
		++cnt;
	}
	fclose(fpmap);
	pdocset->max_docid = max_doc_id;
	pdocinfo = (DOCSINFO*)calloc(max_doc_id + 1, sizeof(DOCSINFO));
	if (pdocinfo == NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		free(pdocset);
		return NULL;
	}

	pdocset->pdocinfo = pdocinfo;
	pdocset->docsnum = cnt;
	return pdocset;
}

DOCFEEDBACK* search_new_docfeedback(unsigned int doc_id, double score)
{
	DOCFEEDBACK* ptr = (DOCFEEDBACK*)calloc(1, sizeof(DOCFEEDBACK));
	if (ptr == NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		return NULL;
	}
	ptr->docid = doc_id;
	ptr->score = score;
	ptr->pnext = NULL;
	return ptr;
}

//insert the document into inverted index according to its keywords
void search_insert_doc(INDEXTABLE* ptable, uint doc_id, const char* url, const char* title, int wordnum, char** ppwords)
{
	INDEXITEM *pindexitem;
	DOCNODE *pdocnode, *predocnode = NULL, *curdocnode;
	WORDNODE *curwordnode;
	unsigned int i, pos;
	char *pword;

	for (i = 0; i < wordnum; ++i)
	{
		pword = *(ppwords + i);//get a word here
		//find the word node
		pos = inverted_index_hash(pword, strlen(pword)) % (ptable->prime_num);
		pindexitem = ptable->pindex + pos;
		curwordnode = pindexitem->phead;
		while (curwordnode && strcmp(curwordnode->pword, pword) < 0)
			curwordnode = curwordnode->pnext;

		//find the position to insert the document node
		if (curwordnode && !strcmp(curwordnode->pword, pword))//the word node is found
		{
			if (curwordnode->phead == NULL)//there's no document assigned to the word node
			{
				//make a new node for document
				pdocnode = inverted_index_new_docnode(doc_id);
				if (pdocnode == NULL)//failed to construct a document node
					continue;
				pdocnode->tf += 1.0 / wordnum;
				curwordnode->phead = pdocnode;
				curwordnode->idf += 1.0;
				continue;
			}
			curdocnode = curwordnode->phead;
			while (curdocnode && curdocnode->docid < doc_id)
			{
				predocnode = curdocnode;
				curdocnode = curdocnode->pnext;
			}
			if (!curdocnode || curdocnode->docid != doc_id)//the document does not exist
			{
				//make a new node for document
				pdocnode = inverted_index_new_docnode(doc_id);
				if (pdocnode == NULL)//failed to construct a document node
					continue;
				pdocnode->tf += 1.0 / wordnum;
				pdocnode->pnext = curdocnode;
				if (predocnode)
					predocnode->pnext = pdocnode;
				else
					curwordnode->phead = pdocnode;
				curwordnode->idf += 1;
			}
			else//the document already exists
				curdocnode->tf += 1.0 / wordnum;
		}
	}
}

//insert the documents in given path recursively
void search_insert_docs(INDEXTABLE* ptable, const DICTIONARY* pdict, DOCSET* pdocset, const char* pfilepath, const char* pmapfile)
{
	FILE *fpmap = NULL, *fpfile = NULL;
	const unsigned int URL_MAX_LENTH = 1024;
	const unsigned int TITLE_MAX_LENTH = 350;//maximum length of title
	unsigned int doc_id;
	long max_file_sz = 1024, cur_file_sz = 0;
	const unsigned int WORD_MAX_LENTH = 75;//max number of bytes for a word
	unsigned int max_word_num = 300;//maximum number of possible words in html
	unsigned int cur_chars_num = 0;
	unsigned int word_num;//number of words segmented
	unsigned int i, j;
	char* purl = NULL;
	char* phtmlbuf = NULL, *ptr;
	char htmlfile[500] = { 0 };//file name for html(0.html 1.html etc.)
	char* title = NULL;//title of html
	char** ppwords = NULL, **pptr;//ppwords is used to store the words segmented
	DOCSINFO* pdocinfo = NULL;
	WORDNODE* pwordnode = NULL;

	if (ptable == NULL || pdict == NULL || pdocset == NULL)
		return;
	fpmap = fopen(pmapfile, "rt");
	if (fpmap == NULL)
	{
		fprintf(stderr, "Can not open file %s\n", pmapfile);
		return;
	}

	//lots of memory allocation below
	purl = (char*)calloc(URL_MAX_LENTH, sizeof(char));
	phtmlbuf = (char*)calloc(max_file_sz + 1, sizeof(char));
	title = (char*)calloc(TITLE_MAX_LENTH, sizeof(char));
	ppwords = (char**)calloc(max_word_num, sizeof(char*));
	if (purl == NULL || phtmlbuf == NULL || title == NULL || ppwords == NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		fclose(fpmap);
		free(purl);
		free(phtmlbuf);
		free(title);
		free(ppwords);
		return;
	}

	for (i = 0; i < max_word_num; ++i)
	{
		ppwords[i] = (char*)calloc(WORD_MAX_LENTH, sizeof(char));
		if (ppwords[i] == NULL)
		{
			fprintf(stderr, "Error allocating memory\n");
			for (j = 0; j < i; ++i)
				free(ppwords[j]);
			fclose(fpmap);
			free(purl);
			free(phtmlbuf);
			free(title);
			free(ppwords);
			return;
		}
	}

	//insert all the documents into inverted index
	while (2 == fscanf(fpmap, "%u %s", &doc_id, purl))
	{
		sprintf(htmlfile, "%s/%d.html", pfilepath, doc_id);//filename for html file
		fpfile = fopen(htmlfile, "rt");
		if (fpfile == NULL)
		{
			fprintf(stderr, "Failed to open file %s\n", htmlfile);
			continue;
		}
		fseek(fpfile, 0, SEEK_END);
		cur_file_sz = ftell(fpfile);//size of current file
		fseek(fpfile, 0, SEEK_SET);//move the pointer to the begining
		if (cur_file_sz > max_file_sz)//increase buffer for storing content of html
		{
			ptr = (char*)realloc(phtmlbuf, (cur_file_sz + 1)*sizeof(char));
			if (ptr == NULL)
			{
				fprintf(stderr, "Error allocating memory\n");
				fclose(fpfile);
				continue;
			}
			max_file_sz = cur_file_sz;
			phtmlbuf = ptr;
		}
		//read data from a html file
		htmlparser_read_file(fpfile, phtmlbuf, max_file_sz + 1);
		fclose(fpfile);
		//get title of the html file
		htmlparser_gettitle(phtmlbuf, title, TITLE_MAX_LENTH);
		//get the basic information of a document
		pdocinfo = search_new_docinfo(purl, title);
		if (pdocinfo == NULL)
			continue;
		memcpy(pdocset->pdocinfo + doc_id, pdocinfo, sizeof(DOCSINFO));
		//Data cleaning for original html's content
		htmlparser_data_clean(phtmlbuf);
		//Chinese words segmentation
		cur_chars_num = strlen(phtmlbuf);
		//increase the memory
		if (cur_chars_num > max_word_num)
		{
			pptr = (char**)realloc(ppwords, cur_chars_num*sizeof(char*));
			if (pptr == NULL)
			{
				fprintf(stderr, "Error reallocating memory\n");
				continue; //skip the document
			}
			ppwords = pptr;
			for (i = 0; i < cur_chars_num - max_word_num; ++i)
			{
				ppwords[max_word_num + i] = (char*)calloc(WORD_MAX_LENTH, sizeof(char));
				if (ppwords[max_word_num + i] == NULL)
				{
					fprintf(stderr, "Error allocating memory\n");
					break;
				}
			}
			max_word_num += i;//change the number of words ppwords can store
			if (max_word_num < cur_chars_num)//can't handle current document
				continue;
		}
		//reverse maximum matching method
		word_num = forward_match(pdict, (const unsigned char*)phtmlbuf, (unsigned char**)ppwords, max_word_num, 0);
		//insert the document into inverted index
		search_insert_doc(ptable, doc_id, purl, title, word_num, ppwords);
	}
	fclose(fpmap);
	free(purl);
	free(phtmlbuf);
	free(title);
	for (i = 0; i < max_word_num; ++i)
		free(ppwords[i]);
	free(ppwords);
	//compute inverse document frequency
	for (i = 0; i < ptable->prime_num; ++i)
	{
		pwordnode = ptable->pindex[i].phead;
		while (pwordnode)
		{
			pwordnode->idf = log(pdocset->docsnum / pwordnode->idf);
			pwordnode = pwordnode->pnext;
		}
	}
}

COREDATA* search_initialize(const char* pdictfilename, const char* pdocspath, const char* pmapfilename)
{
	COREDATA* pcoredata = NULL;
	DICTIONARY* pdict = NULL;
	INDEXTABLE* ptable = NULL;
	DOCSET* pdocset = NULL;

	pcoredata = (COREDATA*)calloc(1, sizeof(COREDATA));
	if (pcoredata == NULL)
		return NULL;
	pdict = dict_initialize();
	dict_from_file(pdict, pdictfilename);
	ptable = inverted_index_load("inverted_index.txt"); 
	pdocset = search_new_docset(pmapfilename);

	if (NULL == pdict || NULL == pdocset || NULL == ptable)
	{
		free(pcoredata);
		return NULL;
	}
	inverted_index_insert_dictionary(ptable, pdict);
	search_insert_docs(ptable, pdict, pdocset, pdocspath, pmapfilename);
	pcoredata->pdict = pdict;
	pcoredata->pdocset = pdocset;
	pcoredata->ptable = ptable;

	return pcoredata;
}

DOCFEEDBACK* search_handle_query(const COREDATA* pcoredata, const char* pquery)
{
	DOCFEEDBACK *pdocfeedback = NULL, *phead = NULL, *ptail = NULL;
	INDEXITEM* pindexitem = NULL;
	WORDNODE* pwordnode = NULL;
	DOCNODE* pdocnode = NULL;
	unsigned int pos;
	double idf;

	if (pcoredata == NULL || pquery == NULL)
		return NULL;
	//find the word node
	pos = inverted_index_hash(pquery, strlen(pquery)) % (pcoredata->ptable->prime_num);
	pindexitem = pcoredata->ptable->pindex + pos;
	pwordnode = pindexitem->phead;
	//find the documents that contain specific query
	while (pwordnode && strcmp(pquery, pwordnode->pword) < 0)
		pwordnode = pwordnode->pnext;
	if (pwordnode == NULL || strcmp(pquery, pwordnode->pword))
		return NULL;
	idf = pwordnode->idf;//idf for this word
	pdocnode = pwordnode->phead;
	while (pdocnode)
	{
		pdocfeedback = search_new_docfeedback(pdocnode->docid, idf*(pdocnode->tf));
		if (phead == NULL)
		{
			phead = pdocfeedback;
			ptail = pdocfeedback;
		}
		else
		{
			ptail->pnext = pdocfeedback;
			ptail = ptail->pnext;
		}
		pdocnode = pdocnode->pnext;
	}
	return phead;
}

//num means the number of word, maxnum means the number of doc
DOCFEEDBACK* search_queries(const COREDATA* pcoredata, char** ppqueries, int num, int maxnum)
{
	DOCFEEDBACK *phead = NULL, *ptail = NULL, *other = NULL, *ptr = NULL;
	DOCFEEDBACK *pre = NULL, *cur = NULL;
	DOCFEEDBACK *pbegain = NULL, *pend = NULL;
	int i = 0, cnt = 0;
	double score = .0;

	if (pcoredata == NULL || ppqueries == NULL || num <= 0)
		return NULL;
	//get the documents satisfying the first query
	phead = search_handle_query(pcoredata, *ppqueries);
	ptr = phead;
	while (ptr)
	{
		ptr = ptr->pnext;
	}
	//merge the documents satisfying subsequent queries and sort them according to the docids
	for (i = 1; i < num; ++i)
	{
		other = search_handle_query(pcoredata, *(ppqueries + i));
		ptr = other;
		while (ptr)
		{
			ptr = ptr->pnext;
		}
		if (phead == NULL)
		{
			phead = other;
			continue;
		}
		while (other)
		{
			ptr = other;
			pre = NULL;
			cur = phead;
			while (cur && cur->docid < ptr->docid)
			{
				pre = cur;
				cur = cur->pnext;
			}
			other = other->pnext;
			ptr->pnext = cur;
			if (pre == NULL)//insert the node at the begining
				phead = ptr;
			else
				pre->pnext = ptr;
		}
	}
	//accumulate the scores of for each docid and free the redundancy nodes
	pend = phead;
	while (pend)
	{
		pbegain = pend;
		score = .0;
		while (pend && pbegain->docid == pend->docid)
		{
			score += pend->score;
			pend = pend->pnext;
		}
		pbegain->score = score;//accumulate the scores into the first node having same docids
		//free the other nodes with the same docids
		ptr = pbegain->pnext;
		while (ptr && pbegain->docid == ptr->docid)
		{
			pbegain->pnext = ptr->pnext;
			free(ptr);
			ptr = pbegain->pnext;
		}
	}
	//sort the documents and keep the most relevant ones
	sort_feedbacks(phead, maxnum);
	//free the others nodes
	ptail = phead;
	while (cnt < maxnum - 1 && ptail)
	{
		ptail = ptail->pnext;
		++cnt;
	}
	if (ptail)//the number of documents retrieved is not less than maxnum
	{
		ptr = ptail->pnext;
		while (ptr)

		{
			ptail->pnext = ptr->pnext;
			free(ptr);
			ptr = ptail->pnext;
		}
	}
	return phead;
}

//sort the documents according to their scores in descending order
void sort_feedbacks(DOCFEEDBACK* pdocfeedback, int maxnum)
{
	int num = 0;
	DOCFEEDBACK *ptr = pdocfeedback, *ptrtmp;
	DOCFEEDBACK* ptrmax = NULL;
	double scoretmp;
	unsigned int docidtmp;
	if (pdocfeedback == NULL || maxnum <= 0)
		return;
	while (ptr && num < maxnum)
	{
		ptrmax = ptr;
		ptrtmp = ptr->pnext;
		while (ptrtmp)
		{
			if (ptrtmp->score > ptrmax->score)
				ptrmax = ptrtmp;
			ptrtmp = ptrtmp->pnext;
		}
		//swap the data of two nodes
		docidtmp = ptr->docid;
		ptr->docid = ptrmax->docid;
		ptrmax->docid = docidtmp;

		scoretmp = ptrmax->score;
		ptrmax->score = ptr->score;
		ptr->score = scoretmp;

		++num;
		ptr = ptr->pnext;
	}
}
