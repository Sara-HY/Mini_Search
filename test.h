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


#ifdef __cplusplus
extern "C"
{
#endif // _cplusplus
	DOCFEEDBACK* query_and(COREDATA* pcoredata, char* src);
	DOCFEEDBACK* query_or(COREDATA* pcoredata, char* src);
	DOCFEEDBACK* query(COREDATA* pcoredata, char* src);
	COREDATA* build();
} 