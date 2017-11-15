/*
 * ExpressionParser.c
 *
 *  Created on: 14. 11. 2017
 *      Author: adam
 */

#include "ExpressionParser.h"

ePrecElem precTable[15][15] =
{		//+   	-    	*   	/   	\  		=     	<    	>   	<=   	>=   	<>   	(    	)   	EOL   	VAR
/*"+"*/	{H,		H,		L,		L,		L,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*"-"*/	{H,		H,		L,		L,		L,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*"*"*/	{H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*"/"*/	{H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*"\"*/	{H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*"="*/	{L,		L,		L,		L,		L,		H,		L,		L,		L,		L,		H,		L,		H,		H,		L},
/*"<"*/	{L,		L,		L,		L,		L,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*">"*/	{L,		L,		L,		L,		L,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*"<="*/{L,		L,		L,		L,		L,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*">="*/{L,		L,		L,		L,		L,		H,		H,		H,		H,		H,		H,		L,		H,		H,		L},
/*"<>"*/{L,		L,		L,		L,		L,		H,		L,		L,		L,		L,		H,		L,		H,		H,		L},
/*"("*/	{L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		EQ,		ERROR,	L},
/*")"*/	{H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		ERROR,	H,		H,		ERROR},
/*"EOL"*/{L,	L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		ERROR,	ERROR,	L},
/*"VAR"*/{H,	H,		H,		H,		H,		H,		H,		H,		H,		H,		H,		ERROR,	H,		H,		ERROR}
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
int tableIndexSelect(tReductToken *tok)
{
	if(tok->firstToken->type == IDENTIFIER)
	{
		return 15;
	}
	unsigned arr[15] = {PLUS,MINUS,ASTERIX,DIV_INT,DIV_DOUBLE,EQUAL,LESS,GREATER,LESS_EQUAL,
				INEQUALITY,LEFT_PARENTHESIS,RIGHT_PARENTHESIS,EOL};
	for(int i = 0;i<15;i++)
		if(arr[i]== tok->firstToken->type)
			return i;
	return -1;
}

int parseExpression()
{
	tStack *stack;
	tStack *rStack;
	stackInit(rStack);
	stackInit(stack);
	tReductToken t;
	t->firstToken->info= NULL;
	t->firstToken->type= EOL;
	stackPush(stack,t);
	bool reduct = false;
	tReductToken actToken = getToken();
	do
	{
		int select;
		if((*reduct) == false)
			select = tableIndexSelect(stackTop(stack));
		else select = tableIndexSelect(stackBeforeTop(stack));
		switch (precTable[select][tableIndexSelect(actToken)])
		{
			case H:
				tReductToken *top = NULL;
				for(top = stackTopPop(stack);top->firstToken != NULL;top = stackTopPop(stack))
				{
					stackPush(rStack,top);
					if((*reduct) == false && stackTop(stack)->priority == L)
					{
						stackPop(stack);
						break;
					}
					else if((*reduct) != false && stackBeforeTop(stack)->priority == L)
					{
						stackPop(stack);
						break;
					}
				}
				applyRule(stack,rStack);
				if(top->firstToken == NULL)
					goto exit;
				//if <y je na vrcholu zásobníku and r: A → y ∈ P
				//then zaměň <y za A & vypiš r na výstup
				//else chyba
				break;
			case L:
				tReductToken priority;
				priority->firstToken = NULL;
				priority->priority = L;
				if((*reduct) == false)
					stackPush(stack,priority);	//push a<
				else stackPushUnderTop(stack,priority);
				(*reduct) = false;
				stackPush(stack,actToken);	//push b
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
	while(actToken->firstToken->type != EOL && stackTop(stack)->firstToken->type != EOL);
	exit:
	return 0;
}

bool areOperandOkey(token *firstOperand,token *secondOperand)
{
	if((firstOperand->type == VALUE_INTEGER || firstOperand ->type == VALUE_DOUBLE) && (secondOperand->type == VALUE_INTEGER || secondOperand->type == VALUE_DOUBLE))
		return true;
	return false;
}

void applyRule(tStack *stack,tStack *rStack,int *reduct)
{
	//E -> i
	if(stackSize(rStack)==1)
	{
		stackPush(stack,stackTopPop(rStack));
		(*reduct) = true;
	}
	else if(stackSize(rStack)>1)
	{
		if(stackTop(rStack)->firstToken->type == IDENTIFIER)
		{
			tReductToken *fID = stackTopPop(rStack);
			switch (stackTop(rStack)->firstToken->type)
			{
			//E -> E+E
			case PLUS:
				tReductToken *fOper = stackTopPop(rStack);
				if(stackTop(rStack)->firstToken->type == IDENTIFIER)
				{
					if(areOperandOkay(stackTop(rStack)->firstToken,fID->firstToken)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_ADD,result->firstToken,TokenToTypeConversion(fID->firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					stackPop(rStack);
					stackPush(stack,result);
					(*reduct) = true;
				}
				break;
			//E -> E-E
			case MINUS:
				tReductToken *fOper = stackTopPop(rStack);
				if(stackTop(rStack)->firstToken->type == IDENTIFIER)
				{
					if(areOperandOkay(stackTop(rStack)->firstToken,fID->firstToken)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_SUB,result->firstToken,TokenToTypeConversion(fID->firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					stackPop(rStack);
					stackPush(stack,result);
					(*reduct) = true;
				}
				break;
			//E -> E*E
			case ASTERIX:
				tReductToken *fOper = stackTopPop(rStack);
				if(stackTop(rStack)->firstToken->type == IDENTIFIER)
				{
					if(areOperandOkay(stackTop(rStack)->firstToken,fID->firstToken)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_MUL,result->firstToken,TokenToTypeConversion(fID->firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					stackPop(rStack);
					stackPush(stack,result);
					(*reduct) = true;
				}
				break;
			//E -> E\E
			case DIV_INT:
				tReductToken *fOper = stackTopPop(rStack);
				if(stackTop(rStack)->firstToken->type == IDENTIFIER)
				{
					if(areOperandOkay(stackTop(rStack)->firstToken,fID->firstToken)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					if((int)(*TokenToTypeConversion(stackTop(rStack)->firstToken))==0)
						return; // dělení nulou
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_DIV,result->firstToken,TokenToTypeConversion(fID->firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					stackPop(rStack);
					stackPush(stack,result);
					(*reduct) = true;
				}
				break;
			//E -> E/E
			case DIV_DOUBLE:
				tReductToken *fOper = stackTopPop(rStack);
				if(stackTop(rStack)->firstToken->type == IDENTIFIER)
				{
					if(areOperandOkay(stackTop(rStack)->firstToken,fID->firstToken)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					if((double)(*TokenToTypeConversion(stackTop(rStack)->firstToken))==0)
						return; // dělení nulou
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_DIV,result->firstToken,TokenToTypeConversion(fID->firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					stackPop(rStack);
					stackPush(stack,result);
					(*reduct) = true;
				}
				break;
			/*case EQUAL:
				tReductToken *fOper = stackTopPop(rStack);
				if(stackTop(rStack)->firstToken->type == IDENTIFIER)
				{
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_DIV,TokenToTypeConversion(fID->firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken),result->firstToken);
					stackPop(rStack);
					stackPush(stack,result);
				}
				break;
			case LESS:
				tReductToken *fOper = stackTopPop(rStack);
				if(stackTop(rStack)->firstToken->type == IDENTIFIER)
				{
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_DIV,TokenToTypeConversion(fID->firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken),result->firstToken);
					stackPop(rStack);
					stackPush(stack,result);
				}
				break;
			case GREATER:
				tReductToken *fOper = stackTopPop(rStack);
				if(stackTop(rStack)->firstToken->type == IDENTIFIER)
				{
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_DIV,TokenToTypeConversion(fID->firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken),result->firstToken);
					stackPop(rStack);
					stackPush(stack,result);
				}
				break;
			case LESS_EQUAL:
				tReductToken *fOper = stackTopPop(rStack);
				if(stackTop(rStack)->firstToken->type == IDENTIFIER)
				{
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_DIV,TokenToTypeConversion(fID->firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken),result->firstToken);
					stackPop(rStack);
					stackPush(stack,result);
				}
				break;
			case INEQUALITY:
				tReductToken *fOper = stackTopPop(rStack);
				if(stackTop(rStack)->firstToken->type == IDENTIFIER)
				{
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_DIV,TokenToTypeConversion(fID->firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken),result->firstToken);
					stackPop(rStack);
					stackPush(stack,result);
				}
				break;*/
			}
		}
	}
	return;
}

