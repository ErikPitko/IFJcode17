#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

typedef struct structToken{
	unsigned type;
	char *info; //Numbers will be in here too.
}token;



int upper2lower(int c);

int isKeyword(char* s);


int isOperator(int c);
// funkce pro simulaci ungetc(c,file);
int getchar0(int c);
