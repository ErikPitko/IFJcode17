#ifndef SCANNER_H_
#define SCANNER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>



typedef struct structToken{
	unsigned type;
	char *info; //Numbers will be in here too.
}token;


token *getToken();
token *getToken0();
token *tokenInit();
int upper2lower(int c);

int isKeyword(char* s);


int isOperator(int c);
// funkce pro simulaci ungetc(c,file);
int getchar0(int c);

#define ERR do{fprintf(stderr,"ERR_1_\n");exit(1);}while(0)// printf("ERR_1_")//-2




#define EOF0 -1

#define NOPE 0  // pri blokovom komentary vrati NOPE
//nemenit poradie pre keyword
#define KEYWORD_START 1
#define AS 1
#define ASC 2
#define DECLARE 3
#define DIM 4
#define DO 5
#define DOUBLE 6
#define ELSE 7
#define END 8
#define CHR 9
#define FUNCTION 10
#define IF 11
#define INPUT 12
#define INTEGER 13
#define LENGTH 14
#define LOOP 15
#define PRINT 16
#define RETURN0 17
#define SCOPE 18
#define STRING 19
#define SUBSTR 20
#define THEN 21
#define WHILE 22
#define KEYWORD_END 22

#define RESERVED_START 23
#define AND 23
#define BOOLEAN 24
#define CONTINUE 25
#define ELSEIF 26
#define EXIT 27
#define FALSE 28
#define FOR 29
#define NEXT 30
#define NOT 31
#define OR 32
#define SHARED 33
#define STATIC 34
#define TRUE 35
#define RESERVED_END 35

#define TOKEN_START 36
#define LESS 36
#define INEQUALITY 37
#define LESS_EQUAL 38
#define EQUAL 39
#define GREATER 40
#define GREATER_EQUAL 41

#define PLUS 42
#define MINUS 43
#define ASTERIX 44
#define DIV_INT 45
#define DIV_DOUBLE 46

#define VALUE_INTEGER 47
#define VALUE_DOUBLE 48
#define VALUE_STRING 49

#define EOL 50
#define IDENTIFIER 51

#define LEFT_PARENTHESIS 52
#define RIGHT_PARENTHESIS 53

#define COMMA 54
#define SEMICOLON 55
#define TOKEN_END 55


#endif /* SCANNER_H_ */
