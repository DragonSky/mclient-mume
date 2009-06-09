#include "ScriptEvaluator.h"
#include "ScriptStack.h"

ScriptStack::ScriptStack(ScriptEvaluator* se, QObject* parent) :
  _se(se), QObject(parent)
{
  // Maximum Stack Size
  _maxStack = 100;

  // Reserve Memory
  _operatorStack.reserve(_maxStack);
  _objectStack.reserve(_maxStack);
}

ScriptStack::~ScriptStack() {
}

bool ScriptStack::pushOperator(Operator* op) {
    if (_operatorStack.size() < _maxStack) {
        _operatorStack.push(*op);
	return true;
    }
    else {
        _se->setError(STACK_OV_ERROR);
	return false;
    }
}

bool ScriptStack::popOperator(Operator* op) {
    if (!_operatorStack.isEmpty()) {
	*op = _operatorStack.pop();
	return true;
    }
    else {
        _se->setError(STACK_UND_ERROR);
	return false;
    }
}

bool ScriptStack::pushObject(ScriptObject* obj) {
    if (_objectStack.size() < _maxStack) {
        _objectStack.push(*obj);
	return true;
    }
    else {
	_se->setError(STACK_OV_ERROR);
	return false;
    }
}

bool ScriptStack::popObject(ScriptObject *obj) {
    if (!_objectStack.isEmpty()) {
        *obj = _objectStack.pop();
	return true;
    }
    else {
	_se->setError(STACK_UND_ERROR);
	return false;
    }
}
