#pragma once
#pragma execution_character_set("UTF-8")

#ifndef STACK_H_
#define STACK_H_

#define INC_STACK 5

typedef struct STACK
{
	int top;     //actual number of stack
	int* buf;
	int capacity;//capacity of stack
}STACK;

void stack_initialize(STACK** ppstack);
void stack_push(STACK* pstack, int elem);
int stack_pop(STACK* pstack, int* stat);
void stack_destroy(STACK** ppstack);
void stack_print(const STACK* pstack);
int stack_isempty(const STACK* pstack);

#endif
