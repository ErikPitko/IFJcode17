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
/*"EOL"*/{L,	L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		L,		ERROR,	EXITPARSE,	L},
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
bool canIDiv(tFooListElem *firstOperand,tFooListElem *secondOperand,bool isDouble, bool *cast);
bool areOperandsSame(tFooListElem *firstOperand,tFooListElem *secondOperand);
bool areOperandsSameArithmethic(tFooListElem *firstOperand,tFooListElem *secondOperand);
bool isTokenOperand(token *operand, bool isArithmethic);
bool isSymbolOperand(tFooListElem *operand, bool isArithmethic);
bool areOperands(tFooListElem *firstOperand,tFooListElem *secondOperand, bool isArithmethic);
void applyRule(tStack *st,tStack *rStack,bool *reduct,int *semanticError,tFooListElem *returnVar,tHashTable *localTable,char * tempName);
char *rand_string(int length);
bool end(int type);
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

bool end(int type)
{
	if(type<=51 && type >=36)
		return false;
	return true;
}

int tableIndexSelect(tReductToken *tok)
{
	if(tok->firstToken == NULL)
		return 13;
	if(tok->firstToken->type == IDENTIFIER ||tok->firstToken->type == EXPR|| tok->firstToken->type == VALUE_INTEGER || tok->firstToken->type == VALUE_DOUBLE || tok->firstToken->type == VALUE_STRING)
	{
		return 14;
	}
	unsigned arr[15] = {PLUS,MINUS,ASTERIX,DIV_INT,DIV_DOUBLE,EQUAL,LESS,GREATER,GREATER_EQUAL,LESS_EQUAL,
				INEQUALITY,LEFT_PARENTHESIS,RIGHT_PARENTHESIS,EOL};
	for(int i = 0;i<15;i++)
		if(arr[i]== tok->firstToken->type)
			return i;
	return 13;
}

char *rand_string(int length) {
    char *string = "abcdefghijklmnopqrstuvwxyz";
    size_t stringLen = 26;
    char *randomString;
    randomString = malloc(sizeof(char) * (length +1));
    if (!randomString) {
        return (char*)0;
    }

    unsigned int key = 0;

    for (int n = 0;n < length;n++) {
        key = rand() % stringLen;
        randomString[n] = string[key];
    }
    randomString[length] = '\0';
    return randomString;
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
	tReductToken priority;
	bool wasOperand = false;
	bool operatorRead = false;
	int semanticError = 0;
	char* temporaryName = rand_string(20);
	tFooListElem elem;
	elem.id = temporaryName;
	list_insert(localTable,elem);
	priority.firstToken = (token*)myMalloc(sizeof(token));
	priority.priority = L;
	priority.firstToken->info = "<";
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
		else
		{
			tReductToken temp = *(stackTop(&stack));
			stackPop(&stack);
			select = tableIndexSelect(stackTop(&stack));
			stackPush(&stack,temp);
		}

		//printf("%s  \t%s \n",strValueOfEnum(stackTop(&stack)->firstToken->type),strValueOfEnum(actToken.firstToken->type));
		switch (precTable[select][tableIndexSelect(&actToken)])
		{
			case H:
				//<a+b
				do
				{
					if(stackTop(&stack)->priority == L)		//b+a
					{
						stackPop(&stack);
						break;
					}
					if(stackTop(&stack)->firstToken->type == EOL)	//b+a
					{
						break;
					}
					stackPush(&rStack,*(stackTop(&stack)));	//b...+...a
					stackPop(&stack);	//b+a
				}while(1);
				stackPrint("stack",&rStack);
				printf("&&&&&APPLY RULE&&&&&\n");
				applyRule(&stack,&rStack,&reduct,&semanticError,returnVar,localTable,temporaryName);
				printf("&&&&&AFTER RULE&&&&&\n");
				break;
			case L:
				//printf("LOOOW");
				{
				if(operatorRead &&!isTokenOperand(actToken.firstToken,false) && actToken.firstToken->type != LEFT_PARENTHESIS)
				{
					//printf("%s SYNTAX\n",strValueOfEnum(actToken.firstToken->type));
					error_msg(SYNTAX_ERR,"Two operators in row");
				}
				if(isOperatorExpr(actToken.firstToken))
				{
				//	printf("%s OPERATOR READ\n",strValueOfEnum(actToken.firstToken->type));
					operatorRead = true;
				}
				if(isTokenOperand(actToken.firstToken,false))
				{
					wasOperand = true;
					operatorRead = false;
					//printf("WAS THERE %s \n",actToken.firstToken->info);
				}
				if(!isTokenOperand(actToken.firstToken,false) && !wasOperand && actToken.firstToken->type != LEFT_PARENTHESIS)
				{
					error_msg(SYNTAX_ERR,"The first token of expression wasn't operand or function call");
				}
				tReductToken tmp = *(stackTop(&stack));
				stackPop(&stack);
				if(reduct)
				{
					stackPush(&stack,priority);
					stackPush(&stack,tmp);
				}
				else
				{
					stackPush(&stack,tmp);
					stackPush(&stack,priority);
				}
				stackPush(&stack,actToken);
				reduct = false;
				actToken.firstToken = getToken();
				break;
				}
			case EQ:
				stackPush(&stack,actToken);
				actToken.firstToken = getToken();
				reduct = false;
				break;
			case ERROR:
				printf("%i  \t%i \n",select,tableIndexSelect(&actToken));
				error_msg(SYNTAX_ERR,"Syntaktická chyba");
				break;
			case EXITPARSE:
				stackPrint("stack",&stack);
				goto exit;
			default:
				printf("%i  %i\n",select,tableIndexSelect(&actToken));
				error_msg(SYNTAX_ERR,"Syntaktická chyba");
		}

	}
	while(!(actToken.firstToken->type == EOL && stackTop(&stack)->firstToken->type == EOL));
	exit:
	printf("****RETURNS: %s ****\n\n",strValueOfEnum(actToken.firstToken->type));
	if(semanticError != 0)
	{
		error_msg(semanticError,"Semantic error in expression");
	}
	PrintInstrList(&globalInstrList);
	return actToken.firstToken;
}

bool canIDiv(tFooListElem *firstOperand,tFooListElem *secondOperand,bool isDouble, bool *cast)
{
	if(isSymbolOperand(firstOperand,true) && isSymbolOperand(secondOperand,true))
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

bool areOperandsSame(tFooListElem *firstOperand,tFooListElem *secondOperand)
{
	if(areOperandsSameArithmethic(firstOperand,secondOperand) || firstOperand->type == VALUE_STRING)
		return firstOperand->type == secondOperand->type;
	return false;
}

bool areOperandsSameArithmethic(tFooListElem *firstOperand,tFooListElem *secondOperand)
{
	if(firstOperand->type == VALUE_INTEGER || firstOperand->type == VALUE_DOUBLE)
		return firstOperand->type == secondOperand->type;
	return false;
}

bool isTokenOperand(token *operand, bool isArithmethic)
{
	//s identifierem to nebude fungovat musí se to ořešit protože identifier musí dávat více informací ohledně typu
	if(operand->type == IDENTIFIER||operand->type == VALUE_INTEGER||operand->type == VALUE_DOUBLE || (operand->type == VALUE_STRING && !isArithmethic))
		return true;
	return false;
}

bool isSymbolOperand(tFooListElem *operand, bool isArithmethic)
{
	//s identifierem to nebude fungovat musí se to ořešit protože identifier musí dávat více informací ohledně typu
	if(operand->type == IDENTIFIER||operand->type == VALUE_INTEGER||operand->type == VALUE_DOUBLE || (operand->type == VALUE_STRING && !isArithmethic))
		return true;
	return false;
}

bool areOperands(tFooListElem *firstOperand,tFooListElem *secondOperand, bool isArithmethic)
{
	//s identifierem to nebude fungovat musí se to ořešit protože identifier musí dávat více informací ohledně typu
	/*operand->type == IDENTIFIER||*/
	if(isSymbolOperand(firstOperand,isArithmethic) && isSymbolOperand(secondOperand,isArithmethic))
		return true;
	return false;
}

bool isOperatorExpr(token *tok)
{
	if(tok->type >=36 && tok->type <=46)
		return true;
	return false;
}

void applyRule(tStack *st,tStack *rStack,bool *reduct,int *semanticError,tFooListElem *returnVar, tHashTable *localTable, char * tempName)
{
	//E -> i
	if(stackLenght(rStack)==1)
	{
		printf("****E -> i****\n");
		tReductToken *tmp = stackTop(rStack);
		tmp->isReduced = true;
		stackPop(rStack);
		stackPush(st,*(tmp));
		(*reduct) = true;
	}
	else if(stackLenght(rStack)>1)
	{
		{
			tReductToken fID = *(stackTop(rStack));	//pop a top stacku (je tam operand)
			stackPop(rStack);	// X + b   => 	pop X
			//stackPop(rStack);	//MUSÍ BÝT JINAK NEBUDE FUNGOVAT BACHA NA HORNÍ ŘÁDEK POKUD BUDEME PRACOVAT JAKO TOP POP MUSÍME VYMAZAT
			if(!isOperatorExpr(fID.firstToken))
			{
			if(fID.firstToken->type == LEFT_PARENTHESIS||fID.firstToken->type == RIGHT_PARENTHESIS)
			{
				tReductToken tmp = *(stackTop(rStack));
				stackPop(rStack);
				printf("****E -> (E)****\n");
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = tempName;
				result->firstToken->type = tmp.firstToken->type;
				result->isReduced = true;
				stackPop(rStack);
				stackPush(st,(*result));
				(*reduct) = true;
			}
			//E -> E+E
			else if(stackTop(rStack)->firstToken->type == PLUS)
			{
				printf("****E -> E+E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = tempName;
				tFooListElem *temporary = function_find(localTable,tempName);
				tFooListElem *firstOper;
				tFooListElem *secondOper;

				if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
					firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
				else
				{
					firstOper = myMalloc(sizeof(tFooListElem));
					firstOper->id = stackTop(rStack)->firstToken->info;
					firstOper->type = stackTop(rStack)->firstToken->type;
				}
				if(fID.firstToken->type == IDENTIFIER)
				{
					secondOper = function_find(localTable,fID.firstToken->info);
				}
				else
				{
					secondOper = myMalloc(sizeof(tFooListElem));
					secondOper->id = fID.firstToken->info;
					secondOper->type = fID.firstToken->type;
				}
				if((firstOper->type == DOUBLE || firstOper->type == VALUE_DOUBLE)&& (secondOper->type == INTEGER|| secondOper->type == VALUE_INTEGER))
				{
					printf("1.DOUBLE 2.INTEGER\n");
					secondOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,secondOper,secondOper,NULL);
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.INTEGER 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,firstOper,firstOper,NULL);
				}
				else if((firstOper->type == DOUBLE||firstOper->type == VALUE_DOUBLE) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.DOUBLE 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					secondOper->type = DOUBLE;
				}
				else if((firstOper->type == STRING||firstOper->type == VALUE_STRING)&&(secondOper->type == STRING||secondOper->type == VALUE_STRING))
				{
					printf("1.STRING 2.STRING\n");
					firstOper->type = STRING;
					secondOper->type = STRING;
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER)&&(secondOper->type == INTEGER||secondOper->type == VALUE_INTEGER))
				{
					firstOper->type = INTEGER;
					secondOper->type = INTEGER;
				}
				else
				{
					printf("PLUS SEMANTIC_ERR");
					*(semanticError)= SEMANTIC_TYPE;
				}
				printf("CAST\n");
				temporary->type = secondOper->type;
				printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
				temporary->is_define = true;
				result->firstToken->type = temporary->type;
				LInsert(&globalInstrList,I_ADD,temporary,firstOper,secondOper);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			//E -> E-E
			else if(stackTop(rStack)->firstToken->type== MINUS)
			{
				printf("****E -> E-E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = tempName;
				tFooListElem *temporary = function_find(localTable,tempName);
				tFooListElem *firstOper;
				tFooListElem *secondOper;

				if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
					firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
				else
				{
					firstOper = myMalloc(sizeof(tFooListElem));
					firstOper->id = stackTop(rStack)->firstToken->info;
					firstOper->type = stackTop(rStack)->firstToken->type;
				}
				if(fID.firstToken->type == IDENTIFIER)
				{
					secondOper = function_find(localTable,fID.firstToken->info);
				}
				else
				{
					secondOper = myMalloc(sizeof(tFooListElem));
					secondOper->id = fID.firstToken->info;
					secondOper->type = fID.firstToken->type;
				}
				if((firstOper->type == DOUBLE || firstOper->type == VALUE_DOUBLE)&& (secondOper->type == INTEGER|| secondOper->type == VALUE_INTEGER))
				{
					printf("1.DOUBLE 2.INTEGER\n");
					secondOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,secondOper,secondOper,NULL);
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.INTEGER 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,firstOper,firstOper,NULL);
				}
				else if((firstOper->type == DOUBLE||firstOper->type == VALUE_DOUBLE) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.DOUBLE 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					secondOper->type = DOUBLE;
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER)&&(secondOper->type == INTEGER||secondOper->type == VALUE_INTEGER))
				{
					firstOper->type = INTEGER;
					secondOper->type = INTEGER;
				}
				else
				{
					printf("SUB SEMANTIC_ERR");
					*(semanticError)= SEMANTIC_TYPE;
				}
				printf("CAST\n");
				temporary->type = secondOper->type;
				printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
				temporary->is_define = true;
				result->firstToken->type = temporary->type;
				LInsert(&globalInstrList,I_SUB,temporary,firstOper,secondOper);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			//E -> E*E
			else if(stackTop(rStack)->firstToken->type== ASTERIX)
			{
				printf("****E -> E*E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = tempName;
				tFooListElem *temporary = function_find(localTable,tempName);
				tFooListElem *firstOper;
				tFooListElem *secondOper;

				if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
					firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
				else
				{
					firstOper = myMalloc(sizeof(tFooListElem));
					firstOper->id = stackTop(rStack)->firstToken->info;
					firstOper->type = stackTop(rStack)->firstToken->type;
				}
				if(fID.firstToken->type == IDENTIFIER)
				{
					secondOper = function_find(localTable,fID.firstToken->info);
				}
				else
				{
					secondOper = myMalloc(sizeof(tFooListElem));
					secondOper->id = fID.firstToken->info;
					secondOper->type = fID.firstToken->type;
				}
				if((firstOper->type == DOUBLE || firstOper->type == VALUE_DOUBLE)&& (secondOper->type == INTEGER|| secondOper->type == VALUE_INTEGER))
				{
					printf("1.DOUBLE 2.INTEGER\n");
					secondOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,secondOper,secondOper,NULL);
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.INTEGER 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,firstOper,firstOper,NULL);
				}
				else if((firstOper->type == DOUBLE||firstOper->type == VALUE_DOUBLE) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.DOUBLE 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					secondOper->type = DOUBLE;
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER)&&(secondOper->type == INTEGER||secondOper->type == VALUE_INTEGER))
				{
					firstOper->type = INTEGER;
					secondOper->type = INTEGER;
				}
				else
				{
					printf("MUL SEMANTIC_ERR");
					*(semanticError)= SEMANTIC_TYPE;
				}
				printf("CAST\n");
				temporary->type = secondOper->type;
				printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
				temporary->is_define = true;
				result->firstToken->type = temporary->type;
				LInsert(&globalInstrList,I_MUL,temporary,firstOper,secondOper);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			//E -> E\E
			else if(stackTop(rStack)->firstToken->type==  DIV_INT)
			{
				printf("****E -> E\E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = tempName;
				tFooListElem *temporary = function_find(localTable,tempName);
				tFooListElem *firstOper;
				tFooListElem *secondOper;

				if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
					firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
				else
				{
					firstOper = myMalloc(sizeof(tFooListElem));
					firstOper->id = stackTop(rStack)->firstToken->info;
					firstOper->type = stackTop(rStack)->firstToken->type;
				}
				if(fID.firstToken->type == IDENTIFIER)
				{
					secondOper = function_find(localTable,fID.firstToken->info);
				}
				else
				{
					secondOper = myMalloc(sizeof(tFooListElem));
					secondOper->id = fID.firstToken->info;
					secondOper->type = fID.firstToken->type;
				}
				if((firstOper->type == DOUBLE || firstOper->type == VALUE_DOUBLE)&& (secondOper->type == INTEGER|| secondOper->type == VALUE_INTEGER))
				{
					printf("1.DOUBLE 2.INTEGER\n");
					secondOper->type = INTEGER;
					LInsert(&globalInstrList,I_FLOAT2INT,secondOper,secondOper,NULL);
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.INTEGER 2.DOUBLE\n");
					firstOper->type = INTEGER;
					LInsert(&globalInstrList,I_FLOAT2INT,firstOper,firstOper,NULL);
				}
				else if((firstOper->type == DOUBLE||firstOper->type == VALUE_DOUBLE) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.DOUBLE 2.DOUBLE\n");
					firstOper->type = INTEGER;
					LInsert(&globalInstrList,I_FLOAT2INT,firstOper,firstOper,NULL);
					secondOper->type = INTEGER;
					LInsert(&globalInstrList,I_FLOAT2INT,secondOper,secondOper,NULL);
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER)&&(secondOper->type == INTEGER||secondOper->type == VALUE_INTEGER))
				{
					firstOper->type = INTEGER;
					secondOper->type = INTEGER;
				}
				else
				{
					printf("INT DIV SEMANTIC_ERR");
					*(semanticError)= SEMANTIC_TYPE;
				}
				printf("CAST\n");
				temporary->type = secondOper->type;
				printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
				temporary->is_define = true;
				result->firstToken->type = temporary->type;
				LInsert(&globalInstrList,I_DIV,temporary,firstOper,secondOper);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			//E -> E/E
			else if(stackTop(rStack)->firstToken->type== DIV_DOUBLE)
			{
				printf("****E -> E/E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = tempName;
				tFooListElem *temporary = function_find(localTable,tempName);
				tFooListElem *firstOper;
				tFooListElem *secondOper;

				if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
					firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
				else
				{
					firstOper = myMalloc(sizeof(tFooListElem));
					firstOper->id = stackTop(rStack)->firstToken->info;
					firstOper->type = stackTop(rStack)->firstToken->type;
				}
				if(fID.firstToken->type == IDENTIFIER)
				{
					secondOper = function_find(localTable,fID.firstToken->info);
				}
				else
				{
					secondOper = myMalloc(sizeof(tFooListElem));
					secondOper->id = fID.firstToken->info;
					secondOper->type = fID.firstToken->type;
				}
				if((firstOper->type == DOUBLE || firstOper->type == VALUE_DOUBLE)&& (secondOper->type == INTEGER|| secondOper->type == VALUE_INTEGER))
				{
					printf("1.DOUBLE 2.INTEGER\n");
					secondOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,secondOper,secondOper,NULL);
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.INTEGER 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,firstOper,firstOper,NULL);
				}
				else if((firstOper->type == DOUBLE||firstOper->type == VALUE_DOUBLE) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.DOUBLE 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					secondOper->type = DOUBLE;
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER)&&(secondOper->type == INTEGER||secondOper->type == VALUE_INTEGER))
				{
					firstOper->type = DOUBLE;
					secondOper->type = DOUBLE;
				}
				else
				{
					printf("DOUBLE DIV SEMANTIC_ERR");
					*(semanticError)= SEMANTIC_TYPE;
				}
				printf("CAST\n");
				temporary->type = secondOper->type;
				printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
				temporary->is_define = true;
				result->firstToken->type = temporary->type;
				LInsert(&globalInstrList,I_DIV,temporary,firstOper,secondOper);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type== LESS)
			{
				printf("****E -> E<E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = tempName;
				tFooListElem *temporary = function_find(localTable,tempName);
				tFooListElem *firstOper;
				tFooListElem *secondOper;

				if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
					firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
				else
				{
					firstOper = myMalloc(sizeof(tFooListElem));
					firstOper->id = stackTop(rStack)->firstToken->info;
					firstOper->type = stackTop(rStack)->firstToken->type;
				}
				if(fID.firstToken->type == IDENTIFIER)
				{
					secondOper = function_find(localTable,fID.firstToken->info);
				}
				else
				{
					secondOper = myMalloc(sizeof(tFooListElem));
					secondOper->id = fID.firstToken->info;
					secondOper->type = fID.firstToken->type;
				}
				if((firstOper->type == DOUBLE || firstOper->type == VALUE_DOUBLE)&& (secondOper->type == INTEGER|| secondOper->type == VALUE_INTEGER))
				{
					printf("1.DOUBLE 2.INTEGER\n");
					secondOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,secondOper,secondOper,NULL);
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.INTEGER 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,firstOper,firstOper,NULL);
				}
				else if((firstOper->type == DOUBLE||firstOper->type == VALUE_DOUBLE) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.DOUBLE 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					secondOper->type = DOUBLE;
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER)&&(secondOper->type == INTEGER||secondOper->type == VALUE_INTEGER))
				{
					firstOper->type = INTEGER;
					secondOper->type = INTEGER;
				}
				else
				{
					printf("LESS SEMANTIC_ERR");
					*(semanticError)= SEMANTIC_TYPE;
				}
				printf("CAST\n");
				temporary->type = secondOper->type;
				printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
				temporary->is_define = true;
				result->firstToken->type = temporary->type;
				LInsert(&globalInstrList,I_LT,temporary,firstOper,secondOper);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type== GREATER)
			{
				printf("****E -> E>E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = tempName;
				tFooListElem *temporary = function_find(localTable,tempName);
				tFooListElem *firstOper;
				tFooListElem *secondOper;

				if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
					firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
				else
				{
					firstOper = myMalloc(sizeof(tFooListElem));
					firstOper->id = stackTop(rStack)->firstToken->info;
					firstOper->type = stackTop(rStack)->firstToken->type;
				}
				if(fID.firstToken->type == IDENTIFIER)
				{
					secondOper = function_find(localTable,fID.firstToken->info);
				}
				else
				{
					secondOper = myMalloc(sizeof(tFooListElem));
					secondOper->id = fID.firstToken->info;
					secondOper->type = fID.firstToken->type;
				}
				if((firstOper->type == DOUBLE || firstOper->type == VALUE_DOUBLE)&& (secondOper->type == INTEGER|| secondOper->type == VALUE_INTEGER))
				{
					printf("1.DOUBLE 2.INTEGER\n");
					secondOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,secondOper,secondOper,NULL);
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.INTEGER 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,firstOper,firstOper,NULL);
				}
				else if((firstOper->type == DOUBLE||firstOper->type == VALUE_DOUBLE) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.DOUBLE 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					secondOper->type = DOUBLE;
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER)&&(secondOper->type == INTEGER||secondOper->type == VALUE_INTEGER))
				{
					firstOper->type = INTEGER;
					secondOper->type = INTEGER;
				}
				else
				{
					printf("GREATER SEMANTIC_ERR");
					*(semanticError)= SEMANTIC_TYPE;
				}
				printf("CAST\n");
				temporary->type = secondOper->type;
				printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
				temporary->is_define = true;
				result->firstToken->type = temporary->type;
				LInsert(&globalInstrList,I_GT,temporary,firstOper,secondOper);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type== EQUAL)
			{
				printf("****E -> E=E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = tempName;
				tFooListElem *temporary = function_find(localTable,tempName);
				tFooListElem *firstOper;
				tFooListElem *secondOper;

				if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
					firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
				else
				{
					firstOper = myMalloc(sizeof(tFooListElem));
					firstOper->id = stackTop(rStack)->firstToken->info;
					firstOper->type = stackTop(rStack)->firstToken->type;
				}
				if(fID.firstToken->type == IDENTIFIER)
				{
					secondOper = function_find(localTable,fID.firstToken->info);
				}
				else
				{
					secondOper = myMalloc(sizeof(tFooListElem));
					secondOper->id = fID.firstToken->info;
					secondOper->type = fID.firstToken->type;
				}
				if((firstOper->type == DOUBLE || firstOper->type == VALUE_DOUBLE)&& (secondOper->type == INTEGER|| secondOper->type == VALUE_INTEGER))
				{
					printf("1.DOUBLE 2.INTEGER\n");
					secondOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,secondOper,secondOper,NULL);
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.INTEGER 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,firstOper,firstOper,NULL);
				}
				else if((firstOper->type == DOUBLE||firstOper->type == VALUE_DOUBLE) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.DOUBLE 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					secondOper->type = DOUBLE;
				}
				else if((firstOper->type == STRING||firstOper->type == VALUE_STRING)&&(secondOper->type == STRING||secondOper->type == VALUE_STRING))
				{
					printf("1.STRING 2.STRING\n");
					firstOper->type = STRING;
					secondOper->type = STRING;
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER)&&(secondOper->type == INTEGER||secondOper->type == VALUE_INTEGER))
				{
					firstOper->type = INTEGER;
					secondOper->type = INTEGER;
				}
				else
				{
					printf("EQUAL SEMANTIC_ERR");
					*(semanticError)= SEMANTIC_TYPE;
				}
				printf("CAST\n");
				temporary->type = secondOper->type;
				printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
				temporary->is_define = true;
				result->firstToken->type = temporary->type;
				LInsert(&globalInstrList,I_EQ,temporary,firstOper,secondOper);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type== GREATER_EQUAL)
			{
				printf("****E -> E>=E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = tempName;
				tFooListElem *temporary = function_find(localTable,tempName);
				tFooListElem *firstOper;
				tFooListElem *secondOper;

				if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
					firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
				else
				{
					firstOper = myMalloc(sizeof(tFooListElem));
					firstOper->id = stackTop(rStack)->firstToken->info;
					firstOper->type = stackTop(rStack)->firstToken->type;
				}
				if(fID.firstToken->type == IDENTIFIER)
				{
					secondOper = function_find(localTable,fID.firstToken->info);
				}
				else
				{
					secondOper = myMalloc(sizeof(tFooListElem));
					secondOper->id = fID.firstToken->info;
					secondOper->type = fID.firstToken->type;
				}
				if((firstOper->type == DOUBLE || firstOper->type == VALUE_DOUBLE)&& (secondOper->type == INTEGER|| secondOper->type == VALUE_INTEGER))
				{
					printf("1.DOUBLE 2.INTEGER\n");
					secondOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,secondOper,secondOper,NULL);
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.INTEGER 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,firstOper,firstOper,NULL);
				}
				else if((firstOper->type == DOUBLE||firstOper->type == VALUE_DOUBLE) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.DOUBLE 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					secondOper->type = DOUBLE;
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER)&&(secondOper->type == INTEGER||secondOper->type == VALUE_INTEGER))
				{
					firstOper->type = INTEGER;
					secondOper->type = INTEGER;
				}
				else
				{
					printf("GREATER EQUAL SEMANTIC_ERR");
					*(semanticError)= SEMANTIC_TYPE;
				}
				printf("CAST\n");
				temporary->type = secondOper->type;
				printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
				temporary->is_define = true;
				result->firstToken->type = temporary->type;
				LInsert(&globalInstrList,I_LT,temporary,firstOper,secondOper);
				LInsert(&globalInstrList,I_NOT,temporary,temporary,NULL);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type ==  LESS_EQUAL)
			{
				printf("****E -> E<=E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = tempName;
				tFooListElem *temporary = function_find(localTable,tempName);
				tFooListElem *firstOper;
				tFooListElem *secondOper;

				if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
					firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
				else
				{
					firstOper = myMalloc(sizeof(tFooListElem));
					firstOper->id = stackTop(rStack)->firstToken->info;
					firstOper->type = stackTop(rStack)->firstToken->type;
				}
				if(fID.firstToken->type == IDENTIFIER)
				{
					secondOper = function_find(localTable,fID.firstToken->info);
				}
				else
				{
					secondOper = myMalloc(sizeof(tFooListElem));
					secondOper->id = fID.firstToken->info;
					secondOper->type = fID.firstToken->type;
				}
				if((firstOper->type == DOUBLE || firstOper->type == VALUE_DOUBLE)&& (secondOper->type == INTEGER|| secondOper->type == VALUE_INTEGER))
				{
					printf("1.DOUBLE 2.INTEGER\n");
					secondOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,secondOper,secondOper,NULL);
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.INTEGER 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,firstOper,firstOper,NULL);
				}
				else if((firstOper->type == DOUBLE||firstOper->type == VALUE_DOUBLE) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.DOUBLE 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					secondOper->type = DOUBLE;
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER)&&(secondOper->type == INTEGER||secondOper->type == VALUE_INTEGER))
				{
					firstOper->type = INTEGER;
					secondOper->type = INTEGER;
				}
				else
				{
					printf("LESS EQUAL SEMANTIC_ERR");
					*(semanticError)= SEMANTIC_TYPE;
				}
				printf("CAST\n");
				temporary->type = secondOper->type;
				printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
				temporary->is_define = true;
				result->firstToken->type = temporary->type;
				LInsert(&globalInstrList,I_GT,temporary,firstOper,secondOper);
				LInsert(&globalInstrList,I_NOT,temporary,temporary,NULL);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type == INEQUALITY)
			{
				printf("****E -> E<>E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = tempName;
				tFooListElem *temporary = function_find(localTable,tempName);
				tFooListElem *firstOper;
				tFooListElem *secondOper;

				if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
					firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
				else
				{
					firstOper = myMalloc(sizeof(tFooListElem));
					firstOper->id = stackTop(rStack)->firstToken->info;
					firstOper->type = stackTop(rStack)->firstToken->type;
				}
				if(fID.firstToken->type == IDENTIFIER)
				{
					secondOper = function_find(localTable,fID.firstToken->info);
				}
				else
				{
					secondOper = myMalloc(sizeof(tFooListElem));
					secondOper->id = fID.firstToken->info;
					secondOper->type = fID.firstToken->type;
				}
				if((firstOper->type == DOUBLE || firstOper->type == VALUE_DOUBLE)&& (secondOper->type == INTEGER|| secondOper->type == VALUE_INTEGER))
				{
					printf("1.DOUBLE 2.INTEGER\n");
					secondOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,secondOper,secondOper,NULL);
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.INTEGER 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					LInsert(&globalInstrList,I_INT2FLOAT,firstOper,firstOper,NULL);
				}
				else if((firstOper->type == DOUBLE||firstOper->type == VALUE_DOUBLE) && (secondOper->type == DOUBLE||secondOper->type == VALUE_DOUBLE))
				{
					printf("1.DOUBLE 2.DOUBLE\n");
					firstOper->type = DOUBLE;
					secondOper->type = DOUBLE;
				}
				else if((firstOper->type == STRING||firstOper->type == VALUE_STRING)&&(secondOper->type == STRING||secondOper->type == VALUE_STRING))
				{
					printf("1.STRING 2.STRING\n");
					firstOper->type = STRING;
					secondOper->type = STRING;
				}
				else if((firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER)&&(secondOper->type == INTEGER||secondOper->type == VALUE_INTEGER))
				{
					firstOper->type = INTEGER;
					secondOper->type = INTEGER;
				}
				else
				{
					printf("INEQUAL SEMANTIC_ERR");
					*(semanticError)= SEMANTIC_TYPE;
				}
				printf("CAST\n");
				temporary->type = secondOper->type;
				printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
				temporary->is_define = true;
				result->firstToken->type = temporary->type;
				LInsert(&globalInstrList,I_EQ,temporary,firstOper,secondOper);
				LInsert(&globalInstrList,I_NOT,temporary,temporary,NULL);
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			}
		}
	}
	return;
}

