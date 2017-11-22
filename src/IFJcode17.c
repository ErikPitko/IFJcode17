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
int main(int argc, char *argv[])
{
	parse_errno ret;
	(void) argc;
	(void) argv;
	garbageInit(2000);
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
	}
	else if(ret == SYNTAX_ERR)
		error_msg(2, "Syntax error");
	else if(ret == SEMANTIC_REDEF)
		error_msg(3, "Semantic ERR");
	else{
		garbageFree();
		return(EXIT_SUCCESS);
	}
}
