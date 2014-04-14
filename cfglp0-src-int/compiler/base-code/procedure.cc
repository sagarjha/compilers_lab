
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

Data_Type args::get_type() {
    return type;
}

string args::get_name() {
    return name;
}

void args::set_name(string arg_name) {
    name = arg_name;
}

Procedure::Procedure(Data_Type proc_return_type, string proc_name, list<args*> arg_list, int line)
{
    return_type = proc_return_type;
    name = proc_name;
    list <args *>::iterator i;
    list <args>::iterator j;
    for (i = arg_list.begin(); i != arg_list.end(); ++i) {
	for (j = arguments.begin(); j!=arguments.end(); j++) { // March 11, 4:00 am, inner loop introduced to check if any two arguments had the same name
	    if ((*j).get_name() == (*i)->get_name())
		{
		    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable name clash amongst function arguments.");
		}
	}
	arguments.push_back(*(*i));
    }
    cur_num_basic_block = 2;
    function_defined=false;
    function_returned=false;
	
    lineno = line;
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

void Procedure::set_basic_block_list(list<Basic_Block *> & bb_list)
{
    basic_block_list = bb_list;
}

void Procedure::push_call_arguments(list<Eval_Result_Value *> new_arg)
{
    // call_arguments = new_arg;
    
    // Deep copy
    int n = new_arg.size();
    for (int i=0; i<n; i++)
	{
	    call_arguments.splice(call_arguments.begin(), new_arg);
	}
}

void Procedure::clear_call_arguments()
{
    int n = call_arguments.size();
    for (int i=0; i<n; i++) {
	call_arguments.pop_back();
    }
}

void Procedure::set_local_list(Symbol_Table & new_list)
{
    // do not set but append instead
    local_symbol_table.set_table_scope(local);
    list <Symbol_Table_Entry *> new_list_of_entries = new_list.get_variable_table();
    for (list <Symbol_Table_Entry*>::iterator i = new_list_of_entries.begin(); i != new_list_of_entries.end(); ++i) {
	local_symbol_table.push_symbol(*i);
	(*i)->set_symbol_scope(local);
    }

}

Data_Type Procedure::get_return_type()
{
    return return_type;
}

bool Procedure::check_function_defined() {
    return function_defined;
}

void Procedure::check_function_returned() {
    if (return_type == void_data_type) {
	return ;
    }	
    if (!function_returned) {
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Function " + name + " does not have a return statement matching its return type");
    }
}

bool Procedure::check_return_type (Data_Type data_type) {
    function_returned = true;
    return (return_type == data_type);
}

bool Procedure::match_argument_list(list<args*> *arg_list) {
    if (function_defined) {
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "The function has already been defined");
    }
    function_defined = true;
    if (arg_list == NULL) {
	return arguments.size() == 0;
    }
    if (arg_list->size() != arguments.size()) {
	return false;
    }
    list<args>::iterator i;
    list<args*>::iterator j;
  
    for (i = arguments.begin(), j = arg_list->begin(); i != arguments.end(); ++i, ++j) {
	// March 11, 4:00 am. Previously, only type was checked here and the name was set
	if (((*i).get_type() != (*j)->get_type()) || ((*i).get_name()).compare((*j)->get_name()) != 0) {
	    return false;
	}
    }
    return true;
}

bool Procedure::match_function_call(list <Ast *> * parameter_list) {
    if (parameter_list == NULL) {
	if (arguments.size() == 0) {
	    return true;
	}
	return false;
    }
    if (parameter_list->size () != arguments.size()) {
	return false;
    }
    list<args>::iterator i = arguments.begin();
    list<Ast*>::iterator j = (*parameter_list).begin();
    for (; i != arguments.end(); ++i,++j) {
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
    for (list<args>::iterator i = arguments.begin(); i != arguments.end(); ++i) {
	string name_tmp = (*i).get_name();
	if (variable_in_symbol_list_check(name_tmp)) {
	    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "A local variable has the same name as a function argument");
	}
	Symbol_Table_Entry * sym_tab_ent = new Symbol_Table_Entry(name_tmp, (*i).get_type(), lineno);
	sym_tab_ent->set_symbol_scope(local);
	local_symbol_table.push_symbol(sym_tab_ent);
    }
    local_symbol_table.set_table_scope(local);
}

Symbol_Table_Entry & Procedure::get_symbol_table_entry(string variable_name)
{
    return local_symbol_table.get_symbol_table_entry(variable_name);
}

void Procedure::print(ostream & file_buffer)
{
    CHECK_INVARIANT((return_type == void_data_type), "Only void return type of funtion is allowed");

    file_buffer << PROC_SPACE << "Procedure: main, Return Type: void\n";

    if ((command_options.is_show_symtab_selected()) || (command_options.is_show_program_selected()))
	{
	    file_buffer << "   Local Declarartions\n";
	    local_symbol_table.print(file_buffer);
	}

    if ((command_options.is_show_program_selected()) || (command_options.is_show_ast_selected()))
	{
	    list<Basic_Block *>::iterator i;
	    for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
		(*i)->print_bb(file_buffer);
	}
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
	    current_bb = get_next_bb(*current_bb);		
	}

    file_buffer << "\n\n";
    file_buffer << LOC_VAR_SPACE << "Local Variables (after evaluating):\n";
    eval_env.print(file_buffer);

    return *result;
}

void Procedure::compile()
{
    // assign offsets to local symbol table
    local_symbol_table.set_start_offset_of_first_symbol(4);
    local_symbol_table.set_size(4);
    local_symbol_table.assign_offsets();

    // compile the program by visiting each basic block
    list<Basic_Block *>::iterator i;
    for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
	(*i)->compile();
}

void Procedure::print_icode(ostream & file_buffer)
{
    file_buffer << "  Procedure: " << name << "\n";
    file_buffer << "  Intermediate Code Statements\n";

    list<Basic_Block *>::iterator i;
    for (i = basic_block_list.begin(); i != basic_block_list.end(); i++)
	(*i)->print_icode(file_buffer);
}

void Procedure::print_assembly(ostream & file_buffer)
{
    print_prologue(file_buffer);

    list<Basic_Block *>::iterator i;
    for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
	(*i)->print_assembly(file_buffer);

    print_epilogue(file_buffer);
}

void Procedure::print_prologue(ostream & file_buffer)
{
    stringstream prologue;

    prologue << "\n\
	.text \t\t\t# The .text assembler directive indicates\n\
	.globl " << name << "\t\t# The following is the code (as oppose to data)\n";

    prologue << name << ":\t\t\t\t# .globl makes main know to the \n\t\t\t\t# outside of the program.\n\
# Prologue begins \n\
	sw $fp, 0($sp)\t\t# Save the frame pointer\n\
	sub $fp, $sp, 4\t\t# Update the frame pointer\n";

    int size = local_symbol_table.get_size();
    if (size > 0)
	prologue << "\n\tsub $sp, $sp, " << (size+4) << "\t\t# Make space for the locals\n";
    else
	prologue << "\n\tsub $sp, $sp, 4\t\t#Make space for the locals\n";

    prologue << "# Prologue ends\n\n";

    file_buffer << prologue.str();
}

void Procedure::print_epilogue(ostream & file_buffer)
{
    stringstream epilogue;

    int size = local_symbol_table.get_size();

    if (size > 0)
	epilogue << "\n# Epilogue Begins\n\tadd $sp, $sp, " << (size+4) << "\n";
    else
	epilogue << "\n#Epilogue Begins\n\tadd $sp, $sp, 4\n";

    epilogue << "\tlw $fp, 0($sp)  \n\tjr        $31\t\t# Jump back to the operating system.\n# Epilogue Ends\n\n";

    file_buffer << epilogue.str();
}
