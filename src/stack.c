
/*
**    stackInit .... inicializace zásobníku
**    stackEmpty ... test na prázdnost zásobníku
**    stackFull .... test na zaplněnost zásobníku
**    stackTop ..... přečte hodnotu z vrcholu zásobníku
**    stackPop ..... odstraní prvek z vrcholu zásobníku
**    stackPush .... vloží prvku do zásobníku
**/

#include "stack.h"

unsigned STACK_SIZE = 200;

void stackInit ( tStack* s ) {
	if(s == NULL)
		return;

	s->top = -1;
	s->arr = (tReductToken*)myMalloc(STACK_SIZE * sizeof(tReductToken));
	s->size = STACK_SIZE;
}

void stackIncrement ( tStack* s ){
	tReductToken *new = (tReductToken*)myRealloc(s->arr, s->size + STACK_SIZE);
	if(new == NULL)
		return;
	s->arr = new;
	s->size += STACK_SIZE;
}

int stackEmpty ( const tStack* s ) {
	return ((s->top < 0) ? 1 : 0);
}

int stackFull ( const tStack* s ) {
	return ((s->top < (int)s->size) ? 0 : 1);
}

tReductToken *stackTop ( const tStack* s) {
	if (stackEmpty(s))
		return NULL;
	else
		return (&(s->arr[s->top]));
}

tReductToken *stackBeforeTop ( const tStack* s) {
	if (stackEmpty(s)&& s->top>1)
		return NULL;
	else
		return (&(s->arr[(s->top)-1]));
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
void stackPushUnderTop(tStack* s, tReductToken token)
{
	tReductToken tmp = *(stackTopPop(s));
	stackPush(s,token);
	stackPush(s,tmp);
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
		if((s->arr[i]).firstToken != NULL)
		printf("\t%i-index = %s \n",i,(s->arr[i]).firstToken->info);
		//else printf("%i-index = %i ",i,(s->arr[i]).priority);
	}
}
void stackClear(tStack* s)
{
	if(s!= NULL)
	{
		myFree(s->arr);
	}
}
/*
int main(){
	garbageInit(400);
	struct structToken *temp;
	struct structToken token;
	token.info = "a";
	token.type = 1;
	tStack st;
	stackInit(&st);
	stackPush(&st, token);
	temp = stackTop(&st);
	fprintf(stdout, "%c \t -- top", (temp) ? temp->info[0]:'0');
	stackPop(&st);
	garbageFree();
	return EXIT_SUCCESS;
}
*/

