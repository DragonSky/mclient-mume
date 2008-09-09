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

#include <QTimer>

#include "textview.h"

#include "wrapper.h"
#include "wrapper_tcp.h"
#include "wrapper_process.h"
#include "wrapper_tty.h"
#include "wrapper_cmd.h"

#include "main.h"
#include "tty.h"
#include "tcp.h"

#include "utils.h" //eval?
#include "list.h"

#include "edit.h" //insert_char

/* Powwow C Functions */
/* main.c */
extern "C" void mainloop() {}

/* utils.c */

/* other */
extern "C" void wrapper_debug(const char *format, ...);
extern "C" void wrapper_toggle_echo() { wrapper->toggleEcho(); };

void Wrapper::toggleEcho() { emit toggleEchoMode(); }

/* utils.c */
extern "C" void wrapper_quit() { wrapper->wrapperQuit(); };

void suspend_powwow() { wrapper->writeToStdout("Error, QtPowwow not suspendable."); }

/* main.c
 * The select() mainloop has been replaced by QTimer and
 * Wrapper::signalStdin, ::getRemoteInput in order to aid portability
 * between systems.
 */
void Wrapper::mainLoop() {
  int sleeptime = computeDelaySleeptime();

  now_updated = 0;
  exec_delays();

  tcp_flush();
  redraw_everything();
  prompt_reset_iac();

  delayTimer->start(sleeptime);
}

int Wrapper::computeDelaySleeptime() {
  int sleeptime = 0;

  if (delays) {
    update_now();
    sleeptime = diff_vtime(&delays->when, &now);
    if (!sleeptime)
      sleeptime = 1;    /* if sleeptime is less than 1 millisec,
                        * set to 1 millisec */
  }

  if (!sleeptime)
    return 10 * mSEC_PER_SEC;

  return sleeptime;
}

/* A socket has gotten input, signal the main loop */
void Wrapper::getRemoteInput(int fd) {
  tcp_fd = fd;
  get_remote_input();
  mainLoop();
}

actionnode* Wrapper::getActions() { return actions; }

QHash<QString, QString> Wrapper::getNUMVARs(int type) {
  QHash<QString, QString> hash;
  int i;
  ptr p = (ptr)0;
  if (type == 0) {
    for (i = -NUMVAR; i < NUMPARAM; i++) {
      if (*VAR[i].num)
        hash[QString("%1").arg(i)] = QString("%1").arg(*VAR[i].num);
    }
  }
  else
  {
    for (i = -NUMVAR; i < NUMPARAM; i++) {
      if (*VAR[i].str && ptrlen(*VAR[i].str)) {
        p = ptrescape(p, *VAR[i].str, 0);
        if (p && ptrlen(p))
          hash[QString("%1").arg(i)] = QString("%1").arg(ptrdata(p));
      }
    }
  }
  ptrdel(p);
  return hash;
}

void Wrapper::getUserBind(QString input) {
  keynode *p;

  for (p = keydefs; (p && (p->seqlen < input.length() || 
       memcmp(input.toAscii().constData(), p->sequence, input.length())));
       p = p->next);

  if (!p) {
    // GH: type the first character and keep processing the rest in the input buffer
    last_edit_cmd = (function_any)0;
    return;
  }
  else if (p->seqlen == input.length()){
    p->funct(p->call_data);
    last_edit_cmd = (function_any)p->funct; // GH: keep track of last command
  }

  mainLoop();
}

void Wrapper::getUserInput(QString input) {
  //qDebug("'%s' (%s)", input.toAscii().data(), edbuf);
  int i, n = input.length();
  confirm = 0;

    /* We have 4 possible line modes:
  * line mode, local echo: line editing functions in effect
  * line mode, no echo: sometimes used for passwords, no line editing
  * char mode, no echo: send a character directly, no local processing
  * char mode, local echo: extremely rare, do as above.
    */

  if (linemode & LM_CHAR) {
    /* char mode. */
    for (i = 0; i < n; i++) {
      wrapper_tcp_write(tcp_fd, input.mid(i,1).toAscii().data(), 1);
      if (!(linemode & LM_NOECHO))
        tty_putc(input.at(i).toAscii() );
      last_edit_cmd = (function_any)0;
    }

  } else if (linemode & LM_NOECHO) {
    /* sending password (line mode, no echo). */
    tty_putc('\n');
    tcp_write(tcp_fd, input.toAscii().data());
    last_edit_cmd = (function_any)0;
  } else {
    /* normal mode (line mode, echo). */
    if (n >= BUFSIZE) n = BUFSIZE - 1;
    memmove(edbuf, input.toAscii().constData(), n);
    edlen = n;
    pos = n;
    edbuf[edlen] = '\0';
    last_edit_cmd = (function_any)enter_line;
    if (line_status == 0)
      tty_printf("%s", input.toAscii().constData());
    enter_line("dummy");
  }

  mainLoop();
}

/* Other */

/* Wrapper C++ Functions */

/* Wrapper Functions */

/* Objects */

WrapperThreader::WrapperThreader(Wrapper* wrapper):
    wrapper(wrapper)
{
  qDebug("WrapperThreader Created");
}

WrapperThreader::~WrapperThreader()
{
  if (wrapper)
    delete wrapper;
}

void WrapperThreader::run() {
  try {
    qDebug("Powwow Started");
    exec();
  } catch (char const * error) {
    throw error;
  }
}

void Wrapper::start() {
  thread->start();
  startPowwow(this, argc, argv);
  if (argc > 2) {
    tcp_open("main", (*initstr ? initstr : NULL), argv[argc - 2], atoi(argv[argc - 1]));
  }
  delayTimer->start(1000);
}


Wrapper::Wrapper(int c, char **v, InputBar *ib, TextView *tv, QObject *parent): QObject(NULL)
{
  m_parent = parent;
  textView = tv;
  inputBar = ib;
  argc = c;
  argv = v;
  thread = new WrapperThreader(this);

  qDebug("Wrapper Started");

  delayTimer = new QTimer(this);
  connect(delayTimer, SIGNAL(timeout()), this, SLOT(delayTimerExpired()) );
}

void Wrapper::delayTimerExpired() {
  mainLoop();
}

Wrapper::~Wrapper() {}
