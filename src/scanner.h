#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

typedef struct structToken{
	unsigned type;
	char *info; //Numbers will be in here too.
	/// strtonum(...)
/*
	int integer;
	double double_num;
*/
}token;


int upper2lower(int c){
	if (c >= 'A' && c <= 'Z')
		return c+32; // posunutie podlaa ASCII table
	return c;
}
