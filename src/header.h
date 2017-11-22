/*
 * header.h
 *
 *  Created on: 15. 11. 2017
 *      Author: adam
 */

#ifndef HEADER_H_
#define HEADER_H_
#include "scanner.h"
#include "symtable.h"
typedef enum
{
	H,
	L,
	EQ,
	ERROR,
	NONE
}ePrecElem;

typedef enum
{
	/***************VOLÁNÍ FUNKCÍ*****************/
    I_MOVE = 0,     	//0		<var> <symb>
    I_CREATEFRAME,    	//1
    I_PUSHFRAME,        //2
    I_POPFRAME,    		//3
    I_DEFVAR,    		//4		<var>
    I_CALL,    			//5		<label>
    I_RETURN,    		//6
	/*********************************************/
	/*********PRÁCE S DATOVÝM ZÁSOBNÍKEM**********/
    I_PUSHS,    		//7		<symb>
    I_POPS,         	//8		<var>
    I_CLEARS,        	//9
	/*********ARITMETIKA BOOL A KONVERZE**********/
    I_ADD,       		//10	<var> <symb1> <symb2>
    I_SUB,         		//11	<var> <symb1> <symb2>
	I_MUL,         		//12	<var> <symb1> <symb2>
	I_DIV,         		//13	<var> <symb1> <symb2>
	//zásobníkové verze
	I_ADDS,				//14
    I_SUBS,    			//15
    I_MULS,    			//16
    I_DIVS,    			//17
	//< > ==
    I_LT,    			//18	<var> <symb1> <symb2>
    I_GT,  				//19	<var> <symb1> <symb2>
    I_EQ,        		//20	<var> <symb1> <symb2>
	I_LTS,				//21
	I_GTS,				//22
	I_EQS,				//23
	//&& || !
	I_AND,				//24	<var> <symb1> <symb2>
    I_OR,        		//25	<var> <symb1> <symb2>
    I_NOT,      		//26	<var> <symb1> <symb2>
	I_ANDS,				//27
	I_ORS,				//28
	I_NOTS,				//29
	//conversions
	I_INT2FLOAT,		//30	<var> <symb>
	I_FLOAT2INT,		//31	<var> <symb>
	I_FLOAT2R2EINT,		//32	<var> <symb>
	I_FLOAT2R2OINT,		//33	<var> <symb>
	I_INT2CHAR,			//34	<var> <symb>
	I_STRI2INT,			//35	<var> <symb1> <symb2>
	I_INT2FLOATS,		//36
	I_FLOAT2INTS,		//37
	I_FLOAT2R2EINTS,	//38
	I_FLOAT2R2OINTS,	//39
	I_INT2CHARS,		//40
	I_STRI2INTS,		//41
	/*********VSTUPNĚ VÝSTUPNÍ INSTRUKCE**********/
	I_READ,				//42	<var> <type>
	I_WRITE,			//43	<symb>
	/*********************************************/
	/*********PRÁCE S ŘETĚZCI**********/
	I_CONCAT,			//44	<var> <symb1> <symb2>
	I_STRLEN,			//45	<var> <symb>
	I_GETCHAR,			//46	<var> <symb1> <symb2>
	I_SETCHAR,			//47	<var> <symb1> <symb2>
	/**********************************/
	/*************TYPY************/
	I_TYPE,				//48	<var> <symb>
	/*****************************/
	/**********INSTRUKCE ŘÍZENÍ TOKU PROGRAMU*********/
	I_LABEL,			//49	<label>
	I_JUMP,				//50	<label>
	I_JUMPIFEQ,			//51	<label> <symb1> <symb2>
	I_JUMPIFNEQ,		//52	<label> <symb1> <symb2>
	I_JUMPIFEQS,		//53	<label>
	I_JUMPIFNEQS,		//54	<label>
	/*************************************************/
	/***************LADÍCÍ INSTRUKCE**************/
	I_BREAK,			//55
	I_DPRINT			//56
	/*********************************************/

} EINSTRUCTION;

typedef enum
{
	CHARTOFLOAT,
	CHARTOID,
	CHARTOINT,
	CHARTOTRUE,
	CHARTOFALSE
}ETokenConversion;

typedef struct
{
	token *firstToken;
	ePrecElem priority;
}tReductToken;

typedef struct
{
	EINSTRUCTION Instruction;
	param *arg1;
	param *arg2;
	param *result;
}tInstruction;

typedef struct tListElement
{
	struct tListElement *nextPtr;
	tInstruction *ptrIntruction;
}tListElement;
typedef struct
{
	tListElement *First;
	tListElement *Last;
	tListElement *Active;
}tInstructionList;

#endif /* HEADER_H_ */
