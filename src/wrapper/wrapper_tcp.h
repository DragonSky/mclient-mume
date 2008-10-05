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

/* Powwow C Functions */
#ifdef __cplusplus
extern "C" {
#endif
  // Creating a Socket:
  void wrapper_tcp_connect(char *addr, int port, char *initstring, int i);
  void wrapper_tcp_connect_slot(char *initstr, int port, int newtcp_fd, int i);
  void wrapper_tcp_assign_id(int fd, char *id);

  // Closing a Socket:
  void wrapper_tcp_close_socket(int fd);

  // Reading/Writing to a Socket:
  int wrapper_tcp_read(int fd, char *buffer, int maxsize);
  int wrapper_tcp_write(int fd, char *data, int len);

  // Deprecated:
  void tcp_spawn(char *id, char *cmd);
#ifdef __cplusplus
}
#endif

#endif /* _WRAPPER_TCP_H_ */
