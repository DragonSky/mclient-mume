#include "AbstractCommandTask.h"

#include "PluginSession.h"
#include "CommandProcessor.h"
#include "CommandEntry.h"

#include "MClientEvent.h"

#include <QApplication>
#include <QDebug>
#include <QVariant>

AbstractCommandTask::AbstractCommandTask(CommandProcessor *ps, QObject *parent)
  : QThread(parent), _commandProcessor(ps) {

  _queue = QStringList();
  _stack = 0;
}


AbstractCommandTask::~AbstractCommandTask() {
}


void AbstractCommandTask::customEvent(QEvent *e) {
  if (e->type() == 10001) {
    MClientEvent* me = static_cast<MClientEvent*>(e);
    
    if (me->dataTypes().contains("SocketConnected")) {
      _socketOpen = true;

    }
    else if (me->dataTypes().contains("SocketDisconnected")) {
      _socketOpen = false;
      
    }
  }
}


void AbstractCommandTask::run() {
  qDebug() << "AbstractCommandTask run" << QThread::currentThread();
  exec();
  qDebug() << "AbstractCommandTask" << QThread::currentThread() << "done";
}


void AbstractCommandTask::parseUserInput(const QString &input) {
  // Add current input to the bottom of the queue
  _queue.prepend(input);
  //qDebug() << "* adding" << _queue.first() << "to queue";
  processStack();

}

bool AbstractCommandTask::processStack() {
  // Reinitialize the stack
  _stack = 0;

  while (!_queue.isEmpty()) {
    qDebug() << "* Command queue:" << _stack << _queue;

    if (_stack++ > 100) {
      _queue.clear();
      qWarning() << "! Stack overflow!";
      displayData("#error: stack overflow.\r\n");
      return false;
    }

    // Pop stack
    QString current(_queue.takeLast());
    //qDebug() << "* popping" << current << "from queue";

    QString command, arguments;    
    QRegExp rx("^\\s*(\\S+)(?:\\s+(.+))?");
    int rxIndex = rx.indexIn(current);
    if (rxIndex >= 0) {
      command = rx.cap(1);
      arguments = rx.cap(2);
    }

    // Otherwise, see if it is a command
    if (findCommand(command, arguments)) continue;
    
    // Lastly, it must be something that needs to be sent to the socket
    else if (_socketOpen) {
      parseArguments(current);
      qDebug() << "* writing" << current << "to socket";

      QVariant* qv = new QVariant(current + "\r\n");
      QStringList sl("SocketWriteData");
      postSession(qv, sl);
      
    }
    else {
      displayData("#no open connections. "
		  "Use '\033[1m#connect\033[0m' to open a connection.\r\n");

    }
  }
  return true;
}


const QString& AbstractCommandTask::parseArguments(QString &arguments,
						   CommandEntryType type) {
  if (type == CMD_ONE_LINE) {
    int lineEnd;
    if ((lineEnd = arguments.indexOf("\r\n")) != -1) {
      _queue.append(arguments.mid(lineEnd + 1));
      arguments.truncate(lineEnd);
      qDebug() << "arguments truncated to" << arguments << _queue;
    }

  } else {
    unsigned int i, leftCount = 0, rightCount = 0, escapedChar = 0;
    for (i = 0; i < (unsigned int) arguments.length(); i++) {
      switch (arguments.at(i).toAscii()) {
      case '\\':
	escapedChar = i;
	break;

      case '{':
	if (escapedChar != i-1 && leftCount >= rightCount) {
	  leftCount++;
	}
	break;

      case '}':
	if (escapedChar != i-1) {
	  rightCount++;
	  if (rightCount > leftCount) {
	    qDebug() << "! WTF?";

	  } else if (leftCount == rightCount) {
	    _queue.append(arguments.mid(i));
	    arguments.truncate(i);
	    
	  }
	}
	break;

      case '\n':
	if (leftCount == 0) {
	  _queue.append(arguments.mid(i));
	  arguments.truncate(i);

	}
	break;

      };
    }
  }

  return arguments;
}


bool AbstractCommandTask::findCommand(const QString &rawCommand,
				      QString &arguments) {
  if (!rawCommand.startsWith(_commandProcessor->getCommandSymbol()))
    return false;
  
  // Remove the command symbol
  QString command(rawCommand);
  command.remove(0, 1);

  // Identify Command
  CommandMapping map = _commandProcessor->getCommandMapping();
  CommandMapping::const_iterator i;
  for (i = map.constBegin(); i != map.constEnd(); ++i) {
    if (i.key().startsWith(command)) {
      // Command was identified

      // Parse the arguments depending on if the command is
      // one- or two-lined
      parseArguments(arguments, i.value()->commandType());

      if (!i.value()->pluginName().isEmpty()) {
	// External commands, relay command to corresponding plugin
	QVariant* qv = new QVariant(arguments);
	QStringList sl;
	sl << i.value()->dataType();
	postSession(qv, sl);
    	return true;

      } else {
	// Internal Commands
	command = i.key();

	if (command == "delim") {
	  /*
	    if (arguments.size() == 1 && arguments.at(0) != ' ') {
	    _delim = arguments.at(0);
	    displayData("#delimeter is now " + arguments + "\r\n");
	    } else
	    displayData("#not allowed\r\n");
	  */

	} else if (command == "qui") {
	  displayData("#you have to write '#quit' - no less, to quit!\r\n");
	  
	} else if (command == "quit") {
	  _commandProcessor->emitQuit();

	} else if (command == "version") {
	  QString output = QString("mClient %1, \251 2009 by Jahara\r\n"
				   "%2"
				   "%3")
	    .arg( // %1
#ifdef SVN_REVISION
		 "SVN Revision " + QString::number(SVN_REVISION)
#else
#ifdef MCLIENT_VERSION
		 MCLIENT_VERSION
#else
		 "unknown version"
#endif
#endif
		 )
	    .arg( // %2

#if __STDC__
		 "Compiled " __TIME__ " " __DATE__ "\r\n"
#else
		 ""
#endif
		 )
	    .arg( // %3
		 tr("Based on Qt %1 (%2 bit)\r\n")
		 .arg(QLatin1String(QT_VERSION_STR),
		      QString::number(QSysInfo::WordSize))
		 );
	  displayData(output);
	  
	} else if (command == "help") {
	  QString output = "\033[1;4m#commands available:\033[0m\r\n";
	  for (i = map.constBegin(); i != map.constEnd(); ++i) {
	    output += QString("\033[1m#%1\033[0m%2\r\n")
	      .arg(i.key(), -15, ' ') // pad 15 characters
	      .arg(i.value()->help());
	  }
	  displayData(output);

	} else if (command == "split") {
	  QChar splitChar = _commandProcessor->getDelimSymbol();
	  // QString::SkipEmptyParts
	  QListIterator<QString> i(arguments.split(splitChar));
	  for (i.toBack(); i.hasPrevious();) {
	    _queue.append(i.previous());
	  }

	}
      }
      qDebug() << "* Command " << command << "was run";
      return true;

    }
  }

  // Unknown command!
  parseArguments(arguments);
  displayData(QString("#unknown command \"" + command + "\"\r\n"));
  return false;

}


void AbstractCommandTask::parseMudOutput(const QString &output,
					 const QStringList &/*tags*/) {
  // match tag blocks
  //findAction(output, tags);
  displayData(output);
  processStack();

}


void AbstractCommandTask::displayData(const QString &output) {
  QVariant* qv = new QVariant(output);
  QStringList sl("DisplayData");
  postSession(qv, sl);
}

void AbstractCommandTask::postSession(QVariant *payload,
			      const QStringList& tags) {
  MClientEventData *med =
    new MClientEventData(payload, tags,
			 _commandProcessor->getPluginSession()->session());
  MClientEvent *me = new MClientEvent(med);

  QApplication::postEvent(_commandProcessor->getPluginSession(), me);
}
