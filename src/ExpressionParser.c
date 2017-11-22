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

/*
 * int b;
 * a = foo();
 * a = foo + b;
 * */
/**************************************HEADER***************************************/
int tableIndexSelect(tReductToken *tok);
bool isOperatorExpr(token *tok);
bool canIDiv(token *firstOperand,token *secondOperand,bool isDouble, bool *cast);
bool areOperandsSame(token *firstOperand,token *secondOperand);
bool areOperandsSameArithmethic(token *firstOperand,token *secondOperand);
bool isOperand(token *operand, bool isArithmethic);
bool areOperands(token *firstOperand,token *secondOperand, bool isArithmethic);
void applyRule(tStack *st,tStack *rStack,bool *reduct,param *returnVar,tHashTable *localTable);
char* strValueOfEnum(int enumValue);
/**************************************HEADER***************************************/

char* strValueOfEnum(int enumValue)
{
	switch(enumValue)
	{
	case 0: return "BLOCK_COMMENT";
	case 1: return "AS";
	case 2: return "ASC";
	case 3: return "DECLARE";
	case 4: return "DIM";
	case 5: return "DO";
	case 6: return "DOUBLE";
	case 7: return "ELSE";
	case 8: return "END";
	case 9: return "CHR";
	case 10: return "FUNCTION";
	case 11: return "IF";
	case 12: return "INPUT";
	case 13: return "INTEGER";
	case 14: return "LENGHT";
	case 15: return "LOOP";
	case 16: return "PRINT";
	case 17: return "RETURN0";
	case 18: return "SCOPE";
	case 19: return "STRING";
	case 20: return "SUBSTR";
	case 21: return "THEN";
	case 22: return "WHILE";
	case 23: return "AND";
	case 24: return "BOOLEAN";
	case 25: return "CONTINUE";
	case 26: return "ELSEIF";
	case 27: return "EXIT";
	case 28: return "FALSE";
	case 29: return "FOR";
	case 30: return "NEXT";
	case 31: return "NOT";
	case 32: return "OR";
	case 33: return "SHARED";
	case 34: return "STATIC";
	case 35: return "TRUE";
	case 36: return "LESS";
	case 37: return "INEQUALITY";
	case 38: return "LESS_EQUAL";
	case 39: return "EQUAL";
	case 40: return "GREATER";
	case 41: return "GREATER_EQUAL";
	case 42: return "PLUS";
	case 43: return "MINUS";
	case 44: return "ASTERIX";
	case 45: return "DIV_INT";
	case 46: return "DIV_DOUBLE";
	case 47: return "VALUE_INTEGER";
	case 48: return "VALUE_DOUBLE";
	case 49: return "VALUE_STRING";
	case 50: return "EOL";
	case 51: return "IDENTIFIER";
	case 52: return "LEFT_PARENTHESIS";
	case 53: return "RIGHT_PARENTHESIS";
	case 54: return "COMMA";
	case 55: return "SEMICOLON";
	case 999: return"EXPR";
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

//returnVar je a = b+8 (jaký datový typ je a)
//localTable lokální tabulka symbolů
//datové typy:
token *parseExpression(token *getSetToken,tFooListElem *returnVar,tHashTable *localTable)
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
	bool wasOperand = false;
	bool operatorRead = false;
	int semanticError = 0;
	priority.firstToken = (token*)myMalloc(sizeof(token));
	if(getSetToken == NULL)
	{
		printf("\n****Parser gives me: NULL****\n");
		actToken.firstToken = getToken();
	}
	else
	{
		printf("\n****Parser gives me: %s****\n",strValueOfEnum(getSetToken->type));
		actToken.firstToken = getSetToken;
	}
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
				applyRule(&stack,&rStack,&reduct,returnVar,localTable);
				printf("#####Rule apllied#####\n");
				stackPrint("stack",&stack);
				//if(top.firstToken == NULL)
				//	goto exit;
				break;
			case L:
				if(operatorRead &&!isOperand(actToken.firstToken,false) && actToken.firstToken->type != LEFT_PARENTHESIS)
				{
					printf("%s \n",strValueOfEnum(actToken.firstToken->type));
					error_msg(SYNTAX_ERR,"Two operators in row");
				}
				if(isOperatorExpr(actToken.firstToken))
				{
					printf("%s \n",strValueOfEnum(actToken.firstToken->type));
					operatorRead = true;
				}
				if(isOperand(actToken.firstToken,false))
				{
					wasOperand = true;
					operatorRead = false;
					printf("WAS THERE %s \n",actToken.firstToken->info);
				}
				if(!isOperand(actToken.firstToken,false) && !wasOperand && actToken.firstToken->type != LEFT_PARENTHESIS)
				{
					error_msg(SYNTAX_ERR,"The first token of expression wasn't operand or function call");
				}
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
				stackPush(&stack,actToken);
				actToken.firstToken = getToken();
				break;
			case ERROR:
				printf("%i  %i\n",select,tableIndexSelect(&actToken));
				error_msg(SYNTAX_ERR,"Syntaktická chyba");
				goto exit;
			default:
				printf("%i  %i\n",select,tableIndexSelect(&actToken));
				error_msg(SYNTAX_ERR,"Syntaktická chyba");
				goto exit;
		}
	}
	while(!((actToken.firstToken->type == EOL) && stackBeforeTop(&stack)->firstToken->type == EOL));
	exit:
	printf("****RETURNS: %s ****\n\n",strValueOfEnum(actToken.firstToken->type));
	if(semanticError != 0)
	{
		error_msg(semanticError,"Semantic error in expression");
	}
	//PrintInstrList(&instList);
	return actToken.firstToken;
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
	if(operand->type == IDENTIFIER||operand->type == VALUE_INTEGER||operand->type == VALUE_DOUBLE || (operand->type == VALUE_STRING && !isArithmethic))
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

bool isOperatorExpr(token *tok)
{
	if(tok->type >=36 && tok->type <=46)
		return true;
	return false;
}

void applyRule(tStack *st,tStack *rStack,bool *reduct,param *returnVar, tHashTable *localTable)
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
		{
			tReductToken fID = *(stackTopPop(rStack));	//pop a top stacku (je tam operátor)
			//stackPop(rStack);	//MUSÍ BÝT JINAK NEBUDE FUNGOVAT BACHA NA HORNÍ ŘÁDEK POKUD BUDEME PRACOVAT JAKO TOP POP MUSÍME VYMAZAT
			//E -> E+E
			if(stackTop(rStack)->firstToken->type == PLUS)
			{
				printf("****E -> E+E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				if(areOperandsSameArithmethic(stackTop(rStack)->firstToken,fID.firstToken) == false)
				{
					if(stackTop(rStack)->firstToken->type == VALUE_DOUBLE && fID.firstToken->type == VALUE_INTEGER)
					{
						//LInsert(&globalInstrList,I_INT2FLOAT,(void*)result->firstToken,(void*)fID.firstToken,NULL);
					}
					else if(stackTop(rStack)->firstToken->type == VALUE_INTEGER && fID.firstToken->type == VALUE_DOUBLE)
					{
						//LInsert(&globalInstrList,I_INT2FLOAT,(void*)result->firstToken->info,(void*)stackTop(rStack)->firstToken->info,NULL);
					}
				}
				//LInsert(&globalInstrList,I_ADD,(void*)result->firstToken,fID.firstToken,stackTop(rStack)->firstToken);
				PrintInstrList(&globalInstrList);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			//E -> E-E
			else if(stackTop(rStack)->firstToken->type== MINUS)
			{
				printf("****E -> E-E****\n");
				stackPop(rStack);	//popnutí MINUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				if(areOperandsSameArithmethic(stackTop(rStack)->firstToken,fID.firstToken) == false)
				{
					if(stackTop(rStack)->firstToken->type == VALUE_DOUBLE && fID.firstToken->type == VALUE_INTEGER)
					{
						//LInsert(&globalInstrList,I_INT2FLOAT,(void*)result->firstToken,(void*)fID.firstToken,NULL);
					}
					else if(stackTop(rStack)->firstToken->type == VALUE_INTEGER && fID.firstToken->type == VALUE_DOUBLE)
					{
						//LInsert(&globalInstrList,I_INT2FLOAT,(void*)result->firstToken,(void*)stackTop(rStack)->firstToken,NULL);
					}
				}
				//LInsert(&globalInstrList,I_SUB,(void*)result->firstToken,fID.firstToken,stackTop(rStack)->firstToken);
				PrintInstrList(&globalInstrList);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			//E -> E*E
			else if(stackTop(rStack)->firstToken->type== ASTERIX)
			{
				printf("****E -> E*E****\n");
				stackPop(rStack);	//popnutí NASOBENI
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				if(areOperandsSameArithmethic(stackTop(rStack)->firstToken,fID.firstToken) == false)
				{
					if(stackTop(rStack)->firstToken->type == VALUE_DOUBLE && fID.firstToken->type == VALUE_INTEGER)
					{
						//LInsert(&globalInstrList,I_INT2FLOAT,(void*)result->firstToken,(void*)fID.firstToken,NULL);
					}
					else if(stackTop(rStack)->firstToken->type == VALUE_INTEGER && fID.firstToken->type == VALUE_DOUBLE)
					{
						//LInsert(&globalInstrList,I_INT2FLOAT,(void*)result->firstToken,(void*)stackTop(rStack)->firstToken,NULL);
					}
				}
				//LInsert(&globalInstrList,I_MUL,(void*)result->firstToken,fID.firstToken,stackTop(rStack)->firstToken);
				PrintInstrList(&globalInstrList);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			//E -> E\E
			else if(stackTop(rStack)->firstToken->type==  DIV_INT)
			{
				printf("****E -> E\E****\n");
				stackPop(rStack);	//popnutí MINUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				if(areOperandsSameArithmethic(stackTop(rStack)->firstToken,fID.firstToken) == false)
				{
					if(stackTop(rStack)->firstToken->type != VALUE_INTEGER)
					{
						//LInsert(&globalInstrList,I_FLOAT2INT,(void*)result->firstToken,(void*)fID.firstToken,NULL);
					}
					if(fID.firstToken->type != VALUE_INTEGER)
					{
						//LInsert(&globalInstrList,I_FLOAT2INT,(void*)result->firstToken,(void*)stackTop(rStack)->firstToken,NULL);
					}
				}
				//LInsert(&globalInstrList,I_DIV,(void*)result->firstToken,fID.firstToken,stackTop(rStack)->firstToken);
				PrintInstrList(&globalInstrList);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			//E -> E/E
			else if(stackTop(rStack)->firstToken->type== DIV_DOUBLE)
			{
				printf("****E -> E/E****\n");
				stackPop(rStack);	//popnutí MINUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				if(areOperandsSameArithmethic(stackTop(rStack)->firstToken,fID.firstToken) == false)
				{
					if(stackTop(rStack)->firstToken->type != VALUE_DOUBLE)
					{
					//	LInsert(&globalInstrList,I_INT2FLOAT,(void*)result->firstToken,(void*)fID.firstToken,NULL);
					}
					if(fID.firstToken->type != VALUE_DOUBLE)
					{
					//	LInsert(&globalInstrList,I_INT2FLOAT,(void*)result->firstToken,(void*)stackTop(rStack)->firstToken,NULL);
					}
				}
				//LInsert(&globalInstrList,I_DIV,(void*)result->firstToken,fID.firstToken,stackTop(rStack)->firstToken);
				PrintInstrList(&globalInstrList);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type== LESS)
			{
				printf("****E -> E<E****\n");
				stackPop(rStack);	//popnutí MINUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
				{
					// ******************CHYBAAAAAA****************
				}
				//LInsert(&globalInstrList,I_LT,(void*)result->firstToken,fID.firstToken,stackTop(rStack)->firstToken);
				PrintInstrList(&globalInstrList);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type== GREATER)
			{
				printf("****E -> E>E****\n");
				stackPop(rStack);	//popnutí MINUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
				{
					// ******************CHYBAAAAAA****************
				}
				//LInsert(&globalInstrList,I_GT,(void*)result->firstToken,fID.firstToken,stackTop(rStack)->firstToken);
				PrintInstrList(&globalInstrList);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type== EQUAL)
			{
				printf("****E -> E=E****\n");
				stackPop(rStack);	//popnutí MINUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
				{
					// ******************CHYBAAAAAA****************
				}
				//LInsert(&globalInstrList,I_EQ,(void*)result->firstToken,fID.firstToken,stackTop(rStack)->firstToken);
				PrintInstrList(&globalInstrList);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type== GREATER_EQUAL)
			{
				printf("****E -> E>=E****\n");
				stackPop(rStack);	//popnutí MINUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
				{
					// ******************CHYBAAAAAA****************
				}
				//LInsert(&globalInstrList,I_LT,(void*)result->firstToken,fID.firstToken,stackTop(rStack)->firstToken);
				//LInsert(&globalInstrList,I_NOT,(void*)result->firstToken,(void*)result->firstToken,NULL);
				PrintInstrList(&globalInstrList);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type ==  LESS_EQUAL)
			{
				printf("****E -> E<=E****\n");
				stackPop(rStack);	//popnutí MINUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
				{
					// ******************CHYBAAAAAA****************
				}
				//LInsert(&globalInstrList,I_GT,(void*)result->firstToken,fID.firstToken,stackTop(rStack)->firstToken);
				//LInsert(&globalInstrList,I_NOT,(void*)result->firstToken,(void*)result->firstToken,NULL);
				PrintInstrList(&globalInstrList);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type == INEQUALITY)
			{
				printf("****E -> E<=E****\n");
				stackPop(rStack);	//popnutí MINUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				if(areOperandsSame(stackTop(rStack)->firstToken,fID.firstToken)== false)
				{
					// ******************CHYBAAAAAA****************
				}
				//LInsert(&globalInstrList,I_EQ,(void*)result->firstToken,fID.firstToken,stackTop(rStack)->firstToken);
				//LInsert(&globalInstrList,I_NOT,(void*)result->firstToken,(void*)result->firstToken,NULL);
				PrintInstrList(&globalInstrList);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type == EXPR)
			{
				stackPop(rStack);
				printf("****E -> (E)****\n");
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->type = EXPR;
				result->firstToken->info = "EXPR";
				stackPop(rStack);
				stackPush(st,(*result));
				(*reduct) = true;
			}
		}
	}
	return;
}

