
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

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "local-environment.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "program.hh"

/****************************** Class Ics_Opd *****************************/

Opd_Category Ics_Opd::get_opd_category() 
{ 
    return type;
} 

Register_Descriptor * Ics_Opd::get_reg()
{
    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
		    "The get_Reg method should not be called for a non-reg operand");
}

Symbol_Table_Entry & Ics_Opd::get_symbol_entry() 
{
    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
		    "The get_Sym_Entry method should not be called for a non-address operand");
}

/****************************** Class Mem_Addr_Opd *****************************/

Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se, Data_Type _data_type) 
{
    symbol_entry = &se;
    type = memory_addr;
    data_type = _data_type;
}

Mem_Addr_Opd & Mem_Addr_Opd::operator=(const Mem_Addr_Opd & rhs)
{
    type = rhs.type;
    symbol_entry = rhs.symbol_entry;

    return *this;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer) 
{
    string name = symbol_entry->get_variable_name();

    file_buffer << name;
}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer) 
{
    Table_Scope symbol_scope = symbol_entry->get_symbol_scope();

    CHECK_INVARIANT(((symbol_scope == local) || (symbol_scope == global)), 
		    "Wrong scope value");

    if (symbol_scope == local)
	{
	    int offset = symbol_entry->get_start_offset();
	    file_buffer << -offset << "($fp)";
	}
    else
	file_buffer << symbol_entry->get_variable_name();
}

Data_Type Mem_Addr_Opd::get_data_type () {
    return data_type;
}

/****************************** Class Register_Addr_Opd *****************************/

Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * reg, Data_Type _data_type) 
{
    type = register_addr;
    register_description = reg;
    data_type = _data_type;
}

Register_Descriptor * Register_Addr_Opd::get_reg()    { return register_description; }

Register_Addr_Opd& Register_Addr_Opd::operator=(const Register_Addr_Opd& rhs)
{
    type = rhs.type;     
    register_description = rhs.register_description ;

    return *this;
}

void Register_Addr_Opd::print_ics_opd(ostream & file_buffer) 
{
    CHECK_INVARIANT((register_description != NULL), "Register cannot be null");

    string name = register_description->get_name();

    file_buffer << name;
}

void Register_Addr_Opd::print_asm_opd(ostream & file_buffer) 
{
    CHECK_INVARIANT((register_description != NULL), "Register cannot be null");

    string name = register_description->get_name();

    file_buffer << "$" << name;
}

Data_Type Register_Addr_Opd::get_data_type () {
    return data_type;
}

/****************************** Class Const_Opd *****************************/

template <class DATA_TYPE>
Const_Opd<DATA_TYPE>::Const_Opd(DATA_TYPE n, Data_Type _data_type) 
{
    type = constant_addr;
    num = n;
    data_type = _data_type;
}

template <class DATA_TYPE>
Const_Opd<DATA_TYPE> & Const_Opd<DATA_TYPE>::operator=(const Const_Opd<DATA_TYPE> & rhs)
{
    type = rhs.type;
    num = rhs.num;

    return *this;
}

template <class DATA_TYPE>
void Const_Opd<DATA_TYPE>::print_ics_opd(ostream & file_buffer) 
{
    if (data_type == int_data_type) {
	file_buffer << num;
    }
    else {
	file_buffer << fixed << setprecision(2) << num;
    }
}

template <class DATA_TYPE>
void Const_Opd<DATA_TYPE>::print_asm_opd(ostream & file_buffer) 
{
    if (data_type == int_data_type) {
	file_buffer << num;
    }
    else {
	file_buffer << fixed << setprecision(2) << num;
    }
}

template <class DATA_TYPE>
Data_Type Const_Opd<DATA_TYPE>::get_data_type()
{
    return data_type;
}

/****************************** Class Icode_Stmt *****************************/

Instruction_Descriptor & Icode_Stmt::get_op()
{ 
    return op_desc; 
}

Ics_Opd * Icode_Stmt::get_opd1()
{
    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd1 not implemented");
}

Ics_Opd * Icode_Stmt::get_opd2()
{
    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd2 not implemented");
}

Ics_Opd * Icode_Stmt::get_result()
{
    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Result not implemented");
}

void Icode_Stmt::set_opd1(Ics_Opd * ics_opd)
{
    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd1 not implemented");
}

void Icode_Stmt::set_opd2(Ics_Opd * ics_opd)
{
    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd2 not implemented");
}

void Icode_Stmt::set_result(Ics_Opd * ics_opd)
{
    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual methos set_Result not implemented");
}

/*************************** Class Move_IC_Stmt *****************************/

Move_IC_Stmt::Move_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * res)
{
    CHECK_INVARIANT((machine_dscr_object.spim_instruction_table[op] != NULL),
		    "Instruction description in spim table cannot be null");

    op_desc = *(machine_dscr_object.spim_instruction_table[op]);
    opd1 = o1;   
    result = res; 
}

Ics_Opd * Move_IC_Stmt::get_opd1()          { return opd1; }
Ics_Opd * Move_IC_Stmt::get_result()        { return result; }

void Move_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Move_IC_Stmt::set_result(Ics_Opd * io) { result = io; }

Move_IC_Stmt& Move_IC_Stmt::operator=(const Move_IC_Stmt& rhs)
{
    op_desc = rhs.op_desc;
    opd1 = rhs.opd1;
    result = rhs.result; 

    return *this;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer)
{
    CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
    CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");

    string operation_name = op_desc.get_name();
    string spaces = op_desc.get_ic_symbol();

    Icode_Format ic_format = op_desc.get_ic_format();

    switch (ic_format)
	{
	case i_r_op_o1:
	    file_buffer << "\t" << operation_name << ":" << spaces;
	    result->print_ics_opd(file_buffer);
	    file_buffer << " <- ";
	    opd1->print_ics_opd(file_buffer);
	    file_buffer << "\n";

	    break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				 "Intermediate code format not supported");
	    break;
	}
}

void Move_IC_Stmt::print_assembly(ostream & file_buffer)
{
    CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
    CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");
    string op_name = op_desc.get_mnemonic();

    Assembly_Format assem_format = op_desc.get_assembly_format();
    switch (assem_format)
	{
	case a_op_r_o1: 
	    file_buffer << "\t" << op_name;
	    file_buffer << " ";
	    result->print_asm_opd(file_buffer);
	    file_buffer << ", ";
	    opd1->print_asm_opd(file_buffer);
	    file_buffer << "\n";

	    break; 

	case a_op_o1_r: 
	    file_buffer << "\t" << op_name;
	    file_buffer << " ";
	    opd1->print_asm_opd(file_buffer);
	    file_buffer << ", ";
	    result->print_asm_opd(file_buffer);
	    file_buffer << "\n";

	    break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
	    break;
	}
}


/*************************** Class Set_Rel_IC_Stmt *****************************/

Set_Rel_IC_Stmt::Set_Rel_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * o2, Ics_Opd * res)
{
    CHECK_INVARIANT((machine_dscr_object.spim_instruction_table[op] != NULL),
		    "Instruction description in spim table cannot be null");

    op_desc = *(machine_dscr_object.spim_instruction_table[op]);
    opd1 = o1; 
    opd2 = o2;
    result = res; 
}

Ics_Opd * Set_Rel_IC_Stmt::get_opd1()          { return opd1; }
Ics_Opd * Set_Rel_IC_Stmt::get_opd2()          { return opd2; }
Ics_Opd * Set_Rel_IC_Stmt::get_result()        { return result; }

void Set_Rel_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Set_Rel_IC_Stmt::set_opd2(Ics_Opd * io)   { opd2 = io; }
void Set_Rel_IC_Stmt::set_result(Ics_Opd * io) { result = io; }

Set_Rel_IC_Stmt& Set_Rel_IC_Stmt::operator=(const Set_Rel_IC_Stmt& rhs)
{
    op_desc = rhs.op_desc;
    opd1 = rhs.opd1;
    opd2 = rhs.opd2;
    result = rhs.result; 

    return *this;
}

void Set_Rel_IC_Stmt::print_icode(ostream & file_buffer)
{
    CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
    CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a move IC Stmt");
    CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");

    string operation_name = op_desc.get_name();
    string spaces = op_desc.get_ic_symbol();
    
    Icode_Format ic_format = op_desc.get_ic_format();

    switch (ic_format)
	{
	case i_r_o1_op_o2: 
	    file_buffer << "\t" << operation_name << ":" << spaces;
	    result->print_ics_opd(file_buffer);
	    file_buffer << " <- ";
	    opd1->print_ics_opd(file_buffer);
	    file_buffer << " , ";
	    opd2->print_ics_opd(file_buffer);
	    file_buffer << "\n";

	    break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				 "Intermediate code format not supported");
	    break;
	}
}

void Set_Rel_IC_Stmt::print_assembly(ostream & file_buffer)
{
    CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a set_rel IC Stmt");
    CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a set_rel IC Stmt");
    CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");
    string op_name = op_desc.get_mnemonic();

    Assembly_Format assem_format = op_desc.get_assembly_format();
    switch (assem_format)
	{
	case a_op_r_o1_o2: 
	    file_buffer << "\t" << op_name << " ";
	    result->print_asm_opd(file_buffer);
	    file_buffer << ", ";
	    opd1->print_asm_opd(file_buffer);
	    file_buffer << ", ";
	    opd2->print_asm_opd(file_buffer);
	    file_buffer << "\n";
	    break; 
			
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
	    break;
	}
}

/******************************* Unary rel statement code ************************/

Set_Unary_Rel_IC_Stmt::Set_Unary_Rel_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * res)
{
    CHECK_INVARIANT((machine_dscr_object.spim_instruction_table[op] != NULL),
		    "Instruction description in spim table cannot be null");

    op_desc = *(machine_dscr_object.spim_instruction_table[op]);
    opd1 = o1; 
    result = res; 
}

Ics_Opd * Set_Unary_Rel_IC_Stmt::get_opd1()          { return opd1; }
Ics_Opd * Set_Unary_Rel_IC_Stmt::get_result()        { return result; }

void Set_Unary_Rel_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Set_Unary_Rel_IC_Stmt::set_result(Ics_Opd * io) { result = io; }

Set_Unary_Rel_IC_Stmt& Set_Unary_Rel_IC_Stmt::operator=(const Set_Unary_Rel_IC_Stmt& rhs)
{
    op_desc = rhs.op_desc;
    opd1 = rhs.opd1;
    result = rhs.result; 

    return *this;
}

void Set_Unary_Rel_IC_Stmt::print_icode(ostream & file_buffer)
{
    CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
    CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");

    string operation_name = op_desc.get_name();
    string spaces = op_desc.get_ic_symbol();    

    Icode_Format ic_format = op_desc.get_ic_format();

    switch (ic_format)
	{
	case i_r_op_o1:
	    file_buffer << "\t" << operation_name << ":" << spaces;
	    result->print_ics_opd(file_buffer);
	    file_buffer << " <- ";
	    opd1->print_ics_opd(file_buffer);
	    file_buffer << "\n";

	    break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				 "Intermediate code format not supported");
	    break;
	}
}

void Set_Unary_Rel_IC_Stmt::print_assembly(ostream & file_buffer)
{
    CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a set_rel IC Stmt");
    CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");
    string op_name = op_desc.get_mnemonic();

    Assembly_Format assem_format = op_desc.get_assembly_format();
    switch (assem_format)
	{
	case a_op_r_o1: 
	    file_buffer << "\t" << op_name << " ";
	    result->print_asm_opd(file_buffer);
	    file_buffer << ", ";
	    opd1->print_asm_opd(file_buffer);
	    file_buffer << "\n";
	    break; 
			
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
	    break;
	}
}

/******************************* Control statement code ************************/

Control_IC_Stmt::Control_IC_Stmt(Tgt_Op opn, Ics_Opd * _opd, int t_label) {
    opd = _opd;
    label = t_label;
    op_desc = *(machine_dscr_object.spim_instruction_table[opn]);
}

Control_IC_Stmt::Control_IC_Stmt(Tgt_Op opn, int t_label) {
    label = t_label;
    op_desc = *(machine_dscr_object.spim_instruction_table[opn]);
	
}

Control_IC_Stmt & Control_IC_Stmt::operator=(const Control_IC_Stmt & rhs) {
    opd = rhs.opd;
    label = rhs.label;
    op_desc = rhs.op_desc;
}
	
Ics_Opd * Control_IC_Stmt::get_opd1() {
    return opd;
}

void Control_IC_Stmt::set_opd1(Ics_Opd * io) {
    opd = io;
}

int Control_IC_Stmt::get_label() {
    return label;
}

void Control_IC_Stmt::set_label(int n) {
    label = n;
}

void Control_IC_Stmt::print_icode(ostream & file_buffer) {
	
    string operation_name = op_desc.get_name();
    string spaces = op_desc.get_ic_symbol();        

    Icode_Format ic_format = op_desc.get_ic_format();

    switch (ic_format)
	{
	case i_op_o1_o2_r: 
	    file_buffer << "\t" << operation_name << ":" << spaces;
	    opd->print_ics_opd(file_buffer);
	    file_buffer << " , zero : goto label" << label << endl;

	    break; 
	case i_op_o1:
	    file_buffer << "\t" << operation_name << " label" << label << endl << endl;
	    break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				 "Intermediate code format not supported");
	    break;
	}
	
}

void Control_IC_Stmt::print_assembly(ostream & file_buffer) {

    string op_name = op_desc.get_mnemonic();

    Assembly_Format assem_format = op_desc.get_assembly_format();
    switch (assem_format)
	{
	case a_op_o1_o2_r: 
	    CHECK_INVARIANT (opd, "Opd cannot be NULL for a control IC Stmt");
	    file_buffer << "\t" << op_name << " ";
	    opd->print_asm_opd(file_buffer);
	    file_buffer << ", $zero, label" << label << endl;
	    break; 
	case a_op_o1:
	    file_buffer << "\t" << op_name << " label" << label << endl;
	    break;
			
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
	    break;
	}
}

/******************************* Label statement code **************************/

Label_IC_Stmt::Label_IC_Stmt(int _num) {
    num = _num;
}

Label_IC_Stmt & Label_IC_Stmt::operator=(const Label_IC_Stmt & rhs) {
    num = rhs.num;
}

int Label_IC_Stmt::get_num() {
    return num;
}

void Label_IC_Stmt::set_num(int _num) {
    num = _num;
}

void Label_IC_Stmt::print_icode(ostream & file_buffer) {
    file_buffer << endl << "label" << num << ":\n";
}

void Label_IC_Stmt::print_assembly(ostream & file_buffer) {
    file_buffer << endl << "label" << num << ":\n";
}

/******************************* Class Ret_IC_Stmt*******************************/

Ret_IC_Stmt::Ret_IC_Stmt (string _fn_name) {
  fn_name = _fn_name;
}

void Ret_IC_Stmt::print_icode(ostream & file_buffer) {
  file_buffer << "\treturn" << endl;
}

void Ret_IC_Stmt::print_assembly(ostream & file_buffer) {
  file_buffer << "\tj epilogue_" << fn_name << endl;
}

/******************************* Class Call_IC_Stmt ****************************/
Call_IC_Stmt::Call_IC_Stmt(string _name) {
  name = _name;
}

void Call_IC_Stmt::print_icode(ostream & file_buffer) {
  file_buffer << "\tcall " << name << endl;
}

void Call_IC_Stmt::print_assembly(ostream & file_buffer) {
  file_buffer << "\tjal " << name << endl;
}

/******************************* Class Code_For_Ast ****************************/

Code_For_Ast::Code_For_Ast()
{
    ics_list = *new list<Icode_Stmt *>;
}

Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg)
{
    ics_list = ic_l;
    result_register = reg;
}

void Code_For_Ast::append_ics(Icode_Stmt & ic_stmt)
{
    ics_list.push_back(&ic_stmt);
    Ics_Opd * result = ic_stmt.get_result();
    result_register = result->get_reg();
}

list<Icode_Stmt *> & Code_For_Ast::get_icode_list()  
{ 
    return ics_list; 
}

void Code_For_Ast::set_icode_list(list<Icode_Stmt *> & new_list)  
{ 
    ics_list = new_list; 
}

Register_Descriptor * Code_For_Ast::get_reg()
{
    return result_register;
}

Code_For_Ast& Code_For_Ast::operator=(const Code_For_Ast& rhs)
{
    ics_list = rhs.ics_list;
    result_register = rhs.result_register;

    return *this;
}

/************************ class Instruction_Descriptor ********************************/

Tgt_Op Instruction_Descriptor::get_op()                   	{ return inst_op; }
string Instruction_Descriptor::get_name()                       { return name; }
string Instruction_Descriptor::get_mnemonic()                   { return mnemonic; }
string Instruction_Descriptor::get_ic_symbol()                  { return ic_symbol; }
Icode_Format Instruction_Descriptor::get_ic_format()            { return ic_format; }
Assembly_Format Instruction_Descriptor::get_assembly_format()   { return assem_format; }

Instruction_Descriptor::Instruction_Descriptor (Tgt_Op op, string temp_name, string temp_mnemonic, 
						string ic_sym, Icode_Format ic_form, Assembly_Format as_form)
{
    inst_op = op;
    name = temp_name; 
    mnemonic = temp_mnemonic;
    ic_symbol = ic_sym;
    ic_format = ic_form;
    assem_format = as_form;
}

Instruction_Descriptor::Instruction_Descriptor()
{
    inst_op = nop;
    name = "";
    mnemonic = "";
    ic_symbol = "";
    ic_format = i_nsy;
    assem_format = a_nsy;
}

template class Const_Opd<int>;
template class Const_Opd<float>;
