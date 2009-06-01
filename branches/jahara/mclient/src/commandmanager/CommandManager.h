#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "MClientPlugin.h"

#include <QHash>
#include <QString>
#include <QStringList>

class MClientEvent;

class CommandManager : public MClientPlugin {
    Q_OBJECT
    
    public:
        CommandManager(QObject* parent=0);
        ~CommandManager();

        // From MClientPlugin
        void customEvent(QEvent* e);
        void configure();
        const bool loadSettings();
        const bool saveSettings() const;
        const bool startSession(QString s);
        const bool stopSession(QString s);

  protected:
        QChar _commandSymbol;

    private:
	QHash<QString, QString> _commandHash;
	QHash<QString, QStringList> _pluginCommandHash;

	bool parseInput(const QString& input);
	bool unregisterCommand(const QString& source);
	void registerCommand(const QStringList& sl);
};


#endif /* COMMANDMANAGER_H */