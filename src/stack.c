
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
	s->arr = (token*)myMalloc(STACK_SIZE * sizeof(token));
	s->size = STACK_SIZE;
}

void stackIncrement ( tStack* s ){
	token *new = (token)myRealloc(s->arr, s->size + STACK_SIZE);
	if(new == NULL)
		return;
	s->arr = new;
	s->size += STACK_SIZE;
}

int stackEmpty ( const tStack* s ) {
	return ((s->top < 0) ? 1 : 0);
}

int stackFull ( const tStack* s ) {
	return ((s->top < s->size) ? 0 : 1);
}

token *stackTop ( const tStack* s) {
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

void stackPush ( tStack* s, token token ) {
	if(stackFull(s))
		stackIncrement(s);
	s->arr[++(s->top)] = token;
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
