#pragma once
#pragma execution_character_set("UTF-8")

#ifndef HTMLPARSER_H_
#define HTMLPARSER_H_


void htmlparser_read_file(FILE* fp, char* buf, long int max_len);
void htmlparser_remove_angle_brackets(char* buf);
void htmlparser_remove_tag(char* buf, const char* tag);
void htmlparser_remove_str(char* buf, const char* target);
void htmlparser_data_clean(char* buf);
void htmlparser_gettitle(const char* buf, char* ptitle, int maxlen);
#endif
