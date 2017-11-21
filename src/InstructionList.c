/*
 * InstructionList.c
 *
 *  Created on: 14. 11. 2017
 *      Author: adam
 */
#include "InstructionList.h"

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
		error_msg(0,"Malloc elementu se nepovedl.");
	}
	else
	{
		elem->Instruction = Instruction;
		elem->nextPtr = NULL;

		if(instrList->First != NULL)
		{
			instrList->Last->nextPtr = elem;
		}else
			instrList->First = elem;
		instrList->Last = elem;
		instrList->Active = elem;
	}

}

void LInsert(tInstructionList *instrList,EINSTRUCTION instruction,void *result,void *arg1,void *arg2)
{
	tInstruction instr;
	instr.Instruction = instruction;
	instr.arg1 = arg1;
	instr.arg2 = arg2;
	instr.result = result;
	LSimpleInsert(instrList,&instr);
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
	return instrList->Active->Instruction;
}

void PrintInstrList(tInstructionList *instrList)
{
	if(instrList != NULL)
	{
		for(tListElement *elem = instrList->Last;elem != NULL; elem = elem->nextPtr)
			printf("%i \n", (EINSTRUCTION)elem->Instruction->Instruction);
	}
}

void *TokenToTypeConversion(token *tok)
{
	if(tok->type == VALUE_INTEGER)
	{
		void *tmp = myMalloc(sizeof(int));
		*((int*)tmp) = atoi(tok->info);
		if(tmp == NULL)
		{
			error_msg(0,"Malloc elementu se nepovedl.");
		}
		return tmp;
	}
	if(tok->type == VALUE_DOUBLE)
	{
		void *tmp = myMalloc(sizeof(double));
		*((double*)tmp) = atoi(tok->info);
		if(tmp == NULL)
		{
			error_msg(0,"Malloc elementu se nepovedl.");
		}
		return tmp;
	}
	if(tok->type == VALUE_STRING)
	{
		void *tmp = myMalloc(((sizeof(char))*strlen(tok->info))+1);
		if(tmp== NULL)
		{
			error_msg(0,"Malloc elementu se nepovedl.");
		}
		strcpy(tmp,tok->info);
		return tmp;
	}
	if(tok->type == TRUE)
	{
		void *tmp = myMalloc(sizeof(bool));
		*((bool*)tmp) = true;
		if(tmp == NULL)
		{
			error_msg(0,"Malloc elementu se nepovedl.");
		}
		return tmp;
	}
	if(tok->type == FALSE)
	{
		void *tmp = myMalloc(sizeof(bool));
		*((bool*)tmp) = false;
		if(tmp == NULL)
		{
			error_msg(0,"Malloc elementu se nepovedl.");
		}
		return tmp;
	}
	return NULL;
}


