/*
 ============================================================================
 Name        : IFJcode17.c
 Author´s    : Michal Šajdík "xsajdi00"
 	       Erik Pitko " "
	       Adam Petráš " "
	       Peter Andris " "
 Version     :
 Copyright   : 
 Description : IFJ17 compiler
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "ExpressionParser.h"
#include "parser.h"

token *lastToken;

int main(int argc, char *argv[])
{
	parse_errno ret;
	(void) argc;
	(void) argv;
	garbageInit(2000);
	LInit(&globalInstrList);

	lastToken = myMalloc(sizeof(struct structToken));
	lastToken->type = NOPE;

	ret = parse();
  /*if (argc == 2){
	FILE *file;
	char *file_name = argv[1];
	
	file = fopen(file_name,"r");
	if (file == NULL)
		perror ("The following error occurred");//
  } else fprintf(stderr,"Wrong number of arguments");
	
	puts("");*/
	if (ret == PARSE_OK){
		garbageFree();
		return (EXIT_SUCCESS);
	}else
		error_msg(ret, "error: %d", ret);
}
