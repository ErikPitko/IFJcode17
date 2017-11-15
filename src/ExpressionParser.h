/*
 * ExpressionParser.h
 *
 *  Created on: 14. 11. 2017
 *      Author: adam
 */

#ifndef EXPRESSIONPARSER_H_
#define EXPRESSIONPARSER_H_
#include <stdbool.h>
typedef enum
{
	HIGH,
	LOW,
	EQ,
	ERROR,
	NONE
}ePrecElem;
typedef struct
{
	token *firstToken;
	ePrecElem priority;
}tReductToken;
int parseExpression();
int tableIndexSelect(token tok);
bool areOperandOkey(token firstOperand,token secondOperand);
#endif /* EXPRESSIONPARSER_H_ */
