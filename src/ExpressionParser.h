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
#define EXPR 999
int parseExpression(token *retValue);
int tableIndexSelect(tReductToken *tok);
bool canIDiv(token *firstOperand,token *secondOperand,bool isDouble, bool *cast);
bool areOperandsSame(token *firstOperand,token *secondOperand);
bool areOperandsSameArithmethic(token *firstOperand,token *secondOperand);
bool isOperand(token *operand, bool isArithmethic);
bool areOperands(token *firstOperand,token *secondOperand, bool isArithmethic);
void applyRule(tStack *st,tStack *rStack,bool *reduct);
char* strValueOfEnum(int enumValue);
#endif /* EXPRESSIONPARSER_H_ */
