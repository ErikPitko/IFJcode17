#ifndef ERROR_H_
#define ERROR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "garbage.h"

typedef enum {
	PARSE_OK,
	//chyba v programu v rámci lexikální analýzy
	LEX_ERR = 1,
	///chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu)
	SYNTAX_ERR = 2,
	//nedefinovaná funkce/proměnná, pokus o redefinici funkce/proměnné
	SEMANTIC_REDEF = 3,
	//sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních
	//výrazech, příp. špatný počet či typ parametrů u volání funkce
	SEMANTIC_TYPE = 4,
	//ostatní sémantické chyby
	SEMANTIC_OTHER = 6,
	//interní chyba překladače tj. neovlivněná vstupním programem
	INTERNAL_ERROR = 99
} parse_errno;

void warning_msg(const char *fmt, ...);
void error_msg(int errno, const char *fmt, ...);

#endif /* ERROR_H_ */
