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
#include "internaleditor.h"

#include "wrapper.h"
#include "wrapper_process.h"

#include "defines.h"
#include "beam.h"
#include "tcp.h"
#include "main.h"

#include <cstdlib>

/* beam.c */

void cmd_shell(char *arg) { Wrapper::self()->createProcess(arg, true); }

int Wrapper::createProcess(char *arg, bool displayStdout = false)
{
  QString program(arg);
  if (program.trimmed().isEmpty())
    return -1;

  WrapperProcess *process = new WrapperProcess(this);

  connect( process, SIGNAL(finished(int, QProcess::ExitStatus)),
           process, SLOT(finishedProcess(int, QProcess::ExitStatus)) );
  if (displayStdout)
    connect( process, SIGNAL(readyReadStandardOutput()),
             process, SLOT(readyReadStandardOutput()) );
  
  //process->setEnvironment(systemEnvironment());
  process->setReadChannelMode(QProcess::MergedChannels);

  process->start(program, QIODevice::ReadOnly);
  int pid = (int)process->pid();
  qDebug("creating process %d: %s", pid, arg);

  if (process->exitStatus() < 0)
    return -1;

  process->m_pid = pid;
  m_processes[pid] = process;

  return pid;
}

void Wrapper::killProcess(int pid)
{
  detectFinishedBeam(pid);
  qDebug("killed %d", pid);
  if (m_processes[pid])
    m_processes[pid]->~WrapperProcess();
  m_processes.remove(pid);
}



/* beam.c */

int wrapper_kill_process(int pid) { Wrapper::self()->killProcess(pid); return pid; }
int wrapper_create_child(char *args, editsess* s) {
  if (Config().useInternalEditor)
  {
    return Wrapper::self()->internalEditor(s);
  }
  else
  {
    return Wrapper::self()->createProcess(args);
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

void Wrapper::detectFinishedBeam(int pid)
{
  editsess **sp;

  /* GH: check for WIFSTOPPED unnecessary since no */
  /* WUNTRACED to waitpid()                        */
  for (sp = &edit_sess; *sp && (*sp)->pid != pid; sp = &(*sp)->next);

  if (*sp) {
    finishBeamEdit(sp);
  }
}

/* Internal Editor */
int Wrapper::internalEditor(editsess* s) {
  qDebug("Running with internal editor");

  InternalEditor *editor = new InternalEditor(&(*s), this, (QWidget*)parent);
  editor->show();
  editor->activateWindow();
  editorHash[s] = editor;

  return -1;
}


void Wrapper::finishedInternalEditor(editsess *s)
{
  qDebug("done with internal editor");
  finishBeamEdit(&s);
  if (editorHash[s]) editorHash[s]->~InternalEditor();
  editorHash.remove(s);
}

void Wrapper::finishBeamEdit(editsess **sp)
{
  int fd, ret;
  editsess *p;

  finish_edit(*sp);
  p = *sp; *sp = p->next;
  fd = p->fd;
  free(p->descr);
  free(p->file);
  free(p);

  /* GH: only send message if found matching session */

  /* send the edit_end message if this is the last editor... */ 
  if ((!edit_sess) && (*edit_end)) {
    int otcp_fd = tcp_fd;  /* backup current socket fd */
    tcp_fd = fd;
    parse_instruction(edit_end, 0, 0, 1);
    history_done = 0;
    tcp_fd = otcp_fd;
  }
}

/* Wrapper Process Object */

void WrapperProcess::readyReadStandardOutput() {
  Wrapper::self()->writeToStdout(QString(readAllStandardOutput()));
}

void WrapperProcess::finishedProcess(int exitCode, QProcess::ExitStatus status) {
  Wrapper::self()->killProcess(m_pid);
}

WrapperProcess::WrapperProcess(Wrapper *parent)
{
  wrapper = parent;
}

WrapperProcess::~WrapperProcess() {}
