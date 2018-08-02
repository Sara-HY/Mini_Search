#pragma once
#pragma execution_character_set("UTF-8")

#ifndef CHINESE_WORD_SEGMENT_H_
#define CHINESE_WORD_SEGMENT_H_

#include "dictionary.h"

//the maximal number of bytes of the sub-sequence to be segmented
#define MAX_WORD_BYTES 75

/*
 * 基于字符串匹配的分词方法:
 * 1)正向最大匹配法（由左到右的方向）；
 * 2)逆向最大匹配法（由右到左的方向）；
*/

int forward_match(const DICTIONARY* pdict,const uchar* src,uchar** output,int max_words,int flag);
//int reverse_match(const DICTIONARY* pdict,const uchar* src,uchar** output,int max_words,int flag);

#endif
