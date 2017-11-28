
/*
**    stackInit .... inicializace zásobníku
**    stackEmpty ... test na prázdnost zásobníku
**    stackFull .... test na zaplněnost zásobníku
**    stackTop ..... přečte hodnotu z vrcholu zásobníku
**    stackPop ..... odstraní prvek z vrcholu zásobníku
**    stackPush .... vloží prvku do zásobníku
**/

#include "cstack.h"

unsigned CSTACK_SIZE = 200;

void cstackInit ( tcStack* s ) {
	if(s == NULL)
		return;

	s->top = -1;
	s->arr = (int*)myMalloc(CSTACK_SIZE * sizeof(int));
	s->size = CSTACK_SIZE;
}

void cstackIncrement ( tcStack* s ){
	int *new = (int*)myRealloc(s->arr, s->size + CSTACK_SIZE);
	if(new == NULL)
		return;
	s->arr = new;
	s->size += CSTACK_SIZE;
}

int cstackEmpty (tcStack* s ) {
	return ((s->top < 0) ? 1 : 0);
}

int cstackFull (tcStack* s ) {
	return ((s->top < (int)s->size) ? 0 : 1);
}

int *cstackTop ( tcStack* s) {
	if (cstackEmpty(s))
		return NULL;
	else
		return (&(s->arr[s->top]));
}

void cstackPop ( tcStack* s ) {
	if (cstackEmpty(s))
		return;
	s->top--;
}

int cstackLenght(tcStack *s)
{
	return ((s->top)+1);
}

int *cstackTopPop ( tcStack* s) {
	int *temp = cstackTop(s);
	cstackPop(s);
	return (temp);
}
void cstackPush ( tcStack* s, int token ) {
	if(cstackFull(s))
		cstackIncrement(s);
	s->arr[++(s->top)] = token;
}

void cstackPrint(char* stackName, tcStack* s){
	printf("%s:\n",stackName);
	if(cstackLenght(s) == 0){
		printf("Stack is empty\n");
		return;
	}
	for(int i =cstackLenght(s)-1;i>=0;i--)
		printf("\t%i-index = %d \n", i, s->arr[i]);
}
void cstackClear(tcStack* s){
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

