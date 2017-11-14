#ifndef _STACK_H_
#define _STACK_H_

#include "scanner.h"
#include "garbage.h"
#include <stdio.h>

extern unsigned STACK_SIZE;

/* ADT zásobník implementovaný ve statickém poli */
typedef struct {
	unsigned size;
	struct structToken *arr; /* pole pro uložení hodnot */
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
token *stackTop( const tStack* s);

/**
 * @brief Odstraní prvek z vrcholu zásobníku
 */
void stackPop( tStack* s);

token *stackTopPop ( tStack* s);
/**
 * @brief Vloží znak na vrchol zásobníku.
 */
void stackPush(tStack* s, token token);

#endif
