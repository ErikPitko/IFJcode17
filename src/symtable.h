/*
 * symtable.h
 *
 * login: xandri04
 */

#ifndef SYMTABLE_H_
#define SYMTABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "garbage.h"
#include "error.h"

#define MAX_SIZE 100 // Pocet zoznamov v tabulke 

// Pomocna struktura simulujuca "token" z syntaktickej analyzy
typedef struct paramP // Prvky podzoznamu
{
	char *id; // Identifikator
	int type; // Datovy typ
	bool is_define;
	struct paramP *next_param; // Ukazatel na dalsi parameter
} param;	

typedef struct // Podzoznam
{
	param *First;
	param *Act;
} tParamListElem;

typedef struct tSymFoo
{
	char *id; // Identifikator
	int type; // Datovy typ
	bool is_define; // Kontrola ci je funkcia definovana
	tParamListElem *param;

	struct tSymFoo *next_item; // Ukazatel na dalsi prvok	
} tFooListElem;

typedef struct  // Zoznam
{
 	tFooListElem *Act; // Ukazatel na aktualny prvok
	tFooListElem *First; // Ukazatel na prvy prvok
} tHashTable;

typedef tHashTable list_array[MAX_SIZE]; // Pole zoznamov

/*********** Funkcie ***********/

// Vypocita index zo zadaneho stringu
int hash_code (const char *string);
void print_Table(tHashTable *local_table);
// Inicializacia
tHashTable *ltab_init ();

// Vkladanie funkcii a premennych
int list_insert (tHashTable *local_table, tFooListElem sym);
// Vkladanie parametrov
int list_insert_param (tHashTable *local_table, tFooListElem sym, param psym);

// Podla id najde zaznam a vrati v tokene vsetky informacie... id, type, is_define
param *param_find (tHashTable *local_table,char* fooId, char* symId);
// Podla id najde zoznam parametru a vrati v tokene vsetky informacie... id, type
tFooListElem *function_find (tHashTable *local_table, char *fooId);

// Nastavuje isdefine na true
int change_isdefine (tHashTable *local_table, tFooListElem sym);
// Funkcia, ktora vracia index hladaneho parametru
int return_index_parameter (tHashTable *local_table, tFooListElem sym, param psym);

// Zisti pocet parametrov vo funkcii
int test_nubmer_param (tHashTable *local_table, char *id);

// Zrusi celu tabulku
void ltab_destroy (tHashTable *local_table);


//Pomocne funkcie
int find_test (tHashTable *local_table, char *id);
int find_param_test (tHashTable *local_table, char *id_fnc, char *id_param);

#endif // SYMTABLE_H_
