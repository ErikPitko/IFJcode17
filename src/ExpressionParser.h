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
int parseExpression();
int tableIndexSelect(tReductToken *tok);
bool areOperandOkey(token *firstOperand,token *secondOperand);
#endif /* EXPRESSIONPARSER_H_ */
