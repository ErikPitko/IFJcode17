/*
 * parser.c
 */

#include "parser.h"

token *currToken;

parse_errno ret;
parse_errno prog_body();
parse_errno main_body();
parse_errno fnc_body();
parse_errno if_body();
parse_errno else_body();
parse_errno while_body();
parse_errno par_list();
parse_errno par_next();
parse_errno arg_list();
parse_errno arg_next();
parse_errno arg_next2();
parse_errno var_type();
parse_errno print_exp();
parse_errno command();
parse_errno assingnment();

/*--------------------------------------------*/

parse_errno check_ID(){
	currToken = getToken();
	if(currToken->type != IDENTIFIER){
		warning_msg("expected IDENTIFIER");
		return (SYNTAX_ERR);
	}
	puts("IDENTIFIER correct");
	return (PARSE_OK);
}

parse_errno check_EOL(){
	currToken = getToken();
	if (currToken->type != EOL){
		warning_msg("expected EOL");
		return (SYNTAX_ERR);
	}
	puts("EOL correct");
	return (PARSE_OK);
}

parse_errno check_LEFTP(){
	currToken = getToken();
	if(currToken->type != LEFT_PARENTHESIS){
		warning_msg("expected (");
		return (SYNTAX_ERR);
	}
	puts("( correct");
	return (PARSE_OK);
}

parse_errno check_AS(){
	currToken = getToken();
	if(currToken->type != AS){
		warning_msg("expected As");
		return (SYNTAX_ERR);
	}
	puts("As correct");
	return (PARSE_OK);

}



/*--------------------------------------------*/
parse_errno prog_body(){
	puts("prog_body() entered.");
	currToken = getToken();

	switch(currToken->type){
	case EOF0:
		return (PARSE_OK);
	case SCOPE:
		puts("SCOPE correct");

		if((ret =main_body()) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

		if ((ret = prog_body()) != PARSE_OK)
			return (ret);
		break;
	case DECLARE:
		puts("DECLARE correct");
		currToken = getToken();
		if(currToken->type != FUNCTION){
			warning_msg("expected FUNCTION after DECLARE | prog_body()");
			return (SYNTAX_ERR);
		}
		puts("FUNCTION correct");

		if((ret = check_ID()) != PARSE_OK)
			return (ret);

		if((ret = check_LEFTP()) != PARSE_OK)
			return (ret);

		if ((ret = par_list()) != PARSE_OK)
			return (ret);

		if ((ret = check_AS()) != PARSE_OK)
			return (ret);

		if ((ret = var_type()) != PARSE_OK)
			return (ret);

		if ((ret = check_EOL()) != PARSE_OK)
			return (ret);

		if ((ret = prog_body()) != PARSE_OK)
			return (ret);
		break;
	case FUNCTION:
		puts("FUNCTION correct");

		if((ret = check_ID()) != PARSE_OK)
			return (ret);

		if((ret = check_LEFTP()) != PARSE_OK)
			return (ret);

		if((ret = par_list()) != PARSE_OK)
			return (ret);

		if((ret = check_AS()) != PARSE_OK)
			return (ret);

		if((ret = var_type()) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

		if((ret = fnc_body()) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

		if((ret = prog_body()) != PARSE_OK)
			return (ret);
		break;
	}
	return (PARSE_OK);
}

parse_errno main_body(){
	puts("main_body() entered");
	currToken = getToken();
	switch(currToken->type){
	case DIM:
		puts("DIM correct");
		if((ret = check_ID()) != PARSE_OK)
			return (ret);

		if((ret = check_AS()) != PARSE_OK)
			return (ret);

		if((ret = var_type()) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

		if((ret = main_body()) != PARSE_OK)
			return (ret);
		break;
	case END:
		puts("END correct");
		currToken = getToken();
		if(currToken->type != SCOPE){
			warning_msg("Expected SCOPE after END");
			return (SYNTAX_ERR);
		}
		puts("SCOPE correct");
		break;
	default:
		if((ret = command()) != PARSE_OK)
			return (ret);

		if((ret = main_body()) != PARSE_OK)
			return (ret);
	}
	return (PARSE_OK);
}

parse_errno fnc_body(){
	puts("fnc_body() entered");
	currToken = getToken();
	switch(currToken->type){
	case DIM:
		puts("DIM correct");
		if((ret = check_ID()) != PARSE_OK)
			return (ret);

		if((ret = check_AS()) != PARSE_OK)
			return (ret);

		if((ret = var_type()) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

		if((ret = fnc_body()) != PARSE_OK)
			return (ret);
		break;
	case END:
		puts("END correct");
		currToken = getToken();
		if(currToken->type != FUNCTION){
			warning_msg("expected FUNCTION after END");
			return (SYNTAX_ERR);
		}
		puts("FUNCTION correct");
		break;
	default:
		if((ret = command()) != PARSE_OK)
			return (ret);

		if((ret = fnc_body()) != PARSE_OK)
			return (ret);
	}
	return (PARSE_OK);
}

parse_errno if_body(){
	puts("if_body() entered");
	currToken = getToken();
	switch(currToken->type){
	case DIM:
		puts("DIM correct");
		if((ret = check_ID()) != PARSE_OK)
			return (ret);

		if((ret = check_AS()) != PARSE_OK)
			return (ret);

		if((ret = var_type()) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

		if((ret = if_body()) != PARSE_OK)
			return (ret);
		break;
	case ELSE:
		puts("ELSE correct");
		if((ret = else_body()) != PARSE_OK)
			return (ret);
		break;
	case END:
		puts("END correct");
		currToken = getToken();
		if(currToken->type != IF){
			warning_msg("expected IF after END");
			return (SYNTAX_ERR);
		}
		puts("IF correct");
		break;
	default:
		if((ret = command()) != PARSE_OK)
			return (ret);

		if((ret = if_body()) != PARSE_OK)
			return (ret);
	}
	return (PARSE_OK);
}

parse_errno else_body(){
	puts("else_body() entered");
	currToken = getToken();
	switch(currToken->type){
	case DIM:
		puts("DIM correct");
		if((ret = check_ID()) != PARSE_OK)
			return (ret);

		if((ret = check_AS()) != PARSE_OK)
			return (ret);

		if((ret = var_type()) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

		if((ret = else_body()) != PARSE_OK)
			return (ret);
		break;
	case END:
		puts("END correct");
		currToken = getToken();
		if(currToken->type != IF){
			warning_msg("expected IF after END");
			return (SYNTAX_ERR);
		}
		puts("IF correct");
		break;
	default:
		if((ret = command()) != PARSE_OK)
			return (ret);

		if((ret = else_body()) != PARSE_OK)
			return (ret);
	}
	return (PARSE_OK);
}

parse_errno while_body(){
	puts("while_body() entered");
	currToken = getToken();
	switch(currToken->type){
	case DIM:
		puts("DIM correct");
		if((ret = check_ID()) != PARSE_OK)
			return (ret);

		if((ret = check_AS()) != PARSE_OK)
			return (ret);

		if((ret = var_type()) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

		if((ret = while_body()) != PARSE_OK)
			return (ret);
		break;
	case LOOP:
		puts("LOOP correct");
		break;
	default:
		if((ret = command()) != PARSE_OK)
			return (ret);

		if((ret = while_body()) != PARSE_OK)
			return (ret);
	}
	return (PARSE_OK);
}

parse_errno par_list(){
	puts("par_list() entered");
	currToken = getToken();
	switch(currToken->type){
	case RIGHT_PARENTHESIS:
		puts(") correct");
		break;
	case IDENTIFIER:
		puts("ID correct");

		if((ret = check_AS()) != PARSE_OK)
			return (ret);

		if((ret = var_type()) != PARSE_OK)
			return (ret);

		if((ret = par_next()) != PARSE_OK)
			return (ret);
	}
	return (PARSE_OK);

}

parse_errno par_next(){
	puts("par_next() entered");
	currToken = getToken();
	switch(currToken->type){
	case RIGHT_PARENTHESIS:
		puts(") correct");
		break;
	case COMMA:
		puts(", correct");

		if((ret = check_ID()) != PARSE_OK)
			return (ret);

		if((ret = check_AS()) != PARSE_OK)
			return (ret);

		if((ret = var_type()) != PARSE_OK)
			return (ret);

		if((ret = par_next()) != PARSE_OK)
			return (ret);
	}
	return (PARSE_OK);
}

parse_errno arg_list(){
	puts("arg_list() entered");
	currToken = getToken();
	switch(currToken->type){
	case RIGHT_PARENTHESIS:
		puts(") correct");
		break;
	case IDENTIFIER:
		puts("ID correct");

		if((ret = arg_next()) != PARSE_OK)
			return (ret);
		break;
	case VALUE_INTEGER:
	case VALUE_STRING:
	case VALUE_DOUBLE:
		puts("constant correct");
		if((ret = arg_next()) != PARSE_OK)
			return (ret);
	}
	return (PARSE_OK);
}

parse_errno arg_next(){
	puts("arg_next() entered");
	currToken = getToken();
	switch(currToken->type){
	case RIGHT_PARENTHESIS:
		puts(") correct");
		break;
	case COMMA:
		puts(", correct");

		if((ret = arg_next2()) != PARSE_OK)
			return (ret);
	}
	return (PARSE_OK);
}

parse_errno arg_next2(){
	puts("arg_next2() entered");
	currToken = getToken();
	switch(currToken->type){
	case IDENTIFIER:
		puts("ID correct");
		if((ret = arg_next()) != PARSE_OK)
			return (ret);
		break;
	case VALUE_INTEGER:
	case VALUE_STRING:
	case VALUE_DOUBLE:
		puts("constant correct");
		if((ret = arg_next()) != PARSE_OK)
			return (ret);
	}
	return (PARSE_OK);
}

parse_errno var_type(){
	puts("var_type() entered");
	currToken = getToken();
	switch(currToken->type){
	case INTEGER:
		puts("integer type correct");
		break;
	case DOUBLE:
		puts("double type correct");
		break;
	case STRING:
		puts("string type correct");
		break;
	case BOOLEAN:
		puts("boolean type correct");
		break;
	}
	return (PARSE_OK);
}

parse_errno print_exp(){
	puts("print_exp() entered");

	if((ret = assingment()) != PARSE_OK)
		return (ret);

	currToken = getToken();

	if(currToken->type != SEMICOLON){
		warning_msg("expected ; after expression in Print");
		return (SYNTAX_ERR);
	}
	puts("; correct");

	currToken = getToken();

	switch(currToken->type){
	case EOL:
		puts("EOL correct");
		break;
	default:
		if((ret = print_exp()) != PARSE_OK)
			return (ret);
	}
	return (PARSE_OK);
}

parse_errno command(){
	puts("command() entered");
	currToken = getToken();
	switch(currToken->type){
	case INPUT:
		puts("INPUT correct");
		if((ret = var_type()) != PARSE_OK)
			return (ret);
		if((ret = check_EOL()) != PARSE_OK)
			return (ret);
		break;
	case IF:
		//TODO EXP
		puts("IF correct");
		break;
	case IDENTIFIER:
		puts("ID correct");
		currToken = getToken();

		if(currToken->type != EQUAL){
			warning_msg("expected = after ID");
			return (SYNTAX_ERR);
		}
		puts("= correct");

		if((ret = assingnment()) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);
		break;
	case RETURN0:
		puts("RETURN correct");
		if((ret = var_type()) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);
		break;
	case DO:
		puts("DO correct");

		currToken = getToken();
		if(currToken->type != WHILE){
			warning_msg("expected WHILE after DO");
			return (SYNTAX_ERR);
		}

		//TODO EXP
		puts("= correct");
		break;
//	case PRINT:
//		puts("PRINT correct");
//
//		if((ret = print_exp()) != PARSE_OK)
//			return (ret);
//		break;
	}
	return (PARSE_OK);
}

parse_errno assingnment(){
	puts("assignment() entered");
	currToken = getToken();

	switch(currToken->type){
	//TODO EXP
	case IDENTIFIER:
		puts("ID correct");

		if((ret = check_LEFTP()) != PARSE_OK)
			return (ret);

		if((ret = arg_list()) != PARSE_OK)
			return (ret);
		break;
	}
	return (PARSE_OK);
}

int main(){
	return (EXIT_SUCCESS);
}
