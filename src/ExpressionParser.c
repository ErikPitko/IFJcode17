/*
 * ExpressionParser.c
 *
 *  Created on: 14. 11. 2017
 *      Author: adam
 */

#include "ExpressionParser.h"
#include "scanner.h"
ePrecElem precTable[] =
{		//+   	-    	*   	/   	\  		=     	<    	>   	<=   	>=   	<>   	(    	)   	EOL   	VAR
/*"+"*/	HIGH,	HIGH,	LOW,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW,
/*"-"*/	HIGH,	HIGH,	LOW,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW,
/*"*"*/	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW,
/*"/"*/	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW,
/*"\"*/	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW,
/*"="*/	LOW,	LOW,	LOW,	LOW,	LOW,	HIGH,	LOW,	LOW,	LOW,	LOW,	HIGH,	LOW,	HIGH,	HIGH,	LOW,
/*"<"*/	LOW,	LOW,	LOW,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW,
/*">"*/	LOW,	LOW,	LOW,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW,
/*"<="*/LOW,	LOW,	LOW,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW,
/*">="*/LOW,	LOW,	LOW,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW,
/*"<>"*/LOW,	LOW,	LOW,	LOW,	LOW,	HIGH,	LOW,	LOW,	LOW,	LOW,	HIGH,	LOW,	HIGH,	HIGH,	LOW,
/*"("*/	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	EQ,		ERROR,	LOW,
/*")"*/	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	ERROR,	HIGH,	HIGH,	ERROR,
/*"EOL"*/LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	ERROR,	ERROR,	LOW,
/*"VAR"*/HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	ERROR,	HIGH,	HIGH,	ERROR
};

int parseExpression()
{
	tStack *stack;
	stackInit(stack);
	token t;
	t->info= NULL;
	t->type= EOL;
	stackPush(stack,t);
	ePrecElem elem = HIGH;
	token actToken = NULL;
	token topToken = NULL;
	do
	{
		actToken = stackTop(stack);
		stackPop(stack);
		switch (elem)
		{
			case HIGH:

				break;
			case LOW:

				break;
			case EQ:

				break;
			case ERROR:

				break;
		}
	}
	while(actToken->type != EOL);
}
