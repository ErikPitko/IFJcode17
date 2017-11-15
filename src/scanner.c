#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "scanner.h"
#include "garbage.h"
//extern struct structToken;

token tmpToken;
int ungetcharpom = 0;

token getToken();
//testove ucely 1. TOKEN_ID OBSAH

int main (){
  garbageInit(400);
	token myToken;
	int i = 0;
	while (myToken.type != EOF0){
		i++;
		myToken = getToken();

		printf("%d. %d",i,myToken.type);
		//if(myToken.info)
			printf("     %s",myToken.info);
		printf("\n");
  }


	return 0;
}
// vrati typ (token.type)a dani nazov v token.info
token getToken() {
		int c;
		//char state[16];
		int state;
    int pomoc;
		//START OF FINDING TOKENS
		//char tmp_s[256];
		char* tmp_s;
		tmp_s = malloc(256*sizeof(char));//problem
		int i = 0;
    int eol_counter=0;

			//EOF first ,0
			if((c = getchar0(c)) != EOF){
				ungetcharpom = c;
			}
			else{
				tmpToken.type = EOF0;
				 tmpToken.info = NULL;
				return tmpToken;
			}
			// EOF first ,1

			//mazanie prazdnych znakov s vynimkou EOL ,0
			while (c = getchar0(c)) {
				if (c == '\n'){
          while(isspace(c)){
            c = getchar0(c);
          }//pre vypis len jedneho EOL
          ungetcharpom = c;

					tmpToken.type = EOL;
					 tmpToken.info = NULL;
					return tmpToken;
				}
				if(!(isspace(c))){
					ungetcharpom = c;
					break;
				}
			}
			//mazanie prazdnych znakov aj EOL ,1
			c = getchar0(c);
			//blokovy komentar + delenie pre double ,0
			if(c == EOF){ ///888
				tmpToken.type = EOF0;
				 tmpToken.info = NULL;
				return tmpToken;
			} else
			if (c == '('){
				tmpToken.type = LEFT_PARENTHESIS;
				 tmpToken.info = NULL;
				return tmpToken;
			}else
			if(c == ')'){
				tmpToken.type = RIGHT_PARENTHESIS;
				 tmpToken.info = NULL;
				return tmpToken;
			}else
      if(c == ','){
        tmpToken.type = COMMA;
				 tmpToken.info = NULL;
				return tmpToken;
      }else
      if(c == ';'){
        tmpToken.type = SEMICOLON;
				 tmpToken.info = NULL;
				return tmpToken;
      }else
			if (c == '/'){
				if ((c = getchar0(c)) == 39) {// /' blokovy komentar zaciatok
						c = getchar0(c);
						if(c == EOF){
							ERR; // EOF v blokovom komentare
						}/*
						while(((c != 39) || ((c = getchar0(c)) !='/')))// '/ koniec blokoveho komentara
              if(c == EOF){
                ERR;
              }*/
            pomoc = 1;
            while(pomoc){
              if(c == 39){
                c = getchar0(c);
                if (c == EOF){
                  ERR;
                }
                if(c == '/'){
                  pomoc=0;
                }
              }else{
                c = getchar0(c);
                if (c == EOF){
                  ERR;
                }
              }
            }

				}
				else {
					ungetcharpom = c; // z podmienky == 39
					tmpToken.type = DIV_DOUBLE;
					 tmpToken.info = NULL;
					return tmpToken; // / - token delenie pre double //////////////////////////////////////////////// /
				}
				tmpToken.type = NOPE; //bol blokovy komentar ak chceme zrusit NOPE vypisiovanie tak zmenit strukturu podmienky
				 tmpToken.info = NULL;
				return tmpToken;
			}else

      //blokovy komentar + delenie pre double ,1

			// ' -jednoriadkovy komentar ,0
			if (c == 39){
				while ((c != '\n') && (c != EOF)) {
					c = getchar0(c);
				}
        //ungetcharpom = c;
        if(c == '\n'){
          tmpToken.type = NOPE; //bol blokovy komentar ak chceme zrusit NOPE vypisiovanie tak zmenit strukturu podmienky
  				 tmpToken.info = NULL;
				  return tmpToken;
        }else
          ERR; //komentar ukonceny EOF
        /*
				if (c == EOF){
          ERR; // jednoriadkovy komentar musi byt ukonceny koncom riadku

					tmpToken.type = EOF0;
					 tmpToken.info = NULL;
					return tmpToken;
				}*/
        /*
				if(c == '\n'){
					tmpToken.type = EOL;
					 tmpToken.info = NULL;
					return tmpToken;
				}*/

			}else
			// ' -jednoriadkovy komentar ,1

			// rozdelenie pre switch ,0
			// 92dec = '\'
			if (c == '+'|| c == '-'|| c == '*'|| c == 92 || c == '<' || c == '>' || c == '=' )
			 state = 1 ;//"OPERATOR"; // druha strana v automatu
			else
			if (c >= '0' && c <='9' ){
				tmp_s[0] = c;
				state = 2;//"POSSIBLE_NUMBER";
			}

			else
			if ((c >= 'A' && c <= 'Z') ||(c >= 'a' && c <= 'z') || (c == '_')){
				state = 3;//"POSSIBLE_IDENTIFIER";
				tmp_s[0] = upper2lower(c);
			}
			else
			if (c == 33){// '!' == 33
				if ((c = getchar0(c)) == 34){ //34 "
				 	state = 4;//"POSSIBLE_STRING";
					if(c == EOF){
						ERR;
					}
				}
				 else{ungetcharpom = c; ERR;}// wrong string format
			}else ERR; // wrong char for example ↑§►↕♫ ...
			// rozdelenie pre switch ,1

			// switch ,0
			switch(state){
				//TODO case term //666 - je treba?
				case 2://"POSSIBLE_NUMBER":
					i= 1;
					while (((c = getchar0(c)) >= '0') && (c<='9')){
							tmp_s[i++] = c; // 222
					}
					if(isspace(c) || isOperator(c)){
						//if (c == '\n')
							tmp_s[i] = '\0';
						tmpToken.type = VALUE_INTEGER;
						tmpToken.info = tmp_s;
						ungetcharpom = c;
						return tmpToken;
					}

					if (c == '.'){ // double?
						tmp_s[i++] = c;
						while (((c = upper2lower(getchar0(c))) >= '0') && (c <='9')){
							tmp_s[i++] = c;
						}
						if(tmp_s[i-1] == '.'){
							ungetcharpom = c;
							ERR; // 123.x // x neni cislo
						}
						if (c != 'e'/*isspace(c) || isOperator(c)*/){
							ungetcharpom = c;
							tmp_s[i] = '\0';
							tmpToken.info = tmp_s;
							tmpToken.type = VALUE_DOUBLE;
							return tmpToken; // 123.123
						}else {
							tmp_s[i++] = c;
						}
						c = getchar0(c);
						if(c == EOF){
							ERR;//123.123e'EOF'
						}
            if(c == '.'){
              ERR;
            }
						if( c ==  '+' || c == '-')//{ // 123.132e+ // 111 dobrovolne znamenko?
							tmp_s[i++] = c;
						else ungetcharpom = c;

						while (((c = getchar0(c)) >= '0') && (c <='9')){ // 123.132e+123
							tmp_s[i++] = c;
						}// konec cyklu
            ungetcharpom = c;
            tmp_s[i] = '\0';
            tmpToken.info = tmp_s;
            tmpToken.type = VALUE_DOUBLE;
            return tmpToken;

					}else { // nebola .
						tmp_s[i] = '\0';
						ungetcharpom = c;
						tmpToken.info = tmp_s;
						tmpToken.type = VALUE_INTEGER;
						return tmpToken;
					}
/*
						tmp_s[i] = '\0';
						tmpToken.info = tmp_s;
						tmpToken.type = DOUBLE;
						ungetcharpom = c;
						return tmpToken; // vrati double
*/
				break;

				//IDENTIFIER OR KEYWORD
				case 3://"POSSIBLE_IDENTIFIER":
				i=1;//first char is already in
				c = getchar0(c);
				c = upper2lower(c);
				while ((c == '_') || ((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'z'))) {
					tmp_s[i++] = c;
					c = getchar0(c);
					c = upper2lower(c);
				}
				tmp_s[i] = '\0';
				ungetcharpom = c;
				//printf("%s",tmp_s);// 555

				int pom = isKeyword(tmp_s);
				if(pom){
					tmpToken.info = NULL; // kvuli debugu inac NULL // 555
					tmpToken.type = pom;
					return tmpToken; // keyword
				}
				else { // identifier
						tmp_s[i] = '\0';
						tmpToken.info = tmp_s;
						tmpToken.type = IDENTIFIER; // navrat identifieru a jeho nazvu
						ungetcharpom = c;
						return tmpToken; // identifier
				}

				break;
				case 4://"POSSIBLE_STRING":
				// In The Name of AllMighty
				// STRING_START
				//TODO check if inside string chars are good

					i= 0;
					while((c = getchar0(c)) != 34) { // c != '"'
						if(c == EOF){
							ERR;// eof pocas stringu
						}
						if (c == 92){//START OF ESC // c == '\'
							//possible escape sequance

							tmp_s[i++] = c;
							c = getchar0(c);//\_
							if(c == EOF){
								ERR; // eof pocas esc 1
							}
							tmp_s[i++] = c;//0-2 default ERR
							switch(c){
								case '0': //0
											c = getchar0(c);
											if(c == EOF){
												ERR;//eof pocas esc 2
											}
											tmp_s[i++] = c;//0x
											if (c == '0'){//00
												c = getchar0(c);
                        if (c == EOF){
                          ERR; // eof pocas esc 3
                        }
												if (c == '0'){
														ERR;//\000 wrong escape sequance
												}
												else if (c <='9'&& c>'0')//001-9
															tmp_s[i++] = c;
															else {
																	ERR;// what did come? char?
															}
											//01-9
  										}else if ((c <= '9') && (c >='0'))
                              if (((c=getchar0(c)) <= '9') && (c >='0'))
    														tmp_s[i++] = c;//01-9x
    														else{
    															ERR; // 01-9x x-neni cislo //some char nor number
    														}
                              else {
                                ERR;  // /0x  x- neni cislo
                              }
								break;
								case '1'://1
												c=getchar0(c);
												if(c<='9'&& c>='0')
													tmp_s[i++] = c;//1x
												else ERR; //wrong escape sequance

												c=getchar0(c);
												if (c<='9'&& c >='0')
													tmp_s[i++] = c;//1xx
												else ERR;//wrong ESC
								break;
								case '2'://2
												c=getchar0(c);//2x
												tmp_s[i++] = c;
												if (c<='4'&& c>='0'){//20-4
														c=getchar0(c);//20-4x
														if(c == EOF){
															ERR;
														}
														tmp_s[i++] = c;
												}
												else
													if(c == '5'){
														c = getchar0(c);//25x
														if(c <= '5' && c >= '0'){
															tmp_s[i++] = c;
														}else ERR;//256+ wrong ESC or char
													}else ERR; //26+ wrong esc or char
								break;
							case 34:// '"'
										//tmp_s[i++] = 34;//neni potrebne
							break;

							case 'n':

							break;
									//magic don`t try to understand
							case 't':

							break;

							case 92:

							break;
							default: ERR;//3+ wrong esc or char
							}
						}// end of ESC
						// ak nieje esc sekvencia
						else {
							tmp_s[i++] = c;
						}
				}
				tmp_s[i]='\0';
				tmpToken.info = tmp_s;//"tohle tam je bez uvozovek\0"
				tmpToken.type = VALUE_STRING;
				return tmpToken;
			// STRING_END
				break;
				case 1://"OPERATOR":
			// OPERATOR_START  ( c == '+'|| c == '-'|| c == '*'|| c == '\\' || c == '<' || c == '>' || c == '=' )
					switch(c){
						case '+':  tmpToken.info = NULL;
											tmpToken.type = PLUS;
											return tmpToken;
											break;
						case '-':  tmpToken.info = NULL;
											tmpToken.type = MINUS;
											return tmpToken;
											break;

						case '<':c = getchar0(c);
										if(c == '>') {
											 tmpToken.info = NULL;
											tmpToken.type = INEQUALITY;
											return tmpToken; // <>
										}else if (c == '='){
												 tmpToken.info = NULL;
												tmpToken.type = LESS_EQUAL;
												return tmpToken; // <=
										}else {
											ungetcharpom = c;
											 tmpToken.info = NULL;
											tmpToken.type = LESS;
											return tmpToken; // <
										}
						break;
						case '=':  tmpToken.info = NULL;
											tmpToken.type = EQUAL;
											return tmpToken;//=
						break;
						case '>': c = getchar0(c);
                        if (c == '='){
													 tmpToken.info = NULL;
													tmpToken.type = GREATER_EQUAL;
													return tmpToken;// >=
												}else{
													ungetcharpom = c;
													 tmpToken.info = NULL;
													tmpToken.type = GREATER;
													return tmpToken;//>
												}
						break;
						case '*':  tmpToken.info = NULL;
											tmpToken.type = ASTERIX;
											return tmpToken;//*
						break;
						case 92:  tmpToken.info = NULL; // '\' == 92
										 tmpToken.type = DIV_INT;
										 return tmpToken;

						break;
						default: ERR; // jinej operator? nemelo by nastat
					}

			// OPERATOR_END
				break;
				default: ERR;// nastane v pripade ze sme nevedeli predpovedat typ
			}//switch ,1


	//return NOPE; // token for EOF
}

int upper2lower(int c){
	if (c >= 'A' && c <= 'Z')
		return c+32;
	return c;
}

int isKeyword(char* s){
	int i = 0;

	char* keyword[] = {
		"as", //1
		"asc",
		"declare",
		"dim",
		"do",
		"double",
		"else",
		"end",
		"chr",
		"function",
		"if",
		"input",
		"integer",
		"length",
		"loop",
		"print",
		"return",
		"scope",
		"string",
		"substr",
		"then",
		"while",
				//reserved
		"and",
		"boolean",
		"continue",
		"elseif",
		"exit",
		"false",
		"for",
		"next",
		"not",
		"or",
		"shared",
		"static",
		"true", //35
		NULL//36
	};


	while (keyword[i] != NULL){
		if (strcmp(s,keyword[i]) == 0){
			return(i+1); // vraci hodnotu daneho keywordu

		}

		i++;
	}
	return 0;
}

int isOperator(int c){
	if(c == '+'|| c == '-'|| c == '*'|| c == 92 || c == '<' || c == '>' || c == '=' )
		return c;
	else return 0;
}
// funkce pro simulaci ungetc(c,file);
int getchar0(int c){
	if(ungetcharpom){
		int pom;
		pom = ungetcharpom;
		ungetcharpom = 0;
		return pom;
	}else return getchar();
}
