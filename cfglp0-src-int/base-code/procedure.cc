
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

#include<string>
#include<fstream>
#include<iostream>

using namespace std;

#include"error-display.hh"
#include"local-environment.hh"

#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

Data_Type argument::get_type() {
    return type;
}

string argument::get_name() {
    return name;
}

void argument::set_name(string arg_name) {
    name = arg_name;
}

Procedure::Procedure(Data_Type proc_return_type, string proc_name, list <argument*> arg_list)
{
    return_type = proc_return_type;
    name = proc_name;
    list <argument *>::iterator i;
    for (i = arg_list.begin(); i != arg_list.end(); ++i) {
	args.push_back(*(*i));
    }
    cur_num_basic_block = 2;
    function_defined=false;
}

Procedure::~Procedure()
{
    list<Basic_Block *>::iterator i;
    for (i = basic_block_list.begin(); i != basic_block_list.end(); i++)
	delete (*i);
}

string Procedure::get_proc_name()
{
    return name;
}

void Procedure::set_basic_block_list(list<Basic_Block *> bb_list)
{
    basic_block_list = bb_list;
}

void Procedure::set_local_list(Symbol_Table & new_list)
{
    local_symbol_table = new_list;
    local_symbol_table.set_table_scope(local);
}

Data_Type Procedure::get_return_type()
{
    return return_type;
}

bool Procedure::match_argument_list(list<argument*> *arg_list) {
    function_defined = true;
    if (arg_list == NULL) {
	return args.size() == 0;
    }
    if (arg_list->size() != args.size()) {
	return false;
    }
    list<argument>::iterator i;
    list<argument*>::iterator j;
    for (i = args.begin(), j = arg_list->begin(); i != args.end(); ++i, ++j) {
	if ((*i).get_type() != (*j)->get_type()) {
	    return false;
	}
	(*i).set_name((*j)->get_name());
    }
    return true;
}

bool Procedure::match_function_call(list <Ast *> * parameter_list) {
    if (parameter_list == NULL) {
    if (args.size() == 0) {
      return true;
    }
    return false;
  }
  if (parameter_list->size () != args.size()) {
	return false;
    }
    list<argument>::iterator i = args.begin();
    list<Ast*>::iterator j = (*parameter_list).begin();
    for (; i != args.end(); ++i,++j) {
      if ((*i).get_type() != (*j)->get_data_type()) {
	    return false;
	}
    }
    return true;
}

bool Procedure::variable_in_symbol_list_check(string variable)
{
    return local_symbol_table.variable_in_symbol_list_check(variable);
}

void Procedure::push_arguments_into_symbol_table()
{
    for (list<argument>::iterator i = args.begin(); i != args.end(); ++i) {
	string name_tmp = (*i).get_name();
	Symbol_Table_Entry * sym_tab_ent = new Symbol_Table_Entry(name_tmp, (*i).get_type());
	local_symbol_table.push_symbol(sym_tab_ent);
    }
}

int Procedure::get_and_increment_basic_block_number() {
    return cur_num_basic_block++;
}

bool Procedure::check_function_defined() {
    return function_defined;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Symbol_Table_Entry & Procedure::get_symbol_table_entry(string variable_name)
{
    return local_symbol_table.get_symbol_table_entry(variable_name);
}

void Procedure::print_ast(ostream & file_buffer)
{
    file_buffer << PROC_SPACE << "Procedure: " << name << "\n\n";

    list<Basic_Block *>::iterator i;
    for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
	(*i)->print_bb(file_buffer);
}
	
Basic_Block & Procedure::get_start_basic_block()
{
    list<Basic_Block *>::iterator i;
    i = basic_block_list.begin();
    return **i;
}

Basic_Block * Procedure::get_next_bb(Basic_Block & current_bb)
{
    bool flag = false;

    list<Basic_Block *>::iterator i;
    for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
	{
	    if((*i)->get_bb_number() == current_bb.get_bb_number())
		{
		    flag = true;
		    continue;
		}
	    if (flag)
		return (*i);
	}
	
    return NULL;
}

Eval_Result & Procedure::evaluate(ostream & file_buffer)
{
    Local_Environment & eval_env = *new Local_Environment();
    local_symbol_table.create(eval_env);
	
    Eval_Result * result = NULL;

    file_buffer << PROC_SPACE << "Evaluating Procedure " << name << "\n";
    file_buffer << LOC_VAR_SPACE << "Local Variables (before evaluating):\n";
    eval_env.print(file_buffer);
    file_buffer << "\n";
	
    Basic_Block * current_bb = &(get_start_basic_block());
    while (current_bb)
	{
	    result = &(current_bb->evaluate(eval_env, file_buffer));
		
	    // check if result type is bb_type
	    if ( result -> get_result_enum() == bb_result) // the last AST in the basic block was a goto or a conditional
		{
		    // wade through the basic block list to find the requisite block with the correct ID
		    list<Basic_Block *>::iterator i;
		    bool found = false;
		    for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
			{
			    // found the block with the id stored in the result (ie the id of the block to go to next)
			    if( (*i)->get_bb_number() == result -> get_block() )
				{
				    current_bb = (*i);
				    found = true;
				    break;
				}
			}
			
		    if (!found) 
			{
			    report_error("Basic block does not exist", NOLINE);
			    // current_bb = get_next_bb(*current_bb);
			}
		}
		
	    else // the last AST was not a control statement
		{
		    // check if the last ast was a return statement
		    if (result->get_result_enum() == return_result) {
			break;
		    }
		    current_bb = get_next_bb(*current_bb);		
		}
	}

    file_buffer << "\n\n";
    file_buffer << LOC_VAR_SPACE << "Local Variables (after evaluating):\n";
    eval_env.print(file_buffer);

    return *result;
}

