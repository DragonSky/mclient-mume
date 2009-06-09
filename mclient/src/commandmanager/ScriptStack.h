#ifndef SCRIPTSTACK_H
#define SCRIPTSTACK_H

#include <QObject>
#include <QStack>
#include "ScriptEvaluator.h"

class ScriptEvaluator;

class ScriptStack : public QObject {
    Q_OBJECT

    public:
        ScriptStack(ScriptEvaluator* se, QObject* parent=0);
        ~ScriptStack();	

	int maxStack() { return _maxStack; }
	int objectStackSize() { return _objectStack.size(); }
	int operatorStackSize() { return _operatorStack.size(); }

	void clear() { _objectStack.clear(); _operatorStack.clear(); }

	bool pushOperator(Operator* op);
	bool popOperator(Operator* op);
	bool pushObject(ScriptObject* obj);
	bool popObject(ScriptObject* obj);

    private:
	int _maxStack;
	ScriptEvaluator* _se;
	QStack<ScriptObject> _objectStack;
	QStack<Operator> _operatorStack;
};

#endif /* SCRIPTSTACK_H */

