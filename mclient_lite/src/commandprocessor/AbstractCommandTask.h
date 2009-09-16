#ifndef ABSTRACTCOMMANDTASK_H
#define ABSTRACTCOMMANDTASK_H

#include <QThread>
#include <QStringList>

#include "CommandEntry.h"

class CommandProcessor;

class AbstractCommandTask : public QThread {
    
    public:
        AbstractCommandTask(CommandProcessor*, QObject *parent=0);
        ~AbstractCommandTask();

	virtual void customEvent(QEvent *e);

 protected:
	void run();
	void parseMudOutput(const QString &, const QStringList &);
        void parseUserInput(const QString &);

	bool _verbose;
	bool _socketOpen;

   private:
	QStringList _queue;
	uint _stack;
	CommandProcessor *_commandProcessor;

	bool processStack();
	
        bool findCommand(const QString &, QString &);

	const QString& parseArguments(QString &,
				      CommandEntryType =CMD_ONE_LINE);

	void displayData(const QString &);
	void postSession(QVariant *, const QStringList &);
};


#endif /* ABSTRACTCOMMANDTASK_H */
