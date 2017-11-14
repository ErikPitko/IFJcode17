/*
 * InstructionList.c
 *
 *  Created on: 14. 11. 2017
 *      Author: adam
 */
#include "InstructionList.h"
#include "garbage.h"
#include "scanner.h"
#include "error.h"
#include <stdbool.h>
tInstructionList ListOfInstructions;

void LInit(tInstructionList *Instr_List)
{
	Instr_List->Active = NULL;
	Instr_List->First = NULL;
	Instr_List->Last = NULL;
}

void LClear(tInstructionList *instrList)
{
	for(tListElement elem;instrList->First != NULL;elem = instrList->First,instrList->First->nextPtr)
	{
		myFree(elem);
	}
	instrList->Active = NULL;
}
void LSimpleInsert(tInstructionList *instrList,tInstruction Instruction)
{
	tListElement elem;
	if((elem =myMalloc(sizeof(struct tListElement))) == NULL)
	{
		error_msg("Malloc elementu se nepovedl.");
	}
	else
	{
		elem->Instruction = Instruction;
		elem->nextPtr = NULL;
		/*
		if(instrList->First != NULL)
		{
			instrList->Last->nextPtr = elem;
		}else
			instrList ->First = elem;
		 */
		instrList = instrList->First!= NULL?instrList->Last->nextPtr = elem: instrList ->First = elem;
		instrList->Last = elem;
		instrList->Active = elem;
	}

}

void LInsert(tInstructionList *instrList,EINSTRUCTION instruction,void *arg1,void *arg2,void *result)
{
	tInstruction instr;
	instr.Instruction = instruction;
	instr.arg1 = arg1;
	instr.arg2 = arg2;
	instr.result = result;
	LInsert(instrList,instr);
}

void LSetStart(tInstructionList *instrList)
{
	instrList->Active = instrList->First;
}
void LFind(tInstructionList *instrList, void* address)
{
	instrList->Active = (tListElement) address;
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
	return instrList->Active->Instruction;
}
void *TokenToTypeConversion(token tok)
{
	if(tok->type == VALUE_INTEGER)
	{
		void *tmp = (int)myMalloc(sizeof(int));
		*((int*)tmp) = atoi(tok->info);
		if(tmp == NULL)
		{
			error_msg("Malloc elementu se nepovedl.");
		}
		return tmp;
	}
	if(tok->type == VALUE_DOUBLE)
	{
		void *tmp = (float)myMalloc(sizeof(float));
		*((float*)tmp) = atoi(tok->info);
		if(tmp == NULL)
		{
			error_msg("Malloc elementu se nepovedl.");
		}
		return tmp;
	}
	if(tok->type == VALUE_STRING)
	{
		void *tmp = mymalloc(((sizeof(char))*strlen(tok->info))+1);
		if(tmp== NULL)
		{
			error_msg("Malloc elementu se nepovedl.");
		}
		strcpy(tmp,token);
		return tmp;
	}
	if(tok->type == TRUE)
	{
		void *tmp = (bool)myMalloc(sizeof(bool));
		*((bool*)tmp) = true;
		if(tmp == NULL)
		{
			error_msg("Malloc elementu se nepovedl.");
		}
		return tmp;
	}
	if(tok->type == FALSE)
	{
		void *tmp = (bool)myMalloc(sizeof(bool));
		*((bool*)tmp) = false;
		if(tmp == NULL)
		{
			error_msg("Malloc elementu se nepovedl.");
		}
		return tmp;
	}
	return NULL;
}


