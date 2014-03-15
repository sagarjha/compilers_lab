
/*********************************************************************************************

                                cfglp : A CFG Language Processor
                                --------------------------------

           About:

           Implemented   by  Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker    (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs306: Language  Processors (theory and lab)  at IIT
           Bombay.

           Release  date Jan  15, 2013.  Copyrights reserved  by Uday
           Khedker. This implemenation has been made available purely
           for academic purposes without any warranty of any kind.

           Documentation  (functionality,   manual, and  design)  and
           related tools are at http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/

#include<string>
#include<fstream>

using namespace std;

#include"local-environment.hh"
#include"error-display.hh"
#include"user-options.hh"
#include<iomanip>
#include<algorithm>
#include<list>

float Eval_Result::get_value()
{
    report_internal_error("Should not reach, Eval_Result : get_value");
}

void Eval_Result::set_value(int number)
{
    report_internal_error("Should not reach, Eval_Result : set_value");
}

void Eval_Result::set_value(float number)
{
    report_internal_error("Should not reach, Eval_Result : set_value");
}

bool Eval_Result::is_variable_defined()
{
    report_internal_error("Should not reach, Eval_Result : is_variable_defined");
}

void Eval_Result::set_variable_status(bool def)
{
    report_internal_error("Should not reach, Eval_Result : set_variable_status");
}

int Eval_Result::get_block()
{
    report_internal_error("Should not reach, Eval_Result : get_block");
}

void Eval_Result::set_block_id(int num)
{
    report_internal_error("Should not reach, Eval_Result : set_block_id		");
}

float Eval_Result_Value::get_value()
{
    report_internal_error("Should not reach, Eval_Result : get_value");
}

void Eval_Result_Value::set_value(int number)
{
    report_internal_error("Should not reach, Eval_Result : set_value");
}

void Eval_Result_Value::set_value(float number)
{
    report_internal_error("Should not reach, Eval_Result : set_value");
}

///////////////////////////////////////////////////////////////////////////////

Eval_Result_Value_Int::Eval_Result_Value_Int()
{
    value = 0;
    defined = false;
    result_type = int_result;
}

Eval_Result_Value_Int::~Eval_Result_Value_Int()
{ }

void Eval_Result_Value_Int::set_value(int number)
{
    value = number;
    defined = true;
}

float Eval_Result_Value_Int::get_value()
{
    return value;
}

void Eval_Result_Value_Int::set_variable_status(bool def)
{
    defined = def;
}

bool Eval_Result_Value_Int::is_variable_defined()
{
    return defined;
}

void Eval_Result_Value_Int::set_result_enum(Result_Enum res)
{
    result_type = res;
}

Result_Enum Eval_Result_Value_Int::get_result_enum()
{
    return result_type;
}

///////////////////////////////////////////////////////////////////////////////

Eval_Result_Value_Float::Eval_Result_Value_Float()
{
    value = 0.00;
    defined = false;
    result_type = float_result;
}

Eval_Result_Value_Float::~Eval_Result_Value_Float()
{ }

void Eval_Result_Value_Float::set_value(float number)
{
    value = number;
    defined = true;
}

float Eval_Result_Value_Float::get_value()
{
    return value;
}

void Eval_Result_Value_Float::set_variable_status(bool def)
{
    defined = def;
}

bool Eval_Result_Value_Float::is_variable_defined()
{
    return defined;
}

void Eval_Result_Value_Float::set_result_enum(Result_Enum res)
{
    result_type = res;
}

Result_Enum Eval_Result_Value_Float::get_result_enum()
{
    return result_type;
}

///////////////////////////////////////////////////////////////////////////////

Eval_Result_Value_Double::Eval_Result_Value_Double()
{
    value = 0;
    defined = false;
    result_type = double_result;
}

Eval_Result_Value_Double::~Eval_Result_Value_Double()
{ }

void Eval_Result_Value_Double::set_value(float number)
{
    value = number;
    defined = true;
}

float Eval_Result_Value_Double::get_value()
{
    return value;
}

void Eval_Result_Value_Double::set_variable_status(bool def)
{
    defined = def;
}

bool Eval_Result_Value_Double::is_variable_defined()
{
    return defined;
}

void Eval_Result_Value_Double::set_result_enum(Result_Enum res)
{
    result_type = res;
}

Result_Enum Eval_Result_Value_Double::get_result_enum()
{
    return result_type;
}


/////////////////////////////////////////////////////////////
// functions for eval_result_bb

Eval_Result_BB::Eval_Result_BB()
{
    block_id = -1;
    result_type = bb_result;
}

Eval_Result_BB::~Eval_Result_BB()
{ }

void Eval_Result_BB::set_block_id(int number)
{
    block_id = number;
}

int Eval_Result_BB::get_block()
{
    return block_id;
}

Result_Enum Eval_Result_BB::get_result_enum()
{
    return result_type;
}

void Eval_Result_BB::set_result_enum(Result_Enum res)
{
}

///////////////////////////////////////////////////////////////////////////////////

Local_Environment::Local_Environment()
{}

Local_Environment::~Local_Environment()
{}

void Local_Environment::print(ostream & file_buffer)
{
    map<string, Eval_Result_Value *>::iterator i;
    for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
	    Eval_Result_Value * vi = variable_table[(*i).first];
	    if (vi != NULL)
		{
		    if (vi->is_variable_defined() == false)
			file_buffer << VAR_SPACE << (*i).first << " : undefined" << "\n";
		    else
			{
			    if (vi->get_result_enum() == int_result || vi->get_result_enum() == return_int_result) {
				file_buffer << VAR_SPACE << (*i).first << " : " << (int) vi->get_value() << "\n";
			    }
			    else if (vi->get_result_enum() == float_result || vi->get_result_enum() == double_result || vi->get_result_enum() == return_float_result) {
				file_buffer << VAR_SPACE << (*i).first << " : " << fixed << setprecision(2) << vi->get_value() << "\n";
			    }
			}
		}
	}
}

void Local_Environment::print_with_return(ostream & file_buffer, bool ifBefore)
{
    map<string, Eval_Result_Value *>::iterator i;
    for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
	    if ((i->first < "return") != ifBefore) {
		continue;
	    }
	    Eval_Result_Value * vi = variable_table[(*i).first];
	    if (vi != NULL)
		{
		    if (vi->is_variable_defined() == false)
			file_buffer << VAR_SPACE << (*i).first << " : undefined" << "\n";
		    else
			{
			    if (vi->get_result_enum() == int_result || vi->get_result_enum() == return_int_result) {
				file_buffer << VAR_SPACE << (*i).first << " : " << (int) vi->get_value() << "\n";
			    }
			    else if (vi->get_result_enum() == float_result || vi->get_result_enum() == double_result || vi->get_result_enum() == return_float_result) {
				file_buffer << VAR_SPACE << (*i).first << " : " << fixed << setprecision(2) << vi->get_value() << "\n";
			    }
			}
		}
	}
}

bool Local_Environment::is_variable_defined(string name)
{
    Eval_Result_Value * i = variable_table[name];
    if (i != NULL)
	return i->is_variable_defined();
    else
	return false;
}

Eval_Result_Value * Local_Environment::get_variable_value(string name)
{
    Eval_Result_Value * i = variable_table[name];
    return i;
}

void Local_Environment::put_variable_value(Eval_Result_Value & i, string name)
{
    variable_table[name] = &i;
}

bool Local_Environment::does_variable_exist(string name)
{
    if (variable_table[name] == NULL)
	return false;
    else
	return true;
}
