/*
 * ExpressionParser.h
 *
 *  Created on: 14. 11. 2017
 *      Author: adam
 */

#ifndef EXPRESSIONPARSER_H_
#define EXPRESSIONPARSER_H_

#include "stack.h"
typedef enum
{
	HIGH,
	LOW,
	EQ,
	ERROR
}ePrecElem;

int parseExpression();

#endif /* EXPRESSIONPARSER_H_ */
