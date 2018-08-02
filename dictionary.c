#pragma once
#pragma execution_character_set("UTF-8")

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"
#include "trietree.h"

/*Initialize dictionary*/
DICTIONARY* dict_initialize()
{
	TRIETREE* ptrees = NULL;
	DICTIONARY* pdict = (DICTIONARY*)calloc(1, sizeof(DICTIONARY));
	ptrees = (TRIETREE*)calloc(0xff,sizeof(TRIETREE));
	if(pdict==NULL || ptrees==NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		free(pdict);
		free(ptrees);
		return NULL;
	}
	pdict->dic_capacity = 0;
	pdict->ptrees=ptrees;
	return pdict;
}

void dict_from_file(DICTIONARY* pdict, const char* filename)
{
	const int max_lenth = 150;//the maximum number of bytes for each word
	uchar* buf;
	FILE* fp;
	unsigned long lenth;

	fp = fopen(filename,"rt");
	if(fp==NULL)
	{
		fprintf(stderr, "failed to open file %s\n", filename);
		return;
	}

	buf=(uchar*)calloc(max_lenth+1, sizeof(uchar));
	if(buf==NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		return;
	}

	while(fgets((char*)buf, max_lenth+1, fp))
	{
		lenth = strlen((char*)buf);
		buf[lenth-1] = 0;//remove '\n'
		if(trietree_insert(pdict->ptrees, buf)) //insert a new word
			++(pdict->dic_capacity); //increase the number of words in dictionary
	}
	fclose(fp);
	free(buf);
}

//int dict_find(const DICTIONARY* pdict, const uchar* chars)
//{
//	TRIENODE* ptr;
//	if(pdict == NULL)
//		return 0;
//	ptr = trietree_find(pdict->ptrees, chars);
//	return (ptr!=NULL && ptr->stat==STAT_WORD)? 1:0;
//}

int dict_match_prefix(const DICTIONARY* pdict, const uchar* chars)
{
	if(pdict==NULL)
		return 0;
	return trietree_match_prefix(pdict->ptrees, chars);
}
