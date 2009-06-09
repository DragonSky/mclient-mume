#ifndef SCRIPTEVALUATOR_H
#define SCRIPTEVALUATOR_H

#include <QObject>

class CommandManager;
class ScriptStack;
class ScriptMemory;

/* Definitions */
#define STACK_OV_ERROR        1
#define STACK_UND_ERROR	      2
#define DYN_STACK_OV_ERROR    3
#define DYN_STACK_UND_ERROR   4
#define SYNTAX_ERROR          5
#define NO_OPERATOR_ERROR     6
#define NO_VALUE_ERROR        7
#define DIV_BY_ZERO_ERROR     8
#define OUT_RANGE_ERROR       9
#define MISSING_PAREN_ERROR  10
#define MISMATCH_PAREN_ERROR 11
#define INTERNAL_ERROR	     12
#define NOT_SUPPORTED_ERROR  13
#define NOT_DONE_ERROR       14
#define NO_MEM_ERROR         15
#define MEM_LIMIT_ERROR      16
#define MAX_LOOP_ERROR       17
#define NO_NUM_VALUE_ERROR   18
#define NO_STRING_ERROR      19
#define NO_LABEL_ERROR       20
#define MISSING_SEPARATOR_ERROR  21
#define HISTORY_RECURSION_ERROR  22
#define USER_BREAK		 23
#define OUT_OF_VAR_SPACE_ERROR   24
#define UNDEFINED_VARIABLE_ERROR 25
#define OUT_BASE_ERROR		 26
#define BAD_ATTR_ERROR		 27
#define INVALID_NAME_ERROR	 28

#define TYPE_NUM 1
#define TYPE_TXT 2
#define TYPE_NUM_VAR 3
#define TYPE_TXT_VAR 4

enum PrintMode { PRINT_NOTHING = 0, PRINT_AS_PTR, PRINT_AS_LONG };

#define REAL_ERROR (error && error != USER_BREAK)
#define MEM_ERROR  (error == NO_MEM_ERROR || error == MEM_LIMIT_ERROR)

typedef struct {
  int  type;
  long num;         /* used for numeric types or as index for all variables */
  QByteArray txt;   /* used for text types */
} ScriptObject;

/* Operation Codes */
enum op_codes {
  OP_NULL=0, OP_COMMA, OP_EQ, OP_OR_OR_EQ, OP_XOR_XOR_EQ, OP_AND_AND_EQ,
  OP_OR_EQ, OP_XOR_EQ, OP_AND_EQ, OP_LSHIFT_EQ, OP_RSHIFT_EQ, OP_PLUS_EQ,
  OP_MINUS_EQ, OP_TIMES_EQ, OP_DIV_EQ, OP_AMPERSAND_EQ, OP_OR_OR, OP_XOR_XOR,
  OP_AND_AND, OP_OR, OP_XOR, OP_AND, OP_LESS, OP_LESS_EQ, OP_GREATER,
  OP_GREATER_EQ, OP_EQ_EQ, OP_NOT_EQ, OP_LSHIFT, OP_RSHIFT, OP_PLUS, OP_MINUS,
  OP_TIMES, OP_DIVISION, OP_AMPERSAND,

  OP_COLON_LESS, OP_COLON_GREATER, OP_LESS_COLON, OP_GREATER_COLON,
  OP_POINT_LESS, OP_POINT_GREATER, OP_LESS_POINT, OP_GREATER_POINT,
  OP_COLON, OP_POINT, OP_QUESTION, OP_ANOTHER_NULL,

  OP_LEFT_PAREN, OP_RIGHT_PAREN, OP_NOT, OP_TILDE,
  OP_PRE_PLUS_PLUS, OP_POST_PLUS_PLUS, OP_PRE_MINUS_MINUS, OP_POST_MINUS_MINUS,
  OP_STAR, OP_PRINT, OP_RANDOM, OP_ATTR, OP_COLON_QUESTION, OP_POINT_QUESTION,
  OP_PRE_PLUS, OP_PRE_MINUS, OP_A_CIRCLE, OP_DOLLAR, OP_PRE_NULL, OP_POST_NULL
};

typedef enum op_codes Operator;

class ScriptEvaluator : public QObject {
    Q_OBJECT

    public:
        ScriptEvaluator(CommandManager* cm, QObject* parent=0);
        ~ScriptEvaluator();

	int eval_any(long *lres, QByteArray *pres, QString* what);
	int evalp(QByteArray *pres, QString* what);
	int evall(long *lres, QString* what);
	int evaln(QString* what);

	void setError(int err_num) { _error = err_num; };
	
    private:
	bool _optionDebug;
	int _error, _depth;
	QString* _line;
	CommandManager* _cm;
	ScriptStack* _stack;
	ScriptMemory* _memory;
	QChar _delim, _cmdChar;

	void printError(int err_num);

	int _eval(int mindepth);
	void checkDelete(ScriptObject* obj);

};

#endif /* SCRIPTEVALUATOR_H */
