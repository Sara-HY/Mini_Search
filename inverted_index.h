#pragma once
#pragma execution_character_set("UTF-8")

#ifndef INVERTED_INDEX_H_
#define INVERTED_INDEX_H_

#include "dictionary.h"

typedef unsigned int uint;

typedef struct DOCNODE
{
	uint docid;    //unique id for this document
	double tf;     //normalized term frequency
	struct DOCNODE* pnext;
}DOCNODE;

/*all the documents contain the specific word are stored in a linked list and sorted in ascending order of document id.*/
typedef struct WORDNODE
{
	char* pword;
	DOCNODE* phead;   //pointer to the first document of a linked list
	double idf;       //inverse document frequency:log(N/(n_i+1))
	struct WORDNODE* pnext;
}WORDNODE;

typedef struct INDEXITEM
{
	uint hashcode;    //hash code for words
	struct WORDNODE* phead;
}INDEXITEM;

typedef struct INDEXTABLE
{
	uint prime_num;   //prime_num for hashing
	struct INDEXITEM* pindex;
}INDEXTABLE;


uint inverted_index_hash(const void* ptr, int lenth);
uint find_prime(uint bound);
WORDNODE* inverted_index_new_wordnode(const char* src);
DOCNODE* inverted_index_new_docnode(int doc_id);
INDEXTABLE* inverted_index_initialize(const DICTIONARY* pdict);
void inverted_index_insert_word(INDEXTABLE* ptable, const char* pword);
void inverted_index_insert_words(INDEXTABLE* ptable, const TRIETREE root, char* buf, int depth);
void inverted_index_insert_dictionary(INDEXTABLE* ptable, const DICTIONARY* pdict);
void inverted_index_destroy_table(INDEXTABLE** pptable);
void inverted_index_save(const INDEXTABLE* ptable, const char* outfilename);
INDEXTABLE* inverted_index_load(const char* infilename);
void inverted_index_print(const INDEXTABLE* ptable);

#endif
