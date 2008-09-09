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

#ifndef _WRAPPER_TCP_H_
#define _WRAPPER_TCP_H_

#include <QTcpSocket>

class Wrapper;

extern "C" void wrapper_tcp_connect(char *addr, int port, char *initstring, int i);
extern "C" void wrapper_tcp_connect_slot(char *initstr, int port, int newtcp_fd, int i);

extern "C" void tcp_spawn(char *id, char *cmd);
//extern "C" int tcp_read(int fd, char *buffer, int maxsize);
//extern "C" void tcp_write(int fd, char *data);
extern "C" int wrapper_tcp_read(int fd, char *buffer, int maxsize);
extern "C" int wrapper_tcp_write(int fd, char *data, int len);
extern "C" void wrapper_tcp_assign_id(int fd, char *id);
extern "C" void wrapper_tcp_close_socket(int fd);

class WrapperSocket: public QTcpSocket
{
  Q_OBJECT

  public:
    WrapperSocket(char*, int, Wrapper *parent);
    ~WrapperSocket();

    int i;
    char *id;
    char *initstring;

  public slots:
    void contentAvailable();
    void socketConnected();
    void socketDisconnected();
    void socketError(QAbstractSocket::SocketError socketError);

  private:
    Wrapper *wrapper;
};

#endif /* _WRAPPER_TCP_H_ */
