/*
 * ExpressionParser.h
 *
 *  Created on: 14. 11. 2017
 *      Author: adam
 */

#ifndef EXPRESSIONPARSER_H_
#define EXPRESSIONPARSER_H_

#include "stack.h"
#include <stdbool.h>
typedef enum
{
	HIGH,
	LOW,
	EQ,
	ERROR
}ePrecElem;
typedef struct
{
	token firstToken;
	token secondToken;
	unsigned operation;
}tReductToken;
int parseExpression();
int tableIndexSelect(token tok);
//bool isIdentifier(token tok);
#endif /* EXPRESSIONPARSER_H_ */
