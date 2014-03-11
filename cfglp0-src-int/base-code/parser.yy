									
/*********************************************************************************************

                                cfglp : A CFG Language Processor
                                --------------------------------

           About:

           Implemented   by  Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker    (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs306: Language  Processors  (theory and  lab)  at  IIT
           Bombay.

           Release  date  Jan  15, 2013.  Copyrights  reserved  by  Uday
           Khedker. This  implemenation  has been made  available purely
           for academic purposes without any warranty of any kind.

           Documentation (functionality, manual, and design) and related
           tools are  available at http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/

%scanner ../scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc

%union 
{
			int integer_value;
			float float_value;
			std::string * string_value;
			list<Ast *> * ast_list;
			Ast * ast;
			Symbol_Table * symbol_table;
			Symbol_Table_Entry * symbol_entry;
			Basic_Block * basic_block;
			list<Basic_Block *> * basic_block_list;
			Procedure * procedure;
			argument * argument_obj;
			list <argument *> * list_of_argument;
			Data_Type D;
};

%token <integer_value> INTEGER_NUMBER
%token <string_value> BASIC_BLOCK
%token <float_value> FLOATING_POINT_NUMBER			
%token <string_value> NAME
%token RETURN INTEGER FLOAT DOUBLE VOID IF ELSE			
%token <string_value> GOTO
%token <integer_value> ASSIGN_OP
%token <integer_value> NOT_EQUAL
%token <integer_value> EQUAL
%token <integer_value> LT LE GT GE

%type <symbol_table> declaration_statement_list
%type <symbol_entry> declaration_statement
%type <basic_block_list> basic_block_list
%type <basic_block> basic_block
%type <ast_list> executable_statement_list
%type <ast_list> statement_list
%type <ast> assignment_statement
%type <ast> variable
%type <ast> constant
%type <ast> modified_variable
%type <ast> conditional_statement
%type <ast> goto_statement
%type <ast> exp_assign_op
%type <ast> exp_eq_ne
%type <ast> exp_le_lt_ge_gt
%type <ast> exp_add_sub
%type <ast> exp_mul_div
%type <ast> singleton
%type <ast> function_call_statement
%type <argument_obj> declaration_argument
%type <list_of_argument> argument_list
%type <argument_obj> argument
%type <ast> return_statement
%type <D> type
%type <list_of_argument> declaration_argument_list
%type <ast_list> exp_assign_op_list

%start program

%%

program:
		declaration_statement_list
		function_declaration_list
		{
		program_object.set_global_table(*$1);
		return_statement_used_flag = false;				// No return statement in the current procedure till now
		}
		procedure_list
		{
		Procedure * main_procedure = program_object.get_main_procedure(cout);
		if (main_procedure == NULL) 
		{
		int line = get_line_number();
		report_error("No main procedure defined",line);
		}

		bool ret = program_object.check_all_functions_defined();
		// check if all the functions have been defined
		if (!ret) {
		int line = get_line_number();
		report_error("Function declared but not defined",line);
		}
		if ($1)
		$1->global_list_in_proc_map_check(get_line_number());
		delete $1;
		}
	|
		procedure_name
		{
		Procedure * main_procedure = program_object.get_main_procedure(cout);
		if (main_procedure == NULL) 
		{
		int line = get_line_number();
		report_error("Function not declared",line);
		}
		  
		return_statement_used_flag = false;				// No return statement in the current procedure till now
		}
		procedure_body
		{
		}
	|
		declaration_statement_list
		{
		  
		program_object.set_global_table(*$1);
		return_statement_used_flag = false;				// No return statement in the current procedure till now

		}
		procedure_name
		{
		Procedure * main_procedure = program_object.get_main_procedure(cout);
		if (main_procedure == NULL) 
		{
		int line = get_line_number();
		report_error("No main procedure defined",line);
		}
		  
		return_statement_used_flag = false;				// No return statement in the current procedure till now
		}
		procedure_body
		{
		  
		bool ret = program_object.set_procedure_map(*current_procedure);
		if (!ret) {
		int line = get_line_number();
		report_error("Function already defined, overloading not allowed",line);
		}
		}
	|
		function_declaration_list procedure_list
		{
		Procedure * main_procedure = program_object.get_main_procedure(cout);
		if (main_procedure == NULL) 
		{
		int line = get_line_number();
		report_error("No main procedure defined",line);
		}
		bool ret = program_object.check_all_functions_defined();
		// check if all the functions have been defined
		if (!ret) {
		int line = get_line_number();
		report_error("Function declared but not defined",line);
		}
		}
		;

function_declaration_list:
		function_declaration_list function_declaration
		{
		}
	|
		function_declaration
		{
		}
		;

function_declaration:
		type
		NAME '(' declaration_argument_list ')' ';'
		{
		Procedure * current_procedure = new Procedure ($1, *($2), *($4));
		// push to procedure_map of program object
		bool ret = program_object.set_procedure_map(*current_procedure);
		if (program_object.variable_in_symbol_list_check(*($2))) {
			int line = get_line_number();
			report_error("Function name the same as that of a global",line);
		}
		if (!ret) { 
		int line = get_line_number();
		report_error("Function already defined, overloading not allowed",line);
		}
		}
	|
		VOID
		NAME '(' declaration_argument_list ')' ';'
		{
		Procedure * current_procedure = new Procedure (void_data_type, *($2), *($4));
		// push to procedure_map of program object
		bool ret = program_object.set_procedure_map(*current_procedure);
		if (program_object.variable_in_symbol_list_check(*($2))) {
			int line = get_line_number();
			report_error("Function name the same as that of a global",line);
		}
		if (!ret) { 
		int line = get_line_number();
		report_error("Function already defined, overloading not allowed",line);
		}
		}
	|
		type
		NAME '(' ')' ';'
		{
		Procedure * current_procedure = new Procedure ($1, *($2), *(new (list <argument*>)));
		// push to procedure_map of program object
		bool ret = program_object.set_procedure_map(*current_procedure);
		if (program_object.variable_in_symbol_list_check(*($2))) {
			int line = get_line_number();
			report_error("Function name the same as that of a global",line);
		}
		if (!ret) { 
		int line = get_line_number();
		report_error("Function already defined, overloading not allowed",line);
		}
		}
	|
		VOID
		NAME '(' ')' ';'
		{
		Procedure * current_procedure = new Procedure (void_data_type, *($2), *(new (list<argument*>)));
		// push to procedure_map of program object
		bool ret = program_object.set_procedure_map(*current_procedure);
		if (program_object.variable_in_symbol_list_check(*($2))) {
			int line = get_line_number();
			report_error("Function name the same as that of a global",line);
		}
		if (!ret) { 
		int line = get_line_number();
		report_error("Function already defined, overloading not allowed",line);
		}
		}
		;

declaration_argument_list:
		declaration_argument_list ',' declaration_argument
		{
		$$= $1;
		$$->push_back ($3);
		}
	|
		declaration_argument
		{
		$$= new (list <argument*>);
		$$->push_back($1);
		}
		;

declaration_argument:
		type NAME
		{
		$$= new argument ($1, *($2));
		}
	|
		type
		{
		$$= new argument (($1), "");
		}
;

procedure_name:
		NAME '(' argument_list ')'
		{
		// create a new procedure
		current_procedure= new Procedure(void_data_type, *($1), *($3));
		program_object.push_to_list(*$1);
		// push to procedure_map of program object
		bool ret = program_object.set_procedure_map(*current_procedure);
		if (!ret) { 
		int line = get_line_number();
		fflush(stdout);
		report_error("Function already defined, overloading not allowed",line);
		}
		}
	|
		NAME '(' ')'
		{
		list <argument *> * new_list = new  (list <argument*>);
		// create a new procedure
		current_procedure= new Procedure(void_data_type, *($1), *new_list);
		program_object.push_to_list(*$1);
		// push to procedure_map of program object
		bool ret = program_object.set_procedure_map(*current_procedure);
		if (!ret) { 
		int line = get_line_number();
		report_error("Function already defined, overloading not allowed",line);
		}

		}
		;


argument_list:
		argument_list ',' argument
		{
		$$= $1;
		$$->push_back ($3);
		}
	|
		argument
		{
		$$= new (list <argument*>);
		$$->push_back($1);
		}
;

argument:
		type NAME
		{
		$$= new argument ($1, *($2));
		}
		;

type:
		INTEGER
		{
		$$= int_data_type;
		}
	|
		FLOAT
		{
		$$= float_data_type;
		}
	|
		DOUBLE
		{
		$$= double_data_type;
		}
		;

procedure_list:
		procedure
		{
		}
	|
		procedure_list procedure
		{
		}
		;

procedure:
		NAME '(' argument_list ')'
		{
		// if the function is main
		if (*$1 == "main") {
		current_procedure = new Procedure (void_data_type, "main", *(new (list<argument*>)));
		// push to procedure_map of program object
		bool ret = program_object.set_procedure_map(*current_procedure);
		if (!ret) { 
		int line = get_line_number();
		report_error("Function already defined, overloading not allowed",line);
		}
		program_object.push_to_list(*$1);
		}
		else {
		current_procedure = program_object.get_procedure(*$1);
		if (current_procedure == NULL) {
		int line = get_line_number();
		report_error("Function declaration not present",line);
		}
		// check that function has been declared at the definition time and that is compatible with its earlier signature
		if (!current_procedure->match_argument_list($3))
		{
			int line = get_line_number();
			report_error("Failed to match argument list", line);
		}
		current_procedure->push_arguments_into_symbol_table();
		program_object.push_to_list(*$1);
		}
		}
		procedure_body
		{
		// find the corresponding program object from the declaration
		// Procedure procedure = ;
		// $($ )= &procedure;
		}
	|
		NAME '(' ')'
		{
		// if the function is main
		if (*$1 == "main") {
		current_procedure = new Procedure (void_data_type, "main", *(new (list<argument*>)));
		// push to procedure_map of program object
		bool ret = program_object.set_procedure_map(*current_procedure);
		if (!ret) { 
		int line = get_line_number();
		report_error("Function already defined, overloading not allowed",line);
		}
		program_object.push_to_list(*$1);
		}
		else {
		current_procedure = program_object.get_procedure(*$1);
		if (current_procedure == NULL) {
		int line = get_line_number();
		report_error("Function declaration not present",line);
		}
		// check that function has been declared at the definition time and that is compatible with its earlier signature
		if (!current_procedure->match_argument_list(NULL)) {
		int line = get_line_number();
		report_error("Failed to match argument list", line);
		}
		program_object.push_to_list(*$1);
		}
		}
		procedure_body
		{
		// find the corresponding program object from the declaration
		// Procedure procedure = ;
		// set some attributes of the procedure
		// i.e. set the names of all the arguments
		// also check if the definition matches the declaration
		// $($ )= &procedure;
		}
		;

procedure_body:
		'{'
		declaration_statement_list
		{
		current_procedure->set_local_list(*$2);
		current_procedure->push_arguments_into_symbol_table();
		delete $2;
		}
		basic_block_list '}'
		{
		  
		// no return statement necessary
		/* if (return_statement_used_flag == false) */
		/* 	{ */
		/* 		int line = get_line_number(); */
		/* 		report_error("Atleast 1 basic block should have a return statement", line); */
		/* 	} */
		    
		    
		if (max_bb_num < max_goto_bb_num)
		{
		int line = get_line_number();
		report_error("Basic block does not exist", line);
		}

	  
		current_procedure->set_basic_block_list(*($4));

		delete $4;
		}
	|
		'{' basic_block_list '}'
		{
		  
		/* if (return_statement_used_flag == false) */
		/* { */
		/* 	int line = get_line_number(); */
		/* 	report_error("Atleast 1 basic block should have a return statement", line); */
		/* } */
		    
		if (max_bb_num < max_goto_bb_num)
		{
		  int line = get_line_number();
		report_error("Basic block does not exist", line);
		}
	  
		current_procedure->set_basic_block_list(*($2));

		delete $2;
		}
		;

declaration_statement_list:
		declaration_statement
		{
		  
		int line = get_line_number();
		program_object.variable_in_proc_map_check($1->get_variable_name(), line);

		string var_name = $1->get_variable_name();
		if (current_procedure && current_procedure->get_proc_name() == var_name)
		{
		int line = get_line_number();
		report_error("Variable name cannot be same as procedure name", line);
		}

		$$ = new Symbol_Table();
		$$->push_symbol($1);
		}
	|
		declaration_statement_list declaration_statement
		{
		  
		// if declaration is local then no need to check in global list
		// if declaration is global then this list is global list
		    
		int line = get_line_number();
		program_object.variable_in_proc_map_check($2->get_variable_name(), line);

		string var_name = $2->get_variable_name();
		if (current_procedure && current_procedure->get_proc_name() == var_name)
		{
		int line = get_line_number();
		report_error("Variable name cannot be same as procedure name", line);
		}

		if ($1 != NULL)
		{
		if($1->variable_in_symbol_list_check(var_name))
		{
		int line = get_line_number();
		report_error("Variable is declared twice", line);
		}

		$$ = $1;
		}

		else
		$$ = new Symbol_Table();

		$$->push_symbol($2);
		}
		;

declaration_statement:
		type NAME ';'
		{
		$$ = new Symbol_Table_Entry(*($2), $1);
		delete $2;
		}
		;

basic_block_list:
		basic_block_list basic_block
		{
		  
		if (!$2)
		{
		int line = get_line_number();
		report_error("Basic block doesn't exist", line);
		}

		bb_strictly_increasing_order_check(current_procedure->get_and_increment_basic_block_number(), $2->get_bb_number());

		$$ = $1;
		$$->push_back($2);
		max_bb_num = max ($2->get_bb_number(), max_bb_num);
		}
	|
		basic_block
		{
		  
		if (!$1)
		{
		int line = get_line_number();
		report_error("Basic block doesn't exist", line);
		}

		bb_strictly_increasing_order_check(current_procedure->get_and_increment_basic_block_number(), $1->get_bb_number());

		max_bb_num = max ($1->get_bb_number(), max_bb_num);

		$$ = new list<Basic_Block *>;
		$$->push_back($1);
		}
	
		;

basic_block:
		BASIC_BLOCK ':' executable_statement_list
		{
		  
		int i = 0;
		for (; (*$1)[i] != ' '; ++i) {
		  
		}
		++i;
		int id = 0;
		for (; (*$1)[i] != '>'; ++i) {
		  id = id *10 +  ((*$1)[i]-'0');
		}
		if ($3!= NULL)
		$$= new Basic_Block (id, *$3);
		else {
		list<Ast *> * ast_list = new list<Ast *>;
		$$= new Basic_Block (id, *ast_list);
		}
		delete $3;
		}
	|
		BASIC_BLOCK ':' executable_statement_list conditional_statement
		{
		  
		int i = 0;
		for (; (*$1)[i] != ' '; ++i) {
		  
		}
		++i;
		int id = 0;
		for (; (*$1)[i] != '>'; ++i) {
		  id = id *10 +  ((*$1)[i]-'0');
		}
		// append conditional statement to executable_statement_list by using push_back
		if ($3 != NULL)
		{
		$3->push_back ($4);
		$$ = new Basic_Block(id, *$3);
		}
		else
		{
		list<Ast *> * ast_list = new list<Ast *>;
		ast_list->push_back ($4);
		$$ = new Basic_Block(id, *ast_list);
		}

		delete $3;
		}
	|
		BASIC_BLOCK ':' executable_statement_list goto_statement
		  {
		    
		int i = 0;
		for (; (*($1))[i] != ' '; ++i) {
		  
		}
		++i;
		int id = 0;
		for (; (*($1))[i] != '>'; ++i) {
		  id = id *10 +  ((*($1))[i]-'0');
		}
		// append goto statement to ast_list by using push_back
		if ($3 != NULL)
		{
		  $3->push_back ($4);
		$$ = new Basic_Block(id, *$3);
		}
		else
		{
		  list<Ast *> * ast_list = new list<Ast *>;
		ast_list->push_back ($4);
		$$ = new Basic_Block(id, *ast_list);
		}

		delete $3;
		}
		;

executable_statement_list:
		statement_list
		{
		  
		$$ = $1;
		}
	|
		statement_list return_statement ';'
		{
		return_statement_used_flag = true;					// Current procedure has an occurrence of return statement

		if ($1 != NULL)
		$$ = $1;

		else
		$$ = new list<Ast *>;

		$$->push_back($2);
		}
		;

return_statement:
		RETURN
		{
		$$ = new Return_Ast(NULL);
		}
	|
		RETURN exp_assign_op
		{
		int line = get_line_number();
		bool ret = current_procedure->check_return_type ($2->get_data_type());
		if (!ret) {
		report_error("Return type mismatch error",line);
		}
		$$ = new Return_Ast($2);
		}
	;

statement_list:
		{
		  $$ = NULL;
		}
	|
		statement_list assignment_statement
		{
		  if ($1 == NULL)
		$$ = new list<Ast *>;

		else
		$$ = $1;

		$$->push_back($2);
		}
	|
		statement_list function_call_statement ';'
		{
		if ($1 == NULL)
		$$ = new list<Ast *>;

		else
		$$ = $1;

		$$->push_back($2);
		}
		;

assignment_statement:
		variable ASSIGN_OP exp_assign_op ';'
		{
		$$ = new Assignment_Ast($1, $3);
		int line = get_line_number();
		$$->check_ast(line);
		}
		;

function_call_statement:
		NAME '(' exp_assign_op_list ')'
		{
		$$ = new Functional_Call_Ast(*$1, *$3);
		Procedure * called_procedure = program_object.get_procedure(*$1);
		if (called_procedure == NULL) {
		int line = get_line_number();
		report_error("Function does not exist", line);
		}
		// check if the call matches the signature
		if (!called_procedure->match_function_call($3)) {
		int line = get_line_number();
		report_error("Function call does not match its signature",line);
		}
		$$->set_data_type(called_procedure->get_return_type());
		}
	|
		NAME '(' ')'
		{
		$$ = new Functional_Call_Ast(*$1, *(new list<Ast*>()));
		Procedure * called_procedure = program_object.get_procedure(*$1);
		if (called_procedure == NULL) {
		int line = get_line_number();
		report_error("Function does not exist", line);
		}
		// check if the call matches the signature
		if (!called_procedure->match_function_call(NULL)) {
		int line = get_line_number();
		report_error("Function call does not match its signature",line);
		}
		$$->set_data_type(called_procedure->get_return_type());
		}
		;

exp_assign_op_list:
		exp_assign_op_list ',' exp_assign_op
		{
		$$ = $1;
		$$->push_back($3);
		}
	|
		exp_assign_op
		{
		$$ = new (list<Ast*>);
		$$->push_back($1);
		}
		;

variable:
		NAME
		{
		Symbol_Table_Entry var_table_entry;

		if (current_procedure->variable_in_symbol_list_check(*$1))
		var_table_entry = current_procedure->get_symbol_table_entry(*$1);

		else if (program_object.variable_in_symbol_list_check(*$1))
		var_table_entry = program_object.get_symbol_table_entry(*$1);

		else
		{
		int line = get_line_number();
		report_error("Variable has not declared", line);
		}

		$$ = new Name_Ast(*$1, var_table_entry);

		delete $1;
		}
	|
		'(' type ')' NAME
		{
		  Symbol_Table_Entry var_table_entry;

		if (current_procedure->variable_in_symbol_list_check(*$4))
		var_table_entry = current_procedure->get_symbol_table_entry(*$4);

		else if (program_object.variable_in_symbol_list_check(*$4))
		var_table_entry = program_object.get_symbol_table_entry(*$4);

		else
		{
		  int line = get_line_number();
		report_error("Variable has not been declared", line);
		}
		Name_Ast * toCast = new Name_Ast(*$4, var_table_entry);
		$$ = new Cast_Name_Ast(toCast, $2);
		delete $4;
		}
		;

constant:
		INTEGER_NUMBER
		{
		  $$ = new Number_Ast<int>($1, int_data_type);
		}
	|
		FLOATING_POINT_NUMBER
		{
		  $$ = new Number_Ast<float>($1, float_data_type);
		}
		;

conditional_statement:
		IF '(' exp_assign_op ')'
		GOTO BASIC_BLOCK ';'
		ELSE
		GOTO BASIC_BLOCK ';'
		{
		  int i = 0;
		for (; (*($6))[i] != ' '; ++i) {
		  
		}
		++i;
		int id1 = 0;
		for (; (*($6))[i] != '>'; ++i) {
		  id1 = id1 *10 +  ((*($6))[i]-'0');
		}

		i = 0;
		for (; (*($10))[i] != ' '; ++i) {
		  
		}
		++i;
		int id2 = 0;
		for (; (*($10))[i] != '>'; ++i) {
		  id2 = id2 *10 +  ((*($10))[i]-'0');
		}

		$$ = new Conditional_Ast ($3, id1, id2);
		}
		;

goto_statement:
		GOTO BASIC_BLOCK ';'
		{
		  int i = 0;
		for (; (*($2))[i] != ' '; ++i) {
		  
		}
		++i;
		int id = 0;
		for (; (*($2))[i] != '>'; ++i) {
		  id = id *10 +  ((*($2))[i]-'0');
		}
		max_goto_bb_num = max (max_goto_bb_num, id);
		$$ = new Goto_Ast (id);
		}
		;

modified_variable:
		variable
		{
		  $$ = $1;
		}
	|
		constant
		{
		  $$ = $1;
		}
		;

exp_assign_op: 
		exp_assign_op ASSIGN_OP exp_eq_ne
		{
		  
		$$ = new Expr_Ast ($1, 1, $3);
		int line = get_line_number();
		$$->check_ast(line);
		}
	|
		exp_eq_ne
		{
		  
		$$ = $1;
		}
		;

exp_eq_ne:
		exp_eq_ne EQUAL exp_le_lt_ge_gt
		{
		  
		$$ = new Expr_Ast ($1, 7, $3);
		int line = get_line_number();
		$$->check_ast(line);
		}
	|
		exp_eq_ne NOT_EQUAL exp_le_lt_ge_gt
		{
		  
		$$ = new Expr_Ast ($1, 6, $3);
		int line = get_line_number();
		$$->check_ast(line);
		}
	|
		exp_le_lt_ge_gt
		{
		  $$ = $1;
		}
		;

exp_le_lt_ge_gt:
		exp_le_lt_ge_gt GE exp_add_sub
		{
		  $$ = new Expr_Ast ($1, 3, $3);
		int line = get_line_number();
		$$->check_ast(line);
		}
	|
		exp_le_lt_ge_gt GT exp_add_sub
		{
		  $$ = new Expr_Ast ($1, 5, $3);
		int line = get_line_number();
		$$->check_ast(line);
		}
	|
		exp_le_lt_ge_gt LE exp_add_sub
		{
		  $$ = new Expr_Ast ($1, 2, $3);
		int line = get_line_number();
		$$->check_ast(line);
		}
	|
		exp_le_lt_ge_gt LT exp_add_sub
		{
		  $$ = new Expr_Ast ($1, 4, $3);
		int line = get_line_number();
		$$->check_ast(line);
		}
	|
		exp_add_sub
		{
		  $$ = $1;
		}
		;

exp_add_sub:
		exp_add_sub '+' exp_mul_div
		{
		  $$ = new Expr_Ast ($1, 8, $3);
		int line = get_line_number();
		$$->check_ast(line);
		}
	|
		exp_add_sub '-' exp_mul_div
		{
		  $$ = new Expr_Ast ($1, 9, $3);
		int line = get_line_number();
		$$->check_ast(line);
		}
	|
		exp_mul_div
		{
		  $$ = $1;
		}
		;

exp_mul_div:
     		exp_mul_div '*' singleton
		{
		  $$ = new Expr_Ast ($1, 10, $3);
		int line = get_line_number();
		$$->check_ast(line);
		}
	|
		exp_mul_div '/' singleton
		{
		  $$ = new Expr_Ast ($1, 11, $3);
		int line = get_line_number();
		$$->check_ast(line);
		}
	|
		singleton
		{
		  $$ = $1;
		}
		;

singleton:
		function_call_statement
		{
		$$ = $1;
		}
	|
		'(' type ')' function_call_statement
		{
		$$ = new Cast_Expr_Ast ($4, $2);
		}
	|	
		modified_variable
		{
		  $$ = $1;
		}
	|
		'-' modified_variable
		{
		$$ = new Expr_Ast ($2, 12, NULL);
		int line = get_line_number();
		$$->check_ast(line);
		}
	|
		'(' exp_assign_op ')'
		{
		  $$ = $2;
		}
	|
		'-' '(' exp_assign_op ')'
		{
		  $$ = new Expr_Ast ($3, 12, NULL);
		int line = get_line_number();
		$$->check_ast(line);
		}
	|	
		'(' type ')' '(' exp_assign_op ')'
		{
		  // make a new cast expression ast object pointer
		 $$ = new Cast_Expr_Ast ($5, $2);
		}
		;
