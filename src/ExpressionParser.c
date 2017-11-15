/*
 * ExpressionParser.c
 *
 *  Created on: 14. 11. 2017
 *      Author: adam
 */

#include "ExpressionParser.h"
#include "scanner.h"
#include "InstructionList.h"

ePrecElem precTable[15][15] =
{		//+   	-    	*   	/   	\  		=     	<    	>   	<=   	>=   	<>   	(    	)   	EOL   	VAR
/*"+"*/	{HIGH,	HIGH,	LOW,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW},
/*"-"*/	{HIGH,	HIGH,	LOW,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW},
/*"*"*/	{HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW},
/*"/"*/	{HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW},
/*"\"*/	{HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW},
/*"="*/	{LOW,	LOW,	LOW,	LOW,	LOW,	HIGH,	LOW,	LOW,	LOW,	LOW,	HIGH,	LOW,	HIGH,	HIGH,	LOW},
/*"<"*/	{LOW,	LOW,	LOW,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW},
/*">"*/	{LOW,	LOW,	LOW,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW},
/*"<="*/{LOW,	LOW,	LOW,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW},
/*">="*/{LOW,	LOW,	LOW,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW,	HIGH,	HIGH,	LOW},
/*"<>"*/{LOW,	LOW,	LOW,	LOW,	LOW,	HIGH,	LOW,	LOW,	LOW,	LOW,	HIGH,	LOW,	HIGH,	HIGH,	LOW},
/*"("*/	{LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	EQ,		ERROR,	LOW},
/*")"*/	{HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	ERROR,	HIGH,	HIGH,	ERROR},
/*"EOL"*/{LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	ERROR,	ERROR,	LOW},
/*"VAR"*/{HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	ERROR,	HIGH,	HIGH,	ERROR}
};



/*bool isIdentifier(token tok)
{
	switch (tok->type)
	{
	case STRING: return true;
	case INTEGER: return true;
	case DOUBLE: return true;
	case BOOLEAN: return true;
	}
	return false;
}*/

/*
-----------double a = 5-----------
1.token =>   .type == double; .info == NULL
2.token => .type == identifier; .info == "a"
3.token => .type == equal ;  .info == NULL
4.token => .type == VALUE_INTEGER; .info =="5"
 */

int tableIndexSelect(token tok)
{
	if(tok->type == IDENTIFIER)
	{
		return 15;
	}
	unsigned arr[15] = {PLUS,MINUS,ASTERIX,DIV_INT,DIV_DOUBLE,EQUAL,LESS,GREATER,LESS_EQUAL,
				INEQUALITY,LEFT_PARENTHESIS,RIGHT_PARENTHESIS,EOL};
	for(int i = 0;i<15;i++)
		if(arr[i]== tok->type)
			return i;
	return -1;
}

int parseExpression()
{
	tStack *stack;
	stackInit(stack);
	token t;
	t->info= NULL;
	t->type= EOL;
	stackPush(stack,t);
	ePrecElem elem;
	token actToken = getToken();
	do
	{
		token top = stackTop(stack);
		switch (precTable[tableIndexSelect(stackTop(stack))][tableIndexSelect(actToken)])
		{
			case HIGH:
				//if <y je na vrcholu zásobníku and r: A → y ∈ P
				//then zaměň <y za A & vypiš r na výstup
				//else chyba
				break;
			case LOW:
				//zaměň a za a< na zásobníku
				stackPush(stack,actToken);
				actToken = getToken();
				break;
			case EQ:
				stackPush(stack,actToken);
				actToken = getToken();
				break;
			case ERROR:
				goto exit;
				break;
			default:
				stackClear(stack);
				return 0;
		}
	}
	while(actToken->type != EOL && stackTop(stack)->type != EOL);
	exit:
	return 0;
}

/*token *applyRule(tStack *stack, token actToken,token topToken)
{
	token *tmp = NULL;
	if(topToken->type == PLUS && actToken->type == IDENTIFIER)
	{
		stackPop(stack);
		tmp->info =
	}
	if()
}*/

