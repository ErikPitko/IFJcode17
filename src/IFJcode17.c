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

int main(int argc, char *argv[]) {
	
  if (argc == 1){
	FILE *file;
	char *file_name = argv[1];
	
	file = fopen(file_name,"r");
	if (file == NULL)
		fprintf(stderr,"Nepodarilo sa otvorit zdrojovy kod");
  } else fprintf(stderr,"Zly pocet argumentou");
	
	puts(""); /* prints  */
	return EXIT_SUCCESS;
}
