#ifndef ADDRESS_CODE_H_
#define ADDRESS_CODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "symtable.h"
#include "scanner.h"
#include "garbage.h"

/*
*
* vrati ukazatel na string vhodny pre vypis v 3ADD //PRINT
*
*/
void I_move_to_global(tFooListElem value);
char * reformString(char *tmp);

/*
*
* vytvori na zaciatku vypisu .IFJcode17
* vhodne aplikovat pred zacatim analyzy
*
*/
void init3ADD();

void I_dim_id_as_var_type(char* id, int type );
void I_input_id(char *id, int type);
void I_do_while(int number, tFooListElem value);
void I_do_while_label(int number);
void I_loop(int number);
void I_if_then(int number, tFooListElem value);
void I_else(int number);
void I_endif(int number);
void I_id_expression(char *id);
void I_id_func1();
void I_arg_i_id(char *id, char *idD);
void I_arg_i_const(char *_const, int type, char *idD);
void I_id_func2(char *id_func);
void I_define_func(char *id_func);
void I_define_return();
void I_print();

#endif /* ADDRESS_CODE_H_ */
