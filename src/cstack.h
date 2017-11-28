/*
 * cstack.h
 *
 *  Created on: Nov 27, 2017
 */

#ifndef CSTACK_H_
#define CSTACK_H_

#include "garbage.h"
#include <stdio.h>
extern unsigned CSTACK_SIZE;

/* ADT zásobník implementovaný ve statickém poli */
typedef struct {
	unsigned size;
	int *arr; /* pole pro uložení hodnot */
	int top; /* index prvku na vrcholu zásobníku */
} tcStack;

/**
 * @brief Provede inicializaci zásobníku - nastaví vrchol zásobníku.
 */
void cstackInit(tcStack* s);

/**
 * @brief Vrací nenulovou hodnotu, pokud je zásobník prázdný, jinak vrací hodnotu 0.
 */
int cstackEmpty(tcStack* s);

/**
 * @brief Vrací nenulovou hodnotu, je-li zásobník plný, jinak vrací hodnotu 0.
 */
int cstackFull(tcStack* s);

/**
 * @brief Vrací znak z vrcholu zásobníku.
 */
int *cstackTop(tcStack* s);
/**
 * @brief Vrací velikost zásobníku.
 */
int cstackLenght(tcStack *s);
/**
 * @brief Odstraní prvek z vrcholu zásobníku
 */
void cstackPop( tcStack* s);

int *cstackTopPop ( tcStack* s);
/**
 * @brief Vloží znak na vrchol zásobníku.
 */
void cstackPush(tcStack* s, int token);
void cstackPrint(char* stackName, tcStack* s);
void cstackClear(tcStack* s);

#endif /* CSTACK_H_ */
