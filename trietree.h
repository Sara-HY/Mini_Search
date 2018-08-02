#pragma once
#pragma execution_character_set("UTF-8")

#ifndef TRIETREE_H_
#define TRIETREE_H_

#define INCR_STEP 2

typedef unsigned char uchar;
typedef enum nodeStat{STAT_INVALID=-1, STAT_NONWORD, STAT_WORD} nodeStat;

/* 可用Trie树结构存储和检索单词，从而实现词汇的智能提示功能*/
typedef struct TRIENODE
{
	uchar keyword;
	uchar numchild;
    uchar capacity;
	struct TRIENODE* childptr;
	nodeStat stat; //state of the node
}TRIENODE, *TRIETREE;


void trienode_initialize(TRIENODE* pnode, uchar key);
void trietree_destroy(TRIETREE root);
int trietree_insert(TRIETREE* ptree, const uchar* chars);
void trietree_remove(const TRIETREE* ptree, const uchar* chars);
TRIENODE* trietree_find(const TRIETREE* ptree, const uchar* chars);
int trietree_match_prefix(const TRIETREE* ptable, const uchar* chars);
void trietree_print(const TRIETREE root, char* buf, int dep);
int binary_search(const TRIENODE* arr, int num, uchar val);

#endif
