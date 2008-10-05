/***************************************************************************
 *   Copyright (C) 2008 by Nils Schimmelmann   *
 *   nschimme@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 ***************************************************************************/

#include <QtGui>

#include "wrapper.h"
#include "wrapper_tty.h"
#include "wrapper_tcp.h"
#include "WrapperSocket.h"

#include <QAbstractSocket>

#include "main.h"
#include "tcp.h"



int Wrapper::writeToSocket(int fd, const char *data, int len)
{
  return socketHash[fd]->write(data, len);
}

void Wrapper::createSocket(char *addr, int port, char *initstr, int i) {
  WrapperSocket *socket = new WrapperSocket(initstr, i, this);

  status(1);
  tty_printf("#trying %s... ", addr);

  socket->connectToHost(QString(addr), port, QIODevice::ReadWrite);
}

void Wrapper::createSocketContinued(WrapperSocket *socket, bool connected)
{
  int newtcp_fd;
  if (connected) {
    newtcp_fd = socket->socketDescriptor();
    socketHash[newtcp_fd] = socket;
  } else {
    newtcp_fd = -1;
  }
  //send_command(newtcp_fd, C_DUMB, 1, 0);   // necessary?
  wrapper_tcp_connect_slot(socket->initstring, socket->peerPort(), socket->i, newtcp_fd);
}

/*
 * C Functions from Powwow
 */
int wrapper_tcp_read(int fd, char *buffer, int maxsize) { return Wrapper::self()->readFromSocket(fd, buffer, maxsize); }
int wrapper_tcp_write(int fd, char *data, int len) { return Wrapper::self()->writeToSocket(fd, data, len); }
void wrapper_tcp_assign_id(int fd, char *id) {  Wrapper::self()->socketHash[fd]->id = id; }
void wrapper_tcp_connect(char *host, int port, char *initstring, int i) { Wrapper::self()->createSocket(host, port, initstring, i); }
void tcp_spawn(char *id, char *cmd) { Wrapper::self()->writeToStdout(QString("todo")); } // TODO

void wrapper_tcp_close_socket(int fd)
{
  Wrapper::self()->socketHash[fd]->deleteLater();
  Wrapper::self()->socketHash.remove(fd);
}


