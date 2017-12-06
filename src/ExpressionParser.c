//BUGLIST:
/*
 * dim a as string =!"aa"
 * dim b as string =!"cc"
 * print a+b
 * */


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
bool areOperandsSame(tFooListElem *firstOperand,tFooListElem *secondOperand);
bool areOperandsSameArithmethic(tFooListElem *firstOperand,tFooListElem *secondOperand);
bool isTokenOperand(token *operand, bool isArithmethic);
bool isSymbolOperand(tFooListElem *operand, bool isArithmethic);
bool areOperands(tFooListElem *firstOperand,tFooListElem *secondOperand, bool isArithmethic);
void applyRule(tStack *st,tStack *rStack,bool *reduct,int *semanticError,tFooListElem *returnVar, tHashTable *localTable, int * counter,int *a,bool *operationPriority, bool *operationCompare);
void convertTo(tFooListElem *firstOper,tFooListElem *secondOper,int *semanticError,bool couldBeString,int counter,bool isDiv);
char* selectTmp(int zeroOrOne,int type,int counter);
void resultRetype(tFooListElem *returnVar,tFooListElem *temporary,int *semanticError,int counter);
bool end(int type);
void printDivByZeroLabel(bool isPrint);
/**************************************HEADER***************************************/
extern tFooListElem exprResult;
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
	int arr[15] = {PLUS,MINUS,ASTERIX,DIV_INT,DIV_DOUBLE,EQUAL,LESS,GREATER,GREATER_EQUAL,LESS_EQUAL,
				INEQUALITY,LEFT_PARENTHESIS,RIGHT_PARENTHESIS,EOL};
	for(int i = 0;i<15;i++)
		if(arr[i] == tok->firstToken->type)
			return i;
	return 13;
}

bool isGlobal(char* str)
{
	if(strcmp(str,"exppom1") == 0 || strcmp(str,"exppom2")== 0 || strcmp(str,"exppom3")==0||strcmp(str,"exppom4")==0)
		return true;
	else if(strcmp(str,"TypeOne") == 0 || strcmp(str,"TypeTwo")== 0)
	{
		return true;
	}
	return false;
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
	bool wasOperation = false;
	bool operationPriority = false;
	bool operationCompare = false;
	int operandCounter = 0;
	int counter = 0;
	int compareCounter = 0;
	int semanticError = 0;
	int a = 0;
	priority.firstToken = (token*)myMalloc(sizeof(token));
	priority.priority = L;
	priority.firstToken->info = (char*)myMalloc(sizeof(char)*2);
	priority.firstToken->info = "<";
	if(getSetToken == NULL)
	{
		//printf("\n****Parser gives me: NULL****\n");
		actToken.firstToken = getToken();
	}
	else
	{
		//printf("\n****Parser gives me: %s****\n",strValueOfEnum(getSetToken->type));
		actToken.firstToken = getSetToken;
	}

	if(actToken.firstToken->type == EOL || (actToken.firstToken->type <=35 && actToken.firstToken->type >=0)||(actToken.firstToken->type <=55 && actToken.firstToken->type >=54))
		error_msg(SYNTAX_ERR,"No expression");
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
			{
				int operatorcnt = 0;
				//stackPrint("stack",&stack);
				do
				{
					//printf("HIGH\n");
					if(stackTop(&stack)->priority == L)		//b+a
					{
						//printf("STACKTOP %s\n",stackTop(&stack)->firstToken->info);
						stackPop(&stack);
						break;
					}
					if(stackTop(&stack)->firstToken->type == EOL)	//b+a
					{
						break;
					}
					if(isOperatorExpr(stackTop(&stack)->firstToken))
						operatorcnt++;
					stackPush(&rStack,*(stackTop(&stack)));	//b...+...a					
					stackPop(&stack);	//b+a				
				}while(1);
				if(operatorcnt>1)
				{
					error_msg(SYNTAX_ERR,"Two operators in row");
				}
				//printf("%i  \n",operatorcnt);
				//stackPrint("rstack",&rStack);
				//printf("&&&&&APPLY RULE&&&&&\n");
				applyRule(&stack,&rStack,&reduct,&semanticError,returnVar,localTable,&counter,&a,&operationPriority,&operationCompare);
				//printf("&&&&&AFTER RULE&&&&&\n");
				break;
			}
			case L:
				//printf("LOOOW\n");
				{
				if(stackTop(&stack)->firstToken != NULL)
					//printf("%s  %s SYNTAX\n",strValueOfEnum(actToken.firstToken->type),strValueOfEnum(stackTop(&stack)->firstToken->type));

				if(isOperatorExpr(actToken.firstToken) && isOperatorExpr(stackTop(&stack)->firstToken) && (actToken.firstToken->type != LEFT_PARENTHESIS || actToken.firstToken->type != RIGHT_PARENTHESIS))
				{

					error_msg(SYNTAX_ERR,"Two operators in row");
				}
				if(isTokenOperand(actToken.firstToken,false))
				{
					operandCounter++;
					wasOperand = true;
					wasOperation = false;
					//printf("WAS THERE %s \n",actToken.firstToken->info);
				}
				else
				{
					if(actToken.firstToken->type != LEFT_PARENTHESIS && actToken.firstToken->type != RIGHT_PARENTHESIS)
					{
						if(wasOperation)
						{
							error_msg(SYNTAX_ERR,"Two operators in row");
						}
						wasOperation = true;
					}
				}

				if(!isTokenOperand(actToken.firstToken,false) && !wasOperand && actToken.firstToken->type != LEFT_PARENTHESIS)
				{
					error_msg(SYNTAX_ERR,"The first token of expression wasn't operand or function call");
				}
				tReductToken tmp = *(stackTop(&stack));
				stackPop(&stack);
				if(reduct)
				{
					tmp.priority =H;
					stackPush(&stack,priority);
					stackPush(&stack,tmp);
				}
				else
				{
					tmp.priority =H;
					stackPush(&stack,tmp);
					stackPush(&stack,priority);
				}
				if(!operationPriority && (actToken.firstToken->type == PLUS || actToken.firstToken->type == MINUS))
				{
					operationPriority = true;
				}
				if((actToken.firstToken->type == LESS || actToken.firstToken->type == GREATER||actToken.firstToken->type == EQUAL || actToken.firstToken->type == GREATER_EQUAL||actToken.firstToken->type == LESS_EQUAL))
				{
					if(!operationCompare)
						operationCompare = true;
					if(compareCounter >= 1)
					{
						error_msg(SYNTAX_ERR,"Too many compare operators in expression");
					}
					compareCounter++;
				}
				stackPush(&stack,actToken);
				reduct = false;
				actToken.firstToken = getToken();
				//printf("%i ",actToken.firstToken->type);
				if(wasOperation && !isTokenOperand(actToken.firstToken,false)&& actToken.firstToken->type != LEFT_PARENTHESIS && actToken.firstToken->type != RIGHT_PARENTHESIS)
				{
					error_msg(SYNTAX_ERR,"Operator on the end of EXPR");
				}
				break;
				}
			case EQ:
				stackPush(&stack,actToken);
				actToken.firstToken = getToken();
				reduct = false;
				break;
			case ERROR:
				//printf("%i  \t%i \n",select,tableIndexSelect(&actToken));
				error_msg(SYNTAX_ERR,"Syntaktická chyba");
				break;
			case EXITPARSE:
				//stackPrint("stack",&stack);
				goto exit;
			default:
				//printf("%i  %i\n",select,tableIndexSelect(&actToken));
				error_msg(SYNTAX_ERR,"Syntaktická chyba");
		}
	}
	while(!(actToken.firstToken->type == EOL && stackTop(&stack)->firstToken->type == EOL));
	exit:
	//printf("****RETURNS: %s ****\n\n",strValueOfEnum(actToken.firstToken->type));
	if(operandCounter == 0)
	{
		semanticError = SYNTAX_ERR;
	}
	if(semanticError != 0)
	{
		error_msg(semanticError,"Semantic error in expression");
	}
	resultRetype(returnVar,&exprResult,&semanticError,counter);
	if(returnVar != NULL && find_test(localTable, returnVar->id))
	{
		printf("MOVE LF@%s GF@%s\n",returnVar->id, exprResult.id);
	}
	
	//stackClear(&stack);
	//stackClear(&rStack);
	return actToken.firstToken;
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
	if(operand->type == IDENTIFIER||operand->type == VALUE_INTEGER||operand->type == VALUE_DOUBLE || operand->type ==DOUBLE ||operand->type == INTEGER||((operand->type == VALUE_STRING||operand->type ==STRING )&& !isArithmethic))
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

void setSemanticError(int *semanticError,int num)
{
	if(!*(semanticError))
	{
		error_msg(num,"Expression error");
	}
}

void convertTo(tFooListElem *firstOper,tFooListElem *secondOper,int *semanticError,bool couldBeString,int counter,bool isDiv)
{
	if(firstOper != NULL && secondOper != NULL)
	{
		if((firstOper->type == DOUBLE|| firstOper->type == VALUE_DOUBLE||firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER||!couldBeString) && (secondOper->type == STRING|| secondOper->type == VALUE_STRING))
		{
			setSemanticError(semanticError,SEMANTIC_TYPE);
		}
		else if((secondOper->type == DOUBLE|| secondOper->type == VALUE_DOUBLE||secondOper->type == INTEGER||secondOper->type == VALUE_INTEGER||!couldBeString) &&(firstOper->type == STRING|| firstOper->type == VALUE_STRING))
		{
			setSemanticError(semanticError,SEMANTIC_TYPE);
		}
	}
	if(isDiv)
		goto firstOper;
	if(firstOper != NULL && secondOper != NULL)
	{	
		if((firstOper->type == DOUBLE|| firstOper->type == VALUE_DOUBLE) && (secondOper->type == INTEGER||secondOper->type == VALUE_INTEGER))
		{
			firstOper:
			if(secondOper->type == INTEGER)
			{
				if(isGlobal(secondOper->id)){
					printf("INT2FLOAT GF@%s GF@%s\n",secondOper->id,secondOper->id);
				}
				else
				{
					printf("INT2FLOAT GF@%s LF@%s\n","TypeTwo",secondOper->id);
					secondOper->id = "TypeTwo";
				}
				secondOper->type = DOUBLE;
			}
			else if(secondOper->type == VALUE_INTEGER)
			{
				if(isGlobal(secondOper->id)){
					printf("INT2FLOAT GF@%s GF@%s\n",selectTmp(0,DOUBLE,counter),secondOper->id);
					secondOper->id = selectTmp(0,DOUBLE,counter);
				}
				else
				{
					printf("INT2FLOAT GF@%s int@%s\n","TypeTwo",secondOper->id);
					secondOper->id = "TypeTwo";
				}
				secondOper->type = DOUBLE;
			}
			if(isDiv)
				goto secondOper;
		}

		if((secondOper->type == DOUBLE|| secondOper->type == VALUE_DOUBLE) && (firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER))
		{
			secondOper:
			if(strcmp(firstOper->id,secondOper->id) == 0)
			return;
			if(firstOper->type == INTEGER)
			{
				if(isGlobal(firstOper->id))
					printf("INT2FLOAT GF@%s GF@%s\n",firstOper->id,firstOper->id);
				else
				{
					printf("INT2FLOAT GF@%s LF@%s\n","TypeOne",firstOper->id);
					firstOper->id = "TypeOne";
				}
				firstOper->type = DOUBLE;

			}
			else if(firstOper->type == VALUE_INTEGER)
			{
				if(isGlobal(firstOper->id)){
					printf("INT2FLOAT GF@%s GF@%s\n",selectTmp(0,DOUBLE,counter),firstOper->id);
					firstOper->id = selectTmp(0,DOUBLE,counter);
				}
				else
				{
					printf("INT2FLOAT GF@%s int@%s\n","TypeOne",firstOper->id);
					firstOper->id ="TypeOne";
				}
				firstOper->type = DOUBLE;
			}
		}


	}
	if(firstOper!= NULL)
	{
		if(firstOper->type == INTEGER||firstOper->type == VALUE_INTEGER)
		{

		}
		else if(firstOper->type == DOUBLE|| firstOper->type == VALUE_DOUBLE)
		{

		}
		else if(firstOper->type == STRING|| firstOper->type == VALUE_STRING)
		{
			firstOper->id = reformString(firstOper->id);
		}
	}
	if(secondOper!= NULL)
	{
		if(secondOper->type == INTEGER||secondOper ->type == VALUE_INTEGER)
		{

		}
		else if(secondOper->type == DOUBLE||secondOper ->type == VALUE_DOUBLE)
		{

		}
		else if(secondOper->type == STRING||secondOper ->type == VALUE_STRING)
		{
			secondOper->id = reformString(secondOper->id);
		}
	}
}

char* selectTmp(int zeroOrOne,int type,int counter)
{
	(void)type;
	int modulo = (counter)%4 +zeroOrOne;
	if(modulo == 0)
	{
		return "exppom1";
	}
	else if(modulo == 1)
	{
		return "exppom2";
	}
	else if (modulo == 2)
	{
		return "exppom3";
	}
	else if (modulo == 3)
	{
		return "exppom4";
	}
	return "";
}

void printValue(char* instr,tFooListElem *temporary,tFooListElem *firstOper,tFooListElem *secondOper)
{
	//printf("ADD LF@%s LF@%s LF@%s\n",temporary->id,secondOper->id,firstOper->id);
	printf("%s GF@%s ",instr,temporary->id);
	if(firstOper!= NULL)
	{
		if(isGlobal(firstOper->id))
		{
			printf("GF@%s ",firstOper->id);
		}
		else if(firstOper->type == VALUE_STRING)
			printf("string@%s ",firstOper->id);
		else if(firstOper->type == VALUE_INTEGER)
			printf("int@%s ",firstOper->id);
		else if(firstOper->type == VALUE_DOUBLE)
			printf("float@%s ",firstOper->id);
		else if(firstOper->type == STRING || firstOper->type == INTEGER ||firstOper->type == DOUBLE)
		{
			printf("LF@%s ",firstOper->id);
		}
	}
	if(secondOper!= NULL)
	{
		if(isGlobal(secondOper->id))
		{
			printf("GF@%s ",secondOper->id);
		}
		else if(secondOper->type == VALUE_STRING)
			printf("string@%s ",secondOper->id);
		else if(secondOper->type == VALUE_INTEGER)
			printf("int@%s ",secondOper->id);
		else if(secondOper->type == VALUE_DOUBLE)
			printf("float@%s ",secondOper->id);
		else if(secondOper->type == STRING || secondOper->type == INTEGER ||secondOper->type == DOUBLE)
		{
			printf("LF@%s ",secondOper->id);
		}
	}
	printf("\n");
}

void resultRetype(tFooListElem *returnVar,tFooListElem *temporary,int *semanticError,int counter)
{
	(void)counter;
	if(returnVar == NULL){
		if(temporary->type == STRING || temporary->type == VALUE_STRING)
			temporary->id = reformString(temporary->id);
		return;
	}
	if(returnVar->type == INTEGER)
	{
		if(temporary->type == DOUBLE)
		{		
			exprResult.type = INTEGER;
			temporary->type = INTEGER;
			if(isGlobal(temporary->id))
				printf("FLOAT2R2EINT GF@%s GF@%s\n","TypeOne",temporary->id);
			else 
				printf("FLOAT2R2EINT GF@%s LF@%s\n","TypeOne",temporary->id);
			exprResult.id = "TypeOne";
		}
		else if(temporary->type == VALUE_DOUBLE)
		{			
			exprResult.type = INTEGER;
			temporary->type = INTEGER;
			if(!isGlobal(temporary->id))
				printf("FLOAT2R2EINT GF@%s float@%s\n","TypeOne",temporary->id);
			else 
				printf("FLOAT2R2EINT GF@%s GF@%s\n","TypeOne",temporary->id);
			exprResult.id = "TypeOne";
			temporary->id = "TypeOne";
		}
		else if (temporary->type == STRING||temporary->type == VALUE_STRING)
		{
			setSemanticError(semanticError,SEMANTIC_TYPE);
		}
	}
	else if(returnVar->type == DOUBLE)
	{
		if(temporary->type == INTEGER)
		{	
			exprResult.type = DOUBLE;
			temporary->type = DOUBLE;
			if(isGlobal(temporary->id))
				printf("INT2FLOAT GF@%s GF@%s\n","TypeOne",temporary->id);
			else
				printf("INT2FLOAT GF@%s LF@%s\n","TypeOne",temporary->id);
			exprResult.id = "TypeOne";
		}
		else if(temporary->type == VALUE_INTEGER)
		{			
			exprResult.type = DOUBLE;
			temporary->type = DOUBLE;
			if(!isGlobal(temporary->id))
				printf("INT2FLOAT GF@%s float@%s\n","TypeOne",temporary->id);
			else 
				printf("INT2FLOAT GF@%s GF@%s\n","TypeOne",temporary->id);
			temporary->id = "TypeOne";
			exprResult.id = "TypeOne";
		}
		else if (temporary->type == STRING||temporary->type == VALUE_STRING)
			setSemanticError(semanticError,SEMANTIC_TYPE);
	}
	else if(returnVar->type == STRING||returnVar->type == VALUE_STRING)
	{		
		if(temporary->type == INTEGER||temporary->type == DOUBLE||temporary->type == VALUE_DOUBLE||temporary->type == VALUE_INTEGER)
			setSemanticError(semanticError,SEMANTIC_TYPE);
	}
	else
	{
		setSemanticError(semanticError,SEMANTIC_OTHER);
	}			
}

void applyRule(tStack *st,tStack *rStack,bool *reduct,int *semanticError,tFooListElem *returnVar, tHashTable *localTable, int * counter,int *a,bool *operationPriority,bool *operationCompare)
{
	//E -> i
	if(stackLenght(rStack)==1)
	{
		//printf("****E -> i****\n");
		tReductToken *tmp = stackTop(rStack);
		tFooListElem *firstOper;
		if(tmp != NULL)
		{
			if(tmp->firstToken->type ==IDENTIFIER)
				if(find_test(localTable,tmp->firstToken->info))
				{
					firstOper = function_find(localTable,tmp->firstToken->info);
				}
				else
				{
					setSemanticError(semanticError,SEMANTIC_REDEF);
					//printf("AAAAAAAAAAA");
					goto semanticreduct;
				}
			else
			{
				firstOper = myMalloc(sizeof(tFooListElem));
				firstOper->id = tmp->firstToken->info;
				firstOper->type = tmp->firstToken->type;
			}
		}
		if(strcmp(tmp->firstToken->info,"0")==0)
			tmp->zeroflag = true;
		//printf("%s  %i",firstOper->id,returnVar->type);
		//printf("%s \n",firstOper->id);
		resultRetype(NULL,firstOper,semanticError,*(counter));
		tFooListElem temporary;
		temporary.id = selectTmp(0,firstOper->type,*(counter));
		temporary.type = firstOper->type;
		exprResult.type = temporary.type;
		exprResult.id = temporary.id;
		tmp->firstToken->type = temporary.type;
		tmp->isReduced = true;
		tmp->firstToken->info = firstOper->id;
		if ((*a) == 0)
		{
			(*a)= (*a)+1;
			//printf("%i \n",(*a));
			printValue("MOVE", &temporary, firstOper, NULL);
		}
		semanticreduct:
		stackPop(rStack);
		stackPush(st,*(tmp));	
		(*reduct) = true;
	}
	else if(stackLenght(rStack)>1)
	{
		{
			tReductToken fID = *(stackTop(rStack));	//pop a top stacku (je tam operand)
			stackPop(rStack);	// X + b   => 	pop X
			if(!isOperatorExpr(fID.firstToken))
			{
			if(fID.firstToken->type == LEFT_PARENTHESIS || fID.firstToken->type == RIGHT_PARENTHESIS)
			{
				tReductToken tmp = *(stackTop(rStack));
				stackPop(rStack);
				//printf("****E -> (E)****\n");
				*(counter) = *(counter)+1;
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = tmp.firstToken->info;
				result->firstToken->type = tmp.firstToken->type;
				result->isReduced = true;
				stackPop(rStack);
				stackPush(st,(*result));
				(*reduct) = true;
			}
			//E -> E+E
			else if(stackTop(rStack)->firstToken->type == PLUS)
			{
				//printf("****E -> E+E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = "EXPR";
				if(*(semanticError)==0)
				{
					tFooListElem *firstOper;
					tFooListElem *secondOper;
					if(stackTop(rStack) != NULL)
					{
						if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
							if(find_test(localTable,stackTop(rStack)->firstToken->info))
							{
								firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
							}
							else
							{
								setSemanticError(semanticError,SEMANTIC_REDEF);
								goto semanticplus;
							}
						else
						{
							firstOper = myMalloc(sizeof(tFooListElem));
							firstOper->id = stackTop(rStack)->firstToken->info;
							firstOper->type = stackTop(rStack)->firstToken->type;
						}
					}
					if(fID.firstToken->type == IDENTIFIER)
					{
						if(find_test(localTable,fID.firstToken->info))
						{
							secondOper = function_find(localTable,fID.firstToken->info);
						}
						else
						{
							setSemanticError(semanticError,SEMANTIC_REDEF);
							goto semanticplus;
						}
					}
					else
					{
						secondOper = myMalloc(sizeof(tFooListElem));
						secondOper->id = fID.firstToken->info;
						secondOper->type = fID.firstToken->type;
					}
					convertTo(firstOper,secondOper,semanticError,true,(*counter),false);
					//printf("CAST\n");
					if(secondOper != NULL && firstOper != NULL)
					{
						int i =0;
						if(*(operationCompare))
						{						
							i++;
						}
						*(counter) = *(counter)+1;
						tFooListElem temporary;
						temporary.type = firstOper->type;
						//printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
						temporary.is_define = true;
						temporary.id = selectTmp(i,temporary.type,(*counter));
						result->firstToken->type = temporary.type;
						result->firstToken->info = temporary.id;
						exprResult.type = temporary.type;
						exprResult.id = temporary.id;
						if(temporary.type == STRING||temporary.type == VALUE_STRING)
						{
							printValue("CONCAT",&temporary,secondOper,firstOper);
						}
						else
						{
							printValue("ADD",&temporary,secondOper,firstOper);
						}												
					}
				}				
				semanticplus:
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			//E -> E-E
			else if(stackTop(rStack)->firstToken->type== MINUS)
			{
				//printf("****E -> E-E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = "EXPR";
				if(*(semanticError)==0)
				{

					tFooListElem *firstOper;
					tFooListElem *secondOper;
					if(stackTop(rStack) != NULL)
					{
						if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
							if(find_test(localTable,stackTop(rStack)->firstToken->info))
							{
								firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
							}
							else
							{
								setSemanticError(semanticError,SEMANTIC_REDEF);
								goto semanticsub;
							}
						else
						{
							firstOper = myMalloc(sizeof(tFooListElem));
							firstOper->id = stackTop(rStack)->firstToken->info;
							firstOper->type = stackTop(rStack)->firstToken->type;
						}
					}
					if(fID.firstToken->type == IDENTIFIER)
					{
						if(find_test(localTable,fID.firstToken->info))
						{
							secondOper = function_find(localTable,fID.firstToken->info);
						}
						else
						{
							setSemanticError(semanticError,SEMANTIC_REDEF);
							goto semanticsub;
						}
					}
					else
					{
						secondOper = myMalloc(sizeof(tFooListElem));
						secondOper->id = fID.firstToken->info;
						secondOper->type = fID.firstToken->type;
					}
					convertTo(firstOper,secondOper,semanticError,false,(*counter),false);
					//printf("CAST\n");
					if(secondOper != NULL && firstOper != NULL)
					{
						int i =0;
						if(*(operationCompare))
						{						
							i++;
						}
						*(counter) = *(counter)+1;
						tFooListElem temporary;
						temporary.type = firstOper->type;
						//printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
						temporary.is_define = true;
						temporary.id = selectTmp(i,temporary.type,(*counter));
						result->firstToken->type = temporary.type;
						result->firstToken->info = temporary.id;
						exprResult.type = temporary.type;
						exprResult.id = temporary.id;
						printValue("SUB",&temporary,secondOper,firstOper);					
					}
				}
				semanticsub:
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			//E -> E*E
			else if(stackTop(rStack)->firstToken->type== ASTERIX)
			{
				//printf("****E -> E*E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = "EXPR";
				if(*(semanticError)==0)
				{
					tFooListElem *firstOper;
					tFooListElem *secondOper;
					if(stackTop(rStack) != NULL)
					{
						if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
							if(find_test(localTable,stackTop(rStack)->firstToken->info))
							{
								firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
							}
							else
							{
								setSemanticError(semanticError,SEMANTIC_REDEF);
								goto semanticasterix;
							}
						else
						{
							firstOper = myMalloc(sizeof(tFooListElem));
							firstOper->id = stackTop(rStack)->firstToken->info;
							firstOper->type = stackTop(rStack)->firstToken->type;
						}
					}
					if(fID.firstToken->type == IDENTIFIER)
					{
						if(find_test(localTable,fID.firstToken->info))
						{
							secondOper = function_find(localTable,fID.firstToken->info);
						}
						else
						{
							setSemanticError(semanticError,SEMANTIC_REDEF);
							goto semanticasterix;
						}
					}
					else
					{
						secondOper = myMalloc(sizeof(tFooListElem));
						secondOper->id = fID.firstToken->info;
						secondOper->type = fID.firstToken->type;
					}
					//if(*(operationPriority)== true)
					{
						//printf("AAA%i\n",*(counter));
						*(operationPriority)= false;
						*(counter) = *(counter)+1;
					}
					convertTo(firstOper,secondOper,semanticError,false,(*counter),false);
					//printf("CAST\n");
					if(secondOper != NULL && firstOper != NULL)
					{
						int i =0;
						if(*(operationCompare))
						{						
							i++;
						}
						tFooListElem temporary;
						temporary.type = firstOper->type;
						//printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
						temporary.is_define = true;

						temporary.id = selectTmp(i,temporary.type,(*counter));
						//firstOper->id = temporary.id;
						result->firstToken->type = temporary.type;
						result->firstToken->info = temporary.id;
						exprResult.type = temporary.type;
						exprResult.id = temporary.id;			
						printValue("MUL",&temporary,secondOper,firstOper);

						//printf("%s\n",temporary.id);
											
					}
				}
				semanticasterix:
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			//E -> E\E
			else if(stackTop(rStack)->firstToken->type==  DIV_INT)
			{
				//printf("****E -> E\\E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = "EXPR";
				if(*(semanticError)==0)
				{
					tFooListElem *firstOper;
					tFooListElem *secondOper;
					if(stackTop(rStack)->zeroflag)
					{
						setSemanticError(semanticError,SEMANTIC_OTHER);
						goto semanticdivint;
					}
					if(stackTop(rStack) != NULL)
					{
						if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
							if(find_test(localTable,stackTop(rStack)->firstToken->info))
							{
								firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
							}
							else
							{
								setSemanticError(semanticError,SEMANTIC_REDEF);
								goto semanticdivint;
							}
						else if(strcmp(stackTop(rStack)->firstToken->info,"0") != 0)
						{
							firstOper = myMalloc(sizeof(tFooListElem));
							firstOper->id = stackTop(rStack)->firstToken->info;
							firstOper->type = stackTop(rStack)->firstToken->type;
						}
					}
					if(fID.firstToken->type == IDENTIFIER)
					{
						if(find_test(localTable,fID.firstToken->info))
						{
							secondOper = function_find(localTable,fID.firstToken->info);
						}
						else
						{
							setSemanticError(semanticError,SEMANTIC_REDEF);
							goto semanticdivint;
						}
					}
					else
					{
						secondOper = myMalloc(sizeof(tFooListElem));
						secondOper->id = fID.firstToken->info;
						secondOper->type = fID.firstToken->type;
					}
					if(secondOper != NULL && firstOper != NULL)
					{
						//if(*(operationPriority)== true)
						{
							//printf("AAA%i\n",*(counter));
							*(operationPriority)= false;
							*(counter) = *(counter)+1;
						}
						int i =0;
						if(*(operationCompare))
						{						
							i++;
						}
						convertTo(firstOper,secondOper,semanticError,false,(*counter),true);	
						//printf("CAST\n");
						tFooListElem temporary;
						temporary.type = INTEGER;
						//printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
						temporary.is_define = true;
						temporary.id = selectTmp(i,temporary.type,(*counter));						
						printValue("DIV",&temporary,secondOper,firstOper);
						exprResult.type = INTEGER;
						exprResult.id = selectTmp(i,INTEGER,*(counter));
						result->firstToken->type = INTEGER;
						result->firstToken->info = exprResult.id;
						printf("FLOAT2R2EINT GF@%s GF@%s\n",selectTmp(i,INTEGER,*(counter)),temporary.id);
					}
				}
				semanticdivint:
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			//E -> E/E
			else if(stackTop(rStack)->firstToken->type== DIV_DOUBLE)
			{
				//printf("****E -> E/E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = "EXPR";
				if(*(semanticError)==0)
				{
					tFooListElem *firstOper;
					tFooListElem *secondOper;
					if(stackTop(rStack)->zeroflag)
					{
						setSemanticError(semanticError,SEMANTIC_OTHER);
						goto semanticdivdouble;
					}
					if(stackTop(rStack) != NULL)
					{
						if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
							if(find_test(localTable,stackTop(rStack)->firstToken->info))
							{
								firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
							}
							else
							{
								setSemanticError(semanticError,SEMANTIC_REDEF);
								goto semanticdivdouble;
							}
						else
						{
							firstOper = myMalloc(sizeof(tFooListElem));
							firstOper->id = stackTop(rStack)->firstToken->info;
							firstOper->type = stackTop(rStack)->firstToken->type;
						}
					}
					if(fID.firstToken->type == IDENTIFIER)
					{
						if(find_test(localTable,fID.firstToken->info))
						{
							secondOper = function_find(localTable,fID.firstToken->info);
						}
						else
						{
							setSemanticError(semanticError,SEMANTIC_REDEF);
							goto semanticdivdouble;
						}
					}
					else
					{
						secondOper = myMalloc(sizeof(tFooListElem));
						secondOper->id = fID.firstToken->info;
						secondOper->type = fID.firstToken->type;
					}
					//printf("CAST\n");
					if(secondOper != NULL && firstOper != NULL)
					{	
						if(strcmp(firstOper->id,"0") == 0)
						{
							setSemanticError(semanticError,SEMANTIC_OTHER);
							goto semanticdivdouble;
						}
						//if(*(operationPriority)== true)
						{
							//printf("AAA%i\n",*(counter));
							*(operationPriority)= false;
							*(counter) = *(counter)+1;
						}
						int i =0;
						if(*(operationCompare))
						{						
							i++;
						}
						convertTo(firstOper,secondOper,semanticError,false,(*counter),true);
						tFooListElem temporary;
						temporary.type = DOUBLE;
						
						temporary.is_define = true;
						temporary.id = selectTmp(i,temporary.type,(*counter));
						result->firstToken->type = temporary.type;
						result->firstToken->info = temporary.id;
						exprResult.type = temporary.type;
						exprResult.id = temporary.id;
						printValue("DIV",&temporary,secondOper,firstOper);
						if(returnVar != NULL)
						{
							if(returnVar->type == INTEGER)
							{
								exprResult.type = INTEGER;
								exprResult.id = selectTmp(i,INTEGER,*(counter));
								result->firstToken->type = INTEGER;
								result->firstToken->info = exprResult.id;
								printf("FLOAT2R2EINT GF@%s GF@%s\n",selectTmp(i,INTEGER,*(counter)),temporary.id);
							}
						}
					}
				}
				semanticdivdouble:
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type== LESS)
			{
				//printf("****E -> E<E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = "EXPR";
				if(*(semanticError)==0)
				{
					tFooListElem *firstOper;
					tFooListElem *secondOper;
					if(stackTop(rStack) != NULL)
					{
						if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
							if(find_test(localTable,stackTop(rStack)->firstToken->info))
							{
								firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
							}
							else
							{
								setSemanticError(semanticError,SEMANTIC_REDEF);
								goto semanticless;
							}
						else
						{
							firstOper = myMalloc(sizeof(tFooListElem));
							firstOper->id = stackTop(rStack)->firstToken->info;
							firstOper->type = stackTop(rStack)->firstToken->type;
						}
					}
					if(fID.firstToken->type == IDENTIFIER)
					{
						if(find_test(localTable,fID.firstToken->info))
						{
							secondOper = function_find(localTable,fID.firstToken->info);
						}
						else
						{
							setSemanticError(semanticError,SEMANTIC_REDEF);
							goto semanticless;
						}
					}
					else
					{
						secondOper = myMalloc(sizeof(tFooListElem));
						secondOper->id = fID.firstToken->info;
						secondOper->type = fID.firstToken->type;
					}
					/*if(*(operationPriority)== true)
					{
						//printf("AAA%i\n",*(counter));
						*(operationPriority)= false;
						*(counter) = *(counter)+1;
					}*/
					int i =0;
					if(*(operationCompare))
					{
						
						i++;
					}
					convertTo(firstOper,secondOper,semanticError,false,(*counter),false);
					//printf("CAST\n");
					tFooListElem temporary;
					temporary.type = firstOper->type;
					//printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
					temporary.is_define = true;
					temporary.id = selectTmp(i,temporary.type,(*counter));
					result->firstToken->type = temporary.type;
					result->firstToken->info = temporary.id;
					exprResult.type = temporary.type;
					exprResult.id = temporary.id;
					printValue("LT",&temporary,secondOper,firstOper);
				}
				semanticless:
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type== GREATER)
			{
//				printf("****E -> E>E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = "EXPR";
				if(*(semanticError)==0)
				{
					tFooListElem *firstOper;
					tFooListElem *secondOper;
					if(stackTop(rStack) != NULL)
					{
						if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
							if(find_test(localTable,stackTop(rStack)->firstToken->info))
							{
								firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
							}
							else
							{
								setSemanticError(semanticError,SEMANTIC_REDEF);
								goto semanticgreater;
							}
						else
						{
							firstOper = myMalloc(sizeof(tFooListElem));
							firstOper->id = stackTop(rStack)->firstToken->info;
							firstOper->type = stackTop(rStack)->firstToken->type;
						}
					}
					if(fID.firstToken->type == IDENTIFIER)
					{
						if(find_test(localTable,fID.firstToken->info))
						{
							secondOper = function_find(localTable,fID.firstToken->info);
						}
						else
						{
							setSemanticError(semanticError,SEMANTIC_REDEF);
							goto semanticgreater;
						}
					}
					else
					{
						secondOper = myMalloc(sizeof(tFooListElem));
						secondOper->id = fID.firstToken->info;
						secondOper->type = fID.firstToken->type;
					}
					convertTo(firstOper,secondOper,semanticError,false,(*counter),false);
					//printf("CAST\n");
					if(secondOper != NULL && firstOper != NULL)
					{
						int i =0;
						if(*(operationCompare))
						{
							
							i++;
						}
						tFooListElem temporary;
						temporary.type = firstOper->type;
						//printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
						temporary.is_define = true;
						temporary.id = selectTmp(i,temporary.type,(*counter));
						result->firstToken->type = temporary.type;
						result->firstToken->info = temporary.id;
						exprResult.type = temporary.type;
						exprResult.id = temporary.id;
						printValue("GT",&temporary,secondOper,firstOper);
					}
				}
				semanticgreater:
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type== EQUAL)
			{
				//printf("****E -> E=E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = "EXPR";
				if(*(semanticError)==0)
				{
					tFooListElem *firstOper;
					tFooListElem *secondOper;
					if(stackTop(rStack) != NULL)
					{
						if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
							if(find_test(localTable,stackTop(rStack)->firstToken->info))
							{
								firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
							}
							else
							{
								setSemanticError(semanticError,SEMANTIC_REDEF);
								goto semanticequal;
							}
						else
						{
							firstOper = myMalloc(sizeof(tFooListElem));
							firstOper->id = stackTop(rStack)->firstToken->info;
							firstOper->type = stackTop(rStack)->firstToken->type;
						}
					}
					if(fID.firstToken->type == IDENTIFIER)
					{
						if(find_test(localTable,fID.firstToken->info))
						{
							secondOper = function_find(localTable,fID.firstToken->info);
						}
						else
						{
							setSemanticError(semanticError,SEMANTIC_REDEF);
							goto semanticequal;
						}
					}
					else
					{
						secondOper = myMalloc(sizeof(tFooListElem));
						secondOper->id = fID.firstToken->info;
						secondOper->type = fID.firstToken->type;
					}
					//printf("CAST\n");
					if(secondOper != NULL && firstOper != NULL)
					{
												int i =0;
						if(*(operationCompare))
						{
							
							i++;
						}
						convertTo(firstOper,secondOper,semanticError,true,(*counter),false);
						tFooListElem temporary;
						temporary.type = firstOper->type;
						//printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
						temporary.is_define = true;
						temporary.id = selectTmp(i,temporary.type,(*counter));
						result->firstToken->type = temporary.type;
						result->firstToken->info = temporary.id;
						exprResult.type = temporary.type;
						exprResult.id = temporary.id;
						printValue("EQ",&temporary,secondOper,firstOper);
					}
				}
				semanticequal:
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type== GREATER_EQUAL)
			{
				//printf("****E -> E>=E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = "EXPR";
				if(*(semanticError)==0)
				{
					tFooListElem *firstOper;
					tFooListElem *secondOper;
					if(stackTop(rStack) != NULL)
					{
						if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
							if(find_test(localTable,stackTop(rStack)->firstToken->info))
							{
								firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
							}
							else
							{
								setSemanticError(semanticError,SEMANTIC_REDEF);
								goto semanticgreaterequal;
							}
						else
						{
							firstOper = myMalloc(sizeof(tFooListElem));
							firstOper->id = stackTop(rStack)->firstToken->info;
							firstOper->type = stackTop(rStack)->firstToken->type;
						}
					}
					if(fID.firstToken->type == IDENTIFIER)
					{
						if(find_test(localTable,fID.firstToken->info))
						{
							secondOper = function_find(localTable,fID.firstToken->info);
						}
						else
						{
							setSemanticError(semanticError,SEMANTIC_REDEF);
							goto semanticgreaterequal;
						}
					}
					else
					{
						secondOper = myMalloc(sizeof(tFooListElem));
						secondOper->id = fID.firstToken->info;
						secondOper->type = fID.firstToken->type;
					}
					//printf("CAST\n");
					if(secondOper != NULL && firstOper != NULL)
					{
												int i =0;
						if(*(operationCompare))
						{
							
							i++;
						}
						convertTo(firstOper,secondOper,semanticError,false,(*counter),false);
						tFooListElem temporary;
						temporary.type = firstOper->type;
						//printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
						temporary.is_define = true;
						temporary.id = selectTmp(i,temporary.type,(*counter));
						result->firstToken->type = temporary.type;
						result->firstToken->info = temporary.id;
						exprResult.type = temporary.type;
						exprResult.id = temporary.id;
						printValue("LT",&temporary,secondOper,firstOper);
						printValue("NOT",&temporary,&temporary,NULL);
					}
				}
				semanticgreaterequal:
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type ==  LESS_EQUAL)
			{
				//printf("****E -> E<=E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = "EXPR";
				if(*(semanticError)==0)
				{
					tFooListElem *firstOper;
					tFooListElem *secondOper;
					if(stackTop(rStack) != NULL)
					{
						if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
							if(find_test(localTable,stackTop(rStack)->firstToken->info))
							{
								firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
							}
							else
							{
								setSemanticError(semanticError,SEMANTIC_REDEF);
							}
						else
						{
							firstOper = myMalloc(sizeof(tFooListElem));
							firstOper->id = stackTop(rStack)->firstToken->info;
							firstOper->type = stackTop(rStack)->firstToken->type;
						}
					}
					if(fID.firstToken->type == IDENTIFIER)
					{
						if(find_test(localTable,fID.firstToken->info))
						{
							secondOper = function_find(localTable,fID.firstToken->info);
						}
						else
						{
							setSemanticError(semanticError,SEMANTIC_REDEF);
						}
					}
					else
					{
						secondOper = myMalloc(sizeof(tFooListElem));
						secondOper->id = fID.firstToken->info;
						secondOper->type = fID.firstToken->type;
					}
					//printf("CAST\n");
					if(secondOper != NULL && firstOper != NULL)
					{
												int i =0;
						if(*(operationCompare))
						{
							
							i++;
						}
						convertTo(firstOper,secondOper,semanticError,false,(*counter),false);
						tFooListElem temporary;
						temporary.type = firstOper->type;
						//printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
						temporary.is_define = true;
						temporary.id = selectTmp(i,temporary.type,(*counter));
						result->firstToken->type = temporary.type;
						result->firstToken->info = temporary.id;
						exprResult.type = temporary.type;
						exprResult.id = temporary.id;
						//printf("%s ",temporary.id);
						printValue("GT",&temporary,secondOper,firstOper);
						printValue("NOT",&temporary,&temporary,NULL);
					}
				}
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			else if(stackTop(rStack)->firstToken->type == INEQUALITY)
			{
				//printf("****E -> E<>E****\n");
				stackPop(rStack);	//popnutí PLUS
				tReductToken *result =	(tReductToken*)myMalloc(sizeof(tReductToken));
				result->firstToken = (token*)myMalloc(sizeof(token));
				result->firstToken->info = "EXPR";
				if(*(semanticError)==0)
				{
					tFooListElem *firstOper;
					tFooListElem *secondOper;
					if(stackTop(rStack) != NULL)
					{
						if(stackTop(rStack)->firstToken->type ==IDENTIFIER)
							if(find_test(localTable,stackTop(rStack)->firstToken->info))
							{
								firstOper = function_find(localTable,stackTop(rStack)->firstToken->info);
							}
							else
							{
								setSemanticError(semanticError,SEMANTIC_REDEF);
								goto semanticinequality;
							}
						else
						{
							firstOper = myMalloc(sizeof(tFooListElem));
							firstOper->id = stackTop(rStack)->firstToken->info;
							firstOper->type = stackTop(rStack)->firstToken->type;
						}
					}
					if(fID.firstToken->type == IDENTIFIER)
					{
						if(find_test(localTable,fID.firstToken->info))
						{
							secondOper = function_find(localTable,fID.firstToken->info);
						}
						else
						{
							setSemanticError(semanticError,SEMANTIC_REDEF);
							goto semanticinequality;
						}
					}
					else
					{
						secondOper = myMalloc(sizeof(tFooListElem));
						secondOper->id = fID.firstToken->info;
						secondOper->type = fID.firstToken->type;
					}
					//printf("CAST\n");
					if(secondOper != NULL && firstOper != NULL)
					{
						int i =0;
						if(*(operationCompare))
						{
							
							i++;
						}
						convertTo(firstOper,secondOper,semanticError,true,(*counter),false);
						tFooListElem temporary;
						temporary.type = firstOper->type;
						//printf("%s  %s\n",strValueOfEnum(firstOper->type),strValueOfEnum(temporary->type));
						temporary.is_define = true;
						temporary.id = selectTmp(i,temporary.type,(*counter));
						result->firstToken->type = temporary.type;
						result->firstToken->info = temporary.id;
						exprResult.type = temporary.type;
						exprResult.id = temporary.id;
						printValue("EQ",&temporary,secondOper,firstOper);
						printValue("NOT",&temporary,&temporary,NULL);
					}
				}
				semanticinequality:
				stackPop(rStack);	//popnutí stacku
				stackPush(st,(*result));
				(*reduct) = true;
			}
			}
		}
	}
	return;
}

