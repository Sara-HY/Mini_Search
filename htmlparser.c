#pragma once
#pragma execution_character_set("UTF-8")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htmlparser.h"
#include "stack.h"

void htmlparser_read_file(FILE* fp, char* buf, long int max_len)
{
	int cnt=0;
	if(fp==NULL || buf==NULL || max_len<0)
		return;
	//read the content of a html file into pcontent
	while(cnt+1<max_len && fgets(buf+cnt, max_len-cnt, fp))
		while(*(buf+cnt))
            ++cnt;
}

//remove <***> from the source of html
void htmlparser_remove_angle_brackets(char* buf)
{
	char *ptr;
	int stat, pos;
	unsigned long max_len;
	STACK* pstack = NULL;

	if(buf == NULL)
		return;
	max_len = strlen(buf);
	stack_initialize(&pstack);
	ptr = buf;
	while(*ptr)
	{
		if('<' == *ptr)
			stack_push(pstack, ptr-buf);
		else if('>' == *ptr)
		{
			pos = stack_pop(pstack, &stat);
			if(stat)//'<' exists
			{
				memmove(buf+pos, ptr+1, max_len-(ptr-buf));
				ptr = (buf+pos);
				continue;
			}
		}
		++ptr;
	}
	stack_destroy(&pstack);
}

//remove specific tags with the format <tag***>**</tag>from html file
//<srcipt**>***</script>
//<style***>***</style>
void htmlparser_remove_tag(char* buf, const char* tag)
{
	char *ptag_begin=0, *ptag_end=0;
	char *pbegin=0, *pend=0;
	char *ptr=0, *p=0;
	unsigned long tag_len, buf_len;

	if(buf==NULL || tag==NULL)
		return;
	tag_len = strlen(tag);
	buf_len = strlen(buf);
	ptag_begin = (char*)calloc(tag_len+2, sizeof(char));
	ptag_end=(char*)calloc(tag_len+4, sizeof(char));
	if(ptag_begin==NULL || ptag_end==NULL)
	{
		fprintf(stderr,"Error allocating memory\n");
		free(ptag_begin);
		free(ptag_end);
		return;
	}
	sprintf(ptag_begin, "<%s",tag);//construct "<tag"
	sprintf(ptag_end, "</%s>",tag);//construct "</tag>"

	ptr = strstr(buf,ptag_begin);
	if(ptr==NULL)//there doesn't exist <tag**>  or </tag> in the string
	{
		free(ptag_begin);
		free(ptag_end);
		return;
	}
	pend = strstr(ptr, ptag_end);//find </tag>
	if(pend==NULL)//there doesn't exist <tag**>  or </tag> in the string
	{
		free(ptag_begin);
		free(ptag_end);
		return;
	}
	while(pend)
	{
		pbegin = strstr(pend+tag_len+4, ptag_begin);//find next <tag**
		if(pbegin==NULL)
            pbegin = buf+buf_len;
		//move the content between </tag> and <tag**
		for(p=pend+tag_len+3; p<pbegin; ++p)
			*(ptr++) = *p;
		pend=strstr(pbegin, ptag_end);
	}
	*ptr=0;
	free(ptag_begin);
	free(ptag_end);
}

//remove specific string in buf
void htmlparser_remove_str(char* buf, const char* target)
{
	char *pbegin=0, *pend=0;
	char *ptr=0, *p=0;
	unsigned long buf_len=0, tar_len;
	if(buf==NULL || target==NULL)
		return;
    
	tar_len = strlen(target);
	buf_len = strlen(buf);
	ptr = strstr(buf,target);
	if(ptr==NULL)
		return;
	pend = ptr + tar_len;
	while(pend)
	{
		pbegin = strstr(pend, target);
		if(pbegin==NULL)
            pbegin = buf + buf_len;
		for(p=pend; p<pbegin; ++p)
			*(ptr++) = *p;
		//consider the target exists successively
		if(*pbegin)
			pend = pbegin + tar_len;
		else
			pend=NULL;
	}
	*ptr=0;
}

//get title of webpage from html source
void htmlparser_gettitle(const char* buf, char* ptitle, int maxlen)
{
	char *ptr_start, *ptr_end;
	long len;
	if(buf==NULL || ptitle==NULL)
		return;
	ptr_start = strstr(buf,"<title>");
	if(ptr_start==NULL)
		return;
	ptr_start += 7;
	ptr_end = strstr(buf, "</title>");
	if(ptr_end==NULL)
		return;
	len = ptr_end - ptr_start;
	len = (len<maxlen)? len:maxlen;
	strncpy(ptitle, ptr_start, len);
	*(ptitle+len)=0;
}

//Data cleaning form html file
void htmlparser_data_clean(char* buf)
{
	htmlparser_remove_tag(buf,"script");
	htmlparser_remove_tag(buf,"style");
	htmlparser_remove_angle_brackets(buf);
	htmlparser_remove_str(buf,"&nbsp");
	htmlparser_remove_str(buf,"\n");
	htmlparser_remove_str(buf,"\t");
	htmlparser_remove_str(buf," ");
}
