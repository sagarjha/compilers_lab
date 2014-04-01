
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
	Data_Type DT;
	pair<Data_Type, string> * decl;
	list<Ast *> * ast_list;
	Ast * ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Basic_Block * basic_block;
	list<Basic_Block *> * basic_block_list;
	Procedure * procedure;
	Tgt_Op target_op;
};

%token <integer_value> INTEGER_NUMBER BBNUM
%token <float_value> FLOATING_POINT_NUMBER
%token <string_value> NAME
%token RETURN INTEGER FLOAT DOUBLE IF ELSE 
%token <string_value> GOTO 
%token <integer_value> ASSIGN
%token <target_op> NOT_EQUAL
%token <target_op> EQUAL
%token <target_op> LT LE GT GE
			
%type <symbol_table> optional_variable_declaration_list
%type <symbol_table> variable_declaration_list
%type <symbol_entry> variable_declaration
%type <decl> declaration
%type <basic_block_list> basic_block_list
%type <basic_block> basic_block
%type <ast_list> executable_statement_list
%type <ast_list> assignment_statement_list
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
%type <DT> type
						
%start program
			
%%

program:
		optional_declaration_list procedure_definition
		{
		    if (NOT_ONLY_PARSE)
		{
		    CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

		program_object.set_procedure_map(current_procedure, get_line_number());
		program_object.global_list_in_proc_map_check();
		}
		}
		;

optional_declaration_list:
		{
		    if (NOT_ONLY_PARSE)
		{
		    Symbol_Table * global_table = new Symbol_Table();
		program_object.set_global_table(*global_table);
		}
		}
	|
		variable_declaration_list
		{
		    if (NOT_ONLY_PARSE)
		{
		    Symbol_Table * global_table = $1;

		CHECK_INVARIANT((global_table != NULL), "Global declarations cannot be null");

		program_object.set_global_table(*global_table);
		}
		}
		;

procedure_definition:
		NAME '(' ')'
		{
		    if (NOT_ONLY_PARSE)
		{
		    CHECK_INVARIANT(($1 != NULL), "Procedure name cannot be null");

		string proc_name = *$1;

		current_procedure = new Procedure(void_data_type, proc_name, get_line_number());
		}
		}

		'{' optional_variable_declaration_list
		{
		    if (NOT_ONLY_PARSE)
		{
		    
		    CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

		Symbol_Table * local_table = $6;

		if (local_table == NULL)
		local_table = new Symbol_Table();

		current_procedure->set_local_list(*local_table);
		max_bb_num = max_goto_bb_num = 0;
		}
		}

		basic_block_list '}'
		{
		    if (NOT_ONLY_PARSE)
		{
		    list<Basic_Block *> * bb_list = $8;

		CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");
		CHECK_INVARIANT((bb_list != NULL), "Basic block list cannot be null");
		CHECK_INVARIANT((max_bb_num >= max_goto_bb_num), "Invalid goto statement present in program");

		current_procedure->set_basic_block_list(*bb_list);
		
		}
		}
		;

optional_variable_declaration_list:
		{
		    if (NOT_ONLY_PARSE)
		{
		    $$ = NULL;
		}
		}
	|
		variable_declaration_list
		{
		    if (NOT_ONLY_PARSE)
		{
		    CHECK_INVARIANT(($1 != NULL), "Declaration statement list cannot be null here");

		$$ = $1;
		}
		}
		;

variable_declaration_list:
		variable_declaration
		{
		    if (NOT_ONLY_PARSE)
		{
		    Symbol_Table_Entry * decl_stmt = $1;

		CHECK_INVARIANT((decl_stmt != NULL), "Non-terminal declaration statement cannot be null");

		string decl_name = decl_stmt->get_variable_name();
		CHECK_INPUT ((program_object.variable_in_proc_map_check(decl_name) == false),
		"Variable name cannot be same as the procedure name", get_line_number());

		if (current_procedure != NULL)
		{
		    CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
		"Variable name cannot be same as procedure name", get_line_number());
		}

		Symbol_Table * decl_list = new Symbol_Table();
		decl_list->push_symbol(decl_stmt);

		$$ = decl_list;
		}
		}
	|
		variable_declaration_list variable_declaration
		{
		    if (NOT_ONLY_PARSE)
		{
		    // if declaration is local then no need to check in global list
		    // if declaration is global then this list is global list
		    
		    Symbol_Table_Entry * decl_stmt = $2;
		Symbol_Table * decl_list = $1;

		CHECK_INVARIANT((decl_stmt != NULL), "The declaration statement cannot be null");
		CHECK_INVARIANT((decl_list != NULL), "The declaration statement list cannot be null");

		string decl_name = decl_stmt->get_variable_name();
		CHECK_INPUT((program_object.variable_in_proc_map_check(decl_name) == false),
		"Procedure name cannot be same as the variable name", get_line_number());
		if (current_procedure != NULL)
		{
		    CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
		"Variable name cannot be same as procedure name", get_line_number());
		}

		CHECK_INPUT((decl_list->variable_in_symbol_list_check(decl_name) == false), 
		"Variable is declared twice", get_line_number());

		decl_list->push_symbol(decl_stmt);
		$$ = decl_list;
		}
		}
		;

variable_declaration:
		declaration ';'
		{
		    if (NOT_ONLY_PARSE)
		{
		    pair<Data_Type, string> * decl_stmt = $1;

		CHECK_INVARIANT((decl_stmt != NULL), "Declaration cannot be null");

		Data_Type type = decl_stmt->first;
		string decl_name = decl_stmt->second;

		Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(decl_name, type, get_line_number());

		$$ = decl_entry;

		}
		}
		;

declaration:
		INTEGER NAME
		{
		    if (NOT_ONLY_PARSE)
		{
		    CHECK_INVARIANT(($2 != NULL), "Name cannot be null");

		string name = *$2;
		Data_Type type = int_data_type;

		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);

		$$ = declar;
		}
		}
	|
		FLOAT NAME
		{
		  if (NOT_ONLY_PARSE)
		{
		  CHECK_INVARIANT(($2 != NULL), "Name cannot be null");

		string name = *$2;
		Data_Type type = float_data_type;

		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);

		$$ = declar;
		}
		}
	|
		DOUBLE NAME
		{
		  if (NOT_ONLY_PARSE)
		{
		  CHECK_INVARIANT(($2 != NULL), "Name cannot be null");

		string name = *$2;
		Data_Type type = double_data_type;

		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);

		$$ = declar;
		}
		}
		;

basic_block_list:
		basic_block_list basic_block
		{
		    if (NOT_ONLY_PARSE)
		{
		    list<Basic_Block *> * bb_list = $1;
		Basic_Block * bb = $2;

		CHECK_INVARIANT((bb_list != NULL), "New basic block cannot be null");
		CHECK_INVARIANT((bb != NULL), "Basic block cannot be null");

		
		bb_strictly_increasing_order_check(bb->get_bb_number());
		max_bb_num = max (max_bb_num, bb->get_bb_number());

		bb_list->push_back($2);
		$$ = bb_list;
		}
		}
	|
		basic_block
		{
		    if (NOT_ONLY_PARSE)
		{
		    Basic_Block * bb = $1;

		CHECK_INVARIANT((bb != NULL), "Basic block cannot be null");
		bb_strictly_increasing_order_check(bb->get_bb_number());
		max_bb_num = max (max_bb_num, bb->get_bb_number());

		list<Basic_Block *> * bb_list = new list<Basic_Block *>;
		bb_list->push_back(bb);

		$$ = bb_list;
		}
		}
		;

basic_block:
		BBNUM ':' executable_statement_list
		{
		    if (NOT_ONLY_PARSE)
		{
		    int bb_number = $1;
		list<Ast *> * exe_stmt = $3;

		CHECK_INPUT((bb_number >= 2), "Illegal basic block label", get_line_number());

		Basic_Block * bb = new Basic_Block(bb_number, get_line_number());

		if (exe_stmt != NULL)
		bb->set_ast_list(*exe_stmt);
		else
		{
		    list<Ast *> * ast_list = new list<Ast *>;
		bb->set_ast_list(*ast_list);
		}

		$$ = bb;
		}
		}
	|
		BBNUM ':' executable_statement_list conditional_statement
		{
		    if (NOT_ONLY_PARSE)
		{
		    // append conditional statement to executable_statement_list by using push_back
		if ($3 != NULL)
		{
		    $3->push_back ($4);
		$$ = new Basic_Block($1, get_line_number());
		$$->set_ast_list(*$3);
		}
		else
		{
		    list<Ast *> * ast_list = new list<Ast *>;
		ast_list->push_back ($4);
		$$ = new Basic_Block($1, get_line_number());
		$$->set_ast_list(*ast_list);
		}

		delete $3;

		}
		}
	|
		BBNUM ':' executable_statement_list goto_statement
		{
		    if (NOT_ONLY_PARSE) {
				  // append goto statement to ast_list by using push_back
		if ($3 != NULL)
		{
		    $3->push_back ($4);
		$$ = new Basic_Block($1, get_line_number());
		$$->set_ast_list(*$3);
		}
		else
		{
		    list<Ast *> * ast_list = new list<Ast *>;
		ast_list->push_back ($4);
		$$ = new Basic_Block($1, get_line_number());
		$$->set_ast_list(*ast_list);
		}

		delete $3;
		}
		}
		;

executable_statement_list:
		assignment_statement_list
		{
		    if (NOT_ONLY_PARSE)
		{
		    $$ = $1;
		}
		}
	|
		assignment_statement_list RETURN ';'
		{
		    if (NOT_ONLY_PARSE)
		{
		    list<Ast *> * assign_list = $1;
		Ast * ret = new Return_Ast(get_line_number());
		list<Ast *> * exe_list = NULL;

		if (assign_list)
		exe_list = assign_list;

		else
		exe_list = new list<Ast *>;

		exe_list->push_back(ret);

		$$ = exe_list;
		}
		}
		;

assignment_statement_list:
		{
		    if (NOT_ONLY_PARSE)
		{
		    $$ = NULL;
		}
		}
	|
		assignment_statement_list assignment_statement
		{
		    if (NOT_ONLY_PARSE)
		{
		    list<Ast *> * assign_list = $1;
		Ast * assign_stmt = $2;
		list<Ast *> * assign_list_new = NULL;

		CHECK_INVARIANT((assign_stmt != NULL), "Assignment statement cannot be null");

		if (assign_list == NULL)
		assign_list_new = new list<Ast *>;

		else
		assign_list_new = assign_list;

		assign_list_new->push_back(assign_stmt);

		$$ = assign_list_new;
		}
		}
		;

assignment_statement:
		variable ASSIGN exp_assign_op ';'
		{
		    if (NOT_ONLY_PARSE)
		{
		    CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast * lhs = $1;
		Ast * rhs = $3;

		Ast * assign = new Assignment_Ast(lhs, rhs, get_line_number());

		assign->check_ast();

		$$ = assign;
		}
		}
		;

type:
		'(' INTEGER ')'
		{
		if (NOT_ONLY_PARSE)
		{
		  $$ = int_data_type;
		}
		}
	|
		'(' FLOAT ')'
		{
		if (NOT_ONLY_PARSE)
		{
		$$ = float_data_type;
		}
		}
	|
		'(' DOUBLE ')'
		{
		if (NOT_ONLY_PARSE)
		{
		$$ = double_data_type;
		}
		}
		;

variable:
		NAME
		{
		    if (NOT_ONLY_PARSE)
		{
		    Symbol_Table_Entry * var_table_entry;

		CHECK_INVARIANT(($1 != NULL), "Variable name cannot be null");

		string var_name = *$1;

		if (current_procedure->variable_in_symbol_list_check(var_name) == true)
		var_table_entry = &(current_procedure->get_symbol_table_entry(var_name));

		else if (program_object.variable_in_symbol_list_check(var_name) == true)
		var_table_entry = &(program_object.get_symbol_table_entry(var_name));

		else
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable has not been declared");

		Ast * name_ast = new Name_Ast(var_name, *var_table_entry, get_line_number());

		$$ = name_ast;
		}
		}
	|
		type NAME
		{
		  if (NOT_ONLY_PARSE)
		{
		  Symbol_Table_Entry * var_table_entry;

		CHECK_INVARIANT(($2 != NULL), "Variable name cannot be null");

		string var_name = *$2;

		if (current_procedure->variable_in_symbol_list_check(var_name) == true)
		var_table_entry = &(current_procedure->get_symbol_table_entry(var_name));

		else if (program_object.variable_in_symbol_list_check(var_name) == true)
		var_table_entry = &(program_object.get_symbol_table_entry(var_name));

		else
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable has not been declared");

		Ast * toCast = new Name_Ast(var_name, *var_table_entry, get_line_number());
		$$ = new Cast_Name_Ast(toCast, $1, get_line_number());
		}
		}
		;

constant:
		INTEGER_NUMBER
		{
		    if (NOT_ONLY_PARSE)
		{
		    int num = $1;

		Ast * num_ast = new Number_Ast<int>(num, int_data_type, get_line_number());

		$$ = num_ast;
		}
		}
	|	
		FLOATING_POINT_NUMBER
		{
		    if (NOT_ONLY_PARSE)
		{
		float fnum = $1;
		Ast * fnum_ast = new Number_Ast<float>(fnum, float_data_type, get_line_number());

		$$ = fnum_ast;
		}
		}
		;

conditional_statement:
		IF '(' exp_assign_op ')'
		GOTO BBNUM ';'
		ELSE
		GOTO BBNUM ';'
		{
		    if (NOT_ONLY_PARSE)
		{
			max_goto_bb_num = max (max_goto_bb_num, $6);
			max_goto_bb_num = max (max_goto_bb_num, $10);
		    $$ = new Conditional_Ast ($3, $6, $10, get_line_number());
		}
		}
		;

goto_statement:
		GOTO BBNUM ';'
		{
		    if (NOT_ONLY_PARSE)
		{
		max_goto_bb_num = max (max_goto_bb_num, $2);
		$$ = new Goto_Ast ($2, get_line_number());
		}
		}
		;

modified_variable:
		variable
		{
		    if (NOT_ONLY_PARSE)
		{
		    $$ = $1;
		}
		}
	|
		constant
		{
		    if (NOT_ONLY_PARSE)
		{
		    $$ = $1;
		}
		}
		;

exp_assign_op: 
		exp_assign_op ASSIGN exp_eq_ne
		{
		    if (NOT_ONLY_PARSE)
		{
		  // pass an integer instead of '=' according to specification
		$$ = new Expr_Ast ($1, nop, $3, get_line_number());
		$$->check_ast();
		}
		}
	|
		exp_eq_ne
		{
		    if (NOT_ONLY_PARSE)
		{
		    $$ = $1;
		}
		}
		;

exp_eq_ne:
		exp_eq_ne EQUAL exp_le_lt_ge_gt
		{
		    if (NOT_ONLY_PARSE)
		{
		    // pass an integer instead of '=' according to specification
		    $$ = new Expr_Ast ($1, seq, $3, get_line_number());
		$$->check_ast();
		}
		}
	|
		exp_eq_ne NOT_EQUAL exp_le_lt_ge_gt
		{
		    if (NOT_ONLY_PARSE)
		{
		    // pass an integer instead of '=' according to specification
		    $$ = new Expr_Ast ($1, sne, $3, get_line_number());
		$$->check_ast();
		}
		}
	|
		exp_le_lt_ge_gt
		{
		    if (NOT_ONLY_PARSE)
		{
		$$ = $1;
		}
		}
		;

exp_le_lt_ge_gt:
		exp_le_lt_ge_gt GE exp_add_sub
		{
		    if (NOT_ONLY_PARSE)
		{
		    // pass an integer instead of '=' according to specification
		    $$ = new Expr_Ast ($1, sge, $3, get_line_number());
		$$->check_ast();
		}
		}
	|
		exp_le_lt_ge_gt GT exp_add_sub
		{
		    if (NOT_ONLY_PARSE)
		{
		    // pass an integer instead of '=' according to specification
		    $$ = new Expr_Ast ($1, sgt, $3, get_line_number());
		$$->check_ast();
		}
		}
	|
		exp_le_lt_ge_gt LE exp_add_sub
		{
		    if (NOT_ONLY_PARSE)
		{
		    // pass an integer instead of '=' according to specification
		    $$ = new Expr_Ast ($1, sle, $3, get_line_number());
		$$->check_ast();
		}
		}
	|
		exp_le_lt_ge_gt LT exp_add_sub
		{
		    if (NOT_ONLY_PARSE)
		{
		    // pass an integer instead of '=' according to specification
		    $$ = new Expr_Ast ($1, slt, $3, get_line_number());
		$$->check_ast();
		}
		}
	|
		exp_add_sub
		{
		    if (NOT_ONLY_PARSE)
		{
		    $$ = $1;
		}
		}
		;

exp_add_sub:
		exp_add_sub '+' exp_mul_div
		{
		  if (NOT_ONLY_PARSE)
		{
		  // fill with the correct operand
		if ($1->get_data_type() == int_data_type) {
		$$ = new Expr_Ast($1,add,$3, get_line_number());
		}
		else {
		$$ = new Expr_Ast($1,add_d,$3, get_line_number());
		}
		$$->check_ast();
		}
		}
	|	
		exp_add_sub '-' exp_mul_div
		{
		  if (NOT_ONLY_PARSE)
		{
		  // fill with the correct operand
		if ($1->get_data_type() == int_data_type) {
		$$ = new Expr_Ast($1,sub,$3, get_line_number());
		}
		else {
		$$ = new Expr_Ast($1,sub_d,$3, get_line_number());		  
		}
		$$->check_ast();
		}
		}
|
		exp_mul_div
		{
		  if (NOT_ONLY_PARSE)
		{
		$$ = $1;
		}
		}
		;

exp_mul_div:
     		exp_mul_div '*' singleton
		{
		  if (NOT_ONLY_PARSE)
		{
		  // fill with the correct operand
		if ($$->get_data_type() == int_data_type) {
		$$ = new Expr_Ast ($1,mul, $3, get_line_number());
		}
		else {
		$$ = new Expr_Ast ($1,mul_d, $3, get_line_number());
		}
		$$->check_ast();
		}
		}
	|
		exp_mul_div '/' singleton
		{
		  if (NOT_ONLY_PARSE)
		{
		  // fill with the correct operand
		if ($$->get_data_type() == int_data_type) {
		$$ = new Expr_Ast ($1,div_i, $3, get_line_number());
		}
		else {
		$$ = new Expr_Ast ($1,div_d, $3, get_line_number());
		}
		$$->check_ast();	  
		}
		}
	|
		singleton
		{
		  if (NOT_ONLY_PARSE)
		{
		$$ = $1;
		}
		}
		;

singleton:
		modified_variable
		{
		    if (NOT_ONLY_PARSE)
		{
		    $$ = $1;
		}
		}
	|
		'-' modified_variable
		{
		  if (NOT_ONLY_PARSE)
		{
		if ($2->get_data_type() == int_data_type) {
		$$ = new Expr_Ast($2,uminus,NULL, get_line_number());
		}
		else {
		$$ = new Expr_Ast($2,uminus_d,NULL, get_line_number());
		}		  
		$$->check_ast();
		}
		}
	|	
		'(' exp_assign_op ')'
		{
		    if (NOT_ONLY_PARSE)
		{
		    $$ = $2;
		}
		}
	|
		'-' '(' exp_assign_op ')'
		{
		  if (NOT_ONLY_PARSE)
		{
		if ($3->get_data_type() == int_data_type) {
		$$ = new Expr_Ast($3,uminus,NULL, get_line_number());
		}
		else {
		$$ = new Expr_Ast($3,uminus_d,NULL, get_line_number());
		}
		$$->check_ast();
		}
		}
	|
		type '(' exp_assign_op ')'
		{
		  if (NOT_ONLY_PARSE)
		{
		$$ = new Cast_Expr_Ast($3, $1, get_line_number());
		}
		}
		;
