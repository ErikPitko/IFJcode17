/*
 * symtable.c
 *
 * login: xandri04
 */

#include "symtable.h"

/*
 * Funkcia: hash_code()
 * Popis: Funkcia vypocita index zo zadaneho stringu
 * Vracia: ukazatel
 */

int hash_code (const char *string) 
{
	int retval = 1;
	int keylen = strlen(string);
	for (int i = 0; i < keylen; i++)
		retval += string[i];

	return (retval % MAX_SIZE);
}

/*
 * Funkcia: *ltab_init()
 * Popis: Funkcia inicializuje tabulku zoznamov
 * Vracia: ukazatel na tabulku
 */

tHashTable *ltab_init ()
{
	tHashTable *local_table = myMalloc(sizeof(tHashTable) * MAX_SIZE); // Alokujeme miesto pre hash tabulku
	
 	for (int i = 0; i < MAX_SIZE; i++)
 	{
 		local_table[i].Act = NULL;
 		local_table[i].First = NULL;
 	}

 	return local_table;
}

/*
 * Funkcia: list_insert()
 * Popis: Funkcia kontroluje duplicitu prvkov, v pripade ze duplicita nenastala 
 * vytvori novy prvok v zozname
 * Vracia: 0 v pripade uspechu, -1 v pripade duplicity
 */

int list_insert (tHashTable *local_table, tFooListElem sym)
{
	if (find_test(local_table, sym.id) != -1)
	{
		int idx = hash_code(sym.id);

		tFooListElem pom = myMalloc(sizeof(struct tSymFoo));

		if (pom == NULL)
			error_msg(INTERNAL_ERROR, "Nepodarilo sa alokovat miesto\n");

		
 		pom->id = myMalloc(strlen(sym.id) * sizeof(char));

 		if (pom->id == NULL)
 		{
			myFree(pom);
			error_msg(INTERNAL_ERROR, "Nepodarilo sa alokovat miesto\n");
		}

		pom->param = myMalloc(sizeof(tParamListElem));

		if (pom->param == NULL)
		{
			myFree(pom);
			myFree(pom->id);
			error_msg(INTERNAL_ERROR, "Nepodarilo sa alokovat miesto\n");
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
 * Funkcia: list_insert_param()
 * Popis: Funkcia kontroluje duplicitu parametrov a v pripade, ze duplicita nie je
 * tak vklada parametre do zoznamu
 * Vracia: 0 v pripade uspechu, -1 v pripade duplicity
 */

int list_insert_param (tHashTable *local_table, param sym, param psym)
{
	if (find_param_test(local_table, sym.id, psym.id) != -1)
	{
		int idx = hash_code(sym.id);

		local_table[idx].Act = local_table[idx].First;

		while (local_table[idx].Act != NULL) //prejde vsetky prvky zoznamu
		{
			if (strcmp(local_table[idx].Act->id, sym.id) == 0) //porovna retazce
			{
				param *pom = myMalloc(sizeof(struct paramP));

				if (pom == NULL)
					error_msg(INTERNAL_ERROR, "Nepodarilo sa alokovat miesto\n");
				
	 			pom->id = myMalloc(strlen(psym.id) * sizeof(char));
	
	 			if (pom->id == NULL) // Kontrola malloc	
				{
					free(pom);
					error_msg(INTERNAL_ERROR, "Nepodarilo sa alokovat miesto\n");
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
 * Funkcia: find()
 * Popis: Funkcia prejde zoznam a najde prvok zo spravym "sym.id" a do tokenu priradi vsetko
 * Vracia: ukazatel na token
 */

param *param_find (tHashTable *local_table,char* fooId, char* symId)
{
	int idx = hash_code(fooId);
	for (local_table[idx].Act = local_table[idx].First;local_table[idx].Act != NULL; local_table[idx].Act = local_table[idx].Act->next_item) // Prejde vsetky prvky zoznamu
	{
		for (local_table[idx].Act->param->Act = local_table[idx].Act->param->First;local_table[idx].Act->param->Act != NULL; local_table[idx].Act->param->Act = local_table[idx].Act->param->Act->next_param) // Prejde vsetky prvky zoznamu
			{
			if (strcmp(local_table[idx].Act->id, symId) == 0) // Porovna retazce
			{
				return local_table[idx].Act->param->Act;
			}
	}
	return NULL;
}

/*
 * Funkcia: param_find()
 * Popis: Funkcia prejde zoznam a najde prvok zo spravym "sym.id" a potom prechadza
 * zoznam parametrov az kym nenajde spravny, nasledne do tokenu priradi vsetko
 * Vracia: ukazatel na token
 */

tFooListElem *function_find (tHashTable *local_table, param sym, param psym)
{
	int idx = hash_code(sym.id);
	for(local_table[idx].Act = local_table[idx].First;local_table[idx].Act != NULL;local_table[idx].Act = local_table[idx].Act->next_item)
		if(strcmp(local_table[idx].Act->id, psym.id) == 0) // Porovna retazce
		{
			return local_table[idx].Act;
		}
	return NULL;
}


/*
 * Funkcia: return_index_parameter ()
 * Popis: Funkcia prejde zoznam a najde prvok zo spravym "sym.id" a potom prechadza
 * zoznam parametrov az kym nenajde spravny, nasledne do tokenu priradi vsetko
 * Vracia: index parametru, alebo ak tam nie je -1
 */
int return_index_parameter (tHashTable *local_table, param sym, param psym)
{
	int idx = hash_code(sym.id);
	int counter = 0;
	

	local_table[idx].Act = local_table[idx].First;

	while (local_table[idx].Act != NULL) // Prejde vsetky prvky zoznamu
	{
		if (strcmp(local_table[idx].Act->id, sym.id) == 0) // Porovna retazce
		{
			local_table[idx].Act->param->Act = local_table[idx].First->param->First;
	
			while (local_table[idx].Act->param->Act != NULL) // Prejdeme vsetky prvky zoznamu parametrov
			{ 
				if(strcmp(local_table[idx].Act->param->Act->id, psym.id) == 0) // Porovna retazce
				{
					return counter;
				}
				
				counter++;

				local_table[idx].Act->param->Act = local_table[idx].Act->param->Act->next_param; // Posunieme sa o prvok dalej v zozname parametrov
			}
		}

		local_table[idx].Act = local_table[idx].Act->next_item; // Posunieme sa o prvok dalej
	}

	return -1;
}

/*
 * Funkcia: change_isdefine()
 * Popis: Funkcia prejde zoznam a najde prvoku zo spravym "sym.id" nastavi is_define na true
 * Vracia: Ak bol "sym.type" nastaveny na nieco ine ako NULL vracia -1, inak 0
 */
int change_isdefine (tHashTable *local_table, param sym)
{
	int idx = hash_code(sym.id);		

	local_table[idx].Act = local_table[idx].First;

	while (local_table[idx].Act != NULL) // Prejde vsetky prvky zoznamu
	{
		if (strcmp(local_table[idx].Act->id, sym.id) == 0) // Porovna retazce
		{
			if(local_table[idx].Act->is_define == false)
			{
				local_table[idx].Act->is_define = true;
				break;
			}
			else
			
				return -1; // Vraciam semanticku chybu
		}			

		local_table[idx].Act = local_table[idx].Act->next_item; // Posunieme sa o prvok dalej
	}
	
	return 0;
}

/*
 * Funkcia: ltab_destroy
 * Popis: Funkcia zmaze celu tabulku
 * Parametre: Ukazatel na tabulku
 */

void ltab_destroy (tHashTable *local_table)
{
	for (int i = 0; i < MAX_SIZE; i++) // Pre kazdy zoznam v tabulke
 	{
 		while (local_table[i].First != NULL) // Prejde vsetky prvky zoznamu
 		{
			while (local_table[i].First->param->First != NULL)
			{
				if ((local_table[i].First->param->First) == (local_table[i].Act->param->Act)) // Zrusi aj aktivny prvok
 				{
 					myFree(local_table[i].Act->param->Act);
 				}
	
 				param *pom2 = local_table[i].First->param->First;
 				local_table[i].First->param->First = local_table[i].First->param->First->next_param;
 				myFree(pom2->id);
 				myFree(pom2);
			}			

 			if (local_table[i].First == local_table[i].Act) // Zrusi aj aktivny prvok
 			{
 				myFree(local_table[i].Act = NULL);
 			}

 			tFooListElem pom = local_table[i].First;
 			local_table[i].First = local_table[i].First->next_item;
 			myFree(pom->id);
			myFree(pom->param);
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

int find_test(tHashTable *local_table, char *id)
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

int find_param_test(tHashTable *local_table, char *id_fnc, char *id_param)
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

int number_param(tHashTable *local_table, param sym)
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
