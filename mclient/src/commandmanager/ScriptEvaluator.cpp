/*
 *  Copyright (C) 1998 by Massimiliano Ghilardi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

#include <QDebug>
#include <QTextStream>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <limits.h>

#include "CommandManager.h"
#include "ScriptEvaluator.h"
#include "ScriptStack.h"
#include "ScriptMemory.h"

#define LEFT  1
#define RIGHT 2

#define BINARY     0
#define PRE_UNARY  LEFT
#define POST_UNARY RIGHT

#define LOWEST_UNARY_CODE 49
/*
 *  it would be 47, but operators 47 '(' and 48 ')'
 *  are treated separately
 */

typedef struct {
    int priority, assoc, syntax;
    QString name;
    Operator code;
} operator_list;

operator_list op_list[] = {
    { 0, 0,     0,	    "",     OP_NULL		},
	
    { 1, LEFT,  BINARY,     ",",    OP_COMMA		},
	
    { 2, RIGHT, BINARY,     "=",    OP_EQ		},
    { 2, RIGHT, BINARY,     "||=",  OP_OR_OR_EQ		},
    { 2, RIGHT, BINARY,     "^^=",  OP_XOR_XOR_EQ	},
    { 2, RIGHT, BINARY,     "&&=",  OP_AND_AND_EQ	},
    { 2, RIGHT, BINARY,     "|=",   OP_OR_EQ		},
    { 2, RIGHT, BINARY,     "^=",   OP_XOR_EQ		},
    { 2, RIGHT, BINARY,     "&=",   OP_AND_EQ		},
    { 2, RIGHT, BINARY,     "<<=",  OP_LSHIFT_EQ	},
    { 2, RIGHT, BINARY,     ">>=",  OP_RSHIFT_EQ	},
    { 2, RIGHT, BINARY,     "+=",   OP_PLUS_EQ		},
    { 2, RIGHT, BINARY,     "-=",   OP_MINUS_EQ		},
    { 2, RIGHT, BINARY,     "*=",   OP_TIMES_EQ		},
    { 2, RIGHT, BINARY,     "/=",   OP_DIV_EQ		},
    { 2, RIGHT, BINARY,     "%=",   OP_AMPERSAND_EQ	},
	
    { 3, LEFT,  BINARY,     "||",   OP_OR_OR		},
	
    { 4, LEFT,  BINARY,     "^^",   OP_XOR_XOR		},
	
    { 5, LEFT,  BINARY,     "&&",   OP_AND_AND		},
	
    { 6, LEFT,  BINARY,     "|",    OP_OR		},
	
    { 7, LEFT,  BINARY,     "^",    OP_XOR		},
	
    { 8, LEFT,  BINARY,     "&",    OP_AND		},
	
    { 9, LEFT,  BINARY,     "<",    OP_LESS		},
    { 9, LEFT,  BINARY,     "<=",   OP_LESS_EQ		},
    { 9, LEFT,  BINARY,     ">",    OP_GREATER		},
    { 9, LEFT,  BINARY,     ">=",   OP_GREATER_EQ	},
    { 9, LEFT,  BINARY,     "==",   OP_EQ_EQ		},
    { 9, LEFT,  BINARY,     "!=",   OP_NOT_EQ		},
	
    {10, LEFT,  BINARY,     "<<",   OP_LSHIFT		},
    {10, LEFT,  BINARY,     ">>",   OP_RSHIFT		},
	
    {11, LEFT,  BINARY,     "+",    OP_PLUS		},
    {11, LEFT,  BINARY,     "-",    OP_MINUS		},
	
    {12, LEFT,  BINARY,     "*",    OP_TIMES		},
    {12, LEFT,  BINARY,     "/",    OP_DIVISION		},
    {12, LEFT,  BINARY,     "%",    OP_AMPERSAND	},
	
    {14, LEFT,  BINARY,     ":<",   OP_COLON_LESS	},
    {14, LEFT,  BINARY,     ":>",   OP_COLON_GREATER	},
    {14, LEFT,  BINARY,     "<:",   OP_LESS_COLON	},
    {14, LEFT,  BINARY,     ">:",   OP_GREATER_COLON	},
    {14, LEFT,  BINARY,     ".<",   OP_POINT_LESS	},
    {14, LEFT,  BINARY,     ".>",   OP_POINT_GREATER	},
    {14, LEFT,  BINARY,     "<.",   OP_LESS_POINT	},
    {14, LEFT,  BINARY,     ">.",   OP_GREATER_POINT	},
    {14, LEFT,  BINARY,     ":",    OP_COLON		},
    {14, LEFT,  BINARY,     ".",    OP_POINT		},
    {14, LEFT,  BINARY,     "?",    OP_QUESTION		},
	
    { 0, 0,     0,	    "",     OP_ANOTHER_NULL	},
	
    { 0, RIGHT, PRE_UNARY,  "(",    OP_LEFT_PAREN	},
    { 0, RIGHT, POST_UNARY, ")",    OP_RIGHT_PAREN	},
	
    {13, RIGHT, PRE_UNARY,  "!",    OP_NOT		},
    {13, RIGHT, PRE_UNARY,  "~",    OP_TILDE		},
    {13, RIGHT, PRE_UNARY,  "++",   OP_PRE_PLUS_PLUS	},
    {13, RIGHT, POST_UNARY, "++",   OP_POST_PLUS_PLUS	},
    {13, RIGHT, PRE_UNARY,  "--",   OP_PRE_MINUS_MINUS	},
    {13, RIGHT, POST_UNARY, "--",   OP_POST_MINUS_MINUS	},
    {13, RIGHT, PRE_UNARY,  "*",    OP_STAR		},
    {13, RIGHT, PRE_UNARY,  "%",    OP_PRINT		},
    {13, RIGHT, PRE_UNARY,  "rand", OP_RANDOM  	        },
    {13, RIGHT, PRE_UNARY,  "attr", OP_ATTR		},
	
    {14, LEFT,  PRE_UNARY,  ":?",   OP_COLON_QUESTION	},
    {14, LEFT,  PRE_UNARY,  ".?",   OP_POINT_QUESTION	},
	
    {15, RIGHT, PRE_UNARY,  "+",    OP_PRE_PLUS		},
    {15, RIGHT, PRE_UNARY,  "-",    OP_PRE_MINUS	},
    {15, RIGHT, PRE_UNARY,  "@",    OP_A_CIRCLE		},
    {15, RIGHT, PRE_UNARY,  "$",    OP_DOLLAR		},
	
    { 0, 0,     PRE_UNARY,  "",     OP_PRE_NULL		},
    { 0, 0,     POST_UNARY, "",     OP_POST_NULL	}
};

static const QString error_msg[] = {
	"unknown error",
	"math stack overflow",
	"math stack underflow",
	"stack overflow",
	"stack underflow",
	"expression syntax",
	"operator expected",
	"value expected",
	"division by zero",
	"operand or index out of range",
	"missing right parenthesis",
	"missing left parenthesis",
	"internal error!",
	"operator not supported",
	"operation not completed (internal error)",
	"out of memory",
	"text/string longer than limit, discarded",
	"infinite loop",
	"numeric value expected",
	"string expected",
	"missing label",
	"missing separator \";\"",
	"#history recursion too deep",
	"user break",
	"too many defined variables",
	"undefined variable",
	"invalid digit in numeric value",
	"bad attribute syntax",
	"invalid variable name",
};

ScriptEvaluator::ScriptEvaluator(CommandManager* cm, QObject* parent) :
  _cm(cm), QObject(parent)
{
  _delim = QChar(';');
  _cmdChar = QChar('#');

  _stack = new ScriptStack(this);
  _memory = new ScriptMemory();
  _optionDebug = true;
}

ScriptEvaluator::~ScriptEvaluator() {
}

/* ----------------- PRIVATE FUNCTIONS --------------------- */

void ScriptEvaluator::printError(int err_num) {
  QString errorMessage;

  if (_error == NO_MEM_ERROR) {
    errorMessage = QString("#system call error: malloc (%1: %2)\n").arg(ENOMEM).arg(strerror(ENOMEM));
  } else {
    errorMessage = QString("#error: %1.\n").arg(error_msg[err_num]);
  }

  qDebug() << errorMessage;
  _cm->displayMessage(errorMessage);
}

int ScriptEvaluator::_eval(int mindepth) {

}

void ScriptEvaluator::checkDelete(ScriptObject* obj) {
//    if (obj->type==TYPE_TXT && obj->txt) {
//         ptrdel(obj->txt);
// 	obj->txt = NULL;
//    }
}



/* ----------------- PUBLIC FUNCTIONS ----------------- */

int ScriptEvaluator::eval_any(long* lres, QByteArray* pres, QString* what) {
  PrintMode printmode;
  long val;
  QTextStream in(pres);
  ScriptObject res;
    
  if (pres)
    printmode = PRINT_AS_PTR;
  else if (lres)
    printmode = PRINT_AS_LONG;
  else
    printmode = PRINT_NOTHING;

  _error = 0;
  _stack->clear();
  _line = what;
    
  _depth = 0;
  (void)_eval(0);
    
  if (!_error)
    (void)_stack->popObject(&res);
  if (_error) {
    if (_optionDebug) {
      qDebug("#result not available\n");
    }
  } else if (printmode!=PRINT_NOTHING || _optionDebug) {
    if (res.type==TYPE_NUM || res.type==TYPE_NUM_VAR) {
      
      val = res.type==TYPE_NUM ? res.num : _memory->number(res.num);
      
      if (printmode==PRINT_AS_PTR) {
	if (!MEM_ERROR) {
	  in << val;
	}
      } else if (printmode==PRINT_AS_LONG)
	*lres=val;
      
      if (_optionDebug) {
	if (_error) {
	  qDebug("#result not available\n");
	} else {
	  qDebug("#result: %ld\n", val);
	}
      }
    } else {
      QByteArray txt(res.type==TYPE_TXT ? res.txt : _memory->string(res.num));
      if (printmode==PRINT_AS_PTR) {
	if (txt && *txt.data()) {
	  if (res.type == TYPE_TXT)
	    /* shortcut! */
	    in << txt;
	  else
	    in << txt;
	} else
	  in << "";
      }
      if (_optionDebug) {
	if (_error) {
	  qDebug("#result not available\n");
	} else if (txt && *txt.data()) {
	  qDebug("#result: %s\n", txt.data());
	} else {
	  qDebug("#result empty\n");
	}
      }
    }
  }
  what = _line;
  
  if (!_error) {
    if (printmode==PRINT_AS_PTR && res.type == TYPE_TXT
	&& res.txt && res.txt)
      /* shortcut! */
      ;
    else
      checkDelete(&res);
  } else {
    while (_stack->objectStackSize() >= 0) {
      _stack->popObject(&res);
      checkDelete(&res);
    }
    res.type = 0;
  }
  
  if (res.type==TYPE_TXT_VAR)
    res.type = TYPE_TXT;
  else if (res.type==TYPE_NUM_VAR)
    res.type = TYPE_NUM;
  
  return res.type;
}

int ScriptEvaluator::evalp(QByteArray* res, QString* what) {
  return eval_any((long *)0, res, what);
}

int ScriptEvaluator::evall(long* res, QString* what) {
  return eval_any(res, (QByteArray *)0, what);
}

int ScriptEvaluator::evaln(QString* what) {
  return eval_any((long *)0, (QByteArray *)0, what);
}

