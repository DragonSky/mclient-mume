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
 
#ifndef WrapperProcess_H
#define WrapperProcess_H

#include <QProcess>

class Wrapper;

/* cmd.c */
extern "C" void cmd_shell(char *arg);

/* beam.c */
extern "C" int wrapper_kill_process(int);
extern "C" int wrapper_create_child(char *);
extern "C" void wrapper_generate_tmpfile(char *, uint, int, int);
extern "C" void sig_chld_bottomhalf(); // replaced by something else

/* wrapper.cpp */

class WrapperProcess: public QProcess
{
  Q_OBJECT

  public:
    WrapperProcess(Wrapper *parent);
    ~WrapperProcess();

    int m_pid;

  public slots:
    void finishedProcess(int, QProcess::ExitStatus);
    void readyReadStandardOutput();

  private:
    Wrapper *wrapper;
};

#endif
