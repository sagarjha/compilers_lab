
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

#ifndef PROCEDURE_HH
#define PROCEDURE_HH

#include<string>
#include<map>
#include<list>

#define PROC_SPACE "   "
#define LOC_VAR_SPACE "      "

using namespace std;

class Procedure;

class args;

class argument
{
    Data_Type type;
    string name;
public:
    argument (Data_Type given_type, string arg_name) {
	type = given_type;
	name = arg_name;
    }
    ~argument () {
    }
    Data_Type get_type();
    string get_name();
    void set_name(string);
};

class Procedure
{
    Data_Type return_type;
    string name;
    Symbol_Table local_symbol_table;
    list<Basic_Block *> basic_block_list;
    list <argument> args;
    int cur_num_basic_block;
    bool function_defined;
    /* To store argument ASTs */
    list <Eval_Result_Value *> call_arguments;
    /* New code ends */
public:
    Procedure(Data_Type proc_return_type, string proc_name, list <argument*> arg_list);
    ~Procedure();

    string get_proc_name();
    void set_basic_block_list(list<Basic_Block *> bb_list);
    void set_local_list(Symbol_Table & new_list);
    /* To set arguments (Eval_Result_Value objects) before evaluating and to clear them afterwards */
    void push_call_arguments(list<Eval_Result_Value *> new_arg);
    void clear_call_arguments();
    /* New code ends */
    Data_Type get_return_type();
    bool match_argument_list(list<argument*> *arg_list);
    bool match_function_call(list <Ast *> * parameter_list);
    bool check_function_defined();
    
    Symbol_Table_Entry & get_symbol_table_entry(string variable_name);

    void print_ast(ostream & file_buffer);

    Basic_Block * get_next_bb(Basic_Block & current_bb);
    Basic_Block & get_start_basic_block();

    Eval_Result & evaluate(ostream & file_buffer);

    bool variable_in_symbol_list_check(string variable);
    void push_arguments_into_symbol_table();
    int get_and_increment_basic_block_number();
};

#endif
