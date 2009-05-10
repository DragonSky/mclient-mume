#include "CommandManager.h"

#include "MClientEvent.h"
#include "PluginManager.h"

#include <QDebug>
#include <QStringList>
#include <QVariant>

Q_EXPORT_PLUGIN2(commandmanager, CommandManager)

CommandManager::CommandManager(QObject* parent) 
        : MClientPlugin(parent) {
    _shortName = "commandmanager";
    _longName = "Command Manager";
    _description = "Parses user input";
    //_dependencies.insert("some_stupid_api", 10);
    _implemented.insert("commandmanager",1);
    _dataTypes << "CommandInput" << "CommandRegister" << "CommandUnregister";

}


CommandManager::~CommandManager() {
    stopAllSessions();
}


void CommandManager::customEvent(QEvent* e) {
    if(e->type() != 10001) {
        qDebug() << "Somehow received the wrong kind of event...";
    } else {
      MClientEvent* me = static_cast<MClientEvent*>(e);
      
      QString s;
      QStringList types = me->dataTypes();
      foreach(s, types) {
	if (s.startsWith("CommandInput")) {
	  QString command = me->payload()->toString();
	  QString arguments;

	  QRegExp whitespace("\\s+");
	  int whitespaceIndex = command.indexOf(whitespace);
	  if (whitespaceIndex >= 0) {
	    arguments = command.mid(whitespaceIndex+1);
	    command = command.left(whitespaceIndex);
	  }
	  qDebug() << "CommandManager got an event: " << command << arguments;

	  if (commandHash.contains(command)) {
	    // Relay command to corresponding plugin
	    QVariant* qv = new QVariant(arguments);
	    QStringList sl;
	    sl << commandHash.value(command);
	    foreach(QString s, _runningSessions) {
	      postEvent(qv, sl, s);
	    }

	  } else {
	    // Unknown command!
	    qDebug() << "Unknown command!";
	    QString errorString = "Unknown command: " + command + "\n";
	    QVariant* qv = new QVariant(errorString);
	    QStringList sl;
	    sl << "XMLDisplayData";
	    foreach(QString s, _runningSessions) {
	      postEvent(qv, sl, s);
	    }

	  }
	}
	else if (s.startsWith("CommandRegister")) {
	  qDebug() << "CommandManager Registering Plugin";
	  QStringList sl = me->payload()->toStringList();
	  // First Element is Registering Source
	  QString source = sl.at(0);
	  QStringList registeredCommands;
	  // All Other Elements should be in pairs (command, dataType)
	  for (int i = 2; i < sl.size(); i += 2) {
	    QString command = sl.at(i-1);
	    QString dataType = sl.at(i);
	    if (commandHash.contains(command)) {
	      qDebug() << "Error, command" << command << "was already added";
	    } else {
	      qDebug() << "Registering command " << command << " " << dataType;
	      commandHash.insert(command, dataType);
	      registeredCommands << command;
	    }
	  }
	  pluginCommandHash.insert(source, registeredCommands);
	}
	else if (s.startsWith("CommandUnregister")) {
	  qDebug() << "CommandManager Unregistering Plugin";
	  QString source = me->payload()->toString();
	  if (!pluginCommandHash.contains(source)) {
	    qDebug() << source << "was not registered! Unable to remove.";
	    return ;
	  }
	  QString s;
	  foreach(s, pluginCommandHash.value(source)) {
	    if (commandHash.remove(s) == 0)
	      qDebug() << "Unable to remove command" << s << "for plugin" << source;
	  }
	  pluginCommandHash.remove(source);
	}
      }
    }
}


void CommandManager::configure() {
}


const bool CommandManager::loadSettings() {
    return true;
}


const bool CommandManager::saveSettings() const {
    return true;
}


const bool CommandManager::startSession(QString s) {
    _runningSessions << s;
    return true;
}


const bool CommandManager::stopSession(QString s) {
    int removed =  _runningSessions.removeAll(s);
    return removed!=0?true:false;
}
