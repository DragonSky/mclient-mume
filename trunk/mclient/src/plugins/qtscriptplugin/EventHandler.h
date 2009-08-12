#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "MClientEventHandler.h"

class QEvent;
class ScriptEngine;

class EventHandler : public MClientEventHandler {
    Q_OBJECT
    
    public:
        EventHandler(QObject* parent=0);
        ~EventHandler();

        void customEvent(QEvent* e);

public slots:
        void parseInput(const QString &input);

    private:
	ScriptEngine* _engine;
	
 signals:
	void evaluate(const QString&);
	void variable(const QString&);
};


#endif /* EVENTHANDLER_H */
