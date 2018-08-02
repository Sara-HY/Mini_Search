#pragma once
#pragma execution_character_set("UTF-8")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Chinese_word_segment.h"

//forward maximum matching method
//flag=0 means excluding the words not in dictionary. Otherwise, flag is non-zero
int forward_match(const DICTIONARY* pdict, const uchar* src, uchar** output, int max_words, int flag)
{
    int cnt=0, match_lenth;
    long lenth=0;
	int min_word_bytes;
	const uchar* ptr=src, *ptr_end=src;
	uchar* buf=NULL;
	if(pdict==NULL || src==NULL || output==NULL || max_words<1)
		return cnt;

	buf = (uchar*)calloc(MAX_WORD_BYTES+1, sizeof(uchar));
	if(buf==NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		return cnt;
	}

	//point to the end of the string
	while(*ptr_end)
        ++ptr_end;

	while((ptr<ptr_end) && cnt<max_words)
	{
		lenth = ptr_end-ptr;
		if(lenth > MAX_WORD_BYTES)
			lenth = MAX_WORD_BYTES;
		strncpy((char*)buf, (char*)ptr, lenth);
        //word's 3 bytes, character's 1 byte
		min_word_bytes = (buf[0]>0x7f)? 3:1;//whether buf[0] is a ASCII character
		buf[lenth]=0;
        //find the maximal number of bytes matched in the given dictionary
		match_lenth = dict_match_prefix(pdict, buf);
        //make sure the length string left is not less than the lower bound
		match_lenth=(match_lenth<min_word_bytes)? min_word_bytes:match_lenth;
		buf[match_lenth] = 0;
		ptr += match_lenth;//change the address of the string to be segmented
		//a word is obtained successfully
		if(flag || match_lenth>min_word_bytes)
			strcpy((char*)*(output+(cnt++)), (char*)buf);
	}
	free(buf);
	return cnt;
}