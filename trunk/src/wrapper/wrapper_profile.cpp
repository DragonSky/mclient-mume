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
#include <QTextStream>
#include <QDebug>
#include <cstdlib>

#include "wrapper.h"
#include "wrapper_profile.h"
#include "wrapper_tty.h"

#include "cprofilemanager.h"
#include "cprofilesettings.h"

#include "main.h"
#include "tcp.h"
#include "utils.h"
#include "cmd2.h" // parsing powwow commands
#include "cmd.h" // wrapper_cmd_var

// As of this moment only one profile can be loaded at once
#define CURRENT_SESSION 0

void Wrapper::loadProfile(const QString &profile) {
  cProfileManager *mgr = cProfileManager::self();
  cProfileSettings *sett = mgr->settings (profile);
  if (!sett) return;   // no profile selected

  QString name = (mgr->visibleProfileName (profile));
  emit setCurrentProfile(name);
  QString definitions = sett->getString ("definitions");
  QString hostname = sett->getString ("server");

  int read_file = 0;

  set_deffile(definitions.toAscii().data());

  QFile f (definitions);
  if (!f.open(QIODevice::ReadWrite | QIODevice::Text)) {
    char portnum[INTLEN];
    tty_printf("#creating %s\n", deffile);
    limit_mem = 1048576;
    if (save_settings() < 0)
      exit(0);
  } else if (read_settings() < 0)
      exit(0);
    else
       read_file = 1;

  if (!definitions.isEmpty()) {
    mgr->assignSession(CURRENT_SESSION, profile);
    connectSession();
  }

  if (read_file && !hostname.isEmpty() && *initstr) {
    parse_instruction(initstr, 0, 0, 1);
    history_done = 0;
  }

  confirm = 0;
}

void Wrapper::connectSession() {
  cProfileManager *mgr = cProfileManager::self();
  cProfileSettings *sett = mgr->settings(mgr->profileName(CURRENT_SESSION));

  if (!sett) return;   // no profile selected

  QString hostname = sett->getString ("server");
  int port = sett->getInt ("port");

  if (port > 0 && !hostname.isEmpty())
    tcp_open("main", (*initstr ? initstr : NULL), hostname.toAscii().data(), port);
}

void Wrapper::clearPowwowMemory() {
  QString command;
  int i, type;
  ptr p = (ptr)0;

  // Delete All Aliases
  aliasnode *alias = sortedaliases;
  while (alias) {
    QTextStream(&command) << alias->name << "=";
    //qDebug("#alias %s", command.toAscii().data() );
    alias = alias->snext;
    parse_alias(command.toAscii().data());
    command.clear();
  }

  // Delete All Actions
  actionnode *action = actions;
  while (action) {
    QTextStream(&command) << "<" << action->label;
    //qDebug() << "#action " << command;
    action = action->next;
    parse_action(command.toAscii().data(), 0);
    command.clear();
  }

  // Delete All Variables
  varnode *v;
  for (type = 0; type <= 1; type++) {
    v = sortednamed_vars[type];
    while (v) {
      QTextStream(&command) << (type ? "$" : "@") << v->name << "=";
      //qDebug() << "#var " << command;
      v = v->snext;
      wrapper_cmd_var(command.toAscii().data());
      command.clear();
    }
  }
  for (i = -NUMVAR; i < NUMPARAM; i++) {
    if (*VAR[i].num) {
      QTextStream(&command) << "@" << i << "=";
      //qDebug() << "#var " << command;
      wrapper_cmd_var(command.toAscii().data());
      command.clear();
    }
  }
  for (i = -NUMVAR; i < NUMPARAM; i++) {
    if (*VAR[i].str && ptrlen(*VAR[i].str)) {
      p = ptrescape(p, *VAR[i].str, 0);
      if (p && ptrlen(p)) {
        QTextStream(&command) << "$" << i << "=";
        //qDebug() << "#var " << command;
        wrapper_cmd_var(command.toAscii().data());
        command.clear();
      }
    }
  }

  // Delete All Marks
  marknode *mark = markers;
  while (mark) {
    QTextStream(&command) << (mark->mbeg ? "^" : "") << mark->pattern << "=";
    qDebug() << "#mark " << command;
    mark = mark->next;
    parse_mark(command.toAscii().data());
    command.clear();
  }

  // Delete All Binds
  keynode *bind = keydefs;
  while (bind) {
    QTextStream(&command) << bind->name << "=";
    qDebug() << "#bind " << command;
    bind = bind->next;
    parse_bind(command.toAscii().data());
    command.clear();
  }

}
