/*
**    stackInit .... inicializace zásobníku
**    stackEmpty ... test na prázdnost zásobníku
**    stackFull .... test na zaplněnost zásobníku
**    stackTop ..... přečte hodnotu z vrcholu zásobníku
**    stackPop ..... odstraní prvek z vrcholu zásobníku
**    stackPush .... vloží prvku do zásobníku
**/

#include "stack.h"

unsigned STACK_SIZE = 1000;

void stackInit ( tStack* s ) {
	if(s == NULL)
		return;

	s->top = -1;
	s->arr = (tReductToken*)myMalloc(STACK_SIZE * sizeof(tReductToken));
	s->size = STACK_SIZE;
}

void stackIncrement ( tStack* s ){
	tReductToken *new = (tReductToken*)myRealloc(s->arr, (s->size + STACK_SIZE)*sizeof(tReductToken));
	if(new == NULL)
		return;
	s->arr = new;
	s->size += STACK_SIZE;
}

int stackEmpty (tStack* s ) {
	return ((s->top < 0) ? 1 : 0);
}

int stackFull (tStack* s ) {
	return ((s->top < (int)s->size) ? 0 : 1);
}

tReductToken *stackTop ( tStack* s) {
	if (stackEmpty(s))
		return NULL;
	else
		return (&(s->arr[s->top]));
}

void stackPop ( tStack* s ) {
	if (stackEmpty(s))
		return;
	s->top--;
}

int stackLenght(tStack *s)
{
	return (s->top)+1;
}

tReductToken *stackTopPop ( tStack* s) {
	tReductToken *temp = stackTop(s);
	stackPop(s);
	return temp;
}
void stackPush ( tStack* s, tReductToken token ) {
	if(stackFull(s))
		stackIncrement(s);
	s->arr[++(s->top)] = token;
}

void stackPrint(char* stackName, tStack* s)
{
	printf("%s:\n",stackName);
	if(stackLenght(s) == 0)
	{
		printf("Stack is empty\n");
		return;
	}
	for(int i =stackLenght(s)-1;i>=0;i--)
	{
		if((s->arr[i]).firstToken->info != NULL)
		printf("\t%i-index = %s \n",i,(s->arr[i]).firstToken->info);
		else printf("\t%i-index = %s \n",i, strValueOfEnum((s->arr[i]).firstToken->type));
	}
}
void stackClear(tStack* s)
{
	if(s!= NULL)
	{
		myFree(s->arr);
	}
}
bool checkMoreOperators(tStack* s)
{
	int counter = 0;
	for(int i =0;i<s->top+1;i++)
	{
		if(s->arr[i].firstToken->type>=36 && s->arr[i].firstToken->type<=46)
		{
			printf("aaa\n");
			counter++;
		}
	}
	if(counter>1)
		return true;
	return false;
}
