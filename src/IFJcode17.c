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
	(void) argc;
	(void) argv;
	garbageInit(2000);
	parse();
  /*if (argc == 2){
	FILE *file;
	char *file_name = argv[1];
	
	file = fopen(file_name,"r");
	if (file == NULL)
		perror ("The following error occurred");//
  } else fprintf(stderr,"Wrong number of arguments");
	
	puts("");*/
	garbageFree();
	return (EXIT_SUCCESS);
}
