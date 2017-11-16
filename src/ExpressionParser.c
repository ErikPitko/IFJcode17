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
	t.firstToken->info= NULL;
	t.firstToken->type= EOL;
	stackPush(stack,t);
	bool reduct = false;
	tReductToken actToken;
	tReductToken top;
	tReductToken priority;
	actToken.firstToken = getToken();
	do
	{
		int select;
		if(reduct == false)
			select = tableIndexSelect(stackTop(stack));
		else select = tableIndexSelect(stackBeforeTop(stack));
		switch (precTable[select][tableIndexSelect(&actToken)])
		{
			case H:
				for(top = *(stackTopPop(stack));top.firstToken != NULL;top = *(stackTopPop(stack)))
				{
					stackPush(rStack,top);
					if(reduct == false && stackTop(stack)->priority == L)
					{
						stackPop(stack);
						break;
					}
					else if(reduct != false && stackBeforeTop(stack)->priority == L)
					{
						stackPop(stack);
						break;
					}
				}
				applyRule(stack,rStack,&reduct);
				if(top.firstToken == NULL)
					goto exit;
				//if <y je na vrcholu zásobníku and r: A → y ∈ P
				//then zaměň <y za A & vypiš r na výstup
				//else chyba
				break;
			case L:
				priority.firstToken = NULL;
				priority.priority = L;
				if(reduct == false)
					stackPush(stack,priority);	//push a<
				else stackPushUnderTop(stack,priority);
				reduct = false;
				stackPush(stack,actToken);	//push b
				actToken.firstToken = getToken();
				break;
			case EQ:
				stackPush(stack,actToken);
				actToken.firstToken = getToken();
				break;
			case ERROR:
				goto exit;
				break;
			default:
				//stackClear(stack);
				return 0;
		}
	}
	while(actToken.firstToken->type != EOL && stackTop(stack)->firstToken->type != EOL);
	exit:
	return 0;
}

bool canIDiv(token *firstOperand,token *secondOperand,bool isDouble, bool *cast)
{
	if((firstOperand->type == VALUE_INTEGER || firstOperand->type == VALUE_DOUBLE)&&(secondOperand->type == VALUE_INTEGER||secondOperand->type == VALUE_DOUBLE))
	{

		if(firstOperand->type == secondOperand->type)
		{
			if(isDouble)
			{
				if(firstOperand->type == VALUE_DOUBLE)
					(*cast)= false;
				else (*cast) = true;
				return true;
			}
			else
			{
				if(firstOperand->type == VALUE_INTEGER)
					(*cast)= false;
				else (*cast) = true;
				return true;
			}
		}
		if(firstOperand->type != secondOperand->type)
			(*cast) = true;
		return true;
	}
	return false;
}

bool areOperandsSame(token *firstOperand,token *secondOperand)
{
	if(firstOperand->type == VALUE_INTEGER || firstOperand->type == VALUE_DOUBLE || firstOperand->type == VALUE_STRING)
		return firstOperand->type == secondOperand->type;
	return false;
}

bool isOperand(token *operand, bool isArithmethic)
{
	//s identifierem to nebude fungovat musí se to ořešit protože identifier musí dávat více informací ohledně typu
	if(/*operand->type == IDENTIFIER||*/operand->type == VALUE_INTEGER||operand->type == VALUE_DOUBLE || (operand->type == VALUE_STRING && !isArithmethic))
		return true;
	return false;
}

bool areOperands(token *firstOperand,token *secondOperand, bool isArithmethic)
{
	//s identifierem to nebude fungovat musí se to ořešit protože identifier musí dávat více informací ohledně typu
	/*operand->type == IDENTIFIER||*/
	if((firstOperand->type == VALUE_INTEGER||firstOperand->type == VALUE_DOUBLE || (firstOperand->type == VALUE_STRING && !isArithmethic))
			&&(secondOperand->type == VALUE_INTEGER||secondOperand->type == VALUE_DOUBLE || (secondOperand->type == VALUE_STRING && !isArithmethic)))
		return true;
	return false;
}

void applyRule(tStack *stack,tStack *rStack,bool *reduct)
{
	//E -> i
	if(stackSize(rStack)==1)
	{
		stackPush(stack,*(stackTopPop(rStack)));
		(*reduct) = true;
	}
	else if(stackSize(rStack)>1)
	{
		if(/*stackTop(rStack)->firstToken->type == IDENTIFIER*/stackTop(rStack)->firstToken->type == VALUE_INTEGER || stackTop(rStack)->firstToken->type == VALUE_DOUBLE || stackTop(rStack)->firstToken->type == VALUE_STRING)
		{
			tReductToken fID = *(stackTopPop(rStack));
			switch (stackTop(rStack)->firstToken->type)
			{
			//E -> E+E
			case PLUS:
				stackPop(rStack);
				//tReductToken *fOper = stackTopPop(rStack);
				if(isOperand(stackTop(rStack)->firstToken,true))
				{
					if(areOperands(stackTop(rStack)->firstToken,fID.firstToken,true) == false)
					{
						// ******************CHYBAAAAAA****************
					}
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_ADD,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					stackPop(rStack);
					stackPush(stack,(*result));
					(*reduct) = true;
				}
				break;
			//E -> E-E
			case MINUS:
				stackPop(rStack);
				//tReductToken *fOper = stackTopPop(rStack);
				if(isOperand(stackTop(rStack)->firstToken,true))
				{
					if(areOperands(stackTop(rStack)->firstToken,fID.firstToken,true)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_SUB,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					stackPop(rStack);
					stackPush(stack,(*result));
					(*reduct) = true;
				}
				break;
			//E -> E*E
			case ASTERIX:
				stackPop(rStack);
				//tReductToken *fOper = stackTopPop(rStack);
				if(isOperand(stackTop(rStack)->firstToken,true))
				{
					if(areOperands(stackTop(rStack)->firstToken,fID.firstToken,true)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_MUL,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					stackPop(rStack);
					stackPush(stack,(*result));
					(*reduct) = true;
				}
				break;
			//E -> E\E
			case DIV_INT:
				stackPop(rStack);
				//tReductToken *fOper = stackTopPop(rStack);
				if(isOperand(stackTop(rStack)->firstToken,true))
				{
					bool cast;
					if(canIDiv(stackTop(rStack)->firstToken,fID.firstToken,false,&cast)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					if(*(int*)(TokenToTypeConversion(stackTop(rStack)->firstToken))==0)
						return; // dělení nulou
					tReductToken *result = myMalloc(sizeof(tReductToken));
					if(cast)
					{

						if(stackTop(rStack)->firstToken->type != VALUE_INTEGER)
						{
							LInsert(I_FLOAT2INT,(void*)result->firstToken,TokenToTypeConversion(stackTopPop(rStack)->firstToken),NULL);
							stackPush(rStack,(*result));
						}
						if(fID.firstToken->type != VALUE_INTEGER)
						{
							LInsert(I_FLOAT2INT,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),NULL);
							fID = *(result);
						}
					}
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_DIV,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					stackPop(rStack);
					stackPush(stack,(*result));
					(*reduct) = true;
				}
				break;
			//E -> E/E
			case DIV_DOUBLE:
				stackPop(rStack);
				//tReductToken *fOper = stackTopPop(rStack);
				if(isOperand(stackTop(rStack)->firstToken,true))
				{
					bool cast;
					if(canIDiv(stackTop(rStack)->firstToken,fID.firstToken,true,&cast)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					if(*(double*)(TokenToTypeConversion(stackTop(rStack)->firstToken))==0)
						return; // dělení nulou
					tReductToken *result = myMalloc(sizeof(tReductToken));
					if(cast)
					{

						if(stackTop(rStack)->firstToken->type != VALUE_DOUBLE)
						{
							LInsert(I_INT2FLOAT,(void*)result->firstToken,TokenToTypeConversion(stackTopPop(rStack)->firstToken),NULL);
							stackPush(rStack,(*result));
						}
						if(fID.firstToken->type != VALUE_DOUBLE)
						{
							LInsert(I_INT2FLOAT,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),NULL);
							fID = *(result);
						}
					}
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_DIV,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					stackPop(rStack);
					stackPush(stack,(*result));
					(*reduct) = true;
				}
				break;
			case LESS:
				stackPop(rStack);
				//tReductToken *fOper = stackTopPop(rStack);
				if(isOperand(stackTop(rStack)->firstToken,true))
				{
					if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_LT,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					stackPop(rStack);
					stackPush(stack,(*result));
					(*reduct) = true;
				}
				break;
			case GREATER:
				stackPop(rStack);
				//tReductToken *fOper = stackTopPop(rStack);
				if(isOperand(stackTop(rStack)->firstToken,true))
				{
					if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_GT,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					stackPop(rStack);
					stackPush(stack,(*result));
					(*reduct) = true;
				}
				break;
			case EQUAL:
				stackPop(rStack);
				//tReductToken *fOper = stackTopPop(rStack);
				if(isOperand(stackTop(rStack)->firstToken,true))
				{
					if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_EQ,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					stackPop(rStack);
					stackPush(stack,(*result));
					(*reduct) = true;
				}
				break;
			case GREATER_EQUAL:
				stackPop(rStack);
				//tReductToken *fOper = stackTopPop(rStack);
				if(isOperand(stackTop(rStack)->firstToken,true))
				{
					if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(I_LT,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					LInsert(I_NOT,(void*)result->firstToken,(void*)result->firstToken,NULL);
					stackPop(rStack);
					stackPush(stack,(*result));
					(*reduct) = true;
				}
				break;
			case LESS_EQUAL:
			stackPop(rStack);
			//tReductToken *fOper = stackTopPop(rStack);
			if(isOperand(stackTop(rStack)->firstToken,true))
			{
				if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
				{
					// ******************CHYBAAAAAA****************
				}
				tReductToken *result = myMalloc(sizeof(tReductToken));
				//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
				LInsert(I_GT,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
				LInsert(I_NOT,(void*)result->firstToken,(void*)result->firstToken,NULL);
				stackPop(rStack);
				stackPush(stack,(*result));
				(*reduct) = true;
			}
				break;
			}
		}
	}
	return;
}

