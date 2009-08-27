#include "EventHandler.h"

#include <QEvent>
#include <QVariant>
#include <QCoreApplication> // for sending command

#include "MClientEvent.h"
#include "ProxyServer.h"
#include "ProxyConnection.h"

#include "PluginSession.h"
#include "CommandProcessor.h"

EventHandler::EventHandler(QObject* parent) : MClientEventHandler(parent) {
  _proxyServer = new ProxyServer(this);

  connect(_proxyServer, SIGNAL(displayMessage(const QString &)),
	  SLOT(displayMessage(const QString &)));

  connect(this, SIGNAL(removeProxyConnection(ProxyConnection *)),
	  _proxyServer, SLOT(removeProxyConnection(ProxyConnection *)));

}


EventHandler::~EventHandler() {
  _proxyServer->deleteLater();
}


void EventHandler::customEvent(QEvent *e) {
  if (e->type() == 10000)
    engineEvent(e);
  else if (e->type() == 10001) {
    
    MClientEvent* me = static_cast<MClientEvent*>(e);
    
    if (me->dataTypes().contains("DisplayData") ||
	me->dataTypes().contains("UserInput")) {
      QByteArray ba = me->payload()->toByteArray();
      emit sendToSocket(ba);
      
    } else if (me->dataTypes().contains("EchoMode")) {
      echoMode(me->payload()->toBool());

    } else if (me->dataTypes().contains("SocketDisconnected")) {
      echoMode(true);

    } else if (me->dataTypes().contains("ProxyCommand")) {
      QString args = me->payload()->toString();
      proxyCommand(args);

    }
    
  }
  else 
    qDebug() << "SocketManagerIO got a customEvent of type" << e->type();
}


void EventHandler::sendUserInput(const QByteArray &input) {
  // TODO: post to the display directly? If we try posting the normal
  // way  we get loopback
  
  // Post to command processor
  QVariant *payload = new QVariant(input.trimmed());
  QStringList tags("UserInput");
  MClientEventData *med = new MClientEventData(payload, tags,
					       _pluginSession->session());
  MClientEvent* me = new MClientEvent(med);
  QCoreApplication::postEvent(_pluginSession->getCommand()->getUserInput(),
			      me);
  
}


bool EventHandler::proxyCommand(const QString &args) {
  const ProxyConnections map = _proxyServer->getConnections();

  if (args.isEmpty()) {
    unsigned int j = 1;
    QString message("#\tIP\tConnection\n");
    ProxyConnections::const_iterator i = map.constBegin();
    while (i != map.constEnd()) {
      QString state;
      switch (i.value()->state()) {
      case PROXY_AUTHENTICATING:
	state = "Authenticating";
	break;
      case PROXY_CONNECTED:
	state = "Connected";
	break;
      case PROXY_DISCONNECTED:
	state = "Disconnected";
	break;
      };
      message += QString("%1\t%2\t%3\t%4\n")
	.arg(QString::number(j++))
	.arg(i.value()->peerAddress())
	.arg(i.key().toString())
	.arg(state);
      ++i;
    }
    displayMessage(message);
    return true;

  } else {
    // Other commands
    QRegExp rx("^(kill)\\s+(\\d+)");
    if (!rx.exactMatch(args)) {
      // Incorrect command syntax
      qDebug() << "* Unknown syntax in proxy command regular expression";
      return false;
    }
    // Parse the command
    QStringList cmd = rx.capturedTexts();
    
    qDebug() << cmd;
    QString command(cmd.at(1));
    QString number(cmd.at(2));

    if (command == "kill") {
      unsigned int j = 1;
      ProxyConnections::const_iterator i = map.constBegin();
      while (i != map.constEnd()) {
	if (j++ == number.toUInt()) {
	  emit removeProxyConnection(i.value());
	  i = map.constEnd();

	} else ++i;
      }
    }
    return true;
  }
}


void EventHandler::echoMode(bool b) {
  QByteArray ba;
  ba += (unsigned char) 255; // IAC
  if (b) ba += (unsigned char) 251; // WILL
  else ba += (unsigned char) 252; // WON'T
  ba += (unsigned char) 1;   // OPT_ECHO
  emit sendToSocket(ba);
}


void EventHandler::displayMessage(const QString &message) {
  QVariant* qv = new QVariant(message);
  QStringList sl("DisplayData");
  postSession(qv, sl);
}
