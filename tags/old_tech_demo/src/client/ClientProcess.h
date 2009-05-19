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
 
#ifndef _CLIENTPROCESS_H_
#define _CLIENTPROCESS_H_

#include <QProcess>
#include "defines.h"

class PowwowWrapper;

class ClientProcess: public QProcess
{
  Q_OBJECT

  public:
    ClientProcess(PowwowWrapper *parent);
    ~ClientProcess();

    int m_pid;

  public slots:
    void finishedProcess(int, QProcess::ExitStatus);
    void readyReadStandardOutput();

  private:
    PowwowWrapper *wrapper;
};

#endif /* _CLIENTPROCESS_H_ */
