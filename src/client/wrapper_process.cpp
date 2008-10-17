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

#include <QDir>

#include "configuration.h"

#include "PowwowWrapper.h"
#include "wrapper_process.h"

#include "defines.h"
#include "beam.h"
#include "tcp.h"
#include "main.h"

#include <cstdlib>

/* beam.c */

void cmd_shell(char *arg) { PowwowWrapper::self()->createProcess(arg, true); }
int wrapper_kill_process(int pid) { PowwowWrapper::self()->killProcess(pid); return pid; }
int wrapper_create_child(char *args, editsess* s) {
  if (false && Config().useInternalEditor) // HACK: ignore internal editor for now
  {
    return PowwowWrapper::self()->internalEditor(s);
  }
  else
  {
    return PowwowWrapper::self()->createProcess(args);
  }
}

void sig_chld_bottomhalf() {} // replaced

void wrapper_generate_tmpfile(char *tmpname, uint key, int pid, int rand)
{
  QString path = QDir::tempPath();
  if (path.right(1) != QDir::separator())
    path += QDir::separator();
  sprintf(tmpname, "%spowwow.%u.%d%d", path.toAscii().constData(), key, pid, rand);
}


