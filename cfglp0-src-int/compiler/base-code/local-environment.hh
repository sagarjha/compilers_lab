
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

#ifndef LOCAL_ENVIRONMENT_HH
#define LOCAL_ENVIRONMENT_HH

#include<string>
#include<map>

#define VAR_SPACE "         "

using namespace std;

typedef enum
    {
	int_result,
	void_result,
	bb_result,
	float_result,
	double_result,
	return_result
    } Result_Enum;

class Eval_Result;
class Local_Environment;

class Eval_Result 
{
protected:
    Result_Enum result_type;

public:
    virtual float get_value();
    virtual void set_value(int value);
    virtual void set_value(float value);    

    virtual bool is_variable_defined();
    virtual void set_variable_status(bool def);

    virtual void set_result_enum(Result_Enum res) = 0;
    virtual Result_Enum get_result_enum() = 0;

    virtual void set_block_id(int next_block);
    virtual int get_block();
};

class Eval_Result_Value:public Eval_Result
{
    int block_id;
public:
    virtual void set_value(int number);
    virtual void set_value(float value);

    virtual float get_value();

    virtual bool is_variable_defined() = 0;
    virtual void set_variable_status(bool def) = 0;

    virtual void set_result_enum(Result_Enum res) = 0;
    virtual Result_Enum get_result_enum() = 0;
};

// to store goto targets
class Eval_Result_BB:public Eval_Result
{
	int block_id;
	
public:	
	Eval_Result_BB();
	~Eval_Result_BB();

	virtual void set_block_id(int next_block);
	virtual int get_block();

	virtual void set_result_enum(Result_Enum res);
	virtual Result_Enum get_result_enum();
};

class Eval_Result_Value_Int:public Eval_Result_Value
{
    int value;
    bool defined;
public:
    Eval_Result_Value_Int();
    ~Eval_Result_Value_Int();

    void set_value(int number);
    float get_value();

    void set_variable_status(bool def);
    bool is_variable_defined();

    void set_result_enum(Result_Enum res);
    Result_Enum get_result_enum();
};

class Eval_Result_Value_Float:public Eval_Result_Value
{
    float value;
    bool defined;
public:
    Eval_Result_Value_Float();
    ~Eval_Result_Value_Float();

    void set_value(float number);
    float get_value();

    void set_variable_status(bool def);
    bool is_variable_defined();

    void set_result_enum(Result_Enum res);
    Result_Enum get_result_enum();
};

class Eval_Result_Value_Double:public Eval_Result_Value
{
    double value;
    bool defined;
public:
    Eval_Result_Value_Double();
    ~Eval_Result_Value_Double();

    void set_value(float number);
    float get_value();

    void set_variable_status(bool def);
    bool is_variable_defined();

    void set_result_enum(Result_Enum res);
    Result_Enum get_result_enum();
};

class Local_Environment
{
    map<string, Eval_Result *> variable_table;

public:
    Local_Environment();
    ~Local_Environment();

    void print(ostream & file_buffer);
    bool is_variable_defined(string name);
    Eval_Result * get_variable_value(string name);
    void put_variable_value(Eval_Result & value, string name);
    bool does_variable_exist(string name);
};

extern Local_Environment interpreter_global_table;

#endif
