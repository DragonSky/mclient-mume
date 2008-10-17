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
 
#ifndef _WRAPPER_PROCESS_H_
#define _WRAPPER_PROCESS_H_

#include <QProcess>
#include "defines.h"

class PowwowWrapper;

/* cmd.c */
extern "C" void cmd_shell(char *arg);

/* beam.c */
extern "C" int wrapper_kill_process(int);
extern "C" int wrapper_create_child(char *, editsess *);
extern "C" void wrapper_generate_tmpfile(char *, uint, int, int);
extern "C" void sig_chld_bottomhalf(); // replaced by something else

#endif /* _WRAPPER_PROCESS_H_ */
