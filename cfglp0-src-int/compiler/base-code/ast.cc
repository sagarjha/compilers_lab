
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

#include<iostream>
#include<fstream>
#include<typeinfo>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"local-environment.hh"
#include"icode.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

Ast::Ast()
{}

Ast::~Ast()
{}

bool Ast::check_ast()
{
  stringstream msg;
  msg << "No check_ast() function for " << typeid(*this).name();
  CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Data_Type Ast::get_data_type()
{
  stringstream msg;
  msg << "No get_data_type() function for " << typeid(*this).name();
  CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::set_data_type(Data_Type data_type)
{
  stringstream msg;
  msg << "No set_data_type() function for " << typeid(*this).name();
  CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}


Symbol_Table_Entry & Ast::get_symbol_entry()
{
  stringstream msg;
  msg << "No get_symbol_entry() function for " << typeid(*this).name();
  CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
  stringstream msg;
  msg << "No print_value() function for " << typeid(*this).name();
  CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
  stringstream msg;
  msg << "No get_value_of_evaluation() function for " << typeid(*this).name();
  CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
  stringstream msg;
  msg << "No set_value_of_evaluation() function for " << typeid(*this).name();
  CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register)
{
  stringstream msg;
  msg << "No create_store_stmt() function for " << typeid(*this).name();
  CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line)
{
  lhs = temp_lhs;
  rhs = temp_rhs;

  ast_num_child = binary_arity;
  node_data_type = void_data_type;

  lineno = line;
}

Assignment_Ast::~Assignment_Ast()
{
  delete lhs;
  delete rhs;
}

Data_Type Assignment_Ast::get_data_type()
{
  return node_data_type;
}

bool Assignment_Ast::check_ast()
{
  CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
  CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");

  if (lhs->get_data_type() == rhs->get_data_type())
    {
      node_data_type = lhs->get_data_type();
      return true;
    }
  else if ((lhs->get_data_type() == float_data_type && rhs->get_data_type() == double_data_type) | (rhs->get_data_type() == float_data_type && lhs->get_data_type() == double_data_type)) {
    node_data_type = lhs->get_data_type();
    return true;
  }

  CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
		  "Assignment statement data type not compatible");
}

void Assignment_Ast::print(ostream & file_buffer)
{
  file_buffer << "\n" << AST_SPACE << "Asgn:";

  file_buffer << "\n" << AST_NODE_SPACE << "LHS (";
  lhs->print(file_buffer);
  file_buffer << ")";

  file_buffer << "\n" << AST_NODE_SPACE << "RHS (";
  rhs->print(file_buffer);
  file_buffer << ")";
}

Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
  CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
  Eval_Result & result = rhs->evaluate(eval_env, file_buffer);

  CHECK_INPUT_AND_ABORT(result.is_variable_defined(), "Variable should be defined to be on rhs of Assignment_Ast", lineno);

  CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");

  lhs->set_value_of_evaluation(eval_env, result);

  // Print the result

  print(file_buffer);

  lhs->print_value(eval_env, file_buffer);

  return result;
}

Code_For_Ast & Assignment_Ast::compile()
{
  /* 
     An assignment x = y where y is a variable is 
     compiled as a combination of load and store statements:
     (load) R <- y 
     (store) x <- R
     If y is a constant, the statement is compiled as:
     (imm_Load) R <- y 
     (store) x <- R
     where imm_Load denotes the load immediate operation.
  */

  CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
  CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

  Code_For_Ast & load_stmt = rhs->compile();

  Register_Descriptor * load_register = load_stmt.get_reg();

  Code_For_Ast store_stmt = lhs->create_store_stmt(load_register);

  // Store the statement in ic_list

  list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

  if (load_stmt.get_icode_list().empty() == false)
    ic_list = load_stmt.get_icode_list();

  if (store_stmt.get_icode_list().empty() == false)
    ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

  Code_For_Ast * assign_stmt;
  if (ic_list.empty() == false)
    assign_stmt = new Code_For_Ast(ic_list, load_register);

  return *assign_stmt;
}

Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
  CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
  CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");
    
  Code_For_Ast load_stmt;

  if ((typeid(*rhs).name() == typeid(Expr_Ast).name()) || (typeid(*rhs).name() == typeid(Cast_Expr_Ast).name()) || (typeid(*rhs).name() == typeid(Cast_Name_Ast).name())) {
    load_stmt = rhs->compile_and_optimize_ast(lra);
		
    Register_Descriptor * intermediate_result = load_stmt.get_reg();
    Register_Descriptor * lhs_allocated_reg = (lhs->get_symbol_entry()).get_register();		
		
		
    if (lhs_allocated_reg != NULL && lhs_allocated_reg != intermediate_result) {
      (lhs->get_symbol_entry()).free_register(lhs_allocated_reg);
    }
		
    (lhs->get_symbol_entry()).update_register(intermediate_result);
		
    lra.optimize_lra(mc_2m, lhs, rhs);
  }

  else {
    lra.optimize_lra(mc_2m, lhs, rhs);
    load_stmt = rhs->compile_and_optimize_ast(lra);
  }

  Register_Descriptor * result_register = load_stmt.get_reg();

  Code_For_Ast store_stmt = lhs->create_store_stmt(result_register);

  list<Icode_Stmt *> ic_list;

  if (load_stmt.get_icode_list().empty() == false)
    ic_list = load_stmt.get_icode_list();

  if (store_stmt.get_icode_list().empty() == false)
    ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

  Code_For_Ast * assign_stmt;
  if (ic_list.empty() == false)
    assign_stmt = new Code_For_Ast(ic_list, result_register);

  return *assign_stmt;
}

///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line)
{
  variable_name = name;
  variable_symbol_entry = &var_entry;

  CHECK_INVARIANT((variable_symbol_entry->get_variable_name() == name),
		  "Variable's symbol entry is not matching its name");

  ast_num_child = zero_arity;
  node_data_type = variable_symbol_entry->get_data_type();
  lineno = line;
}

Name_Ast::~Name_Ast()
{}

Data_Type Name_Ast::get_data_type()
{
  return variable_symbol_entry->get_data_type();
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry()
{
  return *variable_symbol_entry;
}

void Name_Ast::print(ostream & file_buffer)
{
  file_buffer << "Name : " << variable_symbol_entry->get_variable_name();
}

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
  string variable_name = variable_symbol_entry->get_variable_name();

  Eval_Result * loc_var_val = eval_env.get_variable_value(variable_name);
  Eval_Result * glob_var_val = interpreter_global_table.get_variable_value(variable_name);

  file_buffer << "\n" << AST_SPACE << variable_name << " : ";

  if (!eval_env.is_variable_defined(variable_name) && !interpreter_global_table.is_variable_defined(variable_name))
    file_buffer << "undefined";

  else if (eval_env.is_variable_defined(variable_name) && loc_var_val != NULL)
    {
      CHECK_INVARIANT(loc_var_val->get_result_enum() == int_result, "Result type can only be int");
      file_buffer << loc_var_val->get_value() << "\n";
    }

  else
    {
      CHECK_INVARIANT(glob_var_val->get_result_enum() == int_result, 
		      "Result type can only be int and float");

      if (glob_var_val == NULL)
	file_buffer << "0\n";
      else
	file_buffer << glob_var_val->get_value() << "\n";
    }
  file_buffer << "\n";
}

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
  string variable_name = variable_symbol_entry->get_variable_name();

  if (eval_env.does_variable_exist(variable_name))
    {
      CHECK_INPUT_AND_ABORT((eval_env.is_variable_defined(variable_name) == true), 
			    "Variable should be defined before its use", lineno);

      Eval_Result * result = eval_env.get_variable_value(variable_name);
      return *result;
    }

  CHECK_INPUT_AND_ABORT((interpreter_global_table.is_variable_defined(variable_name) == true), 
			"Variable should be defined before its use", lineno);

  Eval_Result * result = interpreter_global_table.get_variable_value(variable_name);
  return *result;
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
  Eval_Result * i;
  string variable_name = variable_symbol_entry->get_variable_name();

  if (variable_symbol_entry->get_data_type() == int_data_type)
    i = new Eval_Result_Value_Int();
  else
    CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Type of a name can be int/float only", lineno);

  if (result.get_result_enum() == int_result)
    i->set_value(result.get_value());
  else
    CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Type of a name can be int/float only", lineno);

  if (eval_env.does_variable_exist(variable_name))
    eval_env.put_variable_value(*i, variable_name);
  else
    interpreter_global_table.put_variable_value(*i, variable_name);
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
  return get_value_of_evaluation(eval_env);
}

Code_For_Ast & Name_Ast::compile()
{
  Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry, node_data_type);
  Register_Descriptor * result_register = machine_dscr_object.get_new_register(node_data_type);
  Ics_Opd * register_opd = new Register_Addr_Opd(result_register, node_data_type);

  Icode_Stmt * load_stmt;
  if (node_data_type == int_data_type) {
    load_stmt = new Move_IC_Stmt(load, opd, register_opd);
  }
  else {
    load_stmt = new Move_IC_Stmt(load_d, opd, register_opd);
  }

  list<Icode_Stmt *> ic_list;
  ic_list.push_back(load_stmt);

  Code_For_Ast & load_code = *new Code_For_Ast(ic_list, result_register);

  return load_code;
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register)
{
  CHECK_INVARIANT((store_register != NULL), "Store register cannot be null");

  Ics_Opd * register_opd = new Register_Addr_Opd(store_register, node_data_type);
  Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry, node_data_type);

  Icode_Stmt * store_stmt;
  if (node_data_type == int_data_type) {
    store_stmt = new Move_IC_Stmt(store, register_opd, opd);
  }
  else {
    store_stmt = new Move_IC_Stmt(store_d, register_opd, opd);
  }

  if (command_options.is_do_lra_selected() == false)
    variable_symbol_entry->free_register(store_register);

  list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
  ic_list.push_back(store_stmt);

  Code_For_Ast & name_code = *new Code_For_Ast(ic_list, store_register);

  return name_code;
}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
  list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

  bool load_needed = lra.is_load_needed();

  Register_Descriptor * result_register = lra.get_register();
  CHECK_INVARIANT((result_register != NULL), "Register cannot be null");
  Ics_Opd * register_opd = new Register_Addr_Opd(result_register, node_data_type);

  Icode_Stmt * load_stmt = NULL;
  if (load_needed)
    {
      Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry, node_data_type);

      if (node_data_type == int_data_type) {
	load_stmt = new Move_IC_Stmt(load, opd, register_opd);
      }
      else {
	load_stmt = new Move_IC_Stmt(load_d, opd, register_opd);
      }
			
      ic_list.push_back(load_stmt);
    }

  Code_For_Ast & load_code = *new Code_For_Ast(ic_list, result_register);

  return load_code;
}

///////////////////////////////////////////////////////////////////////////////

Cast_Name_Ast::Cast_Name_Ast(Ast * given_name, Data_Type given_data_type, int line) {
  name = given_name;
  node_data_type = data_type = given_data_type;
  lineno = line;
  ast_num_child = unary_arity;
}

Cast_Name_Ast::~Cast_Name_Ast() {
  delete name;
}

Data_Type Cast_Name_Ast::get_data_type() {
  return data_type;
}
    
void Cast_Name_Ast::print(ostream & file_buffer) {
  name->print(file_buffer);
}

Eval_Result & Cast_Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
  Eval_Result & result = name->get_value_of_evaluation(eval_env);
  // to cast the result here............STILL TO DO THIS, DUDE!

  switch(data_type){
  case int_data_type:
    {
      int new_result_value = (int) result.get_value();
      Eval_Result_Value_Int * new_res = new Eval_Result_Value_Int();
      new_res->set_value(new_result_value);
      new_res->set_variable_status(true);
      return *new_res;
    }
  case float_data_type:
    {
      float new_result_value = (float) result.get_value();
      Eval_Result_Value_Float * new_res = new Eval_Result_Value_Float();
      new_res->set_value(new_result_value);
      new_res->set_variable_status(true);
      return *new_res;
    }
  case double_data_type:
    {
      double new_result_value = (double) result.get_value();
      Eval_Result_Value_Double * new_res = new Eval_Result_Value_Double();
      new_res->set_value(new_result_value);
      new_res->set_variable_status(true);
      return *new_res;
    }
  }
}

Code_For_Ast & Cast_Name_Ast::compile() {
  CHECK_INVARIANT((name != NULL), "Name cannot be null");

  // first compile the name
  Code_For_Ast & load_stmt = name->compile();
  // nothing to be done if the cast is from float to double or from double to float
  if ((node_data_type == float_data_type && name->get_data_type() == double_data_type) || (node_data_type == double_data_type && name->get_data_type() == float_data_type)) {
    return load_stmt;
  }

  // get the load register of the name
  Register_Descriptor * load_register = load_stmt.get_reg();
  // get a new result register
  Register_Descriptor * result_register = machine_dscr_object.get_new_register(node_data_type);
  // first copy the load statements in the ic_list
  list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
  if (load_stmt.get_icode_list().empty() == false) {
    ic_list = load_stmt.get_icode_list();
  }
  // create the operands
  Ics_Opd * load_register_opd = new Register_Addr_Opd(load_register, node_data_type);
  Ics_Opd * result_register_opd = new Register_Addr_Opd(result_register, node_data_type);
    
  // create a new cast statement
  Icode_Stmt * cast_stmt;
  // for conversion from float or double to int, instr is mfc1, and for the vice-versa it is mtc1
  if (node_data_type == int_data_type) {
    cast_stmt = new Move_IC_Stmt(mfc1, load_register_opd, result_register_opd);
  }
  else {
    cast_stmt = new Move_IC_Stmt(mtc1, load_register_opd, result_register_opd);
  }

  // creates it intermediate code list
  list<Icode_Stmt *> & ic_list1 = * new list <Icode_Stmt *>;
  ic_list1.push_back(cast_stmt);
  // create its new code_for_ast
  Code_For_Ast & cast_code = * new Code_For_Ast(ic_list1, result_register);
  // append at the end
  ic_list.splice(ic_list.end(), cast_code.get_icode_list());
  // create a new code_for_ast and return
  Code_For_Ast * cast_name_stmt = new Code_For_Ast(ic_list, result_register);

  return *cast_name_stmt;
}

Code_For_Ast & Cast_Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {
  CHECK_INVARIANT((name != NULL), "Name cannot be null");

  lra.optimize_lra(mc_2r, NULL, name);
  
  bool load_needed = lra.is_load_needed();
  
  // first compile the name
  Code_For_Ast & load_stmt = name->compile_and_optimize_ast(lra);
  // nothing to be done if the cast is from float to double or from double to float
  if ((node_data_type == float_data_type && name->get_data_type() == double_data_type) || (node_data_type == double_data_type && name->get_data_type() == float_data_type)) {
    return load_stmt;
  }

  // get the load register of the name
  Register_Descriptor * load_register = load_stmt.get_reg();
  // get a new result register
  Register_Descriptor * result_register = machine_dscr_object.get_new_register(node_data_type);

  // first copy the load statements in the ic_list
  list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
  if (load_stmt.get_icode_list().empty() == false && load_needed) {
    ic_list = load_stmt.get_icode_list();
  }
  // create the operands
  Ics_Opd * load_register_opd = new Register_Addr_Opd(load_register, node_data_type);
  Ics_Opd * result_register_opd = new Register_Addr_Opd(result_register, node_data_type);
    
  // create a new cast statement
  Icode_Stmt * cast_stmt;
  // for conversion from float or double to int, instr is mfc1, and for the vice-versa it is mtc1
  if (node_data_type == int_data_type) {
    cast_stmt = new Move_IC_Stmt(mfc1, load_register_opd, result_register_opd);
  }
  else {
    cast_stmt = new Move_IC_Stmt(mtc1, load_register_opd, result_register_opd);
  }

  // creates it intermediate code list
  list<Icode_Stmt *> & ic_list1 = * new list <Icode_Stmt *>;
  ic_list1.push_back(cast_stmt);
  // create its new code_for_ast
  Code_For_Ast & cast_code = * new Code_For_Ast(ic_list1, result_register);
  // append at the end
  ic_list.splice(ic_list.end(), cast_code.get_icode_list());
  // create a new code_for_ast and return
  Code_For_Ast * cast_name_stmt = new Code_For_Ast(ic_list, result_register);

  return *cast_name_stmt;
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type constant_data_type, int line)
{
  constant = number;
  node_data_type = constant_data_type;

  ast_num_child = zero_arity;

  lineno = line;
}

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::~Number_Ast()
{}

template <class DATA_TYPE>
Data_Type Number_Ast<DATA_TYPE>::get_data_type()
{
  return node_data_type;
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::print(ostream & file_buffer)
{
  file_buffer << std::fixed;
  file_buffer << std::setprecision(2);

  file_buffer << "Num : " << constant;
}

template <class DATA_TYPE>
Eval_Result & Number_Ast<DATA_TYPE>::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
  if (node_data_type == int_data_type)
    {
      Eval_Result & result = *new Eval_Result_Value_Int();
      result.set_value(constant);

      return result;
    }
}

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile()
{
  Register_Descriptor * result_register = machine_dscr_object.get_new_register(node_data_type);
  CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");
  Ics_Opd * load_register = new Register_Addr_Opd(result_register, node_data_type);
  Ics_Opd * opd;
  if (node_data_type == int_data_type) {
    opd = new Const_Opd<int>(constant, int_data_type);
  }
  else {
    opd = new Const_Opd<float>(constant, float_data_type);
  }

  Icode_Stmt * load_stmt;
  if (node_data_type == int_data_type) {
    load_stmt = new Move_IC_Stmt(imm_load, opd, load_register);
  }
  else {
    load_stmt = new Move_IC_Stmt(imm_load_d, opd, load_register);
  }

  list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
  ic_list.push_back(load_stmt);

  Code_For_Ast & num_code = *new Code_For_Ast(ic_list, result_register);

  return num_code;
}

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile_and_optimize_ast(Lra_Outcome & lra)
{
  CHECK_INVARIANT((lra.get_register() != NULL), "Register assigned through optimize_lra cannot be null");
  Ics_Opd * load_register = new Register_Addr_Opd(lra.get_register(), node_data_type);
  Ics_Opd * opd;
  if (node_data_type == int_data_type) {
    opd = new Const_Opd<int>(constant, int_data_type);
  }
  else {
    opd = new Const_Opd<float>(constant, float_data_type);  
  }

  Icode_Stmt * load_stmt;
  if (node_data_type == int_data_type) {
    load_stmt = new Move_IC_Stmt(imm_load, opd, load_register);
  }
  else {
    load_stmt = new Move_IC_Stmt(imm_load_d, opd, load_register);
  }

  list<Icode_Stmt *> ic_list;
  ic_list.push_back(load_stmt);

  Code_For_Ast & num_code = *new Code_For_Ast(ic_list, lra.get_register());

  return num_code;
}

///////////////////////////////////////////////////////////////////////////////

// code for the implementation of relational expressions
Expr_Ast::Expr_Ast (Ast* l, Tgt_Op oper, Ast* r, int line)
{
  lhs = l;
  rhs = r;
  op = oper;
  lineno = line;
  ast_num_child = binary_arity;
}

Expr_Ast::~Expr_Ast()
{
  delete lhs,rhs;
}

bool Expr_Ast::check_ast()
{
  CHECK_INVARIANT((lhs != NULL), "Lhs of Relational_Ast cannot be null");
  // rhs can be null
  CHECK_INVARIANT((rhs != NULL || op == uminus || op == uminus_d), "Rhs of Relational_Ast cannot be null");
  
  if (op == uminus || op == uminus_d) {
    node_data_type = lhs->get_data_type();
    return true;
  }

  else if (op == sle || op == sge || op == slt || op == sgt || op == sne || op == seq ) {
    node_data_type = int_data_type;
    return true;
  }
  
  else if ((lhs->get_data_type() == float_data_type && rhs->get_data_type() == double_data_type) | (rhs->get_data_type() == float_data_type && lhs->get_data_type() == double_data_type)) {
    node_data_type = lhs->get_data_type();
    return true;
  }
  
  else if (lhs->get_data_type() == rhs->get_data_type())
    {
      node_data_type = lhs->get_data_type();
      return true;
    }

  CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
		  "Relational statement data type not compatible");
}

Data_Type Expr_Ast::get_data_type()
{
  return node_data_type;
}

// needs to be modified
void Expr_Ast::print(ostream & file_buffer)
{

  string op_str;

  switch (op) {
  case 1:
    op_str = "=";
    break;
  case 2:
    op_str = "LE";
    break;
  case 3:
    op_str = "GE";
    break;
  case 4:
    op_str = "LT";
    break;
  case 5:
    op_str = "GT";
    break;
  case 6:
    op_str = "NE";
    break;
  case 7:
    op_str = "EQ";
    break;
  }
    
  file_buffer << "\n" << AST_NODE_SPACE << "Condition: " << op_str << "\n";

  file_buffer << AST_SUB_NODE_SPACE << "LHS (";
  lhs->print(file_buffer);
  file_buffer << ")\n";

  file_buffer << AST_SUB_NODE_SPACE << "RHS (";
  rhs->print(file_buffer);
  file_buffer << ")";
}

Eval_Result & Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
  // assignment operation
  if (op == 1)
    {
      CHECK_INVARIANT((rhs != NULL), "Rhs of Expr_Ast cannot be null");
      Eval_Result & result = rhs->evaluate(eval_env, file_buffer);

      CHECK_INPUT_AND_ABORT(result.is_variable_defined(), "Variable should be defined to be on rhs of Expr_Ast", lineno);

      CHECK_INVARIANT((lhs != NULL), "Lhs of Expr_Ast cannot be null");
	    
      lhs->set_value_of_evaluation(eval_env, result);

      // Print the result

      print(file_buffer);

      lhs->print_value(eval_env, file_buffer);

      Eval_Result & result_final = *new Eval_Result_Value_Int();
      result_final.set_value(1);
      return result_final;
    }
	
  else
    {
      CHECK_INVARIANT((lhs != NULL), "Lhs of Expr_Ast cannot be null");
      CHECK_INVARIANT((rhs != NULL), "Lhs of Expr_Ast cannot be null");
      Eval_Result & left_eval_result = lhs->evaluate(eval_env, file_buffer);
      Eval_Result & right_eval_result = rhs->evaluate(eval_env, file_buffer);
		
      int l_int_value = left_eval_result.get_value();
      int r_int_value = right_eval_result.get_value();
		
      int ans = 0;
		
      switch (op) {
      case 2: // LE
	ans = (l_int_value <= r_int_value)? 1 : 0;
	break;
      case 3: // GE
	ans = (l_int_value >= r_int_value)? 1 : 0;
	break;
      case 4: // LT
	ans = (l_int_value < r_int_value)? 1 : 0;
	break;
      case 5: // GT
	ans = (l_int_value > r_int_value)? 1 : 0;
	break;
      case 6: // NE
	ans = (l_int_value != r_int_value)? 1 : 0;
	break;
      case 7: // EQ
	ans = (l_int_value == r_int_value)? 1 : 0;
	break;
      }
		
      Eval_Result & result = *new Eval_Result_Value_Int();
      result.set_value(ans);
      return result;
    }
}

Code_For_Ast & Expr_Ast::compile() {
	
  CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
  CHECK_INVARIANT((rhs != NULL || op == uminus || op == uminus_d), "Rhs cannot be null");

  Code_For_Ast & load_stmt_left = lhs->compile();
  Register_Descriptor * load_register_left = load_stmt_left.get_reg();
  load_register_left->set_use_for_expr_result();

  Code_For_Ast set_stmt;
  Register_Descriptor * load_register_right = NULL;
  
  // do not compile rhs if null
  if (rhs) {
    Code_For_Ast & load_stmt_right = rhs->compile();
    load_register_right = load_stmt_right.get_reg();	
    load_register_right->set_use_for_expr_result();

    // op+1 to make the ENUM code of the operation concordant with the self-assigned codes from before
    set_stmt = create_set_stmt(op, load_register_left, load_register_right);
    list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

    if (load_stmt_left.get_icode_list().empty() == false)
      ic_list = load_stmt_left.get_icode_list();

    if (load_stmt_right.get_icode_list().empty() == false)
      ic_list.splice(ic_list.end(), load_stmt_right.get_icode_list());

    if (set_stmt.get_icode_list().empty() == false)
      ic_list.splice(ic_list.end(), set_stmt.get_icode_list());

    // Store the statement in ic_list
    Code_For_Ast * set_stmt_final;
    if (ic_list.empty() == false)
      set_stmt_final = new Code_For_Ast(ic_list, set_stmt.get_reg());
	
    load_register_left->reset_use_for_expr_result();
    if (load_register_right) {
      load_register_right->reset_use_for_expr_result();
    }

    return *set_stmt_final;
  }

  else {
    set_stmt = create_set_stmt(op, load_register_left);
    list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

    if (load_stmt_left.get_icode_list().empty() == false)
      ic_list = load_stmt_left.get_icode_list();

    if (set_stmt.get_icode_list().empty() == false)
      ic_list.splice(ic_list.end(), set_stmt.get_icode_list());
  
    // Store the statement in ic_list
    Code_For_Ast * set_stmt_final;
    if (ic_list.empty() == false)
      set_stmt_final = new Code_For_Ast(ic_list, set_stmt.get_reg());
	
    load_register_left->reset_use_for_expr_result();
    if (load_register_right) {
      load_register_right->reset_use_for_expr_result();
    }

    return *set_stmt_final;      
  }
}

Code_For_Ast & Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {
  CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
  // CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

  lra.optimize_lra(mc_2r, NULL, lhs);
  Code_For_Ast & load_stmt_left = lhs->compile_and_optimize_ast(lra);
  Register_Descriptor * load_register_left = load_stmt_left.get_reg();
  load_register_left->set_use_for_expr_result();

  Register_Descriptor * load_register_right;
  Code_For_Ast set_stmt;
  Code_For_Ast * set_stmt_final;
    
  if (rhs != NULL) {
    lra.optimize_lra(mc_2r, NULL, rhs);
    Code_For_Ast & load_stmt_right = rhs->compile_and_optimize_ast(lra);
    load_register_right = load_stmt_right.get_reg();	
    load_register_right->set_use_for_expr_result();

    // op+1 to make the ENUM code of the operation concordant with the self-assigned codes from before
    set_stmt = create_set_stmt(op, load_register_left, load_register_right);

    // Store the statement in ic_list
    list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

    if (load_stmt_left.get_icode_list().empty() == false)
      ic_list = load_stmt_left.get_icode_list();

    if (load_stmt_right.get_icode_list().empty() == false)
      ic_list.splice(ic_list.end(), load_stmt_right.get_icode_list());

    if (set_stmt.get_icode_list().empty() == false)
      ic_list.splice(ic_list.end(), set_stmt.get_icode_list());

    if (ic_list.empty() == false)
      set_stmt_final = new Code_For_Ast(ic_list, set_stmt.get_reg());
  }

  else {
    set_stmt = create_set_stmt(op, load_register_left);

    // Store the statement in ic_list
    list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

    if (load_stmt_left.get_icode_list().empty() == false)
      ic_list = load_stmt_left.get_icode_list();

    if (set_stmt.get_icode_list().empty() == false)
      ic_list.splice(ic_list.end(), set_stmt.get_icode_list());

    if (ic_list.empty() == false)
      set_stmt_final = new Code_For_Ast(ic_list, set_stmt.get_reg());
	
  }
    
  load_register_left->reset_use_for_expr_result();
  if (rhs != NULL) {
    load_register_right->reset_use_for_expr_result();
  }

  return *set_stmt_final;
  
}

Code_For_Ast & Expr_Ast::create_set_stmt(Tgt_Op opn, Register_Descriptor * reg1, Register_Descriptor * reg2)
{
  CHECK_INVARIANT((reg1 != NULL), "Operand register cannot be null");
  CHECK_INVARIANT((reg2 != NULL), "Operand register cannot be null");
  
  Register_Descriptor * result_register_desc = machine_dscr_object.get_new_register(node_data_type);

  Ics_Opd * result_register_opd = new Register_Addr_Opd(result_register_desc, node_data_type);
  Ics_Opd * reg1_opd = new Register_Addr_Opd(reg1, node_data_type);
  Ics_Opd * reg2_opd = new Register_Addr_Opd(reg2, node_data_type);

  Icode_Stmt * store_temp_stmt = new Set_Rel_IC_Stmt(opn, reg1_opd, reg2_opd, result_register_opd);


  /* NOT SURE IF THIS IS TO BE DONE OR NOT
     if (command_options.is_do_lra_selected() == false) {
     variable_symbol_entry->free_register(reg1);
     variable_symbol_entry->free_register(reg2);
     }
  */
	
  list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
  ic_list.push_back(store_temp_stmt);

  Code_For_Ast & name_code = *new Code_For_Ast(ic_list, result_register_desc);

  return name_code;
}

Code_For_Ast & Expr_Ast::create_set_stmt(Tgt_Op opn, Register_Descriptor * reg1)
{
  CHECK_INVARIANT((reg1 != NULL), "Operand register cannot be null");
	
  Register_Descriptor * result_register_desc = machine_dscr_object.get_new_register(node_data_type);

  Ics_Opd * result_register_opd = new Register_Addr_Opd(result_register_desc, node_data_type);
  Ics_Opd * reg1_opd = new Register_Addr_Opd(reg1, node_data_type);

  Icode_Stmt * store_temp_stmt = new Set_Unary_Rel_IC_Stmt(opn, reg1_opd, result_register_opd);


  /* NOT SURE IF THIS IS TO BE DONE OR NOT
     if (command_options.is_do_lra_selected() == false) {
     variable_symbol_entry->free_register(reg1);
     variable_symbol_entry->free_register(reg2);
     }
  */
	
  list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
  ic_list.push_back(store_temp_stmt);

  Code_For_Ast & name_code = *new Code_For_Ast(ic_list, result_register_desc);

  return name_code;
}

///////////////////////////////////////////////////////////////////////////////

Cast_Expr_Ast::Cast_Expr_Ast(Ast * given_expr, Data_Type given_data_type, int line) {
  expr = given_expr;
  node_data_type = data_type = given_data_type;
  ast_num_child = unary_arity;
  lineno = line;
}

Cast_Expr_Ast::~Cast_Expr_Ast () {
  delete expr;
}

Data_Type Cast_Expr_Ast::get_data_type() {
  return data_type;
}

void Cast_Expr_Ast::print(ostream & file_buffer) {
  expr->print(file_buffer);
}
    
Eval_Result & Cast_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
  Eval_Result & result = expr->get_value_of_evaluation(eval_env);

  switch(data_type){
  case int_data_type:
    {
      int new_result_value = (int) result.get_value();
      Eval_Result_Value_Int * new_res = new Eval_Result_Value_Int();
      new_res->set_value(new_result_value);
      new_res->set_variable_status(true);
      return *new_res;
    }
  case float_data_type:
    {
      float new_result_value = (float) result.get_value();
      Eval_Result_Value_Float * new_res = new Eval_Result_Value_Float();
      new_res->set_value(new_result_value);
      new_res->set_variable_status(true);
      return *new_res;
    }
  case double_data_type:
    {
      double new_result_value = (double) result.get_value();
      Eval_Result_Value_Double * new_res = new Eval_Result_Value_Double();
      new_res->set_value(new_result_value);
      new_res->set_variable_status(true);
      return *new_res;
    }
  }
}

Code_For_Ast & Cast_Expr_Ast::compile() {
  CHECK_INVARIANT((expr != NULL), "Expression cannot be null");

  // first compile expr
  Code_For_Ast & load_stmt = expr->compile();
  // nothing to be done if the cast is from float to double or from double to float
  if ((node_data_type == float_data_type && expr->get_data_type() == double_data_type) || (node_data_type == double_data_type && expr->get_data_type() == float_data_type)) {
    return load_stmt;
  }

  // get the load register of the expr
  Register_Descriptor * load_register = load_stmt.get_reg();
  // get a new result register
  Register_Descriptor * result_register = machine_dscr_object.get_new_register(node_data_type);
  // first copy the load statements in the ic_list
  list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
  if (load_stmt.get_icode_list().empty() == false) {
    ic_list = load_stmt.get_icode_list();
  }
  // create the operands
  Ics_Opd * load_register_opd = new Register_Addr_Opd(load_register, node_data_type);
  Ics_Opd * result_register_opd = new Register_Addr_Opd(result_register, node_data_type);
    
  // create a new cast statement
  Icode_Stmt * cast_stmt;
  // for conversion from float or double to int, instr is mfc1, and for the vice-versa it is mtc1
  if (node_data_type == int_data_type) {
    cast_stmt = new Move_IC_Stmt(mfc1 , load_register_opd, result_register_opd);
  }
  else {
    cast_stmt = new Move_IC_Stmt(mtc1 , load_register_opd, result_register_opd);
  }

  // creates it intermediate code list
  list<Icode_Stmt *> & ic_list1 = * new list <Icode_Stmt *>;
  ic_list1.push_back(cast_stmt);
  // create its new code_for_ast
  Code_For_Ast & cast_code = * new Code_For_Ast(ic_list1, result_register);
  // append at the end
  ic_list.splice(ic_list.end(), cast_code.get_icode_list());
  // create a new code_for_ast and return
  Code_For_Ast * cast_expr_stmt = new Code_For_Ast(ic_list, result_register);

  return *cast_expr_stmt;
}

Code_For_Ast & Cast_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {
  CHECK_INVARIANT((expr != NULL), "Expression cannot be null");

  // first compile expr
  Code_For_Ast & load_stmt = expr->compile_and_optimize_ast(lra);
  // nothing to be done if the cast is from float to double or from double to float
  if ((node_data_type == float_data_type && expr->get_data_type() == double_data_type) || (node_data_type == double_data_type && expr->get_data_type() == float_data_type)) {
    return load_stmt;
  }

  // get the load register of the expr
  Register_Descriptor * load_register = load_stmt.get_reg();
  // get a new result register
  Register_Descriptor * result_register = machine_dscr_object.get_new_register(node_data_type);
  // first copy the load statements in the ic_list
  list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
  if (load_stmt.get_icode_list().empty() == false) {
    ic_list = load_stmt.get_icode_list();
  }
  // create the operands
  Ics_Opd * load_register_opd = new Register_Addr_Opd(load_register, node_data_type);
  Ics_Opd * result_register_opd = new Register_Addr_Opd(result_register, node_data_type);
    
  // create a new cast statement
  Icode_Stmt * cast_stmt;
  // for conversion from float or double to int, instr is mfc1, and for the vice-versa it is mtc1
  if (node_data_type == int_data_type) {
    cast_stmt = new Move_IC_Stmt(mfc1 , load_register_opd, result_register_opd);
  }
  else {
    cast_stmt = new Move_IC_Stmt(mtc1 , load_register_opd, result_register_opd);
  }

  // creates it intermediate code list
  list<Icode_Stmt *> & ic_list1 = * new list <Icode_Stmt *>;
  ic_list1.push_back(cast_stmt);
  // create its new code_for_ast
  Code_For_Ast & cast_code = * new Code_For_Ast(ic_list1, result_register);
  // append at the end
  ic_list.splice(ic_list.end(), cast_code.get_icode_list());
  // create a new code_for_ast and return
  Code_For_Ast * cast_expr_stmt = new Code_For_Ast(ic_list, result_register);

  return *cast_expr_stmt;
}

///////////////////////////////////////////////////////////////////////////////

Conditional_Ast::Conditional_Ast(Ast* predicate, int id1, int id2, int line)
{
  pred = predicate;
  label_id1 = id1;
  label_id2 = id2;
  lineno = line;
}

Conditional_Ast::~Conditional_Ast()
{
  delete pred;
}

void Conditional_Ast::print (ostream & file_buffer)
{
  file_buffer << AST_SPACE << "If_Else statement:";
  pred->print(file_buffer);
  file_buffer << endl << AST_NODE_SPACE << "True Successor: " << label_id1;
  file_buffer << endl << AST_NODE_SPACE << "False Successor: " << label_id2 << endl;
}

Eval_Result & Conditional_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
  Eval_Result & predicate_value = pred -> evaluate(eval_env, file_buffer) ;
    
  int successor;
	
  /* the predicate is true */
  if (predicate_value.get_value() == 1)
    {
      successor = label_id1;
    } 

  /* the predicate is false */
  else if (predicate_value.get_value() == 0)
    {	
      successor = label_id2;
    }
	
  else
    {
      CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,
		      "Predicate evaluation inside if-else statement failed");
    }

  print(file_buffer);
	
  Goto_Ast goto_statement_to_execute = Goto_Ast (successor, lineno);

  file_buffer << AST_SPACE << "Condition " << ((predicate_value.get_value()==0)?"False":"True") << " : Goto (BB " << successor << ")" << endl;
  
  return goto_statement_to_execute.evaluate_without_print(eval_env, file_buffer);
}

Code_For_Ast & Conditional_Ast::compile() {
  Code_For_Ast & expr_stmt = pred->compile();
  Register_Descriptor * reg_to_compare = expr_stmt.get_reg();
	
  reg_to_compare->set_use_for_expr_result();
	
  Code_For_Ast true_stmt = create_bne_stmt(reg_to_compare);
	
  reg_to_compare->reset_use_for_expr_result();
	
  list<Icode_Stmt *> ic_list;
  ic_list = expr_stmt.get_icode_list();
  ic_list.splice(ic_list.end(), true_stmt.get_icode_list());
	
  Goto_Ast false_goto_statement = Goto_Ast (label_id2, lineno);
  Code_For_Ast & false_goto_stmt = false_goto_statement.compile();
	
  ic_list.splice(ic_list.end(), false_goto_stmt.get_icode_list());
	
  Code_For_Ast * cond_stmt_final= new Code_For_Ast(ic_list, reg_to_compare);
  reg_to_compare->reset_use_for_expr_result();
	
  return *cond_stmt_final;
}

Code_For_Ast & Conditional_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {
  Code_For_Ast & expr_stmt = pred->compile_and_optimize_ast(lra);
  Register_Descriptor * reg_to_compare = expr_stmt.get_reg();
	
  reg_to_compare->set_use_for_expr_result();
	
  Code_For_Ast true_stmt = create_bne_stmt(reg_to_compare);
	
  reg_to_compare->reset_use_for_expr_result();
	
  list<Icode_Stmt *> ic_list;
  ic_list = expr_stmt.get_icode_list();
  ic_list.splice(ic_list.end(), true_stmt.get_icode_list());
	
  Goto_Ast false_goto_statement = Goto_Ast (label_id2, lineno);
  Code_For_Ast & false_goto_stmt = false_goto_statement.compile();
	
  ic_list.splice(ic_list.end(), false_goto_stmt.get_icode_list());
	
  Code_For_Ast * cond_stmt_final= new Code_For_Ast(ic_list, reg_to_compare);
  reg_to_compare->reset_use_for_expr_result();
	
  return *cond_stmt_final;
}

Code_For_Ast Conditional_Ast::create_bne_stmt(Register_Descriptor * reg) {
  Ics_Opd * register_opd = new Register_Addr_Opd(reg, node_data_type);
  Icode_Stmt * bne_stmt = new Control_IC_Stmt(bne, register_opd, label_id1);
  list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
  ic_list.push_back(bne_stmt);
  Code_For_Ast & name_code = *new Code_For_Ast(ic_list, reg);
  return name_code;
}

//////////////////////////////////////////////////////////////////////////////////////////

Goto_Ast::Goto_Ast (int num, int line)
{
  bb_id = num;
  lineno = line;
}
Goto_Ast::~Goto_Ast ()
{

}

void Goto_Ast::print (ostream & file_buffer)
{
  file_buffer << AST_SPACE << "Goto statement:";
  file_buffer << endl << AST_NODE_SPACE << "Successor: " << bb_id << endl;
}

Eval_Result & Goto_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
  print (file_buffer);
  file_buffer << AST_SPACE << "GOTO (BB " << bb_id << ")" << endl;
  Eval_Result & result = *new Eval_Result_BB();
  result.set_block_id(bb_id);
  return result;
}

Eval_Result & Goto_Ast::evaluate_without_print(Local_Environment & eval_env, ostream & file_buffer)
{
  Eval_Result & result = *new Eval_Result_BB();
  result.set_block_id(bb_id);
  return result;
}

Code_For_Ast & Goto_Ast::compile() {
	
  Icode_Stmt * goto_stmt = new Control_IC_Stmt(goto_command, bb_id);
  list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
  ic_list.push_back(goto_stmt);
    
  Code_For_Ast * new_code = new Code_For_Ast();
  new_code -> set_icode_list(ic_list);
  return *new_code;
}

Code_For_Ast & Goto_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {
  return compile();
}
//////////////////////////////////////////////////////////////////////////////////////////

Return_Ast::Return_Ast(Ast* _expn, string _fn_name, int line)
{
  fn_name = _fn_name;  
  lineno = line;
  expn = _expn;
  node_data_type = expn->get_data_type();
  ast_num_child = unary_arity;
}

Return_Ast::Return_Ast(string _fn_name, int line)
{
  fn_name = _fn_name;
  lineno = line;
  node_data_type = void_data_type;
  ast_num_child = zero_arity;
}

Return_Ast::~Return_Ast()
{}

void Return_Ast::print(ostream & file_buffer)
{
  file_buffer << "\n" << AST_SPACE << "Return <NOTHING>\n";
}

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
  Eval_Result & result = *new Eval_Result_Value_Int();
  return result;
}

Code_For_Ast & Return_Ast::compile()
{
  if (ast_num_child == zero_arity) {
    // return
    Icode_Stmt * ret_stmt;
    ret_stmt = new Ret_IC_Stmt(fn_name);
    list<Icode_Stmt *> & ic_list = * (new list<Icode_Stmt *>());
    ic_list.push_back(ret_stmt);

    Code_For_Ast * complete_ret_stmt = new Code_For_Ast(ic_list, machine_dscr_object.get_return_register(node_data_type));
    
    return *complete_ret_stmt;
  }
  else {
    // first compile what is to be returned
    Code_For_Ast compiled_return_expn = expn->compile();

    // get the icode list
    list<Icode_Stmt *> & ic_list = compiled_return_expn.get_icode_list();

    // get the register that is occupied by the expn
    Register_Descriptor * load_register_ret = compiled_return_expn.get_reg();

    Ics_Opd * load_register_ret_opd = new Register_Addr_Opd(load_register_ret, node_data_type);
    
    Icode_Stmt * load_stmt;
    Ics_Opd * ret_ics_opd = new Register_Addr_Opd (machine_dscr_object.get_return_register(node_data_type), node_data_type);
    // move it to v1 or f0 depending on the node_data_type
    if (node_data_type == int_data_type) {
      load_stmt = new Move_IC_Stmt(move_reg, load_register_ret_opd, ret_ics_opd);
    }

    else {
      load_stmt = new Move_IC_Stmt(move_reg_d, load_register_ret_opd, ret_ics_opd);
    }

    ic_list.push_back(load_stmt);
    
    // return
    Icode_Stmt * ret_stmt;
    ret_stmt = new Ret_IC_Stmt(fn_name);
    ic_list.push_back(ret_stmt);

    Code_For_Ast * complete_ret_stmt = new Code_For_Ast(ic_list, machine_dscr_object.get_return_register(node_data_type));
    return *complete_ret_stmt;
  }
}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
  if (ast_num_child == zero_arity) {
    // return
    Icode_Stmt * ret_stmt;
    ret_stmt = new Ret_IC_Stmt(fn_name);
    list<Icode_Stmt *> & ic_list = * (new list<Icode_Stmt *>());
    ic_list.push_back(ret_stmt);

    Code_For_Ast * complete_ret_stmt = new Code_For_Ast(ic_list, machine_dscr_object.get_return_register(node_data_type));
    
    return *complete_ret_stmt;
  }
  else {
    // first optimize the register allocation, if that made sense
    lra.optimize_lra(mc_2r, NULL, expn);
    
    // first compile what is to be returned
    Code_For_Ast compiled_return_expn = expn->compile_and_optimize_ast(lra);

    // get the icode list
    list<Icode_Stmt *> & ic_list = compiled_return_expn.get_icode_list();

    // get the register that is occupied by the expn
    Register_Descriptor * load_register_ret = compiled_return_expn.get_reg();

    Ics_Opd * load_register_ret_opd = new Register_Addr_Opd(load_register_ret, node_data_type);
    
    Icode_Stmt * load_stmt;
    Ics_Opd * ret_ics_opd = new Register_Addr_Opd (machine_dscr_object.get_return_register(node_data_type), node_data_type);
    // move it to v1 or f0 depending on the node_data_type
    if (node_data_type == int_data_type) {
      load_stmt = new Move_IC_Stmt(move_reg, load_register_ret_opd, ret_ics_opd);
    }

    else {
      load_stmt = new Move_IC_Stmt(move_reg_d, load_register_ret_opd, ret_ics_opd);
    }

    ic_list.push_back(load_stmt);
    
    // return
    Icode_Stmt * ret_stmt;
    ret_stmt = new Ret_IC_Stmt(fn_name);
    ic_list.push_back(ret_stmt);

    Code_For_Ast * complete_ret_stmt = new Code_For_Ast(ic_list, machine_dscr_object.get_return_register(node_data_type));
    return *complete_ret_stmt;
  }
}

////////////////////////////////////////////////////////////////////////////////////

Functional_Call_Ast::Functional_Call_Ast (string& given_name, list <Ast*> arguments) {
  name = given_name;
  argument = arguments;
}

Functional_Call_Ast::~Functional_Call_Ast() {
  for (list<Ast*>::iterator i = argument.begin(); i != argument.end();) {
    argument.erase(i);
  }
}

Data_Type Functional_Call_Ast::get_data_type() {
  return node_data_type;
}

void Functional_Call_Ast::set_data_type(Data_Type data_type) {
  node_data_type = data_type;
}

bool Functional_Call_Ast::check_ast(int line) {
  CHECK_INVARIANT("Should not reach here", line);
}

void Functional_Call_Ast::print(ostream& file_buffer) {
  file_buffer << endl << AST_SPACE << "FN CALL: " << name << "(";
  for (list <Ast *>::iterator i = argument.begin(); i != argument.end(); i++) {
    file_buffer << endl << AST_NODE_SPACE;
    (*i) -> print(file_buffer);
  }
  file_buffer << ")";
}

Eval_Result & Functional_Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
  Procedure * called_procedure = program_object.get_procedure(name);
  list<Eval_Result_Value *> new_arg_list;
  for (list<Ast *>::iterator i = argument.begin(); i!=argument.end(); i++) {
    Eval_Result & val = (*i)->evaluate(eval_env, file_buffer);
    new_arg_list.push_back((Eval_Result_Value*) &val);
  }
  called_procedure -> push_call_arguments(new_arg_list);
  Eval_Result & result = called_procedure -> evaluate(file_buffer);
  called_procedure -> clear_call_arguments();
  return result;
}

Eval_Result & Functional_Call_Ast::get_value_of_evaluation(Local_Environment & eval_env, ostream & file_buffer) {
  return evaluate(eval_env, file_buffer);
}

Code_For_Ast & Functional_Call_Ast::compile()
{
  // the ic_list for the whole compilation
  list<Icode_Stmt *> ic_list;

  // get the handle of the procedure being called
  Procedure * called_procedure = program_object.get_procedure(name);

  // get the parameters of the function
  list <args> parameters = called_procedure->get_arguments();

  // list of arguments iterator
  list <args> ::iterator j,j1;
  j = j1  = parameters.end();
  list<Ast*>::iterator i,i1;
  i = i1 = argument.end();
  j--;i--;
  // compile each argument ast one by one
  for (; i1!=argument.begin(); --i,--j,--i1,--j1) {
    // compile the ast
    Code_For_Ast arg_code_list = (*i)->compile();
    // get the register that is occupied by the arg
    Register_Descriptor * load_register_ret = arg_code_list.get_reg();
    // get the name of the argument
    string arg_name = (*j).get_name();
    // get the data type of the argument
    Data_Type data_t = (*j).get_type();
    // get the symbol table entry of the argument
    Symbol_Table_Entry * sym_tab_ent = new Symbol_Table_Entry(called_procedure->get_symbol_table_entry(arg_name));
    // create the register operand
    Ics_Opd * register_opd = new Register_Addr_Opd(load_register_ret, data_t);
    // create the memory operand
    Ics_Opd * opd = new Mem_Addr_Opd(*sym_tab_ent, data_t);

    Icode_Stmt * store_stmt;
    if (data_t == int_data_type) {
      store_stmt = new Move_IC_Stmt(store, register_opd, opd);
    }
    else {
      store_stmt = new Move_IC_Stmt(store_d, register_opd, opd);
    }

    ic_list.splice(ic_list.end(), arg_code_list.get_icode_list());
    ic_list.push_back(store_stmt);
  }

  // now make a call icode statement
  Call_IC_Stmt * call_stmt = new Call_IC_Stmt(name);
  ic_list.push_back(call_stmt);

  Register_Descriptor * store_register_for_return = machine_dscr_object.get_new_register(called_procedure->get_return_type());

  // create a new move statement, in case the function returns
  if (called_procedure->get_return_type() != void_data_type) {
    Icode_Stmt * store_ret_stmt;
    Ics_Opd * store_ret_opd = new Register_Addr_Opd (store_register_for_return, called_procedure->get_return_type());
    Ics_Opd * ret_ics_opd = new Register_Addr_Opd (machine_dscr_object.get_return_register(node_data_type), called_procedure->get_return_type());

    if (node_data_type == int_data_type) {
      store_ret_stmt = new Move_IC_Stmt(move_reg, ret_ics_opd, store_ret_opd);
    }
    else {
      store_ret_stmt = new Move_IC_Stmt(move_reg_d, ret_ics_opd, store_ret_opd);
    }
    
    ic_list.push_back(store_ret_stmt);
  }
  
  Code_For_Ast & fun_call_code = *new Code_For_Ast(ic_list, store_register_for_return);
  return fun_call_code;

}

Code_For_Ast & Functional_Call_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
  // the ic_list for the whole compilation
  list<Icode_Stmt *> ic_list;

  // get the handle of the procedure being called
  Procedure * called_procedure = program_object.get_procedure(name);

  // get the parameters of the function
  list <args> parameters = called_procedure->get_arguments();

  // list of arguments iterator
  list <args> ::iterator j,j1;
  j = j1  = parameters.end();
  list<Ast*>::iterator i,i1;
  i = i1 = argument.end();
  j--;i--;
  // compile each argument ast one by one
  for (; i1!=argument.begin(); --i,--j,--i1,--j1) {
    lra.optimize_lra(mc_2r, NULL, *i);
    
    // compile the ast
    Code_For_Ast arg_code_list = (*i)->compile_and_optimize_ast(lra);
    // get the register that is occupied by the arg
    Register_Descriptor * load_register_ret = arg_code_list.get_reg();
    // get the name of the argument
    string arg_name = (*j).get_name();
    // get the data type of the argument
    Data_Type data_t = (*j).get_type();
    // get the symbol table entry of the argument
    Symbol_Table_Entry * sym_tab_ent = new Symbol_Table_Entry(called_procedure->get_symbol_table_entry(arg_name));
    // create the register operand
    Ics_Opd * register_opd = new Register_Addr_Opd(load_register_ret, data_t);
    // create the memory operand
    Ics_Opd * opd = new Mem_Addr_Opd(*sym_tab_ent, data_t);

    Icode_Stmt * store_stmt;
    if (data_t == int_data_type) {
      store_stmt = new Move_IC_Stmt(store, register_opd, opd);
    }
    else {
      store_stmt = new Move_IC_Stmt(store_d, register_opd, opd);
    }

    ic_list.splice(ic_list.end(), arg_code_list.get_icode_list());
    ic_list.push_back(store_stmt);
  }

  // now make a call icode statement
  Call_IC_Stmt * call_stmt = new Call_IC_Stmt(name);
  ic_list.push_back(call_stmt);

  Register_Descriptor * store_register_for_return = machine_dscr_object.get_new_register(called_procedure->get_return_type());

  // create a new move statement, in case the function returns
  if (called_procedure->get_return_type() != void_data_type) {
    Icode_Stmt * store_ret_stmt;
    Ics_Opd * store_ret_opd = new Register_Addr_Opd (store_register_for_return, called_procedure->get_return_type());
    Ics_Opd * ret_ics_opd = new Register_Addr_Opd (machine_dscr_object.get_return_register(node_data_type), called_procedure->get_return_type());

    if (node_data_type == int_data_type) {
      store_ret_stmt = new Move_IC_Stmt(move_reg, ret_ics_opd, store_ret_opd);
    }
    else {
      store_ret_stmt = new Move_IC_Stmt(move_reg_d, ret_ics_opd, store_ret_opd);
    }
    
    ic_list.push_back(store_ret_stmt);
  }
  
  Code_For_Ast & fun_call_code = *new Code_For_Ast(ic_list, store_register_for_return);
  return fun_call_code;

}

template class Number_Ast<int>;
template class Number_Ast<float>;
