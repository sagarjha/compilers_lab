
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

#define AST_SPACE "         "
#define AST_NODE_SPACE "            "
#define AST_IF_SPACE "               "

using namespace std;

class Ast;

class Ast
{
protected:
    Data_Type node_data_type;
public:
    Ast();
    ~Ast();

    virtual Data_Type get_data_type();
    virtual bool check_ast(int line);

    virtual void print_ast(ostream & file_buffer) = 0;
    virtual void print_value(Local_Environment & eval_env, ostream & file_buffer);

    virtual Eval_Result & get_value_of_evaluation(Local_Environment & eval_env);
    virtual void set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result);
    virtual Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer) = 0;
};

class Assignment_Ast:public Ast
{
    Ast * lhs;
    Ast * rhs;

public:
    Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs);
    ~Assignment_Ast();

    Data_Type get_data_type();
    bool check_ast(int line);

    void print_ast(ostream & file_buffer);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Cast_Assignment_Ast:public Ast
{
    Ast * lhs;
    Ast * rhs;
    Data_Type data_type;

public:
    Cast_Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, Data_Type);
    ~Cast_Assignment_Ast();

    Data_Type get_data_type();
    bool check_ast(int line);

    void print_ast(ostream & file_buffer);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Name_Ast:public Ast
{
    string variable_name;
    Symbol_Table_Entry variable_symbol_entry;

public:
    Name_Ast(string & name, Symbol_Table_Entry & var_entry);
    ~Name_Ast();

    Data_Type get_data_type();
    
    void print_ast(ostream & file_buffer);

    void print_value(Local_Environment & eval_env, ostream & file_buffer);
    Eval_Result & get_value_of_evaluation(Local_Environment & eval_env);
    void set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result);
    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Cast_Name_Ast:public Ast
{
    Ast *name;
    Data_Type data_type;

public:
    Cast_Name_Ast(Ast *, Data_Type);
    ~Cast_Name_Ast();

    Data_Type get_data_type();
    
    void print_ast(ostream & file_buffer);

    void print_value(Local_Environment & eval_env, ostream & file_buffer);
    Eval_Result & get_value_of_evaluation(Local_Environment & eval_env);
    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

template <class T>
class Number_Ast:public Ast
{
    T constant;

public:
    Number_Ast(T number, Data_Type constant_data_type);
    ~Number_Ast();

    Data_Type get_data_type();
    void print_ast(ostream & file_buffer);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

// class definition of expr_ast
class Expr_Ast:public Ast
{
    Ast *lhs;
    Ast *rhs;
    int op;

public:
    Expr_Ast (Ast*, int, Ast*);
    ~Expr_Ast();

    Data_Type get_data_type();
    
    bool check_ast(int line);
  
    void print_ast (ostream&);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Cast_Expr_Ast:public Ast
{
    Ast *expr;
    Data_Type data_type;

public:
    Cast_Expr_Ast(Ast *, Data_Type);
    ~Cast_Expr_Ast();

    Data_Type get_data_type();

    void print_ast(ostream & file_buffer);

    void print_value(Local_Environment & eval_env, ostream & file_buffer);
    Eval_Result & get_value_of_evaluation(Local_Environment & eval_env);
    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

// class definition of boolean_expr_ast
class Boolean_Expr_Ast: public Ast
{
    Ast* lhs;
    Ast* rhs;
    int op;

public:
    Boolean_Expr_Ast(Ast*, int, Ast*);
    ~Boolean_Expr_Ast();

    Data_Type get_data_type();
    
    bool check_ast(int line);
  
    void print_ast (ostream &);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};


class Return_Ast:public Ast
{

public:
    Return_Ast();
    ~Return_Ast();

    void print_ast(ostream & file_buffer);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Conditional_Ast:public Ast
{

    Ast * pred;
    int bb_id1;
    int bb_id2;

public:
    Conditional_Ast (Ast*, int, int);
    ~Conditional_Ast();
    void print_ast (ostream &);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
    
};

class Goto_Ast:public Ast
{

    int bb_id;

public:
    Goto_Ast (int);
    ~Goto_Ast ();

    void print_ast (ostream &);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
    Eval_Result & evaluate_without_print(Local_Environment & eval_env, ostream & file_buffer);

};

#endif
