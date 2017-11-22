/*
 * InstructionList.c
 *
 *  Created on: 14. 11. 2017
 *      Author: adam
 */
#include "InstructionList.h"

tInstructionList globalInstrList;

char* intInstructionToString(unsigned u)
{
	switch(u)
	{
	case 0: return "MOVE";
	case 1: return "CREATEFRAME";
	case 2: return "PUSHFRAME";
	case 3: return "POPFRAME";
	case 4: return "DEFVAR";
	case 5: return "CALL";
	case 6: return "RETURN";
	case 7: return "PUSHS";
	case 8: return "POPS";
	case 9: return "CLEARS";
	case 10: return "ADD";
	case 11: return "SUB";
	case 12: return "MUL";
	case 13: return "DIV";
	case 14: return "ADDS";
	case 15: return "SUBS";
	case 16: return "MULS";
	case 17: return "DIVS";
	case 18: return "LT";
	case 19: return "GT";
	case 20: return "EQ";
	case 21: return "LTS";
	case 22: return "GTS";
	case 23: return "EQS";
	case 24: return "AND";
	case 25: return "OR";
	case 26: return "NOT";
	case 27: return "ANDS";
	case 28: return "ORS";
	case 29: return "NOTS";
	case 30: return "INT2FLOAT";
	case 31: return "FLOAT2INT";
	case 32: return "FLOAT2R2EINT";
	case 33: return "FLOAT2R20INT";
	case 34: return "INT2CHAR";
	case 35: return "STRI2INT";
	case 36: return "INT2FLOATS";
	case 37: return "FLOAT2INTS";
	case 38: return "FLOAT2R2EINTS";
	case 39: return "FLOAT2R20INTS";
	case 40: return "INT2CHARS";
	case 41: return "STRI2INTS";
	case 42: return "READ";
	case 43: return "WRITE";
	case 44: return "CONCAT";
	case 45: return "STRLEN";
	case 46: return "GETCHAR";
	case 47: return "SETCHAR";
	case 48: return "TYPE";
	case 49: return "LABEL";
	case 50: return "JUMP";
	case 51: return "JUMPIFEQ";
	case 52: return "JUMPIFNEQ";
	case 53: return "JUMPIFEQS";
	case 54: return "JUMPIFNEQS";
	case 55: return "BREAK";
	case 56: return "DPRINT";
	}
	return "";
}

void LInit(tInstructionList *Instr_List)
{
	Instr_List->Active = NULL;
	Instr_List->First = NULL;
	Instr_List->Last = NULL;
}

void LClear(tInstructionList *instrList)
{
	for(tListElement *elem;instrList->First != NULL;elem = instrList->First)
	{
		instrList->First = instrList->First->nextPtr;
		myFree(elem);
	}
	instrList->Active = NULL;
}
void LSimpleInsert(tInstructionList *instrList,tInstruction *Instruction)
{
	tListElement *elem;
	if((elem =myMalloc(sizeof(struct tListElement))) == NULL)
	{
		error_msg(INTERNAL_ERROR,"Malloc elementu se nepovedl.");
	}
	elem->ptrIntruction = Instruction;
	elem->nextPtr = NULL;
	if(instrList->First != NULL)
	{
		instrList->Last->nextPtr = elem;
	}else
		instrList->First = elem;
	instrList->Last = elem;
	instrList->Active = elem;
}

void LInsert(tInstructionList *instrList,EINSTRUCTION instruction,param *result,param *arg1,param *arg2)
{
	tInstruction *instr;
	if((instr =myMalloc(sizeof(tInstruction))) == NULL)
	{
		error_msg(INTERNAL_ERROR,"Malloc elementu se nepovedl.");
	}
	instr->Instruction = instruction;
	instr->arg1 = arg1;
	instr->arg2 = arg2;
	instr->result = result;
	LSimpleInsert(instrList,instr);
}

void LSetStart(tInstructionList *instrList)
{
	instrList->Active = instrList->First;
}
void LFind(tInstructionList *instrList, void* address)
{
	instrList->Active = (tListElement*) address;
}
void LNext(tInstructionList *instrList)
{
	instrList->Active = instrList->Active->nextPtr;
}
void *LLast(tInstructionList *instrList)
{
	return (void*)instrList->Last;
}
void *LAct(tInstructionList *instrList)
{
	return (void*)instrList->Active;
}
tInstruction *GetInstructionFromActive(tInstructionList *instrList)
{
	return instrList->Active->ptrIntruction;
}

void PrintInstrList(tInstructionList *instrList)
{
	if(instrList != NULL)
	{
		int i = 0;
		for(tListElement *elem = instrList->First; elem != NULL ; elem = elem->nextPtr,i++)
		{
			printf("+++++++++++%i-Instruction: %s %s %s %s\t++++++++++++\n",i, intInstructionToString(elem->ptrIntruction->Instruction),(char*)elem->ptrIntruction->arg1,(char*)elem->ptrIntruction->arg2,(char*)elem->ptrIntruction->result );
		}
	}
}
