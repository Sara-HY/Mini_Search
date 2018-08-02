#pragma once
#pragma execution_character_set("UTF-8")

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include "trietree.h"

typedef struct DICTIONARY
{
	TRIETREE* ptrees;
	unsigned int dic_capacity;
}DICTIONARY;


DICTIONARY* dict_initialize();
void dict_from_buffer(DICTIONARY* pdict, const uchar** pchars, int num);
void dict_from_file(DICTIONARY* pdict, const char* filename);
void dict_remove(DICTIONARY* pdict, const uchar* chars);
int dict_find(const DICTIONARY* pdict, const uchar* chars);
int dict_match_prefix(const DICTIONARY* pdict, const uchar* chars);
void dict_destroy(DICTIONARY** ppdict);
void dict_print(const DICTIONARY* pdict);

#endif
