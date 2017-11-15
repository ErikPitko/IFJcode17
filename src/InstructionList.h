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
#include "scanner.h"
#include <stdbool.h>
void LInit(tInstructionList *instrList);
void LClear(tInstructionList *instrList);
void LSimpleInsert(tInstructionList *instrList,tInstruction Instruction);
void LInsert(EINSTRUCTION instruction,void *result,void *arg1,void *arg2);
void LSetStart(tInstructionList *instrList);
void LFind(tInstructionList *instrList, void* address);
void LNext(tInstructionList *instrList);
void *LLast(tInstructionList *instrList);
void *LAct(tInstructionList *instrList);
tInstruction *GetInstructionFromActive(tInstructionList *instrList);
void *TokenToTypeConversion(token *tok);

#endif /* INSTRUCTIONLIST_H_ */
