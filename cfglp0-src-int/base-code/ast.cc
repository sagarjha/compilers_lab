
/*********************************************************************************************

                                cfglp : A CFG Language Processor
                                --------------------------------

           About:

           Implemented   by  Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker    (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs306: Language  Processors  (theory and  lab)  at  IIT
           Bombay.

           Release  date  Jan  15, 2013.  Copyrhss  reserved  by  Uday
           Khedker. This  implemenation  has been made  available purely
           for academic purposes without any warranty of any kind.

           Documentation (functionality, manual, and design) and related
           tools are  available at http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/

#include<iostream>
#include<fstream>

using namespace std;

#include"user-options.hh"
#include"error-display.hh"
#include"local-environment.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"
#include<iomanip>

Ast::Ast()
{}

Ast::~Ast()
{}

bool Ast::check_ast(int line)
{
    report_internal_error("Should not reach, Ast : check_ast");
}

Data_Type Ast::get_data_type()
{
    report_internal_error("Should not reach, Ast : get_data_type");
}

void Ast::set_data_type(Data_Type data_type)
{
    report_internal_error("Should not reach, Ast : set_data_type");
}


void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
    report_internal_error("Should not reach, Ast : print_value");
}

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
    report_internal_error("Should not reach, Ast : get_value_of_evaluation");
}

void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
    report_internal_error("Should not reach, Ast : set_value_of_evaluation");
}

////////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs)
{
    lhs = temp_lhs;
    rhs = temp_rhs;
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


bool Assignment_Ast::check_ast(int line)
{
    if (lhs->get_data_type() == rhs->get_data_type())
	{
	    node_data_type = lhs->get_data_type();
	    return true;
	}
    else if ((lhs->get_data_type() == float_data_type && rhs->get_data_type() == double_data_type) | (rhs->get_data_type() == float_data_type && lhs->get_data_type() == double_data_type)) {
	node_data_type = lhs->get_data_type();
	return true;
    }

    
    report_error("Assignment statement data type not compatible", line);
}

void Assignment_Ast::print_ast(ostream & file_buffer)
{
    file_buffer << AST_SPACE << "Asgn:\n";

    file_buffer << AST_NODE_SPACE << "LHS (";
    lhs->print_ast(file_buffer);
    file_buffer << ")\n";

    file_buffer << AST_NODE_SPACE << "RHS (";
    rhs->print_ast(file_buffer);
    file_buffer << ")\n";
}

Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    Eval_Result & result = rhs->evaluate(eval_env, file_buffer);

    if (result.is_variable_defined() == false)
	report_error("Variable should be defined to be on rhs", NOLINE);

    lhs->set_value_of_evaluation(eval_env, result);

    // Print the result

    print_ast(file_buffer);

    lhs->print_value(eval_env, file_buffer);

    file_buffer << endl;
  
    return result;
}

////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry)
{
    variable_name = name;
    variable_symbol_entry = var_entry;
}

Name_Ast::~Name_Ast()
{}

Data_Type Name_Ast::get_data_type()
{
    return variable_symbol_entry.get_data_type();
}

void Name_Ast::print_ast(ostream & file_buffer)
{
    file_buffer << "Name : " << variable_name;
}

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
    Eval_Result_Value * loc_var_val = eval_env.get_variable_value(variable_name);
    Eval_Result_Value * glob_var_val = interpreter_global_table.get_variable_value(variable_name);

    file_buffer << AST_SPACE << variable_name << " : ";

    if (!eval_env.is_variable_defined(variable_name) && !interpreter_global_table.is_variable_defined(variable_name))
	file_buffer << "undefined";

    else if (eval_env.is_variable_defined(variable_name) && loc_var_val != NULL)
	{
	    if (loc_var_val->get_result_enum() == int_result)
		file_buffer << (int)loc_var_val->get_value() << "\n";
	    else if (loc_var_val->get_result_enum() == float_result)
		file_buffer << fixed << setprecision (2) << loc_var_val->get_value() << "\n";
	    else if (loc_var_val->get_result_enum() == double_result)
		file_buffer << fixed << setprecision (2) << loc_var_val->get_value() << "\n";
	    else
		report_internal_error("Result type can only be int and float");
	}

    else
	{
	    if (glob_var_val->get_result_enum() == int_result)
		{
		    if (glob_var_val == NULL)
			file_buffer << "0\n";
		    else
			file_buffer << (int)glob_var_val->get_value() ;
		}
	    else if (glob_var_val->get_result_enum() == float_result)
		{
		    if (glob_var_val == NULL)
			file_buffer << "0\n";
		    else
			file_buffer << fixed << setprecision (2) << glob_var_val->get_value() ;
		}
	    else if (glob_var_val->get_result_enum() == double_result)
		{
		    if (glob_var_val == NULL)
			file_buffer << "0\n";
		    else
			file_buffer << fixed << setprecision (2) << glob_var_val->get_value() ;
		}
	    else
		report_internal_error("Result type can only be int and float");
	}
    file_buffer << "\n";
}

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
    if (eval_env.does_variable_exist(variable_name))
	{
	    Eval_Result * result = eval_env.get_variable_value(variable_name);
	    return *result;
	}

    Eval_Result * result = interpreter_global_table.get_variable_value(variable_name);
    return *result;
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
    Eval_Result_Value * i;
    if (result.get_result_enum() == int_result)
	{
	    i = new Eval_Result_Value_Int();
	    i->set_value((int)result.get_value());
	}

    if (result.get_result_enum() == float_result)
	{
	    i = new Eval_Result_Value_Float();
	    i->set_value(result.get_value());
	}

    if (result.get_result_enum() == double_result)
	{
	    i = new Eval_Result_Value_Double();
	    i->set_value(result.get_value());
	}

    if (eval_env.does_variable_exist(variable_name))
	eval_env.put_variable_value(*i, variable_name);
    else
	interpreter_global_table.put_variable_value(*i, variable_name);
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    return get_value_of_evaluation(eval_env);
}

///////////////////////////////////////////////////////////////////////////////

Cast_Name_Ast::Cast_Name_Ast(Ast * given_name, Data_Type given_data_type)
{
    name = given_name;
    data_type = given_data_type;
}

Cast_Name_Ast::~Cast_Name_Ast()
{}

Data_Type Cast_Name_Ast::get_data_type()
{
    return data_type;
}

void Cast_Name_Ast::print_ast(ostream & file_buffer)
{
    name->print_ast(file_buffer);
}

void Cast_Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
    Eval_Result & result = get_value_of_evaluation(eval_env);

    // print the result value here
    if (node_data_type == float_data_type || node_data_type == double_data_type) {
	file_buffer << fixed << setprecision (2) << result.get_value() << endl;
    }
    else {
	file_buffer << (int)result.get_value() << endl;
    }
}

Eval_Result & Cast_Name_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
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

Eval_Result & Cast_Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    return get_value_of_evaluation(eval_env);
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type constant_data_type)
{
    constant = number;
    node_data_type = constant_data_type;
}

///////////////////////////////////////////////////////////////////////////////

// code for the implementation of expressions
Expr_Ast::Expr_Ast (Ast* l, int oper, Ast* r)
{
    lhs = l;
    rhs = r;
    op = oper;
}

Expr_Ast::~Expr_Ast()
{
    delete lhs,rhs;
}

bool Expr_Ast::check_ast(int line)
{
    if (op == 12) {
	node_data_type = lhs->get_data_type();
	return true;
    }
    else if (op == 6 || op == 7) {
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

    report_error("Relational Expression statement data type not compatible", line);
}

Data_Type Expr_Ast::get_data_type()
{
    return node_data_type;
}

// needs to be modified
void Expr_Ast::print_ast(ostream & file_buffer)
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
    case 8:
	op_str = "PLUS";
	break;
    case 9:
	op_str = "MINUS";
	break;
    case 10:
	op_str = "MULT";
	break;
    case 11:
	op_str = "DIV";
	break;
    case 12:
	op_str = "UMINUS";
	break;
    }

    if (op < 8) {
	file_buffer << "\n" << AST_NODE_SPACE << "Condition: " << op_str << "\n";
    }

    else {
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: " << op_str << "\n";
    }

    file_buffer << AST_IF_SPACE << "LHS (";
    lhs->print_ast(file_buffer);
    file_buffer << ")";

    if (op < 12) {
	file_buffer << "\n";
	file_buffer << AST_IF_SPACE << "RHS (";
	rhs->print_ast(file_buffer);
	file_buffer << ")";
    }
}

// needs to be defined
Eval_Result & Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    // assignment operation
    if (op == 1)
	{
	    Eval_Result & result = rhs->evaluate(eval_env, file_buffer);

	    if (result.is_variable_defined() == false)
		report_error("Variable should be defined to be on rhs", NOLINE);
		
	    lhs->set_value_of_evaluation(eval_env, result);

	    // Print the result

	    print_ast(file_buffer);

	    lhs->print_value(eval_env, file_buffer);

	    if (lhs->get_data_type() == int_data_type) {
		Eval_Result & result_final = *new Eval_Result_Value_Int();
		result_final.set_value(result.get_value());
		return result_final;
	    }
	    else {
		Eval_Result & result_final = *new Eval_Result_Value_Float();
		result_final.set_value(result.get_value());
		return result_final;
	    }
	}
	
    else
	{
	    if (op == 12) {
		Eval_Result & left_eval_result = lhs->evaluate(eval_env, file_buffer);
	    
		if (lhs->get_data_type() == int_data_type) {
		    int l_int_value = left_eval_result.get_value();
		    int ans = 0;
		    ans = -l_int_value;
		    Eval_Result & result = *new Eval_Result_Value_Int();
		    result.set_value(ans);
		    return result;
		}

		else {
		    float l_float_value = left_eval_result.get_value();
		    float ans = 0;
		    ans = -l_float_value;
		    Eval_Result & result = *new Eval_Result_Value_Float();
		    result.set_value(ans);
		    return result;
		}

	    }

	    else {
		Eval_Result & left_eval_result = lhs->evaluate(eval_env, file_buffer);
		Eval_Result & right_eval_result = rhs->evaluate(eval_env, file_buffer);;
	    
		if (lhs->get_data_type() == int_data_type) {
		    int l_int_value = left_eval_result.get_value();
		    int r_int_value = right_eval_result.get_value();;
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
		    case 8: // +
			ans = l_int_value + r_int_value;
			break;
		    case 9: // -
			ans = l_int_value - r_int_value;
			break;
		    case 10: // *
			ans = l_int_value * r_int_value;
			break;
		    case 11: // /
			if (r_int_value == 0.0) {
			    report_error ("Floating Point Exception : Dividing by zero\n", NOLINE);
			}
			else {
			    ans = l_int_value / r_int_value;
			}
			break;
		    case 12: // u-
			ans = -l_int_value;
			break;
		    }
		
		    Eval_Result & result = *new Eval_Result_Value_Int();
		    result.set_value(ans);
		    return result;
		}

		else {
		    float l_int_value = left_eval_result.get_value();
		    float r_int_value = right_eval_result.get_value();;

		    int ans_int = 0;
		    float ans = 0;
		
		    switch (op) {
		    case 2: // LE
			ans_int = (l_int_value <= r_int_value)? 1 : 0;
			break;
		    case 3: // GE
			ans_int = (l_int_value >= r_int_value)? 1 : 0;
			break;
		    case 4: // LT
			ans_int = (l_int_value < r_int_value)? 1 : 0;
			break;
		    case 5: // GT
			ans_int = (l_int_value > r_int_value)? 1 : 0;
			break;
		    case 6: // NE
			ans_int = (l_int_value != r_int_value)? 1 : 0;
			break;
		    case 7: // EQ
			ans_int = (l_int_value == r_int_value)? 1 : 0;
			break;
		    case 8: // +
			ans = l_int_value + r_int_value;
			break;
		    case 9: // -
			ans = l_int_value - r_int_value;
			break;
		    case 10: // *
			ans = l_int_value * r_int_value;
			break;
		    case 11: // /
			if (r_int_value == 0.0) {
			    report_error ("Floating Point Exception : Dividing by zero\n",NOLINE);
			}
			else {
			    ans = l_int_value / r_int_value;
			}
			break;
		    case 12: // u-
			ans = -l_int_value;
			break;
		    }

		    if (op < 8) {
			Eval_Result & result = *new Eval_Result_Value_Int();
			result.set_value(ans_int);
			return result;
		    }
		    else if (lhs->get_data_type() == float_data_type) {
			Eval_Result & result = *new Eval_Result_Value_Float();
			result.set_value(ans);
			return result;
		    }
		    else {
			Eval_Result & result = *new Eval_Result_Value_Double();
			result.set_value(ans);
			return result;
		    }
		}
	    }
	}
}

Eval_Result & Expr_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
    // assignment operation
    if (op == 1)
	{
	    Eval_Result & result = rhs->get_value_of_evaluation(eval_env);

	    if (result.is_variable_defined() == false)
		report_error("Variable should be defined to be on rhs", NOLINE);
		
	    lhs->set_value_of_evaluation(eval_env, result);

	    Eval_Result & result_final = *new Eval_Result_Value_Int();
	    result_final.set_value(result.get_value());
	    return result_final;
	}
	
    else
	{
	    if (op == 12) {
		Eval_Result & left_eval_result = lhs->get_value_of_evaluation(eval_env);
	    
		if (lhs->get_data_type() == int_data_type) {
		    int l_int_value = left_eval_result.get_value();
		    int ans = 0;
		    ans = -l_int_value;
		    Eval_Result & result = *new Eval_Result_Value_Int();
		    result.set_value(ans);
		    return result;
		}

		else {
		    float l_float_value = left_eval_result.get_value();
		    float ans = 0;
		    ans = -l_float_value;
		    Eval_Result & result = *new Eval_Result_Value_Float();
		    result.set_value(ans);
		    return result;
		}
	    }

	    else {
		Eval_Result & left_eval_result = lhs->get_value_of_evaluation(eval_env);
		Eval_Result & right_eval_result = rhs->get_value_of_evaluation(eval_env);

		if (lhs->get_data_type() == int_data_type) {
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
		    case 8: // +
			ans = l_int_value + r_int_value;
			break;
		    case 9: // -
			ans = l_int_value - r_int_value;
			break;
		    case 10: // *
			ans = l_int_value * r_int_value;
			break;
		    case 11: // /
			if (r_int_value == 0.0) {
			    report_error ("Floating Point Exception : Dividing by zero\n",NOLINE);
			}
			else {
			    ans = l_int_value / r_int_value;
			}
			break;
		    case 12: // u-
			ans = -l_int_value;
			break;
		    }
		
		    Eval_Result & result = *new Eval_Result_Value_Int();
		    result.set_value(ans);
		    return result;
		}

		else {
		    float l_int_value = left_eval_result.get_value();
		    float r_int_value = right_eval_result.get_value();

		    int ans_int = 0;
		    float ans = 0;
		
		    switch (op) {
		    case 2: // LE
			ans_int = (l_int_value <= r_int_value)? 1 : 0;
			break;
		    case 3: // GE
			ans_int = (l_int_value >= r_int_value)? 1 : 0;
			break;
		    case 4: // LT
			ans_int = (l_int_value < r_int_value)? 1 : 0;
			break;
		    case 5: // GT
			ans_int = (l_int_value > r_int_value)? 1 : 0;
			break;
		    case 6: // NE
			ans_int = (l_int_value != r_int_value)? 1 : 0;
			break;
		    case 7: // EQ
			ans_int = (l_int_value == r_int_value)? 1 : 0;
			break;
		    case 8: // +
			ans = l_int_value + r_int_value;
			break;
		    case 9: // -
			ans = l_int_value - r_int_value;
			break;
		    case 10: // *
			ans = l_int_value * r_int_value;
			break;
		    case 11: // /
			if (r_int_value == 0.0) {
			    report_error ("Floating Point Exception : Dividing by zero\n",NOLINE);
			}
			else {
			    ans = l_int_value / r_int_value;
			}
			break;
		    case 12: // u-
			ans = -l_int_value;
			break;
		    }

		    if (op < 8) {
			Eval_Result & result = *new Eval_Result_Value_Int();
			result.set_value(ans_int);
			return result;
		    }
		    else if (lhs->get_data_type() == float_data_type) {
			Eval_Result & result = *new Eval_Result_Value_Float();
			result.set_value(ans);
			return result;
		    }
		    else {
			Eval_Result & result = *new Eval_Result_Value_Double();
			result.set_value(ans);
			return result;
		    }
		}
	    }
	}
}

///////////////////////////////////////////////////////////////////////////////

Cast_Expr_Ast::Cast_Expr_Ast(Ast * given_expr, Data_Type given_data_type)
{
    expr = given_expr;
    data_type = given_data_type;
}

Cast_Expr_Ast::~Cast_Expr_Ast()
{}

Data_Type Cast_Expr_Ast::get_data_type()
{
    return data_type;
}

void Cast_Expr_Ast::print_ast(ostream & file_buffer)
{
    expr->print_ast(file_buffer);
}

void Cast_Expr_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
    Eval_Result & result = get_value_of_evaluation(eval_env);

    // print the result value here
    if (node_data_type == float_data_type || node_data_type == double_data_type) {
	file_buffer << fixed << setprecision (2) << result.get_value() << endl;
    }
    else {
	file_buffer << (int)result.get_value() << endl;
    }
}

Eval_Result & Cast_Expr_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
    Eval_Result & result = expr->get_value_of_evaluation(eval_env);
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

Eval_Result & Cast_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    return get_value_of_evaluation(eval_env);
}


///////////////////////////////////////////////////////////////////////////////

// code for the implementation of boolean expressions
Boolean_Expr_Ast::Boolean_Expr_Ast (Ast* l, int oper, Ast* r)
{
    lhs = l;
    rhs = r;
    op = oper;
}

Boolean_Expr_Ast::~Boolean_Expr_Ast()
{
    delete lhs, rhs;
}

bool Boolean_Expr_Ast::check_ast(int line)
{
    if (lhs->get_data_type() == rhs->get_data_type())
	{
	    node_data_type = lhs->get_data_type();
	    return true;
	}

    report_error("Boolean Expression statement data type not compatible", line);
}

Data_Type Boolean_Expr_Ast::get_data_type()
{
    return node_data_type;
}

// needs to be modified
void Boolean_Expr_Ast::print_ast(ostream & file_buffer)
{
    file_buffer << AST_SPACE << "Bool_Expr:\n";

    file_buffer << AST_NODE_SPACE"LHS (";
    lhs->print_ast(file_buffer);
    file_buffer << ")\n";

    file_buffer << AST_NODE_SPACE << "RHS (";
    rhs->print_ast(file_buffer);
    file_buffer << ")\n";
}

// needs to be modified
Eval_Result & Boolean_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{

}

///////////////////////////////////////////////////////////////////////////////


template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::~Number_Ast()
{}

template <class DATA_TYPE>
Data_Type Number_Ast<DATA_TYPE>::get_data_type()
{
    return node_data_type;
}


template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::print_ast(ostream & file_buffer)
{
    if (node_data_type == float_data_type || node_data_type == double_data_type) {
	file_buffer << "Num : " << fixed << setprecision (2) << constant;
    }
    else {
	file_buffer << "Num : " << (int)constant;
    }
}

template <class DATA_TYPE>
Eval_Result & Number_Ast<DATA_TYPE>::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    if (node_data_type == int_data_type)
	{
	    Eval_Result_Value_Int * result = new Eval_Result_Value_Int ();
	    result->set_value(constant);

	    return *result;
	}
    else if (node_data_type == float_data_type)
	{
	    Eval_Result_Value_Float * result = new Eval_Result_Value_Float ();
	    result->set_value(constant);

	    return *result;
	}
    if (node_data_type == double_data_type)
	{
	    Eval_Result_Value_Double * result = new Eval_Result_Value_Double ();
	    result->set_value(constant);

	    return *result;
	}
}

template <class DATA_TYPE>
Eval_Result & Number_Ast<DATA_TYPE>::get_value_of_evaluation(Local_Environment & eval_env) {
    if (node_data_type == int_data_type)
	{
	    Eval_Result_Value_Int * result = new Eval_Result_Value_Int ();
	    result->set_value(constant);

	    return *result;
	}
    else if (node_data_type == float_data_type)
	{
	    Eval_Result_Value_Float * result = new Eval_Result_Value_Float ();
	    result->set_value(constant);

	    return *result;
	}
    if (node_data_type == double_data_type)
	{
	    Eval_Result_Value_Double * result = new Eval_Result_Value_Double ();
	    result->set_value(constant);

	    return *result;
	}
}

///////////////////////////////////////////////////////////////////////////////

Return_Ast::Return_Ast(Ast * _expn) {
    expn = _expn;
}

Return_Ast::~Return_Ast()
{}

void Return_Ast::print_ast(ostream & file_buffer)
{
    if (expn == NULL) {
	// void return type
	file_buffer << endl << AST_SPACE << "RETURN <NOTHING>";
    }
	
	
    else {
	file_buffer << endl << AST_SPACE << "RETURN ";
	expn->print_ast(file_buffer);
    }
}

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    print_ast(file_buffer);
    
    if (expn != NULL) {
	Eval_Result & result = expn->get_value_of_evaluation(eval_env);
	return result;
	}
    
    else {
	Eval_Result & result = *new Eval_Result_Value_Int ();
	result.set_result_enum(return_result);
	return result;
    }
}

template class Number_Ast<int>;
template class Number_Ast<float>;
template class Number_Ast<double>;

////////////////////////////////////////////////////////////////////////////////

Conditional_Ast::Conditional_Ast(Ast* predicate, int id1, int id2)
{
    pred = predicate;
    bb_id1 = id1;
    bb_id2 = id2;
}

Conditional_Ast::~Conditional_Ast()
{
    delete pred;
}

void Conditional_Ast::print_ast (ostream & file_buffer)
{
    file_buffer << AST_SPACE << "If_Else statement:";
    pred->print_ast(file_buffer);
    file_buffer << endl << AST_NODE_SPACE << "True Successor: " << bb_id1;
    file_buffer << endl << AST_NODE_SPACE << "False Successor: " << bb_id2 << endl;
}

Eval_Result & Conditional_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    Eval_Result & predicate_value = pred -> evaluate(eval_env, file_buffer) ;
	
    int successor;
	
    /* the predicate is true */
    if (predicate_value.get_value() == 1)
	{
	    successor = bb_id1;
	} 

    /* the predicate is false */
    else if (predicate_value.get_value() == 0)
	{	
	    successor = bb_id2;
	}
	
    else
	{
	    report_error ("Predicate evaluation inside if-else statement failed", NOLINE);
	}

    print_ast(file_buffer);
	
    Goto_Ast goto_statement_to_execute = Goto_Ast (successor);

    file_buffer << AST_SPACE << "Condition " << ((predicate_value.get_value()==0)?"False":"True") << " : Goto (BB " << successor << ")" << endl;
  
    return goto_statement_to_execute.evaluate_without_print(eval_env, file_buffer);
}


//////////////////////////////////////////////////////////////////////////////////////////

Goto_Ast::Goto_Ast (int num)
{
    bb_id = num;
}
Goto_Ast::~Goto_Ast ()
{

}

void Goto_Ast::print_ast (ostream & file_buffer)
{
    file_buffer << AST_SPACE << "Goto statement:";
    file_buffer << endl << AST_NODE_SPACE << "Successor: " << bb_id << endl;
}

Eval_Result & Goto_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    print_ast (file_buffer);
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

//////////////////////////////////////////////////////////////////////////////////////////

Functional_Call_Ast::Functional_Call_Ast (string& given_name, list <Ast*> arguments) {
    name = given_name;
    args = arguments;
}

Functional_Call_Ast::~Functional_Call_Ast() {
    for (list<Ast*>::iterator i = args.begin(); i != args.end();) {
	args.erase(i);
    }
}

Data_Type Functional_Call_Ast::get_data_type() {
    return node_data_type;
}

void Functional_Call_Ast::set_data_type(Data_Type data_type) {
    node_data_type = data_type;
}

bool Functional_Call_Ast::check_ast(int line) {
    report_error("Should not reach here", line);
}

void Functional_Call_Ast::print_ast(ostream& file_buffer) {
    file_buffer << endl << AST_SPACE << "FN CALL: " << name << "(";
    for (list <Ast *>::iterator i = args.begin(); i != args.end(); i++) {
	file_buffer << endl << AST_NODE_SPACE;
	(*i) -> print_ast(file_buffer);
    }
    file_buffer << ")";
}

Eval_Result & Functional_Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    
}
