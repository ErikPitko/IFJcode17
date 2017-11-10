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
 * @brief allocates memory and creates item in H-table
 */
void *myMalloc(size_t n);

/**
 * @brief frees memory and removes item from H-table
 * @returns non-zero on success
 */
int myFree(void* key);
void mem_clear();
void garbageFree();

#endif /* HTAB_H_ */
