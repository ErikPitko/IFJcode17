/*
 * htab.h
 *
 *  Created on: Apr 19, 2017
 *      Author:		Erik Pitko(xpitko00), FIT
 *      Compiled:	gcc 5.4.0
 */

#ifndef HTAB_H_
#define HTAB_H_

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief calculates index for given string
 */
size_t hash_function(const void *val);

typedef struct htab_listitem{
	void* key;
	struct htab_listitem *next;
} htab_listitem;

typedef struct htab{
	size_t arr_size;
	size_t n;
	htab_listitem *list[];
} htab_t;

/**
 * @brief initializes new H-table with given size
 */
int garbageInit(unsigned size);

/**
 * @brief creates new element in list on index calculated by hash_function()
 */
void *myMalloc(size_t n);

/**
 * @brief removes item with given key
 * @returns non-zero on success
 */
int myFree(void* key);
void mem_clear();
void garbageFree();

int main(void);

#endif /* HTAB_H_ */
