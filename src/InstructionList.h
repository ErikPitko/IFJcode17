/*
 * InstructionList.h
 *
 *  Created on: 13. 11. 2017
 *      Author: adam
 */

#ifndef INSTRUCTIONLIST_H_
#define INSTRUCTIONLIST_H_
#include "header.h"
#include "garbage.h"
#include "scanner.h"
#include "error.h"
#include <stdbool.h>

extern tInstructionList globalInstrList;

void LInit(tInstructionList *instrList);
void LClear(tInstructionList *instrList);
void LSimpleInsert(tInstructionList *instrList,tInstruction *Instruction);
void LInsert(tInstructionList *instrList,EINSTRUCTION instruction,tFooListElem *result,tFooListElem *arg1,tFooListElem *arg2);
void LSetStart(tInstructionList *instrList);
void LFind(tInstructionList *instrList, void* address);
void LNext(tInstructionList *instrList);
void *LLast(tInstructionList *instrList);
void *LAct(tInstructionList *instrList);
tInstruction *GetInstructionFromActive(tInstructionList *instrList);
void PrintInstrList(tInstructionList *instrList);
#endif /* INSTRUCTIONLIST_H_ */
