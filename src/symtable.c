/*
 * symtable.c
 *
 * login: xandri04
 */

#include "symtable.h"

/*
 * Funkcia: *ltab_init()
 * Popis: Funkcia inicializuje tabulku
 * Vracia: ukazatel
 */

int hash_code ( const char *string ) 
{
	int retval = 1;
	int keylen = strlen(string);
	for ( int i=0; i < keylen; i++ )
		retval += string[i];

	return ( retval % MAX_SIZE );
}

/*
 * Funkcia: *ltab_init()
 * Popis: Funkcia inicializuje tabulku
 * Vracia: ukazatel na tabulku
 */

list *ltab_init()
{
	list *local_table = myMalloc(sizeof(list) * MAX_SIZE); //naalokujeme miesto pre hash tabulku

 	for(int i = 0; i < MAX_SIZE; i++) //pre kazdy zoznam v tabulke
 	{
 		local_table[i].Act = NULL;
 		local_table[i].First = NULL;
 	}

 	return local_table; //vratime adresu
}

/*
 * Funkcia: list_insert
 * Popis: Funkcia  zavola funkciu find_test(local_table, sym.id), podla ktorej
 * bud vytvarame novy prvok, alebo vraciame chybu
 * Parametre: Ukazatel na tabulku, ukazatel na token
 * Vracia: 0 v pripade uspechu, -1 v pripade duplicity
 */

int list_insert(list *local_table, symbol sym)
{
	if(find_test(local_table, sym.id) != -1)
	{
		int idx = hash_code(sym.id);

		list_item pom = myMalloc(sizeof(struct itemI));

		if(pom == NULL)
			return 0;

 		pom->id = myMalloc(strlen(sym.id) * sizeof(char));

 		if (pom->id == NULL) // Kontrola malloc
 		{
			myFree(pom);
			return 0;
		}

		pom->param = myMalloc(sizeof(list_param));

		if (pom->param == NULL) // Kontrola malloc
		{
			myFree(pom);
			myFree(pom->id);
			return 0;
		}
		
		pom->param->First = NULL;		
		pom->param->Act = NULL;		
		
		strcpy(pom->id, sym.id);
		pom->type = sym.type;
		pom->is_define = sym.is_define;
	
		pom->next_item = local_table[idx].First;
		local_table[idx].First = pom;
	}
	else
		return -1; // Semanticka chyba

	return 0;
}


/*
 * Funkcia: list_insert_param
 * Popis: Funkcia kontroluje duplicitu parametrov a v pripade, ze duplicita nie je
 * tak vklada parametre do tabulky
 * Parametre: Ukazatel na tabulku, ukazatel na token
 * Vracia: 0 v pripade uspechu, -1 v pripade duplicity
 */

int list_insert_param(list *local_table, symbol sym, psymbol psym)
{
	if(find_param_test(local_table, sym.id, psym.id) != -1)
	{
		int idx = hash_code(sym.id);

		local_table[idx].Act = local_table[idx].First;

		while(local_table[idx].Act != NULL) //prejde vsetky prvky zoznamu
		{
			if(strcmp(local_table[idx].Act->id, sym.id) == 0) //porovna retazce
			{
				param *pom = myMalloc(sizeof(struct paramP));

				if(pom == NULL)
					return 0;
				
	 			pom->id = myMalloc(strlen(psym.id) * sizeof(char));
	
	 			if (pom->id == NULL) // Kontrola malloc	
				{
					free(pom);
					return 0;
				}
		
				strcpy(pom->id, psym.id);
				pom->type = psym.type;
	
				pom->next_param = local_table[idx].First->param->First;
				local_table[idx].First->param->First = pom;
			}

			local_table[idx].Act = local_table[idx].Act->next_item; //posunieme sa o prvok dalej
		}
	}
	else
		return -1;	
	
	return 0;
}

/*
 * Funkcia: find(list *local_table, symbol sym)
 * Popis: 
 * Vracia:
 */

void find(list *local_table, symbol sym)
{
	int idx = hash_code(sym.id);

	local_table[idx].Act = local_table[idx].First;

	while(local_table[idx].Act != NULL) //prejde vsetky prvky zoznamu
	{
		if(strcmp(local_table[idx].Act->id, sym.id) == 0) //porovna retazce
		{
			sym.id = local_table[idx].Act->id;
			sym.type = local_table[idx].Act->type;
			sym.is_define = local_table[idx].Act->is_define;
		}
		local_table[idx].Act = local_table[idx].Act->next_item; //posunieme sa o prvok dalej
	}

}

/*
 * Funkcia: param_find(list *local_table, symbol sym, psymbol psym)
 * Popis: 
 * Vracia:
 */

void param_find(list *local_table, symbol sym, psymbol psym)
{
	int idx = hash_code(sym.id);

	local_table[idx].Act = local_table[idx].First;

	while(local_table[idx].Act != NULL) //prejde vsetky prvky zoznamu
	{
		if(strcmp(local_table[idx].Act->id, sym.id) == 0) //porovna retazce
		{
			local_table[idx].Act->param->Act = local_table[idx].First->param->First;
	
			while(local_table[idx].Act->param->Act != NULL) //prejde vsetky prvky zoznamu
			{
				if(strcmp(local_table[idx].Act->param->Act->id, psym.id) == 0) //porovna retazce
				{
					psym.id = local_table[idx].Act->param->Act->id;
					psym.type = local_table[idx].Act->param->Act->type;
				}

				local_table[idx].Act->param->Act = local_table[idx].Act->param->Act->next_param; //posunieme sa o prvok dalej
			}
		}

		local_table[idx].Act = local_table[idx].Act->next_item; //posunieme sa o prvok dalej
	}

}

void change_isdefine(list *local_table, symbol sym)
{
	int i = hash_code(sym.id);		

	local_table[i].Act = local_table[i].First;

	while(local_table[i].Act != NULL) // Prejde vsetky prvky zoznamu
	{
		if(strcmp(local_table[i].Act->id, sym.id) == 0) // Porovna retazce
		{
			local_table[i].Act->is_define = true;
		}			

		local_table[i].Act = local_table[i].Act->next_item; // Posunieme sa o prvok dalej
	}
}


/*
 * Funkcia: ltab_destroy
 * Popis: Funkcia zmaze celu tabulku
 * Parametre: Ukazatel na tabulku
 * Vracia: nic
 */

void ltab_destroy(list *local_table)
{
	for(int i = 0; i < MAX_SIZE; i++) // Pre kazdy zoznam v tabulke
 	{
 		while(local_table[i].First != NULL) // Prejde vsetky prvky zoznamu
 		{
			while(local_table[i].First->param->Act != NULL)
			{
				if(local_table[i].First->param->First == local_table[i].Act->param->Act) // Zrusi aj aktivny prvok
 				{
 					local_table[i].Act->param->Act = NULL;
 				}
	
 				param *pom2 = local_table[i].First->param->First;
 				local_table[i].First->param->First = local_table[i].First->param->First->next_param;
 				myFree(pom2->id);
 				myFree(pom2);
			}			

 			if(local_table[i].First == local_table[i].Act) // Zrusi aj aktivny prvok
 			{
 				local_table[i].Act = NULL;
 			}

			

 			list_item pom = local_table[i].First;
 			local_table[i].First = local_table[i].First->next_item;
 			myFree(pom->id);
 			myFree(pom); // Uvolnime prvok zoznamu
 		}
 	}

 	myFree(local_table); // Uvolnime tabulku
}

/*************************************************/
/*		  Pomocne funkcie		 */
/*************************************************/

/*
 * Funkcia: find_test
 * Popis: Funkcia sluzi ako pomocna funkcia, ktora vyhladavanie v tabulke podla "id" 
 * Parametre: Ukazatel na tabulku, premennu "id"
 * Vracia: 0 v pripade nenajdenia prvku, -1 v pripade najdenia prvku
 */

int find_test(list *local_table, char *id)
{
	int i = hash_code(id);
	
	local_table[i].Act = local_table[i].First;

	while(local_table[i].Act != NULL) // Prejde vsetky prvky zoznamu
	{
		if(strcmp(local_table[i].Act->id, id) == 0) // Porovna retazce
			return -1;			

		local_table[i].Act = local_table[i].Act->next_item; // Posunieme sa o prvok dalej
	}
	
	return 0;
}

/*
 * Funkcia: find_param_test
 * Popis: Funkcia sluzi ako pomocna funkcia, ktora vyhladavanie v tabulke podla "id_fnc" a  "id_param"
 * Parametre: Ukazatel na tabulku, premennu "id"
 * Vracia: 0 v pripade nenajdenia prvku, -1 v pripade najdenia prvku
 */

int find_param_test(list *local_table, char *id_fnc, char *id_param)
{
	int idx = hash_code(id_fnc);

	local_table[idx].Act = local_table[idx].First;

	while(local_table[idx].Act != NULL) //prejde vsetky prvky zoznamu
	{
		if(strcmp(local_table[idx].Act->id, id_fnc) == 0) //porovna retazce
		{
			local_table[idx].Act->param->Act = local_table[idx].First->param->First;

	
			while(local_table[idx].Act->param->Act != NULL) //prejde vsetky prvky zoznamu
			{
				if(strcmp(local_table[idx].Act->param->Act->id, id_param) == 0) //porovna retazce
				{
					return -1;
				}

				local_table[idx].Act->param->Act = local_table[idx].Act->param->Act->next_param; //posunieme sa o prvok dalej
			}
		}

		local_table[idx].Act = local_table[idx].Act->next_item; //posunieme sa o prvok dalej
	}

	return 0;
}

// Funkcia ktora vracia pocet parametrov funkcie 

int number_param(list *local_table, symbol sym)
{
	int idx = hash_code(sym.id);
	int counter = 0;	

	local_table[idx].Act = local_table[idx].First;

	while(local_table[idx].Act != NULL) //prejde vsetky prvky zoznamu
	{
		if(strcmp(local_table[idx].Act->id, sym.id) == 0) //porovna retazce
		{
			local_table[idx].Act->param->Act = local_table[idx].First->param->First;
	
			while(local_table[idx].Act->param->Act != NULL) //prejde vsetky prvky zoznamu
			{
				counter++;	
				
				local_table[idx].Act->param->Act = local_table[idx].Act->param->Act->next_param; //posunieme sa o prvok dalej
			}
		}

		local_table[idx].Act = local_table[idx].Act->next_item; //posunieme sa o prvok dalej
	}
	
	return counter;
}

