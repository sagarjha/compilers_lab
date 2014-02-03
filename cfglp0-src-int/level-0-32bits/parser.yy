						
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
%token <string_value> NAME
%token RETURN INTEGER IF ELSE 
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
%type <ast_list> assignment_statement_list
%type <ast> assignment_statement
%type <ast> predicate
%type <ast> variable
%type <ast> constant
%type <ast> modified_variable
%type <ast> conditional_statement
%type <ast> goto_statement
%type <ast> flp
%type <ast> slp
%type <ast> tlp
%type <ast> tlt
						
%start program

%%

program:
	declaration_statement_list procedure_name
	{
		program_object.set_global_table(*$1);
		return_statement_used_flag = false;				// No return statement in the current procedure till now
	}
	procedure_body
	{
		program_object.set_procedure_map(*current_procedure);

		if ($1)
			$1->global_list_in_proc_map_check(get_line_number());

		delete $1;
	}
|
	procedure_name
	{
		return_statement_used_flag = false;				// No return statement in the current procedure till now
	}
	procedure_body
	{
		program_object.set_procedure_map(*current_procedure);
	}
;

procedure_name:
	NAME '(' ')'
	{
		current_procedure = new Procedure(void_data_type, *$1);
	}
;

procedure_body:
	'{' declaration_statement_list
	{
		current_procedure->set_local_list(*$2);
		delete $2;
	}
	basic_block_list '}'
	{
	/* if (return_statement_used_flag == false) */
	/* 	{ */
	/* 		int line = get_line_number(); */
	/* 		report_error("Atleast 1 basic block should have a return statement", line); */
	/* 	} */

		current_procedure->set_basic_block_list(*$4);

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

		current_procedure->set_basic_block_list(*$2);

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
	INTEGER NAME ';'
	{
		$$ = new Symbol_Table_Entry(*$2, int_data_type);

		delete $2;
	}
;

basic_block_list:
	basic_block_list basic_block
	{
		if (!$2)
		{
			int line = get_line_number();
			report_error("Basic block doesn't exist1", line);
		}

		bb_strictly_increasing_order_check($$, $2->get_bb_number());

		$$ = $1;
		$$->push_back($2);
	}
|
	basic_block
	{
		if (!$1)
		{
		    int line = get_line_number();
			report_error("Basic block doesn't exist2", line);
		}

		$$ = new list<Basic_Block *>;
		$$->push_back($1);
	}
	
;

basic_block:
	BASIC_BLOCK ':' executable_statement_list
	{
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
	}
|
	BASIC_BLOCK ':' executable_statement_list conditional_statement
	{
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
	assignment_statement_list
	{
	  $$ = $1;
	}
|
	assignment_statement_list RETURN ';'
	{
	  Ast * ret = new Return_Ast();

	  return_statement_used_flag = true;					// Current procedure has an occurrence of return statement

	  if ($1 != NULL)
	    $$ = $1;

	  else
	    $$ = new list<Ast *>;

	  $$->push_back(ret);
	}
;

assignment_statement_list:
	{
	  $$ = NULL;
	}
|
	assignment_statement_list assignment_statement
{
  if ($1 == NULL)
    $$ = new list<Ast *>;

  else
    $$ = $1;

  $$->push_back($2);
	}
;

assignment_statement:
	variable ASSIGN_OP variable ';'
	{
		$$ = new Assignment_Ast($1, $3);

		int line = get_line_number();
		$$->check_ast(line);
	}
|
	variable ASSIGN_OP constant ';'
	{
		$$ = new Assignment_Ast($1, $3);

		int line = get_line_number();
		$$->check_ast(line);
	}
|
	variable ASSIGN_OP flp ';'
	{
		$$ = new Assignment_Ast($1, $3);

		int line = get_line_number();
		$$->check_ast(line);
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
			report_error("Variable has not been declared", line);
		}

		$$ = new Name_Ast(*$1, var_table_entry);

		delete $1;
	}
;

constant:
	INTEGER_NUMBER
	{
		$$ = new Number_Ast<int>($1, int_data_type);
	}
;

conditional_statement:
	IF '(' flp ')'
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

flp: 
	flp ASSIGN_OP slp
	{	
		// pass an integer instead of '=' according to specification
		$$ = new Relational_Expr_Ast ($1, 1, $3);
		int line = get_line_number();
		$$->check_ast(line);
	}
|
	slp
	{
		$$ = $1;
	}
;

slp:
	slp EQUAL tlp
	{
		// pass an integer instead of '=' according to specification
		$$ = new Relational_Expr_Ast ($1, 7, $3);
		int line = get_line_number();
		$$->check_ast(line);
	}
|
	slp NOT_EQUAL tlp
	{
		// pass an integer instead of '=' according to specification
		$$ = new Relational_Expr_Ast ($1, 6, $3);
		int line = get_line_number();
		$$->check_ast(line);
	}
|
	tlp
	{
		$$ = $1;
	}
;

tlp:
	tlp GE tlt
	{
		// pass an integer instead of '=' according to specification
		$$ = new Relational_Expr_Ast ($1, 3, $3);
		int line = get_line_number();
		$$->check_ast(line);
	}
|
	tlp GT tlt
	{
		// pass an integer instead of '=' according to specification
		$$ = new Relational_Expr_Ast ($1, 5, $3);
		int line = get_line_number();
		$$->check_ast(line);
	}
|
	tlp LE tlt
	{
		// pass an integer instead of '=' according to specification
		$$ = new Relational_Expr_Ast ($1, 2, $3);
		int line = get_line_number();
		$$->check_ast(line);
	}
|
	tlp LT tlt
	{
		// pass an integer instead of '=' according to specification
		$$ = new Relational_Expr_Ast ($1, 4, $3);
		int line = get_line_number();
		$$->check_ast(line);
	}
|
	tlt
	{
		$$ = $1;
	}
;

tlt:
	modified_variable
	{
		$$ = $1;
	}
;
