#include "address_code.h"


void I_move_to_global(tFooListElem value){
	printf("MOVE GF@_string0 string@\n");
	printf("FLOAT2INT GF@_int0 float@0.0\n");
	printf("INT2FLOAT GF@_float0 int@0\n");
	
	switch(value.type){
	case STRING:
		printf("MOVE GF@_pom_string GF@%s\n", value.id);
		break;
	case DOUBLE:
		printf("MOVE GF@_pom_double GF@%s\n", value.id);
		break;
	case INTEGER:
		printf("MOVE GF@_pom_integer GF@%s\n", value.id);
		break;
	default:
		error_msg(99, "unexpected type: %s : %d", value.id, value.type);
	}
}

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
	printf("CREATEFRAME\n");
	printf("DEFVAR GF@_pom_integer\n");
	printf("DEFVAR GF@_pom_double\n");
	printf("DEFVAR GF@_pom_string\n");

	printf("MOVE GF@_pom_integer int@0\n");
	printf("MOVE GF@_pom_double float@0\n");
	printf("MOVE GF@_pom_string string@\n");

	printf("DEFVAR GF@exppom1string\n");
	printf("DEFVAR GF@exppom1double\n");
	printf("DEFVAR GF@exppom1integer\n");

	printf("MOVE GF@exppom1integer int@0\n");
	printf("MOVE GF@exppom1double float@0\n");
	printf("MOVE GF@exppom1string string@\n");

	printf("DEFVAR GF@exppom2string\n");
	printf("DEFVAR GF@exppom2double\n");
	printf("DEFVAR GF@exppom2integer\n");

	printf("MOVE GF@exppom2integer int@0\n");
	printf("MOVE GF@exppom2double float@0\n");
	printf("MOVE GF@exppom2string string@\n");

	printf("DEFVAR GF@exppom3string\n");
	printf("DEFVAR GF@exppom3double\n");
	printf("DEFVAR GF@exppom3integer\n");
	
	printf("MOVE GF@exppom3integer int@0\n");
	printf("MOVE GF@exppom3double float@0\n");
	printf("MOVE GF@exppom3string string@\n");


	printf("DEFVAR GF@_string0\n");       //prazdny string nemenit
	printf("TYPE GF@_string0 GF@_string0\n");

	printf("DEFVAR GF@_int0\n");     // integerova hodnota 0 nemenit
	printf("FLOAT2INT GF@_int0 float@0.0\n");

	printf("DEFVAR GF@_float0\n");   // realna nula 0.0
	printf("INT2FLOAT GF@_float0 int@0\n");
	printf("PUSHFRAME\n");
	printf("JUMP labelSCOPE\n");

}


/*
*
* udela deklaraci premennej
* @param id - nazov premennej
* @param where - 1=>GF , 0 =>LF
*
*/
void I_dim_id_as_var_type(tFooListElem value){
int  where = 0;

  if(value.type == INTEGER){
    if(where){
      printf("DEFVAR GF@_%s\n",value.id);
      printf("MOVE GF@_%s int@0\n",value.id);
    }
    else{
      printf("DEFVAR LF@%s\n",value.id);
      printf("MOVE LF@%s int@0\n",value.id);
    }
  }else
  if(value.type == DOUBLE){
    if(where){
      printf("DEFVAR GF@_%s\n",value.id);
      printf("MOVE GF@_%s float@0.0\n",value.id);
    }
    else{
      printf("DEFVAR LF@%s\n",value.id);
      printf("MOVE LF@%s float@0.0\n",value.id);
    }
  }else
  if(value.type == STRING){
    if(where){
      printf("DEFVAR GF@_%s\n",value.id);
      printf("MOVE GF@_%s string@!""\n",value.id);
    }
    else{
      printf("DEFVAR LF@%s\n",value.id);
      printf("MOVE LF@%s int@0\n",value.id);
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
void I_input_id(tFooListElem value){//READ var type
  int where = 0;
  if(value.type == STRING){
    if(where){
      printf("READ GF@_%s\n",value.id);
    }else printf("READ LF@_%s\n",value.id);
  }else
    if(value.type == DOUBLE){
      if(where){
        printf("READ GF@_%s\n",value.id);
      }else printf("READ LF@_%s\n",value.id);
    }else
    if(value.type == INTEGER){
      if(where){
        printf("READ GF@_%s\n",value.id);
      }else printf("READ LF@_%s\n",value.id);
    }//else error

}


/*
*
* vytvori zaciatok cyklu
* @param number - unikatne cislo pre kazdy while dolezite pre I_loop
*
*/
void I_do_while_label(int number){
	printf("DEFVAR LF@_wpom%d\n", number);
  printf("LABEL w_label%d\n",number);
}

void I_do_while(int number, tFooListElem value)
{
	printf("MOVE LF@_wpom%d GF@%s\n", number, value.id);

	//switch(value.type){
	/*case INTEGER:
	case VALUE_INTEGER:
		printf("JUMPIFEQ w_label%d_end LF@_wpom%d int@0\n", number, number);
		break;
	case DOUBLE:
	case VALUE_DOUBLE:
		printf("JUMPIFEQ w_label%d_end LF@_wpom%d float@0.0\n", number, number);
		break;*/
	//case BOOLEAN:
		printf("JUMPIFEQ w_label%d_end LF@_wpom%d bool@false\n", number, number);
		//break;
	//default:
		//error_msg(SEMANTIC_TYPE, "wrong type in while statement: %s : %d", value.id, value.type);
	//}
}

/*
*
* ukonci cyklus pre lokalitu
* @param number - unikatne cislo pro dany while napriklad jeho postupnost
*
*/
void I_loop(int number){
  printf("JUMP w_label%d\n",number);
  printf("LABEL w_label%d_end\n",number);
}

//TODO porovnavanie stringov
void I_if_then(int number, tFooListElem value){
	printf("DEFVAR LF@_pom%d\n", number);
	printf("MOVE LF@_pom%d GF@%s\n", number, value.id);
	/*switch(value.type){
	case INTEGER:
	case VALUE_INTEGER:
		printf("JUMPIFEQ label_if%d LF@_pom%d int@0\n", number, number);
		break;
	case DOUBLE:
	case VALUE_DOUBLE:
		printf("JUMPIFEQ label_if%d LF@_pom%d float@0.0\n", number, number);
		break;
	case BOOLEAN:*/
		printf("JUMPIFEQ label_if%d LF@_pom%d bool@false\n", number, number);
/*		break;
	default:
		error_msg(SEMANTIC_TYPE, "wrong type in if statement: %s : %d", value.id, value.type);
	}*/
}

//void I_else(int number){
//  printf("JUMPIFEQ label_end%d LF@_pom%d 1\n", number, number);
//
//  printf("LABEL label%d\n", number);
//}

void I_jump_endif(int number){
  printf("JUMP label_if%d\n", number);
}

void I_if(int number){
  printf("LABEL label_if%d\n", number);
}

void I_endif(int number){
  printf("LABEL label_if%d\n", number);
}
/*
*
* id = EXP
* @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
*/
void I_id_expression(char *id){
  //expression();//zistit od adama nazov premennej pro vysledek EXP

  printf("MOVE LF@%s EXP\n",id);


}


/*
*
* id = func1(... - vola pri detekovani funkcie (
* @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
*/
void I_createFrame(){
  printf("CREATEFRAME\n");

}

/*
*
* arg_i_id - vola pri kazdom argumente
* @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
*/
void I_arg_i_id(char *id, char *idD){

    printf("MOVE TF@%s LF@%s\n", idD, id);

}

/*
*
* arg_i_const - vola pri kazdom argumente
* @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
*/
void I_arg_i_const(char *_const, int type, char *idD){

	printf("DEFVAR TF@%s\n", idD);

  if(type == VALUE_INTEGER)
    printf("MOVE TF@%s int@%s\n", idD, _const);
  else if(type == VALUE_DOUBLE)
    printf("MOVE TF@%s double@%s\n", idD, _const);
  else if(type == VALUE_STRING)
  {
    _const = reformString(_const);
    printf("MOVE TF@%s int@%s\n", idD, _const);
  }
}
/*
*
* id = func2 ...) - vola pri detekovani konce parametru )
* @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
*/
void I_callFunc(char *id_func){
  printf("PUSHFRAME\n");
  printf("CALL label%s\n", id_func);
  printf("POPFRAME\n");
}

/*
*
* id = define_func
* @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
*/
void I_define_func(char *id_func){
  printf("LABEL label%s\n", id_func);
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
void I_print(tFooListElem value){ // type z expression
	if (value.type == STRING){
		value.id = reformString(value.id);
		printf("WRITE string@%s\n", value.id);
	}else
	printf("WRITE GF@%s\n", value.id);
}

/*
*
* pridari do promenne @id hodnotu z posledneho vypoctu // GF
* @param id - nazov premennej do ktorej priradime
* @param type_of_a - typ z @id
* @param number - counter
*
*/
void I_priradenie(tFooListElem value){
	static int number = 0;

  if(value.type == STRING){
      printf("MOVE LF@%s GF@_pom_string\n",value.id);  //string pretypovavat nebudu
  }else


  if(value.type == INTEGER){

    printf("JUMPIFEQ label_int%d GF@_pom_double GF@_float0\n", number);  //zisti ci potrebujeme pretypovavat
    printf("JUMPIFEQ label_int0%d GF@_pom_integer GF@_int0\n",number);  // skoci si pre priradenie 0

    printf("FLOAT2R2EINT LF@_%s GF@_pom_double\n",value.id); // pretypuje a priradenie
    printf("JUMP label_end%d\n",number);    //ukoncenie

    printf("LABEL label_int%d\n",number);  // aby sme mohli preskocit pretypovanie
    printf("MOVE LF@%s GF@_pom_integer\n",value.id);  // priradenie
    printf("JUMP label_end%d\n",number); // ukoncenie pre pripad ze nepriradujeme 0

    //museli sme pretypovat
    printf("LABEL label_int0%d\n",number); // ak je 0
    printf("MOVE LF@%s GF@_int0\n",value.id);

    printf("LABEL label_end%d\n",number);
  }else

  if(value.type == DOUBLE){


        printf("JUMPIFEQ label_double%d GF@_pom_integer GF@_int0\n", number);  //zisti ci potrebujeme pretypovavat
        printf("JUMPIFEQ label_double0%d GF@_pom_double GF@_float0\n",number);  // skoci si pre priradenie 0.0

        printf("INT2FLOAT LF@_%s GF@_pom_integer\n",value.id); // pretypuje a priradi
        printf("JUMP label_end%d\n",number);  // ukoncenie

        printf("LABEL label_double%d\n",number);  // aby sme mohli preskocit pretypovanie
        printf("MOVE LF@%s GF@_pom_double\n",value.id);  // priradenie
        printf("JUMP label_end%d\n",number); // ukoncenie pre pripad ze nepriradujeme 0.0

        //museli sme pretypovat
        printf("LABEL label_double0%d\n",number); // ak je 0
        printf("MOVE LF@%s GF@_double0\n",value.id);

        printf("LABEL label_end%d\n",number);


  }//else error?

  number++;

}
