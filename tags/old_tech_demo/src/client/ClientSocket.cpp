#include "ClientSocket.h"

#include "PowwowWrapper.h"
#include "wrapper_tcp.h"
#include "wrapper_tty.h"

#include <QAbstractSocket>

#include "main.h"
#include "tcp.h"


/*
 * ClientSocket contains various variables that are necessary to allow
 * a connecting socket to attempt to connect without blocking.
 */
ClientSocket::ClientSocket(char* initstring, int i, PowwowWrapper *wrapper)
  : QTcpSocket(), initstring(initstring), i(i), wrapper(wrapper) {

  connect(this, SIGNAL(connected()), SLOT(socketConnected()) );
  connect(this, SIGNAL(disconnected()), SLOT(socketDisconnected()) );
  connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
          SLOT(socketError(QAbstractSocket::SocketError)) );

  connect(this, SIGNAL(readyRead()), SLOT(contentAvailable()) );
}

ClientSocket::~ClientSocket() {}

void ClientSocket::socketError(QAbstractSocket::SocketError socketError) {
  if (state() == QAbstractSocket::UnconnectedState) { // Only provide messages during #connect
    switch (socketError) {
      case QAbstractSocket::RemoteHostClosedError:
        tty_printf("remote host closed the connection!\n");
        break;
      case QAbstractSocket::HostNotFoundError:
        tty_printf("unknown host!\n");
        break;
      case QAbstractSocket::ConnectionRefusedError:
        tty_printf("connection was refused by the host!\n");
        break;
      default:
        tty_printf("error: %s\n", errorString().toAscii().data());
    }
    PowwowWrapper::self()->createSocketContinued(this, FALSE);
  }
  qDebug("socket error: %s", errorString().toAscii().data());
}


void ClientSocket::socketConnected() {
  tty_printf("connected!\n");
  PowwowWrapper::self()->createSocketContinued(this, TRUE);
}

void ClientSocket::contentAvailable() {
  PowwowWrapper::self()->getRemoteInput(socketDescriptor());
}

void ClientSocket::socketDisconnected() {
  tcp_close(id);
}
