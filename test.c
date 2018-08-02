#pragma once
#pragma execution_character_set("UTF-8")

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "dictionary.h"
#include "Chinese_word_segment.h"
#include "stack.h"
#include "htmlparser.h"
#include "inverted_index.h"
#include "search.h"

DOCFEEDBACK* query_and(COREDATA* pcoredata, char* src)
{
	const int max_words = 10;
	const int max_lenth = 20;
	const int max_line_lenth = 300;
	unsigned long valid_line_lenth;
	const char * dictfile = "data\\default_out.dic";
	int i, match_num, choose;
	DICTIONARY* pdict = NULL;
	pdict = pcoredata->pdict;
	/*pdict = dict_initialize();
	if (pdict == NULL)
	{
		fprintf(stderr, "Error initializing dictionary\n");
		return NULL;
	}
	dict_from_file(pdict, dictfile);*/
	uchar ** output = (uchar **)calloc(max_words, sizeof(uchar *));
	for (i = 0; i<max_words; ++i)
		output[i] = (uchar *)calloc(max_lenth, sizeof(uchar));

	match_num = forward_match(pdict, src, output, max_words, 1);

	/*const char * pdictfile = "data\\default_out.dic";
	const char * pdocspath = "data\\html";
	const char * pmapfile = "data\\webdownloader.map";
	COREDATA* pcoredata = search_initialize(pdictfile, pdocspath, pmapfile);*/

	DOCFEEDBACK* pdocfeedback = NULL;
	pdocfeedback = search_queries(pcoredata, (char **)output, match_num, 20);

	// pdocfeedback->docid, pdocfeedback->score, pcoredata->pdocset->pdocinfo[pdocfeedback->docid].url, pcoredata->pdocset->pdocinfo[pdocfeedback->docid].title);
	//	pdocfeedback = pdocfeedback->pnext;
	//search_destroy(&pcoredata);
	/*for (i = 0; i<max_words; i++)
	{
		free(*(output + i));
	}
	free(output);*/
	return pdocfeedback;
}

DOCFEEDBACK* query_or(COREDATA* pcoredata, char* src)
{
	const int max_words = 10;
	const int max_lenth = 20;
	const int max_line_lenth = 300;
	unsigned long valid_line_lenth;
	/*const char * dictfile = "data\\default_out.dic";*/
	int i, match_num, choose;
	DICTIONARY* pdict = NULL;
	pdict = pcoredata->pdict;
	/*pdict = dict_initialize();
	if (pdict == NULL)
	{
		fprintf(stderr, "Error initializing dictionary\n");
		return NULL;
	}
	dict_from_file(pdict, dictfile);*/
	uchar ** output = (uchar **)calloc(max_words, sizeof(uchar *));
	for (i = 0; i<max_words; ++i)
		output[i] = (uchar *)calloc(max_lenth, sizeof(uchar));

	match_num = forward_match(pdict, src, output, max_words, 1);
	DOCFEEDBACK* pdocfeedback = NULL;
	DOCFEEDBACK *phead = NULL, *ptail = NULL, *ptr = NULL;
	DOCFEEDBACK *cur = NULL, *pre = NULL;
	phead = malloc(sizeof(DOCFEEDBACK));
	phead->pnext = NULL;
	for (i = 0; i<match_num; i++)
	{
		pdocfeedback = search_queries(pcoredata, (char **)output + i, 1, 20);
		while (pdocfeedback)
		{
			pre = phead;
			cur = phead->pnext;
			ptr = pdocfeedback;
			while (cur && cur->docid != ptr->docid)
			{
				pre = cur;
				cur = cur->pnext;
			}
			if (cur == NULL)
			{
				pre = phead;
				cur = phead->pnext;
				ptr = pdocfeedback;
				while (cur && cur->score > ptr->score)
				{
					pre = cur;
					cur = cur->pnext;
				}
				pdocfeedback = pdocfeedback->pnext;
				ptr->pnext = cur;
				pre->pnext = ptr;
			}
			else
			{
				if (cur->score < ptr->score)
				{
					cur->score = ptr->score;
				}
				pdocfeedback = pdocfeedback->pnext;
			}
		}
	}
	ptail = phead;
	i = 0;
	while (i < 20 && ptail)
	{
		ptail = ptail->pnext;
		++i;
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

	phead = phead->pnext;
	return phead;
}

DOCFEEDBACK* query(COREDATA* pcoredata, char* src)
{
	DOCFEEDBACK* pdocfeedback_and = NULL;
	DOCFEEDBACK* pdocfeedback_or = NULL;
	DOCFEEDBACK* ptail, *ptr;

	int i;

	pdocfeedback_and = query_and(pcoredata, src);
	pdocfeedback_or = query_or(pcoredata, src);


	//将or搜索结果连接在and搜索结果后面
	if (pdocfeedback_and != NULL)
	{
		ptail = pdocfeedback_and;
		while (ptail->pnext)
			ptail = ptail->pnext;
		i = 0;
		while (i < 20 && pdocfeedback_or)
		{
			ptr = pdocfeedback_and;
			while (ptr && pdocfeedback_or->docid != ptr->docid)
			{
				ptr = ptr->pnext;
			}
			if (ptr == NULL)
			{
				ptail->pnext = pdocfeedback_or;
				ptail = ptail->pnext;
			}
			pdocfeedback_or = pdocfeedback_or->pnext;
			i++;
		}
		return pdocfeedback_and;
	}
	else
		return pdocfeedback_or;
}

COREDATA* build()
{
	const char * pdictfile = "data\\default_out.dic";
	const char * pdocspath = "data\\html";
	const char * pmapfile = "data\\webdownloader.map";
	COREDATA* pcoredata = NULL;
	pcoredata = search_initialize(pdictfile, pdocspath, pmapfile);

	return pcoredata;
}
