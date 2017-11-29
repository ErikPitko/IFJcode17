/*
 * parser.c
 */

#include "parser.h"

token *currToken;
token *lastToken;

tHashTable *hTable;
tHashTable *lTable;
tFooListElem curr_function;
tFooListElem called_function;
tFooListElem *returnVal;

extern tFooListElem exprResult;

/*********counters************/

int param_counter;

int cycle_counter;
int if_counter;
tcStack cstack;
tcStack ifstack;


/*****************************/

bool curr_function_declared = false;
void debug(const char *form, ...);

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
	debug("IDENTIFIER correct");
	return (PARSE_OK);
}

parse_errno check_EOL(){
	currToken = getToken();
	if (currToken->type != EOL){
		warning_msg("expected EOL");
		return (SYNTAX_ERR);
	}
	debug("EOL correct");
	return (PARSE_OK);
}

parse_errno check_LEFTP(){
	currToken = getToken();
	if(currToken->type != LEFT_PARENTHESIS){
		warning_msg("expected (");
		return (SYNTAX_ERR);
	}
	debug("( correct");
	return (PARSE_OK);
}

parse_errno check_AS(){
	currToken = getToken();
	if(currToken->type != AS){
		warning_msg("expected As");
		return (SYNTAX_ERR);
	}
	debug("As correct");
	return (PARSE_OK);

}

/*--------------------------------------------*/

void debug(const char *form, ...){
	return;
	va_list args;
	fprintf(stdout, "%s", "PARSER: ");
	vfprintf(stdout, form, args);
	fprintf(stdout, "\n");
	va_end(args);
}

/*--------------------------------------------*/


parse_errno prog_body(){
	debug("prog_body() entered.");

	currToken = getToken();

	switch(currToken->type){
	case EOF0:
		debug("EOF correct");
		return (PARSE_OK);
	case EOL:
		debug("EOL correct");
		if ((ret = prog_body()) != PARSE_OK)
			return (ret);
		break;
	case SCOPE:
		debug("SCOPE correct");

		lTable = ltab_init();

		curr_function.id = "SCOPE";
		curr_function.is_define = true;
		curr_function.type = -1;
		curr_function.is_main = 1;
		list_insert(hTable, curr_function);

		debug("********SCOPE********");
		I_define_func(curr_function.id);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

		if((ret =main_body()) != PARSE_OK)
			return (ret);

//		ltab_destroy(lTable);
		lTable = NULL;
		debug("*********************");

		if ((ret = prog_body()) != PARSE_OK)
			return (ret);
		break;
	case DECLARE:
		debug("DECLARE correct");
		currToken = getToken();
		if(currToken->type != FUNCTION){
			warning_msg("expected FUNCTION after DECLARE | prog_body()");
			return (SYNTAX_ERR);
		}
		debug("FUNCTION correct");

		if((ret = check_ID()) != PARSE_OK)
			return (ret);

		curr_function.id = currToken->info;
		curr_function.is_define = false;
		curr_function.is_main = 0;
		list_insert(hTable, curr_function);

		if((ret = check_LEFTP()) != PARSE_OK)
			return (ret);

		if ((ret = par_list()) != PARSE_OK)
			return (ret);

		if ((ret = check_AS()) != PARSE_OK)
			return (ret);

		if ((ret = var_type()) != PARSE_OK)
			return (ret);

		curr_function.type = currToken->type;
		change_isdefine(hTable, curr_function);

		if ((ret = check_EOL()) != PARSE_OK)
			return (ret);

		curr_function.id = NULL;

		if ((ret = prog_body()) != PARSE_OK)
			return (ret);
		break;
	case FUNCTION:
		debug("FUNCTION correct");
		lTable = ltab_init();
		param_counter = 0;

		if((ret = check_ID()) != PARSE_OK)
			return (ret);

		curr_function.id = currToken->info;
		curr_function.is_define = false;
		curr_function.is_main = 0;

		if(list_insert(hTable, curr_function))
			curr_function_declared = true;

		debug("******FUNCTION*******");
		I_define_func(curr_function.id);

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


		if(number_param(hTable, curr_function.id) == -1)
			return (SEMANTIC_TYPE);
		if(number_param(hTable, curr_function.id) != param_counter)
			return (SEMANTIC_TYPE);


//		ltab_destroy(lTable);
		lTable = NULL;
		curr_function.id = NULL;
		curr_function_declared = false;
		param_counter = 0;

		debug("*********************");

		if((ret = prog_body()) != PARSE_OK)
			return (ret);
		break;
	default:
		debug("Expected FUNCTION, DECLARE, SCOPE or EOF");
		return (SYNTAX_ERR);
	}
	return (PARSE_OK);
}

parse_errno main_body(){
	debug("main_body() entered");
	currToken = getToken();
	switch(currToken->type){
	case END:
		debug("END correct");
		currToken = getToken();
		if(currToken->type != SCOPE){
			warning_msg("Expected SCOPE after END");
			return (SYNTAX_ERR);
		}
		debug("SCOPE correct");
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
	debug("fnc_body() entered");
	currToken = getToken();
	switch(currToken->type){
	case END:
		debug("END correct");
		currToken = getToken();
		if(currToken->type != FUNCTION){
			warning_msg("expected FUNCTION after END");
			return (SYNTAX_ERR);
		}
		debug("FUNCTION correct");
		I_define_return();
		break;
	default:

		if((ret = command()) != PARSE_OK)
			return (ret);

		if((ret = fnc_body()) != PARSE_OK)
			return (ret);
	}
	return (PARSE_OK);
}

parse_errno if_body(unsigned loc_counter){
	debug("if_body() entered");
	currToken = getToken();
	int label_num = *cstackTop(&ifstack);
	int tmp1, tmp2;
	switch(currToken->type){
	case ELSE:
		debug("ELSE correct");
//		cstackPrint("KKT", &ifstack);

//		if(loc_counter != 0){
			tmp1 = *cstackTopPop(&ifstack);
			tmp2 = *cstackTopPop(&ifstack);

			I_jump_endif(*cstackTop(&ifstack));

			cstackPush(&ifstack, tmp2);
			cstackPush(&ifstack, tmp1);
//		}
		cstackPop(&ifstack);


//		cstackPrint("KKT", &ifstack);
		I_if(*cstackTopPop(&ifstack));
		if(loc_counter == 0)
			I_endif(label_num);

		currToken = getToken();

		if(currToken->type == EOL)
			currToken = getToken();

		if((ret = else_body(label_num, loc_counter)) != PARSE_OK)
			return (ret);

		break;
	case ELSEIF:
		debug("ELSEIF correct");
		debug("*** ELSEIF GENERATION ***");

//		cstackPrint("KKT", &ifstack);

		tmp1 = *cstackTopPop(&ifstack);
		tmp2 = *cstackTopPop(&ifstack);

		I_jump_endif(*cstackTop(&ifstack));

		cstackPush(&ifstack, tmp2);
		cstackPush(&ifstack, tmp1);


//		cstackPrint("KKT1", &ifstack);
		I_if(*cstackTopPop(&ifstack));

		currToken = parseExpression(NULL, NULL, lTable);

		I_if_then(*cstackTop(&ifstack), exprResult);

		cstackPush(&ifstack, if_counter++);


		if(currToken->type != THEN){
			warning_msg("expected THEN after EXP");
			return (SYNTAX_ERR);
		}
		debug("THEN correct");

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

		loc_counter++;
		if((ret = if_body(0)) != PARSE_OK)
			return (ret);

		debug("*********************");
		break;

	case END:
		debug("END correct");
		currToken = getToken();
		if(currToken->type != IF){
			warning_msg("expected IF after END");
			return (SYNTAX_ERR);
		}
		debug("IF correct");

//		if(loc_counter == 0){
			cstackPop(&ifstack);
			cstackPop(&ifstack);
			I_endif(*cstackTopPop(&ifstack));
//		}else{
//			I_endif(*cstackTopPop(&ifstack));
//			cstackPop(&ifstack);
//			cstackPop(&ifstack);
//		}


		debug("*********************");
		break;
	default:
		if((ret = command()) != PARSE_OK)
			return (ret);

		if((ret = if_body(loc_counter)) != PARSE_OK)
			return (ret);
	}
	return (PARSE_OK);
}

parse_errno else_body(int label_num, unsigned local_counter){
	debug("else_body() entered");
	switch(currToken->type){
	case END:
		debug("END correct");
		currToken = getToken();
		if(currToken->type != IF){
			warning_msg("expected IF after END");
			return (SYNTAX_ERR);
		}
		debug("IF correct");

		I_endif(*cstackTopPop(&ifstack));

		debug("*********************");
		break;
	default:
		if((ret = command()) != PARSE_OK)
			return (ret);

		currToken = getToken();

		if((ret = else_body(label_num, local_counter)) != PARSE_OK)
			return (ret);
	}
	return (PARSE_OK);
}

parse_errno while_body(){
	debug("while_body() entered");
	currToken = getToken();
	switch(currToken->type){
	case LOOP:
		debug("LOOP correct");
		I_loop(*cstackTopPop(&cstack));
		debug("*********************");
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
	debug("par_list() entered");
	currToken = getToken();
	switch(currToken->type){
	case RIGHT_PARENTHESIS:
		debug(") correct");
		break;
	case IDENTIFIER:
		debug("ID correct");

		tFooListElem sym;
		param p;
		sym.id = currToken->info;
		p.id = currToken->info;

		if((ret = check_AS()) != PARSE_OK)
			return (ret);

		if((ret = var_type()) != PARSE_OK)
			return (ret);

		sym.type = currToken->type;
		p.type = currToken->type;


		if(curr_function_declared){
			int index;
			//TODO funkcia bez parametrov
			if((index = return_index_parameter(hTable, curr_function, p)) == -1)
				return(SEMANTIC_REDEF);
			else
				debug("index OK");
			if(index != param_counter)
				return(SEMANTIC_TYPE);
			param_counter++;
			debug("called parameter correct");
		}

		if(!list_insert_param(hTable, curr_function, p) && curr_function_declared)
			return (SEMANTIC_REDEF);

		if(lTable)
			if(list_insert(lTable, sym))
				return (SEMANTIC_REDEF);


		if((ret = par_next()) != PARSE_OK)
			return (ret);
		break;
	default:
		debug("Expected ) or IDENTIFIER");
		return (SYNTAX_ERR);
	}
	return (PARSE_OK);

}

parse_errno par_next(){
	debug("par_next() entered");
	currToken = getToken();
	switch(currToken->type){
	case RIGHT_PARENTHESIS:
		debug(") correct");
		break;
	case COMMA:
		debug(", correct");

		if((ret = check_ID()) != PARSE_OK)
			return (ret);

		tFooListElem sym;
		param p;
		sym.id = currToken->info;
		p.id = currToken->info;

		if((ret = check_AS()) != PARSE_OK)
			return (ret);

		if((ret = var_type()) != PARSE_OK)
			return (ret);

		sym.type = currToken->type;
		p.type = currToken->type;

		if(curr_function_declared){
			int index;
			if((index = return_index_parameter(hTable, curr_function, p)) == -1)
				return(SEMANTIC_REDEF);
			else
				debug("index OK");
			if(index != param_counter)
				return(SEMANTIC_TYPE);
			param_counter++;
			debug("called parameter correct");
		}

		if(!list_insert_param(hTable, curr_function, p) && curr_function_declared)
			return (SEMANTIC_REDEF);

		if(lTable)
			if(list_insert(lTable, sym))
				return (SEMANTIC_REDEF);

		if((ret = par_next()) != PARSE_OK)
			return (ret);
		break;
	default:
		debug("Expected ) or ,");
		return (SYNTAX_ERR);
	}
	return (PARSE_OK);
}

parse_errno arg_list(){
	debug("arg_list() entered");
	currToken = getToken();
	switch(currToken->type){
	case RIGHT_PARENTHESIS:
		debug(") correct");
		break;
	case IDENTIFIER:
		debug("ID correct");

		param p;
		p.id = currToken->info;

		if(return_index_parameter(hTable, called_function, p) != param_counter)
			return (SEMANTIC_TYPE);

		I_arg_i_id(currToken->info, param_find(hTable, called_function.id, p.id)->id);

		param_counter++;
		if((ret = arg_next()) != PARSE_OK)
			return (ret);
		break;
	case VALUE_INTEGER:
	case VALUE_STRING:
	case VALUE_DOUBLE:
		debug("constant correct");
		param *tmp = return_parameter_from_index(hTable, called_function, param_counter);
		if(tmp == NULL)
			return (SEMANTIC_TYPE);

		if(tmp->type != currToken->type){
			if(tmp->type == VALUE_INTEGER && currToken->type != VALUE_INTEGER)
				return (SEMANTIC_TYPE);
			if(tmp->type == VALUE_DOUBLE && currToken->type != VALUE_DOUBLE)
				return (SEMANTIC_TYPE);
			if(tmp->type == VALUE_STRING && currToken->type != VALUE_STRING)
				return (SEMANTIC_TYPE);
		}

		I_arg_i_const(currToken->info, currToken->type, tmp->id);
		param_counter++;
		if((ret = arg_next()) != PARSE_OK)
			return (ret);
		break;
	default:
		debug("Expected ), IDENTIFIER or value");
		return (SYNTAX_ERR);
	}
	return (PARSE_OK);
}

parse_errno arg_next(){
	debug("arg_next() entered");
	currToken = getToken();
	switch(currToken->type){
	case RIGHT_PARENTHESIS:
		debug(") correct");
		break;
	case COMMA:
		debug(", correct");

		if((ret = arg_next2()) != PARSE_OK)
			return (ret);
		break;
	default:
		debug("Expected ) or ,");
		return (SYNTAX_ERR);
	}
	return (PARSE_OK);
}

parse_errno arg_next2(){
	debug("arg_next2() entered");
	currToken = getToken();
	switch(currToken->type){
	case IDENTIFIER:
		debug("ID correct");

		param p;
		p.id = currToken->info;

		if(return_index_parameter(hTable, called_function, p) != param_counter)
			return (SEMANTIC_TYPE);

		I_arg_i_id(currToken->info, param_find(hTable, curr_function.id, p.id)->id);

		param_counter++;
		if((ret = arg_next()) != PARSE_OK)
			return (ret);
		break;
	case VALUE_INTEGER:
	case VALUE_STRING:
	case VALUE_DOUBLE:
		debug("constant correct");

		param *tmp = return_parameter_from_index(hTable, called_function, param_counter);
		if(tmp == NULL)
			return (SEMANTIC_TYPE);

		if(tmp->type != currToken->type){
			if(tmp->type == VALUE_INTEGER && currToken->type != VALUE_INTEGER)
				return (SEMANTIC_TYPE);
			if(tmp->type == VALUE_DOUBLE && currToken->type != VALUE_DOUBLE)
				return (SEMANTIC_TYPE);
			if(tmp->type == VALUE_STRING && currToken->type != VALUE_STRING)
				return (SEMANTIC_TYPE);
		}

		I_arg_i_const(currToken->info, currToken->type, tmp->id);
		param_counter++;

		if((ret = arg_next()) != PARSE_OK)
			return (ret);
		break;
	default:
		debug("Expected IDENTIFIER or value");
		return (SYNTAX_ERR);
	}
	return (PARSE_OK);
}

parse_errno var_type(){
	debug("var_type() entered");
	currToken = getToken();
	switch(currToken->type){
	case INTEGER:
		debug("integer type correct");
		break;
	case DOUBLE:
		debug("double type correct");
		break;
	case STRING:
		debug("string type correct");
		break;
	case BOOLEAN:
		debug("boolean type correct");
		break;
	default:
		debug("Expected var type");
		return (SYNTAX_ERR);
	}
	return (PARSE_OK);
}

parse_errno print_exp(){
	debug("print_exp() entered");
	currToken = getToken();
	switch(currToken->type){
	case SEMICOLON:
		debug("; correct");
		if((ret = print_exp()) != PARSE_OK)
			return (ret);
		break;
	case EOL:
		debug("EOL correct");
		tFooListElem val;
		val.id = "\n";
		val.type = STRING;
		I_print(val);
		break;
	default:
		if((ret = assignment()) != PARSE_OK)
			return (ret);

		I_print(exprResult);

		switch(currToken->type){
		case SEMICOLON:
			debug("; correct");
			if((ret = print_exp()) != PARSE_OK)
				return (ret);
			break;
		case EOL:
			debug("EOL correct");
			tFooListElem val;
			val.id = "\n";
			val.type = STRING;
			I_print(val);
			break;
		default:
			warning_msg("expected ; or EOL after assignment() in Print");
			return (SYNTAX_ERR);
		}
	}
	return (PARSE_OK);
}

parse_errno command(){
	debug("command() entered");
	switch(currToken->type){
	case INPUT:
		debug("INPUT correct");
		if((ret = check_ID()) != PARSE_OK)
			return (ret);

		if(curr_function.id && !(find_test(lTable, currToken->info)))
			return(SEMANTIC_REDEF);
		debug("ID defined");

		I_input_id(*function_find(lTable, currToken->info));

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);
		break;
	case IF:
		debug("IF correct");

		currToken = parseExpression(NULL, NULL, lTable);

		debug("*** IF GENERATION ***");
		for(int i = 0; i < 3; i++)
			cstackPush(&ifstack, if_counter++);

		I_if_then(*cstackTop(&ifstack), exprResult);

		if(currToken->type != THEN){
			warning_msg("expected THEN after EXP");
			return (SYNTAX_ERR);
		}
		debug("THEN correct");

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

		if((ret = if_body(0)) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);

		debug("*********************");
		break;
	case IDENTIFIER:
		debug("ID correct");

		if(curr_function.id && !(find_test(lTable, currToken->info)))
			return(SEMANTIC_REDEF);
		debug("ID defined");

		returnVal = function_find(lTable, currToken->info);

		currToken = getToken();

		if(currToken->type != EQUAL){
			warning_msg("expected = after ID");
			return (SYNTAX_ERR);
		}
		debug("= correct");

		currToken = getToken();
		if((ret = assignment()) != PARSE_OK)
			return (ret);

		if(currToken->type != EOL){
			warning_msg("expected EOL after assignment()");
			return (SYNTAX_ERR);
		}
		debug("EOL correct");
		break;
	case DO:
		debug("DO correct");
		int main_int = curr_function.is_main;
		curr_function.is_main = 0;
		currToken = getToken();
		if(currToken->type != WHILE){
			warning_msg("expected WHILE after DO");
			return (SYNTAX_ERR);
		}
		debug("WHILE correct");

		debug("********WHILE********");

		cstackPush(&cstack, cycle_counter);
		I_do_while_label(cycle_counter);

		currToken = parseExpression(NULL, NULL, lTable);

		I_do_while(cycle_counter++, exprResult);

		if(currToken->type != EOL){
			warning_msg("expected EOL after assignment()");
			return (SYNTAX_ERR);
		}

		if((ret = while_body()) != PARSE_OK)
			return (ret);

		if((ret = check_EOL()) != PARSE_OK)
			return (ret);
		curr_function.is_main = main_int;
		break;
	case PRINT:
		debug("PRINT correct");

		if((ret = print_exp()) != PARSE_OK)
			return (ret);
		break;
	case DIM:
			debug("DIM correct");

			if((ret = check_ID()) != PARSE_OK)
				return (ret);

			if(!lTable){
				warning_msg("ID: %s declared outside of function", currToken->info);
				return(SEMANTIC_REDEF);
			}

			tFooListElem p;
			p.id = currToken->info;

			if((ret = check_AS()) != PARSE_OK)
				return (ret);

			if((ret = var_type()) != PARSE_OK)
				return (ret);

			p.type = currToken->type;

			if(list_insert(lTable, p))
				return(SEMANTIC_REDEF);

			I_dim_id_as_var_type(p);

			currToken = getToken();

			switch(currToken->type){
			case EOL:
				debug("EOL correct");
				break;
			case EQUAL:
				debug("EQUAL correct");

				returnVal = &p;

				currToken = getToken();
				if((ret = assignment()) != PARSE_OK)
					return (ret);

				if(currToken->type != EOL)
					return (SYNTAX_ERR);
			}
			break;
	case RETURN0:
		if(!strcmp(curr_function.id, "SCOPE")){
			warning_msg("Unexpected RETURN in SCOPE");
			return (SYNTAX_ERR);
		}
		debug("RETURN correct");
		currToken = getToken();

		if(!find_test(hTable, currToken->info)){
			if(!find_test(lTable, currToken->info)){
				return (SEMANTIC_REDEF);
			}
			debug("NOT FUNCTION -> ExpressionParser");
			currToken = parseExpression(currToken, &curr_function, lTable);
		}else{
			return (SYNTAX_ERR);
		}

		I_move_to_global(exprResult);

		if(currToken->type != EOL){
			warning_msg("expected EOL after assignment()");
			return (SYNTAX_ERR);
		}
		break;
	case EOL:
		debug("EOL correct");
		break;
	default:
		debug("Unexpected command");
		return (SYNTAX_ERR);
	}
	return (PARSE_OK);
}

parse_errno assignment(){
	debug("assignment() entered");

	switch(currToken->type){
	case IDENTIFIER:
		debug("ID correct");

		called_function.id = currToken->info;

		if(!find_test(hTable, currToken->info)){
			if(!find_test(lTable, currToken->info)){
				return (SEMANTIC_REDEF);
			}
			debug("NOT FUNCTION -> ExpressionParser");
			currToken = parseExpression(currToken, returnVal, lTable);
			break;
		}

		I_createFrame();

		if((ret = check_LEFTP()) != PARSE_OK)
			return (ret);

		if((ret = arg_list()) != PARSE_OK)
			return (ret);

		if (param_counter != number_param(hTable, called_function.id))
			return (SEMANTIC_TYPE);

		I_callFunc(called_function.id);

		I_priradenie(*returnVal);

		param_counter = 0;
		called_function.id = NULL;
		currToken = getToken();
		returnVal = NULL;
		break;
	default:
	{
		lastToken = myMalloc(sizeof(token));
		memcpy(lastToken, currToken, sizeof(token));
		currToken = parseExpression(lastToken, returnVal, lTable);
		returnVal = NULL;
	}
	}
	return (PARSE_OK);
}

parse_errno parse(){
	init3ADD();
	hTable = ltab_init();
	cstackInit(&cstack);
	cstackInit(&ifstack);
	parse_errno rett;
	rett = prog_body();
	debug("FILE PARSING COMPLETE:");
	if(rett == PARSE_OK)
		debug("\tSUCCESS");
	else{
		debug("\tFAILED");
		char str[12];
		sprintf(str, "%d", currToken->type);
		printf("failure at token: %s : %s\n", currToken->info, str);
	}
	cstackClear(&cstack);
	ltab_destroy(hTable);
	return (rett);
}

