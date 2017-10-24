#
include < stdio.h > #include < stdlib.h > #include < stdbool.h > #include < ctype.h > #include < string.h >
  //TODO otestovat a opravit je v tom bordel
  #include "scanner.h"

token tmpToken;

#
define RETURN(type) tmpToken.type = type;\
return tmpToken

static char * token[] = {

  "eof", // 0 EndOfFile

  "less", //1 <
  "inequality", //2 <>
  "less_equal", //3 <=
  "equal", //4 =     /////////////////////////////AJ ROVNOST AJ PRIRADENIE????
  "more", //5 >
  "more_equal", //6 >=

  "plus", //7 +
  "minus", // 8 -
  "asterix", // 9 *
  "div_int", // 10 \ sa
  "div_double", // 11 /

  //keywords
  "as", //12
  "asc",
  "declare",
  "dim",
  "do",
  "double", // 17
  "else",
  "end",
  "chr",
  "function",
  "if",
  "input",
  "integer", // 24
  "length",
  "loop",
  "print",
  "return",
  "scope",
  "string", // 30
  "substr",
  "then",
  "while", // 33
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
  "true", // 46

  "identifier" // 47
};

static char * keyword[] = {
  "as", //0
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
  "true", //34
  NULL
};

int isKeyword(char * s) {
  int i = 0;
  while (keyword[i] != NULL) {
    if (strcmp(s, keyword[i]))
      RETURN(i + 12);
    i++;
  }
  return 0;
}

// vrati typ (token.type)a dani nazov v token.info
token getToken(FILE * file) {
  int c;
  char state[16]; //if ERR *state -> state[];//magic??
  //token tmpToken;  // pouzite pre navrat hodnot funkce
  //START OF FINDING TOKENS
  char tmp_s[256]; //magic
  int i;
  do {
    i = 0;
    //c = getc(file); // LOAD FROM FILE

    //c = upper2Lower(c);//IN THE CASE OF CHAR use later

    //dodelat mazanie vsetkych prazdnych znakov
    while (isspace(c = getc(file)))
      if (c == EOF)
        RETURN(0);
      // preskakuje biele znaky

    if (c == '/')
      if (c = getc(file) == '/') { //blokovy komentar
        c = getc(file);
        while (c != '/' && c = getc(file) != '/') // koniec blokoveho komentara
      } else {
        //		ungetc(c,file);
        tmpToken.info = NULL;
        RETURN(11); // / - token delenie pre double //////////////////////////////////////////////// /
      } else
    if (c == 39) { // ' - jednoriadkovy komentar
      while (c != '\n' && c != EOF)
        c = getc(file);
      if (c == EOF)
        RETURN(0);
    } else

    //
    if (c == '+' || c == '-' || c == '*' || c == '\\' || c == '<' || c == '>' || c == '=')
      state = "OPERATOR"; // druha strana v automatu
    else
    if (c >= '0' && c <= '9')
      state = "POSSIBLE_NUMBER";
    else
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_')) {
      state = "POSSIBLE_IDENTIFIER";
      tmp_s[0] = upper2lower(c);
    } else
    if (c == '!')
      if (c = getc(file) == 34) //34 "
        state = "POSSIBLE_STRING";
      else ERR; // wrong string format
    else ERR; // wrong char for example ↑§►↕♫ ...

    switch (state) {
      //TODO case term
    case "POSSIBLE_NUMBER":
      //NUMBER_START
      i = 0;
      while (c = getc(file) >= '0' && c <= '9')
        tmp_s[i++] = c;
      if (c == '.') { // double?
        tmp_s[i++] = c;
        while (c = getc(file) >= '0' && c <= '9')
          tmp_s[i++] = c;
        else if (c = getc(file) == 'e') {
          tmp_s[i++] = c;
          if (c == '+' || c == '-');
          while (c = getc(file) >= '0' && c <= '9')
            tmp_s[i++] = c;
        }
        tmpToken.info = tmp_s;
        ungetc(c, file);
        RETURN(17); // double
      } else {
        tmpToken.info = tmp_s;
        ungetc(c, file);
        RETURN(24); //integer
      }

      //		tmpToken.info//////////////////////////////////sdasdasddddddddddd
      //NUMBER_END
      break;
    case "POSSIBLE_IDENTIFIER":
      //IDENTIFIER OR KEYWORD
      i = 1; //first char is already in
      while (c = uppper2lower(getc(file))) {
        tmp_s[i++] = c;
        /// keyword
        if (isKeyword(tmp_s)) {
          tmpToken.info = NULL;
          RETURN(isKeyword(tmp_s)); // keyword
        } else
        if (c != '_' && (c < '0' || c > '9') && (c < 'a' || c > 'z')) {
          tmp_s[i] = '\0'; /////////////////////////////////////////////////////upravit na ynak konca riadku
          tmpToken.info = tmp_s;
          RETURN(47); // navrat identifieru a jeho nazvu
        }

      }
      tmpToken = NULL;
      RETURN(0); // eof nebo jinej nulovej znak === asi jenom eof nebo NULL

      //end of token1

      //HEH
      break;
    case "POSSIBLE_STRING":
      //  In The Name of AllMighty
      // STRING_START
      //TODO check if inside string chars are good
      i = 0;
      while (c = getc(file) != EOF || c != '"') {

        if (c == '\\') {
          //possible escape sequance
          tmp_s[i++] = c;
          c = getc(file); //\_
          tmp_s[i++] = c; //0-2 default ERR
          switch (c) {
          case '0': //0
            c = getc(file);
            tmp_s[i++] = c; //0x
            if (c == '0') //00
              if (c = getc(file) == '0')
                ERR; //\000 wrong escape sequance
              else if (c <= '9' && c > '0') //001-9
              tmp_s[i++] = c;
            else ERR; // what did come? char?
            else if (c = getc(file) <= '9' && c >= '0')
              tmp_s[i++] = c; //01-9x
            else ERR; // some char nor number
            break;
          case '1': //1
            c = getc(file);
            if (c <= '9' && c >= '0')
              tmp_s[i++] = c; //1x
            else ERR; //wrong escape sequance

            c = getc(file);
            if (c <= '9' && c >= '0')
              tmp_s[i++] = c; //1xx
            else ERR; //wrong ESC
            break;
          case '2': //2
            c = getc(file); //2x
            tmp_s[i++] = c;
            if (c <= '4' && c >= '0') { //20-4
              c = getc(file); //20-4x
              tmp_s[i++] = c;
              else if (c == '5') {
                c = getc(file); //255
                tmp_s[i++] = c;
              } else ERR; //256+ wrong ESC or char
            } else ERR; //26+ wrong esc or char
          default:
            ERR; //3+ wrong esc or char
          }
        } // end of ESC

        tmp_s[i++] = c; //hocijaky znaky treba zmenit + pridelat dynamickou alokaci //TODO
      }
      if (c == EOF)
        RETURN(0);
      if (c == '"') {
        tmp_s[i] = '\0';
        tmpToken.info = tmp_s; //"tohle tam je bez uvozovek\0"
        RETURN(30); //string
      }
      // STRING_END
      break;
    case "OPERATOR":
      // OPERATOR_START  ( c == '+'|| c == '-'|| c == '*'|| c == '\\' || c == '<' || c == '>' || c == '=' )
      switch (c) {
      case '<':
        if (c = getc(file) == '>') {
          tmpToken.info = NULL;
          RETURN(2); //<>
        } else if (c == '=') {
          tmpToken.info = NULL;
          RETURN(3); // <=
        } else {
          ungetc(c, file);
          tmpToken.info = NULL;
          RETURN(1); //<
        }
        break;
      case '=':
        tmpToken.info = NULL;
        RETURN(4); //=
        break;
      case '>':
        if (c = getc(file) == '=') {
          tmpToken.info = NULL;
          RETURN(6); // >=
        } else {
          ungetc(c, file);
          tmpToken.info = NULL;
          RETURN(5); //>
        }
        break;
      case '*':
        tmpToken.info = NULL;
        RETURN(9); //*
        break;
      case '\\':
        tmpToken.info = NULL;
        RETURN(10);

        break;
      default:
        ERR; // jinej operator?
      }

      // OPERATOR_END
      break;
    default:
      ERR; // WRONG STATE? HOW?
    } //switch

  } while (!(c == EOF));
  RETURN(0); // token for EOF
}
