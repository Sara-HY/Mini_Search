#pragma once
#pragma execution_character_set("UTF-8")

#ifndef SEARCH_H_
#define SEARCH_H_

#include "inverted_index.h"

typedef struct DOCSINFO
{
	char* url;
	char* title;
}DOCSINFO;

typedef struct DOCSET
{
	DOCSINFO* pdocinfo;
	uint max_docid;
	uint docsnum;
}DOCSET;

typedef struct COREDATA
{
	DOCSET* pdocset; //document set
	DICTIONARY* pdict; //dictionary
	INDEXTABLE* ptable; //inverted index table
}COREDATA;

typedef struct DOCFEEDBACK
{
	unsigned int docid;
	double score;     //tf*idf
	struct DOCFEEDBACK* pnext;
}DOCFEEDBACK;


DOCSINFO* search_new_docinfo(const char* url, const char* title);
DOCSET* search_new_docset(const char* pmapfile);
DOCFEEDBACK* search_new_docfeedback(unsigned int doc_id, double score);
void search_insert_doc(INDEXTABLE* ptable, uint doc_id, const char* url, const char* title, int wordnum, char** ppwords);
void search_insert_docs(INDEXTABLE* ptable, const DICTIONARY* pdict, DOCSET* pdocset, const char* pfilepath, const char*
                        pmapfile);
void search_destroy_docset(DOCSET** ppdocset);
void search_print(const COREDATA* pcoredata);

COREDATA* search_initialize(const char* pdictfilename, const char* pdocspath, const char* pmapfilename);
void search_destroy(COREDATA** ppcoredata);

DOCFEEDBACK* search_handle_query(const COREDATA* pcoredata, const char* pquery);
DOCFEEDBACK* search_queries(const COREDATA* pcoredata, char** ppqueries, int num, int max_num);
void sort_feedbacks(DOCFEEDBACK* pdocfeedback, int maxnum);

#endif
