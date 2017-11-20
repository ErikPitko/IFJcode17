#ifndef _STACK_H_
#define _STACK_H_

#include "scanner.h"
#include "garbage.h"
#include "header.h"
#include <stdio.h>

extern unsigned STACK_SIZE;

/* ADT zásobník implementovaný ve statickém poli */
typedef struct {
	unsigned size;
	tReductToken *arr; /* pole pro uložení hodnot */
	int top; /* index prvku na vrcholu zásobníku */
} tStack;

/**
 * @brief Provede inicializaci zásobníku - nastaví vrchol zásobníku.
 */
void stackInit(tStack* s);

/**
 * @brief Vrací nenulovou hodnotu, pokud je zásobník prázdný, jinak vrací hodnotu 0.
 */
int stackEmpty(const tStack* s);

/**
 * @brief Vrací nenulovou hodnotu, je-li zásobník plný, jinak vrací hodnotu 0.
 */
int stackFull(const tStack* s);

/**
 * @brief Vrací znak z vrcholu zásobníku.
 */
tReductToken *stackTop( const tStack* s);
/**
 * @brief Vrací předposlední znak z vrcholu zásobníku.
 */
tReductToken *stackBeforeTop ( const tStack* s);
/**
 * @brief Vrací velikost zásobníku.
 */
int stackLenght(tStack *s);
/**
 * @brief Odstraní prvek z vrcholu zásobníku
 */
void stackPop( tStack* s);

tReductToken *stackTopPop ( tStack* s);
/**
 * @brief Vloží znak na vrchol zásobníku.
 */
void stackPush(tStack* s, tReductToken token);
/**
 * @brief Vloží znak pod vrchol zásobníku.
 */
void stackPushUnderTop(tStack* s, tReductToken token);
void stackPrint(char* stackName, tStack* s);
#endif
