#pragma once
#pragma execution_character_set("UTF-8")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "trietree.h"

//initialize a node in trie-tree
void trienode_initialize(TRIENODE* pnode, uchar key)
{
	if(pnode == NULL)
		return;
	pnode->keyword = key;
	pnode->numchild = 0;
	pnode->stat = STAT_NONWORD;
	pnode->childptr = NULL;
	pnode->capacity = 0;
}

//insert a word into the Trie tree
int trietree_insert(TRIETREE* ptree, const uchar* chars)
{
	unsigned char i;
	int pos;
	TRIENODE* pnode = NULL;
	if(ptree==NULL || chars==NULL)
		return 0;
	//a new word begining
    if(ptree[*chars] == NULL)
	{
		ptree[*chars] = (TRIENODE*)malloc(sizeof(TRIENODE));
		trienode_initialize(ptree[*chars], *chars);
	}
	pnode = ptree[*chars];
	while(*(++chars))
	{
		pos = 0;
		while(pos<(pnode->numchild) && (pnode->childptr[pos].keyword)<*chars)
			++pos;
		//insert a node
		if((pos>=pnode->numchild) || ((pnode->childptr[pos].keyword)>*chars))
		{
			//lack of memory
			if(pnode->numchild >= pnode->capacity)
			{
				TRIENODE* ptr = (TRIENODE*)realloc(pnode->childptr, sizeof(TRIENODE)*(pnode->capacity+INCR_STEP));
				if(ptr==NULL)
				{
					fprintf(stderr, "Error reallocating memory\n");
					return 0;
				}
				pnode->childptr = ptr;
				pnode->capacity += INCR_STEP;
			}
            // insert ahead
			for(i=pnode->numchild; i>pos; --i)
				pnode->childptr[i] = pnode->childptr[i-1];
			//initialize the node newly inserted
			trienode_initialize(pnode->childptr+pos, *chars);
			pnode->numchild += 1;
		}
		//get the node directly and walk down
		pnode = pnode->childptr+pos;
	}
	if(pnode->stat == STAT_NONWORD)
	{
		pnode->stat = STAT_WORD;//the node is the end of a word
		return 1;
	}
	return 0;//the word already exists,failed to insert
}

////find a word in the tree and returns the corresponding node with binary search
//TRIENODE* trietree_find(const TRIETREE* ptree, const uchar* chars)
//{
//	TRIENODE* pnode;
//	int pos;
//	if(ptree==NULL || chars==NULL)
//		return NULL;
//	
//    pnode = (TRIENODE*)ptree[*(chars)];
//	if(pnode == NULL)
//		return NULL;
//    
//	while(*(++chars))
//	{
//		//binary search
//		pos=binary_search(pnode->childptr, pnode->numchild, *chars);
//		if(pos<0)
//			return NULL;
//		pnode = pnode->childptr+pos;
//	}
//	if( pnode->stat == STAT_WORD)
//		return pnode;
//	return NULL;
//}

//match a string(bytes oriented) in the dictionary as much as possible
int trietree_match_prefix(const TRIETREE* ptable, const uchar* chars)
{
	TRIENODE* pnode;
	int depth=0, pos, match_lenth=0;
	if(ptable==NULL || chars==NULL)
		return 0;
	pnode = ptable[*(chars+depth)];//get the root of trie-tree
	if(pnode == NULL)//the first byte doesn't match
		return 0;
	while(*(chars+(++depth)))
	{
		if(pnode->stat == STAT_WORD)//match a word
			match_lenth = depth;
		pos = binary_search(pnode->childptr, pnode->numchild, *(chars+depth));
		if(pos < 0)//the given byte doesn't exist
			return match_lenth;
		pnode = pnode->childptr+pos;
	}
	if(pnode->stat == STAT_WORD)//match a word
		match_lenth = depth;
	return match_lenth;
}

int binary_search(const TRIENODE* a, int num, uchar val)
{
	int low=0, high=num-1, mid;
	if(a == NULL)
		return -1;
	while(low <= high)
	{
		mid = (low+high)/2;
		if(a[mid].keyword > val)
			high = mid-1;
		else if(a[mid].keyword < val)
			low = mid+1;
		else
			return mid;
	}
	return -1;
}
