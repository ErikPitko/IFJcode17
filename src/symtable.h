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
typedef struct 
{
	char *id;
	unsigned type;
	
	bool is_define; // Kontrola ci je to param
} symbol;

typedef struct 
{
	char *id;
	unsigned type;
	
} psymbol;

/******* Lokalna tabulka zo zoznamami ******/

typedef struct paramP // Prvky podzoznamu
{
	char *id; // odlis nazov
	unsigned type; // odlis nazov

	struct paramP *next_param;
} *param;	

typedef struct listPL // Podzoznam
{
	param First;
	param Act;
} list_param;

typedef struct itemI
{
	char *id; // sluzi ako kluc
	unsigned type; // typ
	bool is_define; // Kontrola ci je to param... Nechat alebo dat doprec??
	
	list_param *param;

	struct itemI *next_item; // Ukazatel na dalsi prvok	
} *list_item;

typedef struct listL // Zoznam
{
 	list_item Act; // Ukazatel na aktualny prvok
	list_item First; // Ukazatel na prvy prvok
} list;

typedef list list_array[MAX_SIZE]; // Pole zoznamov

/*********** Funkcie ***********/

int hash_code (const char *string);

// Inicializacia
list *ltab_init();

// Vkladanie funkcii a premennych
int list_insert(list *local_table, symbol sym);
// Vkladanie parametrov
int list_insert_param(list *local_table, symbol sym, psymbol psym);
// Podla id najde zaznam a vrati v tokene vsetky informacie... id, type, is_define
void find(list *local_table, symbol sym);
// Podla id najde zaznam parametru a vrati v tokene vsetky informacie... id, type
void param_find(list *local_table, symbol sym, psymbol psym);

// Zrusi celu tabulku
void ltab_destroy(list *local_table);

// Pomocna funkcia kontrolujuca duplicitu funkcii a premennych
int find_test(list *local_table, char *id);
// Pomocna funkcia kontrolujuca duplicitu paramtrov 
int find_param_test(list *local_table, char *id_fnc, char *id_param);
// Pomocna funkcia vracajuca pocet parametrov funkcie
int number_param(list *local_table, symbol sym);

#endif // SYMTABLE_H_

