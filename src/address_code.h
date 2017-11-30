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
#include "header.h"
#include "ExpressionParser.h"

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

void I_dim_id_as_var_type(tFooListElem value);
void I_input_id(tFooListElem value);
void I_do_while(int number, tFooListElem value);
void I_do_while_label(int number);
void I_loop(int number);
void I_if_then(int number, tFooListElem value);
void I_jump_endif(int number);
void I_if(int number);
void I_endif(int number);
void I_id_expression(char *id);
void I_createFrame();
void I_arg_i_id(char *id, char *idD);
void I_arg_i_const(char *_const, int type, char *idD);
void I_callFunc(char *id_func);
void I_define_func(char *id_func);
void I_define_return();
void I_print();

/*
*
* pridari do promenne @id hodnotu z posledneho vypoctu // GL
* @param id - nazov premennej do ktorej priradime
* @param type_of_a - typ z @id
* @param number - counter
*
*/
void I_priradenie(tFooListElem value);

#endif /* ADDRESS_CODE_H_ */
