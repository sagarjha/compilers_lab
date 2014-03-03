									
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
						
%start program

%%

program:
		declaration_statement_list function_declaration_list 
		{
		#if 0
		  
		program_object.set_global_table(*$1);
		return_statement_used_flag = false;				// No return statement in the current procedure till now

		#endif
		}
		procedure_list
		{
		#if 0
		  
		program_object.set_procedure_map(*current_procedure);

		if ($1)
		$1->global_list_in_proc_map_check(get_line_number());

		delete $1;
		#endif
		}
	|
		procedure_name
		{
		#if 0
		  
		return_statement_used_flag = false;				// No return statement in the current procedure till now
		#endif
		}
		procedure_body
		{
		#if 0
		  
		program_object.set_procedure_map(*current_procedure);
		#endif
		}
	|
		declaration_statement_list procedure_name
		{
		#if 0
		  
		return_statement_used_flag = false;				// No return statement in the current procedure till now
		#endif
		}
		procedure_body
		{
		#if 0
		  
		program_object.set_procedure_map(*current_procedure);
		#endif
		}
	|
		function_declaration_list procedure_list
		{

		}
		;

function_declaration_list:
		function_declaration_list function_declaration
	|
		function_declaration
		;

function_declaration:
		type
		NAME '(' declaration_argument_list ')' ';'
	|
		VOID
		NAME '(' declaration_argument_list ')' ';'
	|
		type
		NAME '(' ')' ';'
	|
		VOID
		NAME '(' ')' ';'
		;

declaration_argument_list:
		declaration_argument_list ',' declaration_argument
	|
		declaration_argument
		;

declaration_argument:
		type NAME
		{
		#if 0
		$$ = new argument ($1, *$2);
		#endif
		}
	|
		type
		{
		#if 0
		$$ = new argument ($1, "dummy");
		#endif
		}
;

procedure_name:
		NAME '(' argument_list ')'
		{
		#if 0
		  
		current_procedure = new Procedure(void_data_type, *$1);
		#endif
		}
	|
		NAME '(' ')'
		{
		#if 0
		  
		current_procedure = new Procedure(void_data_type, *$1);
		#endif
		}
		;


argument_list:
		argument_list ',' argument
	|
		argument
		{
		#if 0
		$$ = new (list <argument>);
		$$.push(*$1);
		#endif
		}
;

argument:
		type NAME
		{
		#if 0
		$$ = new argument ($1, *$2);
		#endif
		}
		;

type:
		INTEGER
		{
		#if 0
		$$ = int_data_type;
		#endif
		}
	|
		FLOAT
		{
		#if 0
		$$ = float_data_type;
		#endif
		}
	|
		DOUBLE
		{
		#if 0
		$$ = double_data_type;
		#endif
		}
		;

procedure_list:
		procedure
	|
		procedure_list procedure
		;

procedure:
		NAME '(' argument_list ')'
		procedure_body
	|
		NAME '(' ')'
		procedure_body
		;

procedure_body:
		'{'
		declaration_statement_list
		{
		#if 0
		  
		current_procedure->set_local_list(*$2);
		delete $2;
		#endif
		}
		basic_block_list '}'
		{
		#if 0
		  
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

	  
		current_procedure->set_basic_block_list(*$4);

		delete $4;
		#endif
		}
	|
		'{' basic_block_list '}'
		{
		#if 0
		  
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
	  
		current_procedure->set_basic_block_list(*$2);

		delete $2;
		#endif
		}
		;

declaration_statement_list:
		declaration_statement
		{
		#if 0
		  
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
		#endif
		}
	|
		declaration_statement_list declaration_statement
		{
		#if 0
		  
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
		#endif
		}
		;

declaration_statement:
		type NAME ';'
		{
		#if 0
		  $$ = new Symbol_Table_Entry(*$2, int_data_type);
		delete $2;
		#endif
		}
		;

basic_block_list:
		basic_block_list basic_block
		{
		#if 0
		  
		if (!$2)
		{
		int line = get_line_number();
		report_error("Basic block doesn't exist", line);
		}

		bb_strictly_increasing_order_check($2->get_bb_number());

		$$ = $1;
		$$->push_back($2);
		max_bb_num = max ($2->get_bb_number(), max_bb_num);
		#endif
		}
	|
		basic_block
		{
		#if 0
		  
		if (!$1)
		{
		  int line = get_line_number();
		report_error("Basic block doesn't exist", line);
		}

		bb_strictly_increasing_order_check($1->get_bb_number());

		max_bb_num = max ($1->get_bb_number(), max_bb_num);

		$$ = new list<Basic_Block *>;
		$$->push_back($1);
		#endif
		}
	
		;

basic_block:
		BASIC_BLOCK ':' executable_statement_list
		{
		#if 0
		  
		int i = 0;
		for (; (*($1))[i] != ' '; ++i) {
		  
		}
		++i;
		int id = 0;
		for (; (*($1))[i] != '>'; ++i) {
		  id = id *10 +  ((*($1))[i]-'0');
		}
		if ($3 != NULL)
		$$ = new Basic_Block (id, *$3);
		else {
		  list<Ast *> * ast_list = new list<Ast *>;
		$$ = new Basic_Block (id, *ast_list);
		}
		delete $3;
		#endif
		}
	|
		BASIC_BLOCK ':' executable_statement_list conditional_statement
		{
		#if 0
		  
		int i = 0;
		for (; (*($1))[i] != ' '; ++i) {
		  
		}
		++i;
		int id = 0;
		for (; (*($1))[i] != '>'; ++i) {
		  id = id *10 +  ((*($1))[i]-'0');
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
		#endif
		}
	|
		BASIC_BLOCK ':' executable_statement_list goto_statement
		  {
		#if 0
		    
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
		#endif
		}
		;

executable_statement_list:
		statement_list
		{
		#if 0
		  
		$$ = $1;
		#endif
		}
	|
		statement_list return_statement ';'
		{
		#if 0
		  Ast * ret = new Return_Ast();

		return_statement_used_flag = true;					// Current procedure has an occurrence of return statement

		if ($1 != NULL)
		$$ = $1;

		else
		$$ = new list<Ast *>;

		$$->push_back(ret);
		#endif
		}
		;

return_statement:
		RETURN
	|
		RETURN exp_assign_op
	;

statement_list:
		{
		#if 0
		  $$ = NULL;
		#endif
		}
	|
		statement_list assignment_statement
		{
		#if 0
		  if ($1 == NULL)
		$$ = new list<Ast *>;

		else
		$$ = $1;

		$$->push_back($2);
		#endif
		}
	|
		statement_list function_call_statement ';'
		{
		  
		}
		;

assignment_statement:
		variable ASSIGN_OP exp_assign_op ';'
		{
		#if 0
		  $$ = new Assignment_Ast($1, $3);
		int line = get_line_number();
		$$->check_ast(line);
		#endif
		}
		;

function_call_statement:
		NAME '(' exp_assign_op_list ')'
		{
                #if 0
		$$ = new Assignment_Ast();
		#endif
		}
	|
		NAME '(' ')'
		{
                #if 0
		$$ = new Assignment_Ast();
		#endif
		}
		;

exp_assign_op_list:
		exp_assign_op_list ',' exp_assign_op
	|
		exp_assign_op
		;

variable:
		NAME
		{
		#if 0
		  Symbol_Table_Entry var_table_entry;

		if (current_procedure->variable_in_symbol_list_check(*$1))
		var_table_entry = current_procedure->get_symbol_table_entry(*$1);

		else if (program_object.variable_in_symbol_list_check(*$1))
		var_table_entry = program_object.get_symbol_table_entry(*$1);

		else
		{
		int line = get_line_number();
		report_error("Variable has not been declared", line);
		}

		$$ = new Name_Ast(*$1, var_table_entry);

		delete $1;
		#endif
		}
	|
		'(' type ')' NAME
		{
		#if 0
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
		$$ = new Cast_Name_Ast(toCast, int_data_type);
		delete $4;
		#endif
		}
		;

constant:
		INTEGER_NUMBER
		{
		#if 0
		  $$ = new Number_Ast<int>($1, int_data_type);
		#endif
		}
	|
		FLOATING_POINT_NUMBER
		{
		#if 0
		  $$ = new Number_Ast<float>($1, float_data_type);
		#endif
		}
		;

conditional_statement:
		IF '(' exp_assign_op ')'
		GOTO BASIC_BLOCK ';'
		ELSE
		GOTO BASIC_BLOCK ';'
		{
		#if 0
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
		#endif
		}
		;

goto_statement:
		GOTO BASIC_BLOCK ';'
		{
		#if 0
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
		#endif
		}
		;

modified_variable:
		variable
		{
		#if 0
		  $$ = $1;
		#endif
		}
	|
		constant
		{
		#if 0
		  $$ = $1;
		#endif
		}
		;

exp_assign_op: 
		exp_assign_op ASSIGN_OP exp_eq_ne
		{
		#if 0
		  
		$$ = new Expr_Ast ($1, 1, $3);
		int line = get_line_number();
		$$->check_ast(line);
		#endif
		}
	|
		exp_eq_ne
		{
		#if 0
		  
		$$ = $1;
		#endif
		}
		;

exp_eq_ne:
		exp_eq_ne EQUAL exp_le_lt_ge_gt
		{
		#if 0
		  
		$$ = new Expr_Ast ($1, 7, $3);
		int line = get_line_number();
		$$->check_ast(line);
		#endif
		}
	|
		exp_eq_ne NOT_EQUAL exp_le_lt_ge_gt
		{
		#if 0
		  
		$$ = new Expr_Ast ($1, 6, $3);
		int line = get_line_number();
		$$->check_ast(line);
		#endif
		}
	|
		exp_le_lt_ge_gt
		{
		#if 0
		  $$ = $1;
		#endif
		}
		;

exp_le_lt_ge_gt:
		exp_le_lt_ge_gt GE exp_add_sub
		{
		#if 0
		  $$ = new Expr_Ast ($1, 3, $3);
		int line = get_line_number();
		$$->check_ast(line);
		#endif
		}
	|
		exp_le_lt_ge_gt GT exp_add_sub
		{
		#if 0
		  $$ = new Expr_Ast ($1, 5, $3);
		int line = get_line_number();
		$$->check_ast(line);
		#endif
		}
	|
		exp_le_lt_ge_gt LE exp_add_sub
		{
		#if 0
		  $$ = new Expr_Ast ($1, 2, $3);
		int line = get_line_number();
		$$->check_ast(line);
		#endif
		}
	|
		exp_le_lt_ge_gt LT exp_add_sub
		{
		#if 0
		  $$ = new Expr_Ast ($1, 4, $3);
		int line = get_line_number();
		$$->check_ast(line);
		#endif
		}
	|
		exp_add_sub
		{
		#if 0
		  $$ = $1;
		#endif
		}
		;

exp_add_sub:
		exp_add_sub '+' exp_mul_div
		{
		#if 0
		  $$ = new Expr_Ast ($1, 8, $3);
		int line = get_line_number();
		$$->check_ast(line);
		#endif
		}
	|
		exp_add_sub '-' exp_mul_div
		{
		#if 0
		  $$ = new Expr_Ast ($1, 9, $3);
		int line = get_line_number();
		$$->check_ast(line);
		#endif
		}
	|
		exp_mul_div
		{
		#if 0
		  $$ = $1;
		#endif
		}
		;

exp_mul_div:
     		exp_mul_div '*' singleton
		{
		#if 0
		  $$ = new Expr_Ast ($1, 10, $3);
		int line = get_line_number();
		$$->check_ast(line);
		#endif
		}
	|
		exp_mul_div '/' singleton
		{
		#if 0
		  $$ = new Expr_Ast ($1, 11, $3);
		int line = get_line_number();
		$$->check_ast(line);
		#endif
		}
	|
		singleton
		{
		#if 0
		  $$ = $1;
		#endif
		}
		;

singleton:
		function_call_statement
	|
		'(' type ')' function_call_statement
		{
		#if 0
		$$ = $4;
		#endif
		}
	|	
		modified_variable
		{
		#if 0
		  $$ = $1;
		#endif
		}
	|
		'-' modified_variable
		{
		#if 0
		  $$ = new Expr_Ast ($2, 12, NULL);
		int line = get_line_number();
		$$->check_ast(line);
		#endif
		}
	|
		'(' exp_assign_op ')'
		{
		#if 0
		  $$ = $2;
		#endif
		}
	|
		'-' '(' exp_assign_op ')'
		{
		#if 0
		  $$ = new Expr_Ast ($3, 12, NULL);
		int line = get_line_number();
		$$->check_ast(line);
		#endif
		}
	|	
		'(' type ')' '(' exp_assign_op ')'
		{
		#if 0
		  // make a new cast expression ast object pointer
		 $$ = new Cast_Expr_Ast ($5, int_data_type);
		#endif
		}
		;
