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
#include "InstructionList.h"
int parseExpression();
int tableIndexSelect(tReductToken *tok);
bool canIDiv(token *firstOperand,token *secondOperand,bool isDouble, bool *cast);
bool areOperandsSame(token *firstOperand,token *secondOperand);
bool isOperand(token *operand, bool isArithmethic);
bool areOperands(token *firstOperand,token *secondOperand, bool isArithmethic);
void applyRule(tStack *stack,tStack *rStack,bool *reduct);
#endif /* EXPRESSIONPARSER_H_ */
