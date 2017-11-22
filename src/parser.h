/*
 * parser.h
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "scanner.h"
#include "ExpressionParser.h"
#include "garbage.h"
#include "error.h"
#include "symtable.h"

parse_errno parse();

#endif /* PARSER_H_ */
