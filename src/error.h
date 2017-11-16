#ifndef ERROR_H_
#define ERROR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "garbage.h"

typedef enum {
	PARSE_OK,
	SYNTAX_ERR,
	SEMANTIC_REDEF
} parse_errno;

void warning_msg(const char *fmt, ...);
void error_msg(int errno, const char *fmt, ...);

#endif /* ERROR_H_ */
