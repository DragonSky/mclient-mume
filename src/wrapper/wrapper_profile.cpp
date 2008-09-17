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

#include <QFile>

#include "wrapper.h"
#include "wrapper_profile.h"
#include "wrapper_tty.h"

#include "cprofilemanager.h"
#include "cprofilesettings.h"

#include "main.h"
#include "tcp.h"
#include "utils.h"

void Wrapper::loadProfile(QString &profile) {
  cProfileManager *mgr = cProfileManager::self();
  cProfileSettings *sett = mgr->settings (profile);
  if (!sett) return;   // no profile selected

  QString name = (mgr->visibleProfileName (profile));
  emit setCurrentProfile(name);
  QString definitions = sett->getString ("definitions");
  QString server = sett->getString ("server");
  int port = sett->getInt ("port");
  int read_file = 0;

  set_deffile(definitions.toAscii().data());

  if (access(deffile,R_OK) == -1 || access(deffile,W_OK) == -1) {
    char portnum[INTLEN];
    tty_printf("#creating %s\n", deffile);
    limit_mem = 1048576;
    if (save_settings() < 0)
      exit(0);
  } else if (read_settings() < 0)
      exit(0);
    else
       read_file = 1;

  if (!definitions.isEmpty() && port > 0) {
    /* assume last two args are hostname and port number */
    my_strncpy(hostname, server.toAscii().data(), BUFSIZE-1);
    portnumber = port;
  }
  
  if (*hostname)
    tcp_open("main", (*initstr ? initstr : NULL), hostname, portnumber);

  if (read_file && !*hostname && *initstr) {
    parse_instruction(initstr, 0, 0, 1);
    history_done = 0;
  }

  confirm = 0;
}
