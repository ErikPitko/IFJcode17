/*
 * InstructionList.c
 *
 *  Created on: 14. 11. 2017
 *      Author: adam
 */
#include "InstructionList.h"
#include "garbage.h"

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
		garbageFree();
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
void *TokenToTypeConversion(ETokenConversion eToken,char *token)
{
	if(eToken == CHARTOINT)
	{
		return (myMalloc(sizeof(int))= atoi(token));
	}
	if(eToken == CHARTOFLOAT)
	{
		return (myMalloc(sizeof(float))= atoi(token));
	}
	if(eToken == CHARTOID)
	{
		void *tmp = mymalloc(((sizeof(char))*strlen(token))+1);
		strcpy(tmp,token);
		return tmp;
	}
	if(eToken == CHARTOTRUE)
	{
		return (myMalloc(sizeof(int))= 1);
	}
	if(eToken == CHARTOFALSE)
	{
		return (myMalloc(sizeof(int))= 0);
	}
	return NULL;
}


