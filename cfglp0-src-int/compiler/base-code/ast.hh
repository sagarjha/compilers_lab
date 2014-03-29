
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

#ifndef AST_HH
#define AST_HH

#include<string>
#include<iostream>
#include<iomanip>
#include<typeinfo>
#include<list>

#define AST_SPACE "         "
#define AST_NODE_SPACE "            "
#define AST_SUB_NODE_SPACE "               "

using namespace std;

class Ast;

class Ast
{
protected:
    typedef enum
	{
	    zero_arity = 0,
	    unary_arity = 1,
	    binary_arity = 2,
	    ternary_arity = 3
	}Ast_Arity;

    Data_Type node_data_type;
    Ast_Arity ast_num_child;

    int lineno;

public:
    Ast();
    ~Ast();

    virtual Data_Type get_data_type();
    virtual bool check_ast();
    virtual Symbol_Table_Entry & get_symbol_entry();

    virtual void print(ostream & file_buffer) = 0;
    virtual void print_value(Local_Environment & eval_env, ostream & file_buffer);

    virtual Eval_Result & get_value_of_evaluation(Local_Environment & eval_env);
    virtual void set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result);
    virtual Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer) = 0;

    virtual Code_For_Ast & compile() = 0;
    virtual Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra) = 0;
    virtual Code_For_Ast & create_store_stmt(Register_Descriptor * store_register);
};

class Assignment_Ast:public Ast
{
    Ast * lhs;
    Ast * rhs;

public:
    Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line);
    ~Assignment_Ast();

    Data_Type get_data_type();
    bool check_ast();

    void print(ostream & file_buffer);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

    Code_For_Ast & compile();
    Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class Cast_Assignment_Ast:public Ast
{
    Ast * lhs;
    Ast * rhs;
    Data_Type data_type;

public:
    Cast_Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, Data_Type, int);
    ~Cast_Assignment_Ast();

    Data_Type get_data_type();
    bool check_ast(int line);

    void print(ostream & file_buffer);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

    Code_For_Ast & compile();
    Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class Name_Ast:public Ast
{
    string variable_name;
    Symbol_Table_Entry * variable_symbol_entry;

public:
    Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line);
    ~Name_Ast();

    Data_Type get_data_type();
    Symbol_Table_Entry & get_symbol_entry();

    void print(ostream & file_buffer);

    void print_value(Local_Environment & eval_env, ostream & file_buffer);
    Eval_Result & get_value_of_evaluation(Local_Environment & eval_env);
    void set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result);
    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

    Code_For_Ast & compile();
    Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
    Code_For_Ast & create_store_stmt(Register_Descriptor * store_register);
};

class Cast_Name_Ast:public Ast
{
    Ast *name;
    Data_Type data_type;

public:
    Cast_Name_Ast(Ast *, Data_Type, int);
    ~Cast_Name_Ast();

    Data_Type get_data_type();
    
    void print(ostream & file_buffer);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

    Code_For_Ast & compile();
    Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

template <class T>
class Number_Ast:public Ast
{
    T constant;

public:
    Number_Ast(T number, Data_Type constant_data_type, int line);
    ~Number_Ast();

    Data_Type get_data_type();

    void print(ostream & file_buffer);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

    Code_For_Ast & compile();
    Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

// class definition of expr_ast
class Expr_Ast:public Ast
{
    Ast *lhs;
    Ast *rhs;
    Tgt_Op op;

public:
    Expr_Ast (Ast*, Tgt_Op, Ast*, int);
    ~Expr_Ast();

    Data_Type get_data_type();
    bool check_ast();
  
    void print (ostream&);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

    Code_For_Ast & compile();
    Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
    Code_For_Ast & create_set_stmt(Tgt_Op opn, Register_Descriptor * reg1, Register_Descriptor * reg2);
    Code_For_Ast & create_set_stmt(Tgt_Op opn, Register_Descriptor * reg1);    
};

// class definition of cast_expr_ast
class Cast_Expr_Ast:public Ast
{
    Ast *expr;
    Data_Type data_type;
public:
    Cast_Expr_Ast(Ast *, Data_Type, int);
    ~Cast_Expr_Ast();

    Data_Type get_data_type();

    void print(ostream & file_buffer);
    
    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

    Code_For_Ast & compile();
    Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class Return_Ast:public Ast
{

public:
    Return_Ast(int line);
    ~Return_Ast();

    void print(ostream & file_buffer);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

    Code_For_Ast & compile();
    Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class Conditional_Ast:public Ast
{

    Ast * pred;
    int label_id1;
    int label_id2;

public:
    Conditional_Ast (Ast*, int, int, int);
    ~Conditional_Ast();
    void print (ostream &);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

    Code_For_Ast & compile();
    Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
    Code_For_Ast create_bne_stmt(Register_Descriptor * reg);
};

class Goto_Ast:public Ast
{
    int bb_id;

public:
    Goto_Ast (int, int);
    ~Goto_Ast ();

    void print (ostream &);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
    Eval_Result & evaluate_without_print(Local_Environment & eval_env, ostream & file_buffer);
    Code_For_Ast & compile();
    Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

#endif
