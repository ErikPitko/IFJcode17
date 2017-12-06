#include "address_code.h"

void I_move_to_global(tFooListElem value) {
	printf("MOVE GF@_string0 string@\n");
	printf("FLOAT2INT GF@_int0 float@0.0\n");
	printf("INT2FLOAT GF@_float0 int@0\n");

	switch (value.type) {
	case STRING:
	case VALUE_STRING:
		printf("MOVE GF@_pom_string GF@%s\n", value.id);
		break;
	case DOUBLE:
	case VALUE_DOUBLE:
		printf("MOVE GF@_pom_double GF@%s\n", value.id);
		break;
	case INTEGER:
	case VALUE_INTEGER:
		printf("MOVE GF@_pom_integer GF@%s\n", value.id);
		break;
	default:
		error_msg(99, "unexpected type: %s : %d", value.id, value.type);
	}
}

void null_global() {
	printf("MOVE GF@_pom_integer int@0\n");
	printf("MOVE GF@_pom_double float@0\n");
	printf("MOVE GF@_pom_string string@\n");
}

void printDefineStrLenFunction()  //int strlen (string s)
{
	printf("LABEL labellength\n");
	printf("STRLEN GF@_pom_integer LF@s\n");
	printf("RETURN\n");
}

void printDefineChrFunction()  //string inttochar (int i)
{
	printf("LABEL labelchr\n");
	printf("INT2CHAR GF@_pom_string LF@i\n");
	printf("RETURN\n");
}

void printDefineAscFunction() //int asc (string s, int i)
{
	printf("LABEL labelasc\n"); //puvodne

  printf("STRLEN GF@_pom_integer LF@s\n");
  printf("LT GF@_pom_double GF@_pom_integer LF@i\n"); // GF@_pom_double predstavuje bool hodnotu
  printf("JUMPIFEQ label_ascfunction bool@true GF@_pom_double\n"); // pokud je mimo "s" tak preskoci na pridanie 0

  //puvodne 0
  printf("SUB LF@i LF@i int@1\n");
  printf("GETCHAR GF@_pom_string LF@s LF@i\n");
  printf("STRI2INT GF@_pom_integer GF@_pom_string int@0\n");
  //puvodne 1

  printf("JUMP label_endofascfunction\n");
  printf("LABEL label_ascfunction\n");
  //priradenie nuly
  printf("MOVE GF@_pom_integer int@0\n");


  printf("LABEL label_endofascfunction\n");
  printf("MOVE GF@_pom_double float@0\n"); // alien
  printf("RETURN\n");
}

//string strstr(string s,int i, int n)
void printDefineSubStrFunction() {
	printf("LABEL labelsubstr\n");
	printf("MOVE GF@pom_AIM3 int@0\n");
	printf("MOVE GF@_pom_string string@\n"); // vyprazdneni stringu

  printf("STRLEN GF@pom_AIM2 LF@s\n"); // strlen(s)

  printf("SUB GF@pom_AIM3 GF@pom_AIM2 LF@i\n"); // strlen(s) - i

  // n < 0
  printf("LT GF@pom_AIM1 LF@n int@0\n");
  printf("JUMPIFEQ label_untilend GF@pom_AIM1 bool@true\n");
  //  n >  (strlen(s) - i)
  printf("GT GF@pom_AIM1 LF@n GF@pom_AIM3\n"); //  n >  (strlen(s) - i)
  printf("JUMPIFEQ label_untilend GF@pom_AIM1 bool@true\n");

  // i <= 0
  printf("LT GF@pom_AIM1 LF@i int@1\n");
  printf("JUMPIFEQ label_empty GF@pom_AIM1 bool@true\n");
  // s == !""
  printf("EQ GF@pom_AIM1 LF@s string@!""\n");
  printf("JUMPIFEQ label_empty GF@pom_AIM1 bool@true\n");

  // vse v ramci toho co sem udelal ve tri hodiny rano
	//printf("ADD LF@n LF@n LF@i\n");
	printf("SUB LF@i LF@i int@1\n"); // tohle nechapu ked tka vzmayat kvuli indexu
	printf("LABEL label_substrloop\n"); // zaciatok cyklu
	printf("GETCHAR GF@pom_AIM4 LF@s LF@i\n");  // postupne nacitani od i
	printf("CONCAT GF@_pom_string GF@_pom_string GF@pom_AIM4\n"); // spojeni stringu
	printf("ADD LF@i LF@i int@1\n");         // i++;
  printf("SUB LF@n LF@n int@1\n");          // n--;
  printf("GT GF@pom_AIM1 LF@n int@0\n");   // n > 0
	printf("JUMPIFEQ label_substrloop GF@pom_AIM1 bool@true\n"); // cyklenie ak n > 0
  printf("JUMP label_konecfunkcesubstr\n"); // ukoncenie

  printf("LABEL label_untilend\n"); // nevieme kedy skonci
  // az do konca dopln, n = (strlen(s) - i);
  printf("SUB LF@n GF@pom_AIM2 LF@i\n");
  //skoci hore kde to uz jednou udelane je
  printf("JUMP label_substrloop\n");
  printf("JUMP label_konecfunkcesubstr\n"); // ukoncenie - dead_zone

  printf("LABEL label_empty\n"); // zzaciatok prazdneho
  // prazdny retazec
  printf("MOVE GF@_pom_string string@!""\n");
  printf("JUMP label_konecfunkcesubstr\n"); // ukoncenie



//  printf("LABEL label_substrfunctiongood\n");
  //konec snad
  printf("LABEL label_konecfunkcesubstr\n");
  printf("RETURN\n");
}

/*
 *
 * vrati ukazatel na string vhodny pre vypis v 3ADD //PRINT
 *
 */
char * reformString(char *tmp) {
	int i = 0;
	int j = 0;
	char *pom;
	pom = myMalloc(3 * (strlen(tmp) + 1) * sizeof(char));
	int c;
	c = tmp[j++];
	while (c != '\0') {

		if ((c <= 32) || (c == 35)) {
			pom[i++] = 92; //'\'
			pom[i++] = '0';
			pom[i++] = c / 10 + 48; // nastavi sa na cislo v charu
			pom[i++] = c % 10 + 48;
		} else if (c == 92) { //'\' <- sme nasli
			pom[i++] = c;
			c = tmp[j++];

			if (c <= 32) { //mozno upravit '\n' + '\t' + '\'
				//  pom[i++] = 92;//'\'
				pom[i++] = '0';
				pom[i++] = c / 10 + 48; // nastavi sa na cislo v charu
				pom[i++] = c % 10 + 48;
			}

			if (c == 'n') {
				pom[i++] = '0';
				pom[i++] = '1';
				pom[i++] = '0';
				//  c = tmp[j++]; // aby sme si udrzali aktivnu hodnotu
			}

			if (c == 't') {
				pom[i++] = '0';
				pom[i++] = '0';
				pom[i++] = '9';
				//  c = tmp[j++]; // aby sme si udrzali aktivnu hodnotu
			}

			if (c == 92) {
				pom[i++] = '0';
				pom[i++] = '9';
				pom[i++] = '2';
				//  c = tmp[j++]; // aby sme si udrzali aktivnu hodnotu
			}

			if (c == 34) { //'je tam " '
				pom[i++] = '0';
				pom[i++] = '3';
				pom[i++] = '4';
			}

			if ((c >= 48) && (c <= 57)) { //ak to uz bola nejaka ESC
				pom[i++] = c;
				c = tmp[j++];

				pom[i++] = c;
				c = tmp[j++];

				pom[i++] = c;
				//  c = tmp[j++];
			}
		} else {
			pom[i++] = c; //normalny znak
		}
		c = tmp[j++];
	}
	pom[i] = c;

	return pom;
}

/*
 *
 * vytvori na zaciatku vypisu .IFJcode17
 * vhodne aplikovat pred zacatim analyzy
 *
 */
void init3ADD() {
	printf(".IFJcode17\n");
	printf("CREATEFRAME\n");
	printf("DEFVAR GF@_pom_integer\n");
	printf("DEFVAR GF@_pom_double\n");
	printf("DEFVAR GF@_pom_string\n");

	null_global();

  printf("DEFVAR GF@pom_AIM1\n");
  printf("DEFVAR GF@pom_AIM2\n");
  printf("DEFVAR GF@pom_AIM3\n");
  printf("DEFVAR GF@pom_AIM4\n");

	printf("DEFVAR GF@exppom1\n");
	printf("DEFVAR GF@exppom2\n");
	printf("DEFVAR GF@exppom3\n");
	printf("DEFVAR GF@exppom4\n");

	printf("DEFVAR GF@TypeOne\n");
	printf("DEFVAR GF@TypeTwo\n");

	printf("DEFVAR GF@_string0\n");       //prazdny string nemenit
	printf("TYPE GF@_string0 GF@_string0\n");

	printf("DEFVAR GF@_int0\n");     // integerova hodnota 0 nemenit
	printf("FLOAT2INT GF@_int0 float@0.0\n");

	printf("DEFVAR GF@_float0\n");   // realna nula 0.0
	printf("INT2FLOAT GF@_float0 int@0\n");
	printf("PUSHFRAME\n");
	printf("JUMP labelSCOPE\n");

	printDefineAscFunction();
	printDefineChrFunction();
	printDefineStrLenFunction();
	printDefineSubStrFunction();

}

/*
 *
 * udela deklaraci premennej
 * @param id - nazov premennej
 * @param where - 1=>GF , 0 =>LF
 *
 */
void I_dim_id_as_var_type(tFooListElem value) {
	int where = 0;

	if (value.type == INTEGER) {
		if (where) {
			printf("DEFVAR GF@_%s\n", value.id);
			printf("MOVE GF@_%s int@0\n", value.id);
		} else {
			printf("DEFVAR LF@%s\n", value.id);
			printf("MOVE LF@%s int@0\n", value.id);
		}
	} else if (value.type == DOUBLE) {
		if (where) {
			printf("DEFVAR GF@_%s\n", value.id);
			printf("MOVE GF@_%s float@0.0\n", value.id);
		} else {
			printf("DEFVAR LF@%s\n", value.id);
			printf("MOVE LF@%s float@0.0\n", value.id);
		}
	} else if (value.type == STRING) {
		if (where) {
			printf("DEFVAR GF@_%s\n", value.id);
			printf("MOVE GF@_%s string@!" "\n", value.id);
		} else {
			printf("DEFVAR LF@%s\n", value.id);
			printf("MOVE LF@%s string@\n", value.id);
		}
	}   //else error
}

/*
 *
 * do dane premenne prida nacitanou hodnotu
 * @param id - nazov premennej
 * @param where - 1=>GF , 0 =>LF
 *
 */
void I_input_id(tFooListElem value) {   //READ var type
	if (value.type == STRING) {
			printf("READ LF@%s string\n", value.id);
	} else if (value.type == DOUBLE) {
			printf("READ LF@%s float\n", value.id);
	} else if (value.type == INTEGER) {
			printf("READ LF@%s int\n", value.id);
	}   //else error

}

/*
 *
 * vytvori zaciatok cyklu
 * @param number - unikatne cislo pre kazdy while dolezite pre I_loop
 *
 */
void I_do_while_label(int number) {
	printf("LABEL w_label%d\n", number);
}

void I_do_while(int number, tFooListElem value) {
	printf("JUMPIFEQ w_label%d_end GF@%s bool@false\n", number, value.id);
}

/*
 *
 * ukonci cyklus pre lokalitu
 * @param number - unikatne cislo pro dany while napriklad jeho postupnost
 *
 */
void I_loop(int number) {
	printf("JUMP w_label%d\n", number);
	printf("LABEL w_label%d_end\n", number);
}

//TODO porovnavanie stringov
void I_if_then(int number, tFooListElem value) {
	printf("JUMPIFEQ label_if%d GF@%s bool@false\n", number, value.id);
}

//void I_else(int number){
//  printf("JUMPIFEQ label_end%d LF@_pom%d 1\n", number, number);
//
//  printf("LABEL label%d\n", number);
//}

void I_jump_endif(int number) {
	printf("JUMP label_if%d\n", number);
}

void I_if(int number) {
	printf("LABEL label_if%d\n", number);
}

void I_endif(int number) {
	printf("LABEL label_if%d\n", number);
}
/*
 *
 * id = EXP
 * @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
 */
void I_id_expression(char *id) {
	//expression();//zistit od adama nazov premennej pro vysledek EXP

	printf("MOVE LF@%s EXP\n", id);

}

/*
 *
 * id = func1(... - vola pri detekovani funkcie (
 * @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
 */
void I_createFrame() {
	printf("CREATEFRAME\n");

}

/*
 *
 * arg_i_id - vola pri kazdom argumente
 * @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
 */
void I_arg_i_id(char *id, char *idD) {
	printf("DEFVAR TF@%s\n", idD);
	printf("MOVE TF@%s LF@%s\n", idD, id);

}

void I_move_var(char *to, char *from) {
	printf("MOVE LF@%s LF@%s\n", to, from);

}

/*
 *
 * arg_i_const - vola pri kazdom argumente
 * @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
 */
void I_arg_i_const(char *_const, int type, char *idD) {

	printf("DEFVAR TF@%s\n", idD);

	if (type == VALUE_INTEGER)
		printf("MOVE TF@%s int@%s\n", idD, _const);
	else if (type == VALUE_DOUBLE)
		printf("MOVE TF@%s double@%s\n", idD, _const);
	else if (type == VALUE_STRING) {
		_const = reformString(_const);
		printf("MOVE TF@%s string@%s\n", idD, _const);
	}
}
/*
 *
 * id = func2 ...) - vola pri detekovani konce parametru )
 * @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
 */
void I_callFunc(char *id_func) {
	printf("PUSHFRAME\n");
	printf("CALL label%s\n", id_func);
	printf("POPFRAME\n");
}

/*
 *
 * id = define_func
 * @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
 */
void I_define_func(char *id_func) {
	printf("LABEL label%s\n", id_func);
}

/*
 *
 * id = define_return
 * @param id - znak do premennej do ktorej to budeme zapisovat EXP/func
 */
void I_define_return() {
	printf("RETURN\n");
}

/*
 *
 *
 *
 */
void I_print(tFooListElem value) { // type z expression
	value.id = reformString(value.id);
	if (isGlobal(value.id)) {
		printf("WRITE GF@%s\n", value.id);
	} else {
		printf("WRITE string@%s\n", value.id);
	}
}

/*
 *
 * pridari do promenne @id hodnotu z posledneho vypoctu // GF
 * @param id - nazov premennej do ktorej priradime
 * @param type_of_a - typ z @id
 * @param number - counter
 *
 */

void I_priradenie(tFooListElem value) {
	static int number = 0;

	if (value.type == STRING) {
		printf("MOVE LF@%s GF@_pom_string\n", value.id); //string pretypovavat nebudu
	} else if (value.type == INTEGER) {
		printf("JUMPIFEQ label_int%d GF@_pom_double GF@_float0\n", number); //zisti ci potrebujeme pretypovavat
		printf("FLOAT2R2EINT LF@%s GF@_pom_double\n", value.id); // pretypuje a priradenie
		printf("JUMP label_end%d\n", number);    //ukoncenie

		printf("LABEL label_int%d\n", number); // aby sme mohli preskocit pretypovanie
		printf("MOVE LF@%s GF@_pom_integer\n", value.id);  // priradenie
		printf("JUMP label_end%d\n", number); // ukoncenie pre pripad ze nepriradujeme 0

		printf("LABEL label_end%d\n", number);

	} else if (value.type == DOUBLE) {
		printf("JUMPIFEQ label_double%d GF@_pom_integer GF@_int0\n", number); //zisti ci potrebujeme pretypovavat
		printf("INT2FLOAT LF@%s GF@_pom_integer\n", value.id); // pretypuje a priradi
		printf("JUMP label_end%d\n", number);  // ukoncenie

		printf("LABEL label_double%d\n", number); // aby sme mohli preskocit pretypovanie
		printf("MOVE LF@%s GF@_pom_double\n", value.id);  // priradenie
		printf("JUMP label_end%d\n", number); // ukoncenie pre pripad ze nepriradujeme 0.0

		printf("LABEL label_end%d\n", number);
	}

	number++;
}
