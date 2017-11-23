/*
 * ExpressionParser.h
 *
 *  Created on: 14. 11. 2017
 *      Author: adam
 */

#ifndef EXPRESSIONPARSER_H_
#define EXPRESSIONPARSER_H_
#include <stdbool.h>
#include "stack.h"
#include "header.h"
#include "scanner.h"
#include "error.h"
#include "InstructionList.h"
#include "symtable.h"
#define EXPR 999
token *parseExpression(token *getSetToken,tFooListElem *returnVar,tHashTable *localTable);
#endif /* EXPRESSIONPARSER_H_ */
