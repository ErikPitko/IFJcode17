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

char* strValueOfEnum(int enumValue)
{
	switch(enumValue)
	{
	case 51: return "IDENTIFIER";
	case 999: return"EXPR";
	case 47: return "VALUE_INTEGER";
	case 48: return "VALUE_DOUBLE";
	case 49: return "VALUE_STRING";
	case 42: return "PLUS";
	case 43: return "MINUS";
	case 44: return "ASTERIX";
	case 45: return "DIV_INT";
	case 46: return "DIV_DOUBLE";
	case 39: return "EQUAL";
	case 36: return "LESS";
	case 40: return "GREATER";
	case 41: return "GREATER_EQUAL";
	case 38: return "LESS_EQUAL";
	case 37: return "INEQUALITY";
	case 52: return "LEFT_PARENTHESIS";
	case 53: return "RIGHT_PARENTHESIS";
	case 50: return "EOL";
	}
	return "*Unknown*";
}

int tableIndexSelect(tReductToken *tok)
{
	if(tok->firstToken->type == IDENTIFIER ||tok->firstToken->type == EXPR|| tok->firstToken->type == VALUE_INTEGER || tok->firstToken->type == VALUE_DOUBLE || tok->firstToken->type == VALUE_STRING)
	{
		return 14;
	}
	unsigned arr[15] = {PLUS,MINUS,ASTERIX,DIV_INT,DIV_DOUBLE,EQUAL,LESS,GREATER,GREATER_EQUAL,LESS_EQUAL,
				INEQUALITY,LEFT_PARENTHESIS,RIGHT_PARENTHESIS,EOL};
	for(int i = 0;i<15;i++)
		if(arr[i]== tok->firstToken->type)
			return i;
	return EOL;
}

int parseExpression(token *retValue)
{
	tStack stack;
	tStack rStack;
	stackInit(&rStack);
	stackInit(&stack);
	tReductToken t;
	t.firstToken = (token*)myMalloc(sizeof(token));
	t.firstToken->info= "EOL";
	t.firstToken->type= EOL;
	stackPush(&stack,t);
	bool reduct = false;
	tReductToken actToken;
	tReductToken top;
	tReductToken priority;
	priority.firstToken = (token*)myMalloc(sizeof(token));
	actToken.firstToken = getToken();
	do
		{
		int select;
		if(reduct == false)
			select = tableIndexSelect(stackTop(&stack));
		else select = tableIndexSelect(stackBeforeTop(&stack));
		switch (precTable[select][tableIndexSelect(&actToken)])
		{
			case H:
				if(stackTop(&stack)->firstToken->type == EXPR && stackBeforeTop(&stack)->firstToken->type == EOL)
					goto exit;
				do
				{
					top = *(stackTopPop(&stack));
					if(top.priority == L || top.firstToken->type == EOL)
						break;
					stackPush(&rStack,top);
				}
				while(1);
				stackPrint("rstack",&rStack);
				printf("######Apply rule######\n");
				applyRule(&stack,&rStack,&reduct);
				printf("#####Rule apllied#####\n");
				stackPrint("stack",&stack);
				//if(top.firstToken == NULL)
				//	goto exit;
				break;
			case L:
				priority.priority = L;
				priority.firstToken->info = "<";
				if(reduct == false)
					stackPush(&stack,priority);	//push E<
				else stackPushUnderTop(&stack,priority);	//push <E
				reduct = false;
				stackPush(&stack,actToken);	//push b
				actToken.firstToken = getToken();
				break;
			case EQ:
				printf("!!EQUAL!!\n");
				stackPush(&stack,actToken);
				actToken.firstToken = getToken();
				break;
			case ERROR:
				printf("%i  %i\n",select,tableIndexSelect(&actToken));
				warning_msg("Spatna kombinace tokenu");
				goto exit;
			default:
				printf("%i  %i\n",select,tableIndexSelect(&actToken));
				warning_msg("Syntaktická chyba");
				goto exit;
		}
	}
	//!(!reduct &&(actToken.firstToken->type == EOL && stackTop(&stack)->firstToken->type == EOL)) || !(reduct &&(actToken.firstToken->type == EOL && stackBeforeTop(&stack)->firstToken->type == EOL))
	while(!((actToken.firstToken->type == EOL) && stackBeforeTop(&stack)->firstToken->type == EOL));
	exit:
	if (actToken.firstToken!= NULL)
	{
		retValue = actToken.firstToken;
	}
	printf("****RETURNS: %i ****",actToken.firstToken->type);
	//PrintInstrList(&instList);
	return 0;
}

bool canIDiv(token *firstOperand,token *secondOperand,bool isDouble, bool *cast)
{
	if(isOperand(firstOperand,true) && isOperand(secondOperand,true))
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
	if(areOperandsSameArithmethic(firstOperand,secondOperand) || firstOperand->type == VALUE_STRING)
		return firstOperand->type == secondOperand->type;
	return false;
}

bool areOperandsSameArithmethic(token *firstOperand,token *secondOperand)
{
	if(firstOperand->type == VALUE_INTEGER || firstOperand->type == VALUE_DOUBLE)
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
	if(isOperand(firstOperand,isArithmethic) && isOperand(secondOperand,isArithmethic))
		return true;
	return false;
}

void applyRule(tStack *st,tStack *rStack,bool *reduct)
{
	//E -> i
	if(stackLenght(rStack)==1)
	{
		printf("****E -> i****\n");
		tReductToken *tmp = stackTopPop(rStack);
		tmp->firstToken->type = EXPR;
		tmp->firstToken->info = "EXPR";
		stackPush(st,*(tmp));
		(*reduct) = true;
	}
	else if(stackLenght(rStack)>1)
	{
		//if(isOperand(stackTop(rStack)->firstToken,false))
		{
			//tReductToken fID = *(stackTopPop(rStack));

			stackPop(rStack);	//MUSÍ BÝT JINAK NEBUDE FUNGOVAT BACHA NA HORNÍ ŘÁDEK POKUD BUDEME PRACOVAT JAKO TOP POP MUSÍME VYMAZAT
			//E -> E+E
			if(stackTop(rStack)->firstToken->type == PLUS)
			{
				printf("****E -> E+E****\n");
				stackPop(rStack);
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				//tReductToken *fOper = stackTopPop(rStack);
				//if(isOperand(stackTop(rStack)->firstToken,true))
				//{
				/*if(areOperandsSameArithmethic(stackTop(rStack)->firstToken,fID.firstToken) == false)
				{
					if(stackTop(rStack)->firstToken->type == VALUE_DOUBLE && fID.firstToken->type == VALUE_INTEGER)
					{
						LInsert(instList,I_INT2FLOAT,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),NULL);
						fID = (*result);
					}
					else if(stackTop(rStack)->firstToken->type == VALUE_INTEGER && fID.firstToken->type == VALUE_DOUBLE)
					{
						LInsert(instList,I_INT2FLOAT,(void*)result->firstToken,TokenToTypeConversion(stackTopPop(rStack)->firstToken),NULL);
						stackPush(rStack,(*result));
					}
				}*/
				//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
				//LInsert(instList,I_ADD,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
				stackPop(rStack);
				stackPush(st,(*result));
				(*reduct) = true;
				//}
			}
			//E -> E-E
			else if(stackTop(rStack)->firstToken->type== MINUS)
			{
				printf("****E -> E-E****\n");
				stackPop(rStack);
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				//tReductToken *fOper = stackTopPop(rStack);
				//if(isOperand(stackTop(rStack)->firstToken,true))
				//{

				/*if(areOperandsSameArithmethic(stackTop(rStack)->firstToken,fID.firstToken)== false)
				{
					if(stackTop(rStack)->firstToken->type == VALUE_DOUBLE && fID.firstToken->type == VALUE_INTEGER)
					{
						LInsert(instList,I_INT2FLOAT,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),NULL);
						fID = (*result);
					}
					else if(stackTop(rStack)->firstToken->type == VALUE_INTEGER && fID.firstToken->type == VALUE_DOUBLE)
					{
						LInsert(instList,I_INT2FLOAT,(void*)result->firstToken,TokenToTypeConversion(stackTopPop(rStack)->firstToken),NULL);
						stackPush(rStack,(*result));
					}
				}*/

				//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
				//LInsert(instList,I_SUB,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
				stackPop(rStack);
				stackPush(st,(*result));
				(*reduct) = true;
				//}
			}
			//E -> E*E
			else if(stackTop(rStack)->firstToken->type== ASTERIX)
			{
				printf("****E -> E*E****\n");
				stackPop(rStack);
				tReductToken *result =(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				/*//tReductToken *fOper = stackTopPop(rStack);
				//if(isOperand(stackTop(rStack)->firstToken,true))
				//{
				//tReductToken *result = myMalloc(sizeof(tReductToken));
				if(areOperandsSameArithmethic(stackTop(rStack)->firstToken,fID.firstToken)== false)
				{
					if(stackTop(rStack)->firstToken->type == VALUE_DOUBLE && fID.firstToken->type == VALUE_INTEGER)
					{
						LInsert(instList,I_INT2FLOAT,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),NULL);
						fID = (*result);
					}
					else if(stackTop(rStack)->firstToken->type == VALUE_INTEGER && fID.firstToken->type == VALUE_DOUBLE)
					{
						LInsert(instList,I_INT2FLOAT,(void*)result->firstToken,TokenToTypeConversion(stackTopPop(rStack)->firstToken),NULL);
						stackPush(rStack,(*result));
					}
				}
				//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
				//LInsert(instList,I_MUL,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));*/
				stackPop(rStack);
				stackPush(st,(*result));
				(*reduct) = true;
				//}
			}
			//E -> E\E
			else if(stackTop(rStack)->firstToken->type==  DIV_INT)
			{
				stackPop(rStack);
				printf("****E -> E\E****\n");
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				/*tReductToken *fOper = stackTopPop(rStack);
				if(isOperand(stackTop(rStack)->firstToken,true))
				{
					bool cast;
					if(canIDiv(stackTop(rStack)->firstToken,fID.firstToken,false,&cast)== false)
					{
						error_msg("Spatne operandy u deleni.", 53);
					//}
					if(*(int*)(TokenToTypeConversion(stackTop(rStack)->firstToken))==0)
						error_msg("Deleni nulou", 57);
					tReductToken *result = myMalloc(sizeof(tReductToken));
					if(cast)
					{

						if(stackTop(rStack)->firstToken->type != VALUE_INTEGER)
						{
							LInsert(instList,I_FLOAT2INT,(void*)result->firstToken,TokenToTypeConversion(stackTopPop(rStack)->firstToken),NULL);
							stackPush(rStack,(*result));
						}
						if(fID.firstToken->type != VALUE_INTEGER)
						{
							LInsert(instList,I_FLOAT2INT,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),NULL);
							fID = *(result);
						}
					}
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					LInsert(instList,I_DIV,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));*/
					stackPop(rStack);
					stackPush(st,(*result));
					(*reduct) = true;
				//}
			}
			//E -> E/E
			else if(stackTop(rStack)->firstToken->type== DIV_DOUBLE)
			{
				stackPop(rStack);
				printf("****E -> E/E****\n");
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				//tReductToken *fOper = stackTopPop(rStack);
				/*if(isOperand(stackTop(rStack)->firstToken,true))
				{
					bool cast;
					if(canIDiv(stackTop(rStack)->firstToken,fID.firstToken,true,&cast)== false)
					{
						error_msg("Spatne operandy u deleni.", 53);
					}
					if(*(double*)(TokenToTypeConversion(stackTop(rStack)->firstToken))==0)
						error_msg("Deleni nulou.", 57);
					tReductToken *result = myMalloc(sizeof(tReductToken));
					if(cast)
					{

						if(stackTop(rStack)->firstToken->type != VALUE_DOUBLE)
						{
							//LInsert(instList,I_INT2FLOAT,(void*)result->firstToken,TokenToTypeConversion(stackTopPop(rStack)->firstToken),NULL);
							stackPush(rStack,(*result));
						}
						if(fID.firstToken->type != VALUE_DOUBLE)
						{
							//LInsert(instList,I_INT2FLOAT,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),NULL);
							fID = *(result);
						}
					}
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					//LInsert(instList,I_DIV,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));*/
					stackPop(rStack);
					stackPush(st,(*result));
					(*reduct) = true;
			//}
			}
			else if(stackTop(rStack)->firstToken->type== LESS)
			{
				stackPop(rStack);
				printf("****E -> E<E****\n");
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				//tReductToken *fOper = stackTopPop(rStack);
				/*if(isOperand(stackTop(rStack)->firstToken,true))
				{
					if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					//LInsert(instList,I_LT,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));*/
					stackPop(rStack);
					stackPush(st,(*result));
					(*reduct) = true;
				//}
			}
			else if(stackTop(rStack)->firstToken->type== GREATER)
			{
				stackPop(rStack);
				printf("****E -> E>E****\n");
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				//tReductToken *fOper = stackTopPop(rStack);
				/*if(isOperand(stackTop(rStack)->firstToken,true))
				{
					if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					//LInsert(instList,I_GT,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));*/
					stackPop(rStack);
					stackPush(st,(*result));
					(*reduct) = true;
				//}
			}
			else if(stackTop(rStack)->firstToken->type== EQUAL)
			{
				stackPop(rStack);
				printf("****E -> E=E****\n");
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				//tReductToken *fOper = stackTopPop(rStack);
				/*if(isOperand(stackTop(rStack)->firstToken,true))
				{
					if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					//LInsert(instList,I_EQ,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));*/
					stackPop(rStack);
					stackPush(st,(*result));
					(*reduct) = true;
				//}
			}
			else if(stackTop(rStack)->firstToken->type== GREATER_EQUAL)
			{
				stackPop(rStack);
				printf("****E -> E>=E****\n");
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				//tReductToken *fOper = stackTopPop(rStack);
				/*if(isOperand(stackTop(rStack)->firstToken,true))
				{
					if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					//LInsert(instList,I_LT,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					//LInsert(instList,I_NOT,(void*)result->firstToken,(void*)result->firstToken,NULL);*/
					stackPop(rStack);
					stackPush(st,(*result));
					(*reduct) = true;
				//}
			}
			else if(stackTop(rStack)->firstToken->type==  LESS_EQUAL)
			{
				stackPop(rStack);
				printf("****E -> E<=E****\n");
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				//tReductToken *fOper = stackTopPop(rStack);
				/*if(isOperand(stackTop(rStack)->firstToken,true))
				{
					if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
					{
						// ******************CHYBAAAAAA****************
					}
					tReductToken *result = myMalloc(sizeof(tReductToken));
					//EINSTRUCTION instruction,void *arg1,void *arg2,void *result
					//LInsert(instList,I_GT,(void*)result->firstToken,TokenToTypeConversion(fID.firstToken),TokenToTypeConversion(stackTop(rStack)->firstToken));
					//LInsert(instList,I_NOT,(void*)result->firstToken,(void*)result->firstToken,NULL);*/
					stackPop(rStack);
					stackPush(st,(*result));
					(*reduct) = true;
			//}
			}
		}
	}
	return;
}

