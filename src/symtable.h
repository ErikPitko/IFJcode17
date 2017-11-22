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

typedef struct listPL // Podzoznam
{
	param *First;
	param *Act;
} list_param;

typedef struct itemI
{
	char *id; // Identifikator
	int type; // Datovy typ
	bool is_define; // Kontrola ci je funkcia definovana
	
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

// Vypocita index zo zadaneho stringu
int hash_code (const char *string);

// Inicializacia
list *ltab_init ();

// Vkladanie funkcii a premennych
int list_insert (list *local_table, param sym);
// Vkladanie parametrov
int list_insert_param (list *local_table, param sym, param psym);

// Podla id najde zaznam a vrati v tokene vsetky informacie... id, type, is_define
param *find (list *local_table, char* symId);
// Podla id najde zoznam parametru a vrati v tokene vsetky informacie... id, type
param *param_find (list *local_table, param sym, param psym);

// Nastavuje isdefine na true
int change_isdefine (list *local_table, param sym);
// Funkcia, ktora vracia index hladaneho parametru
int return_index_parameter (list *local_table, param sym, param psym);

// Zisti pocet parametrov vo funkcii
int test_nubmer_param (list *local_table, char *id);

// Zrusi celu tabulku
void ltab_destroy (list *local_table);


//Pomocne funkcie
int find_test (list *local_table, char *id);
int find_param_test (list *local_table, char *id_fnc, char *id_param);

#endif // SYMTABLE_H_
