/*
 * parser.c
 */

#include "parser.h"

token *currToken;
token *lastToken;

list *hTable;
list *lTable;
param curr_function;
param *returnVal;
int tokenGetPos = 0;

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
parse_errno assignment();

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

		lTable = ltab_init();

		curr_function.id = "SCOPE";
		curr_function.is_define = true;
		curr_function.type = -1;
		list_insert(hTable, curr_function);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

		if((ret =main_body()) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

//		ltab_destroy(lTable);

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

		curr_function.id = currToken->info;
		curr_function.is_define = false;


		if((ret = check_LEFTP()) != PARSE_OK)
			return (ret);

		if ((ret = par_list()) != PARSE_OK)
			return (ret);

		if ((ret = check_AS()) != PARSE_OK)
			return (ret);

		if ((ret = var_type()) != PARSE_OK)
			return (ret);

		curr_function.type = currToken->type;
		list_insert(hTable, curr_function);

		if ((ret = check_EOL()) != PARSE_OK)
			return (ret);

		curr_function.id = NULL;

		if ((ret = prog_body()) != PARSE_OK)
			return (ret);
		break;
	case FUNCTION:
		puts("FUNCTION correct");

		lTable = ltab_init();

		if((ret = check_ID()) != PARSE_OK)
			return (ret);

		curr_function.id = currToken->info;
		curr_function.is_define = false;
		list_insert(hTable, curr_function);

		if((ret = check_LEFTP()) != PARSE_OK)
			return (ret);

		if((ret = par_list()) != PARSE_OK)
			return (ret);

		if((ret = check_AS()) != PARSE_OK)
			return (ret);

		if((ret = var_type()) != PARSE_OK)
			return (ret);

		curr_function.type = currToken->type;
		change_isdefine(hTable, curr_function);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

		if((ret = fnc_body()) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

//		ltab_destroy(lTable);
		curr_function.id = NULL;

		if((ret = prog_body()) != PARSE_OK)
			return (ret);
		break;
	default:
		puts("Expected FUNCTION, DECLARE, SCOPE or EOF");
		return (SYNTAX_ERR);
	}
	return (PARSE_OK);
}

parse_errno main_body(){
	puts("main_body() entered");
	currToken = getToken();
	switch(currToken->type){
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
	case END:
		puts("END correct");
		currToken = getToken();
		if(currToken->type != FUNCTION){
			warning_msg("expected FUNCTION after END");
			return (SYNTAX_ERR);
		}
		puts("FUNCTION correct");
//		ltab_destroy(lTable);
		lTable = NULL;
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
	case ELSE:
		puts("ELSE correct");

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

		if((ret = if_body()) != PARSE_OK)
			return (ret);
	}
	return (PARSE_OK);
}

parse_errno else_body(){
	puts("else_body() entered");
	currToken = getToken();
	switch(currToken->type){
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


		param p;
		p.id = currToken->info;

		if((ret = check_AS()) != PARSE_OK)
			return (ret);

		if((ret = var_type()) != PARSE_OK)
			return (ret);

		p.type = currToken->type;
		if(list_insert_param(hTable, curr_function, p))
			return (SEMANTIC_REDEF);

		if(lTable)
			if(list_insert(lTable, p))
				return (SEMANTIC_REDEF);

		if((ret = par_next()) != PARSE_OK)
			return (ret);
		break;
	default:
		puts("Expected ) or IDENTIFIER");
		return (SYNTAX_ERR);
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

		param p;
		p.id = currToken->info;

		if((ret = check_AS()) != PARSE_OK)
			return (ret);

		if((ret = var_type()) != PARSE_OK)
			return (ret);

		p.type = currToken->type;
		if(list_insert_param(hTable, curr_function, p))
			return (SEMANTIC_REDEF);

		if(lTable)
			if(list_insert(lTable, p))
				return (SEMANTIC_REDEF);

		if((ret = par_next()) != PARSE_OK)
			return (ret);
		break;
	default:
		puts("Expected ) or ,");
		return (SYNTAX_ERR);
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
		break;
	default:
		puts("Expected ), IDENTIFIER or value");
		return (SYNTAX_ERR);
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
		break;
	default:
		puts("Expected ) or ,");
		return (SYNTAX_ERR);
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
		break;
	default:
		puts("Expected IDENTIFIER or value");
		return (SYNTAX_ERR);
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
	default:
		puts("Expected var type");
		return (SYNTAX_ERR);
	}
	return (PARSE_OK);
}

parse_errno print_exp(){
	puts("print_exp() entered");

	if((ret = assignment()) != PARSE_OK)
		return (ret);

	if(currToken->type != SEMICOLON){
		warning_msg("expected ; after assignment() in Print");
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
	switch(currToken->type){
	case INPUT:
		puts("INPUT correct");
		if((ret = check_ID()) != PARSE_OK)
			return (ret);

		if(curr_function.id && !(find_test(lTable, currToken->info)))
			return(SEMANTIC_REDEF);
		puts("ID defined");

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);
		break;
	case IF:
		puts("IF correct");

		currToken = parseExpression(NULL, NULL, lTable);

		if(currToken->type != THEN){
			warning_msg("expected THEN after EXP");
			return (SYNTAX_ERR);
		}
		puts("THEN correct");

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

		if((ret = if_body()) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);
		break;
	case IDENTIFIER:
		puts("ID correct");

		if(curr_function.id && !(find_test(lTable, currToken->info)))
			return(SEMANTIC_REDEF);
		puts("ID defined");

		returnVal = find(lTable, currToken->info);

		currToken = getToken();

		if(currToken->type != EQUAL){
			warning_msg("expected = after ID");
			return (SYNTAX_ERR);
		}
		puts("= correct");

		if((ret = assignment()) != PARSE_OK)
			return (ret);

		if(currToken->type != EOL){
			warning_msg("expected EOL after assignment()");
			return (SYNTAX_ERR);
		}
		puts("EOL correct");
		break;
	case RETURN0:
		puts("RETURN correct");
		if((ret = assignment()) != PARSE_OK)
			return (ret);

		if(currToken->type != EOL){
			warning_msg("expected EOL after assignment()");
			return (SYNTAX_ERR);
		}
		break;
	case DO:
		puts("DO correct");
		currToken = getToken();
		if(currToken->type != WHILE){
			warning_msg("expected WHILE after DO");
			return (SYNTAX_ERR);
		}
		puts("WHILE correct");

		currToken = parseExpression(NULL, NULL, lTable);

		if(currToken->type != EOL){
			warning_msg("expected EOL after assignment()");
			return (SYNTAX_ERR);
		}

		if((ret = while_body()) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);
		break;
	case PRINT:
		puts("PRINT correct");

		if((ret = print_exp()) != PARSE_OK)
			return (ret);
		break;
	case DIM:
			puts("DIM correct");

			if((ret = check_ID()) != PARSE_OK)
				return (ret);

			if(!lTable){
				warning_msg("ID: %s declared outside of function", currToken->info);
				return(SEMANTIC_REDEF);
			}

			param p;
			p.id = currToken->info;

			if((ret = check_AS()) != PARSE_OK)
				return (ret);

			if((ret = var_type()) != PARSE_OK)
				return (ret);

			p.type = currToken->type;

			if(list_insert(lTable, p))
				return(SEMANTIC_REDEF);

			if((ret = check_EOL()) != PARSE_OK)
				return (ret);
			break;
	default:
		puts("Unexpected command");
		return (SYNTAX_ERR);
	}
	return (PARSE_OK);
}

parse_errno assignment(){
	puts("assignment() entered");
	currToken = getToken();

	switch(currToken->type){
	case IDENTIFIER:
		puts("ID correct");


		if(!find_test(hTable, currToken->info)){
			if(!find_test(lTable, currToken->info)){
				return (SEMANTIC_REDEF);
			}
			puts("NOT FUNCTION -> ExpressionParser");
			currToken = parseExpression(currToken, returnVal, lTable);
			break;
		}

		if((ret = check_LEFTP()) != PARSE_OK)
			return (ret);

		if((ret = arg_list()) != PARSE_OK)
			return (ret);

		currToken = getToken();
		break;
	default:
	{
		lastToken = myMalloc(sizeof(token));
		memcpy(lastToken, currToken, sizeof(token));
		currToken = parseExpression(lastToken, returnVal, lTable);
	}
	}
	return (PARSE_OK);
}

parse_errno parse(){
	hTable = ltab_init();
	parse_errno rett;
	rett = prog_body();
	puts("FILE PARSING COMPLETE:");
	if(rett == PARSE_OK)
		puts("\tSUCCESS");
	else{
		puts("\tFAILED");
		char str[12];
		sprintf(str, "%d", currToken->type);
		printf("failure at token: %s : %s\n", currToken->info, str);
	}
	ltab_destroy(hTable);
	return (rett);
}

