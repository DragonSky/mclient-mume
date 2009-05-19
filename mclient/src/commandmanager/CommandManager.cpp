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

    _commandSymbol = QChar('#');
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
	  QString cmd = me->payload()->toString();
	  parseInput(cmd);

	}
	else if (s.startsWith("CommandRegister")) {
	  qDebug() << "CommandManager Registering Plugin";
	  QStringList sl = me->payload()->toStringList();
	  registerCommand(sl);

	}
	else if (s.startsWith("CommandUnregister")) {
	  qDebug() << "CommandManager Unregistering Plugin";
	  QString source = me->payload()->toString();
	  unregisterCommand(source);

	}
      }
    }
}


bool CommandManager::unregisterCommand(const QString& source) {
  if (!_pluginCommandHash.contains(source)) {
    qDebug() << source << "was not registered! Unable to remove.";
    return false;
  }

  QString s;
  foreach(s, _pluginCommandHash.value(source)) {
    if (_commandHash.remove(s) == 0)
      qDebug() << "Unable to remove command" << s << "for plugin" << source;
  }
  _pluginCommandHash.remove(source);
  return true;
}


void CommandManager::registerCommand(const QStringList& sl) {
  // First Element is Registering Source
  QString source = sl.at(0);
  QStringList registeredCommands;

  // All Other Elements should be in pairs (command, dataType)
  // TODO: Rewrite this as a QHash rather than a QStringList
  for (int i = 2; i < sl.size(); i += 2) {
    QString command = sl.at(i-1);
    QString dataType = sl.at(i);
    if (_commandHash.contains(command)) {
      qDebug() << "Error, command" << command << "was already added";
    } else {
      qDebug() << "Registering command " << command << " " << dataType;
      _commandHash.insert(command, dataType);
      registeredCommands << command;
    }
  }
  _pluginCommandHash.insert(source, registeredCommands);
}


bool CommandManager::parseInput(const QString& input) {    
  if (!input.startsWith(_commandSymbol)) {
      // Send to Socket
      QByteArray ba(input.toAscii());
      QVariant* qv = new QVariant(ba.append("\n"));
      QStringList sl;
      sl << "SendToSocketData";
      foreach(QString s, _runningSessions) {
	postEvent(qv, sl, s);
      }
      return true;

  } else {
    // Parse as a Plugin's Command
    QString command(input.mid(1));
    QString arguments("");

    QRegExp whitespace("\\s+");
    int whitespaceIndex = command.indexOf(whitespace);
    qDebug() << input << whitespaceIndex;

    if (whitespaceIndex >= 0) {
      arguments = command.mid(whitespaceIndex+1);
      command = command.left(whitespaceIndex);
    }
    qDebug() << "* CommandManager got an event: " << command << arguments;
  
    if (_commandHash.contains(command)) {
      // Relay command to corresponding plugin
      QVariant* qv = new QVariant(arguments);
      QStringList sl;
      sl << _commandHash.value(command);
      foreach(QString s, _runningSessions) {
	postEvent(qv, sl, s);
      }
      return true;
      
    } else {
      // Unknown command!
      QString errorString = "#unknown command \"" + command + "\"\n";
      QVariant* qv = new QVariant(errorString);
      QStringList sl;
      sl << "XMLDisplayData";
      foreach(QString s, _runningSessions) {
	postEvent(qv, sl, s);
      }
      
    }
  }
  return false;
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
