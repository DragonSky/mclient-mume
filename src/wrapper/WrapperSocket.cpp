#include "WrapperSocket.h"

#include "wrapper.h"
#include "wrapper_tcp.h"
#include "wrapper_tty.h"

#include <QAbstractSocket>

#include "main.h"
#include "tcp.h"


/*
 * WrapperSocket contains various variables that are necessary to allow
 * a connecting socket to attempt to connect without blocking.
 */
WrapperSocket::WrapperSocket(char* initstring, int i, Wrapper *wrapper)
  : QTcpSocket(), initstring(initstring), i(i), wrapper(wrapper) {

  connect(this, SIGNAL(connected()), SLOT(socketConnected()) );
  connect(this, SIGNAL(disconnected()), SLOT(socketDisconnected()) );
  connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
          SLOT(socketError(QAbstractSocket::SocketError)) );

  connect(this, SIGNAL(readyRead()), SLOT(contentAvailable()) );
}

WrapperSocket::~WrapperSocket() {}

void WrapperSocket::socketError(QAbstractSocket::SocketError socketError) {
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
    Wrapper::self()->createSocketContinued(this, FALSE);
  }
  qDebug("socket error: %s", errorString().toAscii().data());
}


void WrapperSocket::socketConnected() {
  tty_printf("connected!\n");
  Wrapper::self()->createSocketContinued(this, TRUE);
}

void WrapperSocket::contentAvailable() {
  Wrapper::self()->getRemoteInput(socketDescriptor());
}

void WrapperSocket::socketDisconnected() {
  tcp_close(id);
}
