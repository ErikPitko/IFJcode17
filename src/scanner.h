#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

extern int ungetcharpom;




typedef struct structToken{
	unsigned type;
	char *info; //Numbers will be in here too.
}token;



int upper2lower(int c){
	if (c >= 'A' && c <= 'Z')
		return c+32;
	return c;
}

int isKeyword(char* s){
	int i = 0;

	char* keyword[] = {
		"as", //1
		"asc",
		"declare",
		"dim",
		"do",
		"double",
		"else",
		"end",
		"chr",
		"function",
		"if",
		"input",
		"integer",
		"length",
		"loop",
		"print",
		"return",
		"scope",
		"string",
		"substr",
		"then",
		"while",
				//reserved
		"and",
		"boolean",
		"continue",
		"elseif",
		"exit",
		"false",
		"for",
		"next",
		"not",
		"or",
		"shared",
		"static",
		"true", //35
		NULL//36
	};


	while (keyword[i] != NULL){
		if (strcmp(s,keyword[i]) == 0){
			return(i+1); // vraci hodnotu daneho keywordu

		}

		i++;
	}
	return 0;// don`t even think about it
}

int isOperator(int c){
	if(c == '+'|| c == '-'|| c == '*'|| c == 92 || c == '<' || c == '>' || c == '=' )
		return c;
	else return 0;
}
// funkce pro simulaci ungetc(c,file);
int getchar0(int c){
	if(ungetcharpom){
		int pom;
		pom = ungetcharpom;
		ungetcharpom = 0;
		return pom;
	}else return getchar();
}
