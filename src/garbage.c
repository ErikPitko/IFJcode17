#include <stdio.h>
#include "garbage.h"

htab_t *htable;


/**************HEADER*************/
void *hTableAdd(void *key);
int hTableRemove(void* key);
/*********************************/

size_t hash_function(const void* val) {
	uintptr_t ad = (uintptr_t) val;
	return ((size_t) (ad ^ (ad >> 16)));
}

void mem_clear() {
	htab_listitem* item = NULL;
	htab_listitem* prev = NULL;

	for (unsigned i = 0; i < htable->arr_size; i++) {
		prev = htable->list[i];
		item = htable->list[i];
		while (item) {
			item = item->next;
			free(prev->key);
			free(prev);
			htable->n--;
			prev = item;
		}
		htable->list[i] = NULL;
	}
}

void garbageFree() {
	if (!htable)
		return;
	mem_clear(htable);
	free(htable);
}

int garbageInit(unsigned size) {
	htable = malloc(sizeof(htab_t) + size * sizeof(htab_listitem*));

	if (!htable)
		return (0);

	htable->arr_size = size;
	htable->n = 0;
	return (1);
}

/**
 * @brief function for creating & allocing space for new item
 */
static htab_listitem* create_item(void *key) {
	htab_listitem* item;

	item = malloc(sizeof(htab_listitem));
	if (!item)
		return (NULL);

	item->key = key;
	item->next = NULL;
	return (item);
}

void *myMalloc(size_t n) {
	void *key = malloc(n);
	if(key == NULL){
		error_msg(99, "Could not allocate memory");
	}
	unsigned index = hash_function(key) % htable->arr_size;

	htab_listitem* item = htable->list[index];
	htab_listitem* prev = NULL;

	if (item == NULL) {
		// null pointer from hashtable
		item = htable->list[index] = create_item(key);
		if (item == NULL)
			return (NULL); // cant malloc memory
		htable->n++;
	} else {
		// cycle through items in list and search for item
		for (; item != NULL; item = item->next) {
			if(item->key != NULL)
			if (item->key == key) {
				return NULL;
			}
			prev = item;
		}

		// item not found, create one
		item = create_item(key);

		if (!item)
			return (NULL); // cant malloc memory

		prev->next = item;
		htable->n++;
	}
	return (item->key);
}

void *hTableAdd(void *key) {
	unsigned index = hash_function(key) % htable->arr_size;

	htab_listitem* item = htable->list[index];
	htab_listitem* prev = NULL;

	if (item == NULL) {
		// null pointer from hashtable
		item = htable->list[index] = create_item(key);
		if (item == NULL)
			return (NULL); // cant malloc memory
		htable->n++;
	} else {
		// cycle through items in list and search for item
		for (; item != NULL; item = item->next) {
			if(item->key != NULL)
			if (item->key == key) {
				return NULL;
			}
			prev = item;
		}

		// item not found, create one
		item = create_item(key);

		if (!item)
			return (NULL); // cant malloc memory

		prev->next = item;
		htable->n++;
	}
	return (item->key);
}

htab_listitem *htab_find(void* key){
	unsigned index = hash_function(key) % htable->arr_size;

	// cycle through items
	for(htab_listitem* item = htable->list[index]; item != NULL; item = item->next){
		if(item->key == key)
			return (item); // item found
	}

	return (NULL);
}

void *myRealloc(void* key, size_t n){
	if(key == NULL)
		return (myMalloc(n));

	htab_listitem *temp;
	if((temp = htab_find(key)) != NULL){
		if(hTableRemove(temp->key))
			warning_msg("Could not remove item from garbage htable");
		void* new = realloc(key, n);
		if(new == NULL)
			error_msg(99, "Could not allocate memory");
		hTableAdd(new);
		return (new);
	}

	return NULL;
}

int hTableRemove(void* key) {
	if(!key)
		return(0);
	unsigned index = hash_function(key) % htable->arr_size;
	htab_listitem* prev = htable->list[index];

	for (htab_listitem* item = htable->list[index]; item != NULL; item =
			item->next) {
		if (item->key == key) {
			if (item == htable->list[index]) {
				htable->list[index] = item->next;
			}else
			prev->next = item->next;
			free(item);
			htable->n--;
			return (0);
		}
		prev = item;
	}
	return (1);
}

int myFree(void* key) {
	if(!key)
		return(0);
	unsigned index = hash_function(key) % htable->arr_size;
	htab_listitem* prev = htable->list[index];

	for (htab_listitem* item = htable->list[index]; item != NULL; item =
			item->next) {
		if (item->key == key) {
			if (item == htable->list[index]) {
				htable->list[index] = item->next;
			}else
			prev->next = item->next;

			free(item->key);
			free(item);
			htable->n--;
			return (0);
		}
		prev = item;
	}
	return (1);
}
