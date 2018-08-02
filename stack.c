
#pragma execution_character_set("UTF-8")

#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

/* 栈顶为0，表示不含元素 */
void stack_initialize(STACK** ppstack)
{
	if(*ppstack == NULL)
		*ppstack=(STACK*)calloc(1, sizeof(STACK));
	(*ppstack)->top = 0;
	(*ppstack)->capacity = 0;
	(*ppstack)->buf = NULL;
}

void stack_push(STACK* pstack, int elem)
{
	int* newbuf = NULL;
	if(pstack==NULL)
		return;
	if(pstack->top == pstack->capacity) //the stack if full
	{
		newbuf = (int*)realloc(pstack->buf, (pstack->capacity+INC_STACK)*sizeof(int));
		if(newbuf==NULL)
		{
			fprintf(stderr, "Error reallocating memory\n");
			return;
		}
		pstack->buf = newbuf;
		pstack->capacity += INC_STACK;
	}
	*(pstack->buf + pstack->top++) = elem;
}

/* stat表示pop成功与否，若为1则成功，为0则失败。 返回pop的值 */
int stack_pop(STACK* pstack, int* stat)
{
	if(pstack==NULL ||pstack->top==0)
	{
		*stat = 0; //pop error
		return 0;
	}
	*stat = 1;//pop an element successfully
	return *(pstack->buf+(--(pstack->top)));
}

void stack_destroy(STACK** ppstack)
{
	if(*ppstack)
	{
		free((*ppstack)->buf);
		free(*ppstack);
		*ppstack = NULL;
	}
}

int stack_isempty(const STACK* pstack)
{
	if(pstack == NULL)
		return 1;
	return (pstack->top>0)? 0:1;
}
