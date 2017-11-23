#include "address_code.h"

/*
*
* vrati ukazatel na string vhodny pre vypis v 3ADD //PRINT
*
*/
char * reformString(char *tmp){
  int i = 0;
  int j = 0;
  char *pom;
  pom = myMalloc(3*(strlen(tmp )+1) * sizeof(char));
  int c;
  while(tmp[j] != '\0'){
    c = tmp[j++];
    if((c <= 32) || (c == 35)){
      pom[i++] = 92;//'\'
      pom[i++] = '0';
      pom[i++] = c/10 +48; // nastavi sa na cislo v charu
      pom[i++] = c%10 +48;
    }
    else
      if(c == 92){
        c = tmp[j++];
        if(c == 34)
          pom[i++] = c;
        if((c <= 32)||(c == 92)){//mozno upravit '\n' + '\t' + '\'
          pom[i++] = 92;//'\'
          pom[i++] = '0';
          pom[i++] = c/10 +48; // nastavi sa na cislo v charu
          pom[i++] = c%10 +48;
        }
      }else {
        pom[i++] = c; //normalny znak
      }
  }
  pom[i] = '\0';

  return pom;
}

/*
*
* vytvori na zaciatku vypisu .IFJcode17
* vhodne aplikovat pred zacatim analyzy
*
*/
void init3ADD(){
  printf(".IFJcode17\n");
printf("DEFVAR GF@_pom_integer\n");
printf("DEFVAR GF@_pom_double\n");
printf("DEFVAR GF@_pom_string\n");

}


/*
*
* udela deklaraci premennej
* @param id - nazov premennej
* @param where - 1=>GF , 0 =>LF
*
*/
void I_dim_id_as_var_type(char* id, int type ){
int  where = 0;

  if(type == INTEGER){
    if(where){
      printf("DEFVAR GF@_%s\n",id);
      printf("MOVE GF@_%s int@0\n");
    }
    else{
      printf("DEFVAR LF@_%s\n",id);
      printf("MOVE LF@_%s int@0\n");
    }
  }else
  if(type == DOUBLE){
    if(where){
      printf("DEFVAR GF@_%s\n",id);
      printf("MOVE GF@_%s float@0.0\n");
    }
    else{
      printf("DEFVAR LF@_%s\n",id);
      printf("MOVE LF@_%s float@0.0\n");
    }
  }else
  if(type == STRING){
    if(where){
      printf("DEFVAR GF@_%s\n",id);
      printf("MOVE GF@_%s string@!""\n");
    }
    else{
      printf("DEFVAR LF@_%s\n",id);
      printf("MOVE LF@_%s int@0\n");
    }
  }//else error
}


/*
*
* do dane premenne prida nacitanou hodnotu
* @param id - nazov premennej
* @param where - 1=>GF , 0 =>LF
*
*/
void I_input_id(char *id, int type){//READ var type
  int where = 0;
  if(type == STRING){
    if(where){
      printf("READ GF@_%s\n",id);
    }else printf("READ LF@_%s\n",id);
  }else
    if(type == DOUBLE){
      if(where){
        printf("READ GF@_%s\n",id);
      }else printf("READ LF@_%s\n",id);
    }else
    if(type == INTEGER){
      if(where){
        printf("READ GF@_%s\n",id);
      }else printf("READ LF@_%s\n",id);
    }//else error

}


/*
*
* vytvori zaciatok cyklu
* @param number - unikatne cislo pre kazdy while dolezite pre I_loop
*
*/
void I_do_while_label(int number){
  printf("label%d0",number);

//  expression();//zistit od adama nazov premennej pro vysledek EXP
}

void I_do_while(int number)
{
  printf("DEFVAR LF@_pom%d\n", number);
  printf("MOVE LF@_pom%d EXP\n",number);

  printf("JUMPIFEQ label%d LF@_pom%d 0\n", number, number);

}

/*
*
* ukonci cyklus pre lokalitu
* @param number - unikatne cislo pro dany while napriklad jeho postupnost
*
*/
void I_loop(int number){
  printf("JUMP label%d0\n",number);
  printf("label%d\n",number);
}

void I_if_then(int number){
  printf("DEFVAR LF@_pom%d\n", number);
  printf("MOVE LF@_pom%d EXP\n", number);

  printf("JUMPIFEQ label%d LF@_pom%d 0\n", number, number);
}

void I_else(int number){
  printf("JUMPIFEQ label_end%d LF@_pom%d 1\n", number, number);

  printf("LABEL label%d\n", number);
}

void I_endif(int number){
  printf("LABEL label_end%d\n", number);
}
/*
*
* id = EXP
* @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
*/
void I_id_expression(char *id){
  //expression();//zistit od adama nazov premennej pro vysledek EXP

  printf("MOVE LF@_%s EXP\n",id);


}


/*
*
* id = func1(... - vola pri detekovani funkcie (
* @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
*/
void I_id_func1(){
  printf("CREATEFRAME\n");

}

/*
*
* arg_i_id - vola pri kazdom argumente
* @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
*/
void I_arg_i_id(char *id, char *idD){

    printf("MOVE TF@_%s LF@_%s\n", idD, id);

}

/*
*
* arg_i_const - vola pri kazdom argumente
* @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
*/
void I_arg_i_const(char *_const, int type, char *idD){


  if(type == INTEGER)
    printf("MOVE TF@_%s integer@%s\n", idD, _const);
  else if(type == DOUBLE)
    printf("MOVE TF@_%s double@%s\n", idD, _const);
  else if(type == STRING)
  {
    _const = reformString(_const);
    printf("MOVE TF@_%s integer@%s\n", idD, _const);
  }
}
/*
*
* id = func2 ...) - vola pri detekovani konce parametru )
* @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
*/
void I_id_func2(char *id_func){
  printf("PUSHFRAME\n");
  printf("CALL label%s\n", id_func);
}

/*
*
* id = define_func
* @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
*/
void I_define_func(char *id_func){
  printf("label%s\n", id_func);
}

/*
*
* id = define_return
* @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
*/
void I_define_return(){
  printf("RETURN\n");
}

/*
*
*
*
*/
void I_print(){ // type z expression
  printf("PRINT EXP\n");
}
