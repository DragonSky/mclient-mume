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
#include "telnetfilter.h"
#include "mumexmlparser.h"
#include "mainwindow.h"
#include "CGroup.h"
#include "mmapper2pathmachine.h"
#include "prespammedpath.h"

#include "wrapper.h"
#include "wrapper_tcp.h"
#include "wrapper_process.h"
#include "wrapper_profile.h"
#include "wrapper_tty.h"
#include "wrapper_cmd.h"

#include "main.h"
#include "tty.h"
#include "tcp.h"

#include "utils.h" //eval?
#include "list.h"

#include "edit.h" //insert_char

Wrapper *Wrapper::_self = 0;

Wrapper *Wrapper::self()
{
  if (!_self) {
    if (mainWindow = 0)
      qDebug("no main window");
    _self = new Wrapper(mainWindow);
  }
  return _self;
}

Wrapper::Wrapper(QObject *parent) : QObject(parent) {
  /* Create the Delayed Label Timer */
  delayTimer = new QTimer;
  connect(delayTimer, SIGNAL(timeout()), this, SLOT(delayTimerExpired()) );

  /* Create the Telnet Filter */
  filter = new TelnetFilter(this);
  connect(this, SIGNAL(analyzeUserStream( const char*, int )), filter, SLOT(analyzeUserStream( const char*, int )));
  connect(this, SIGNAL(analyzeMudStream( const char*, int )), filter, SLOT(analyzeMudStream( const char*, int )));
  connect(filter, SIGNAL(sendToMud(const QByteArray&)), this, SLOT(sendToMud(const QByteArray&)));
  connect(filter, SIGNAL(sendToUser(const QByteArray&)), this, SLOT(sendToUser(const QByteArray&)));
}

Wrapper::~Wrapper()
{
  _self = 0;
}

/* Powwow Initialization Function */
void Wrapper::start(int argc, char** argv) {
  parserXml = new MumeXmlParser(mainWindow->getMapData(), this);
  qDebug("Created XML Parser");

  connect(filter, SIGNAL(parseNewMudInputXml(IncomingData&)), parserXml, SLOT(parseNewMudInput(IncomingData&)));
  connect(filter, SIGNAL(parseNewUserInputXml(IncomingData&)), parserXml, SLOT(parseNewUserInput(IncomingData&)));

  connect(parserXml, SIGNAL(sendToMud(const QByteArray&)), this, SLOT(sendToMud(const QByteArray&)));
  connect(parserXml, SIGNAL(sendToUser(const QByteArray&)), this, SLOT(sendToUser(const QByteArray&)));
  connect(parserXml, SIGNAL(setNormalMode()), filter, SLOT(setNormalMode()));

  connect(parserXml, SIGNAL(event(ParseEvent* )), mainWindow->getPathMachine(), SLOT(event(ParseEvent* )), Qt::QueuedConnection);
  connect(parserXml, SIGNAL(releaseAllPaths()), mainWindow->getPathMachine(), SLOT(releaseAllPaths()), Qt::QueuedConnection);
  connect(parserXml, SIGNAL(showPath(CommandQueue, bool)),mainWindow->getPrespammedPath(), SLOT(setPath(CommandQueue, bool)), Qt::QueuedConnection);

  //Group Manager Support
  connect(parserXml, SIGNAL(sendScoreLineEvent(QByteArray)), mainWindow->getGroupManager(), SLOT(parseScoreInformation(QByteArray)), Qt::QueuedConnection);
  connect(parserXml, SIGNAL(sendPromptLineEvent(QByteArray)), mainWindow->getGroupManager(), SLOT(parsePromptInformation(QByteArray)), Qt::QueuedConnection);
  connect(parserXml, SIGNAL(sendGroupTellEvent(QByteArray)), mainWindow->getGroupManager(), SLOT(sendGTell(QByteArray)), Qt::QueuedConnection);

  startPowwow(argc, argv);
  qDebug("Started Powwow!");
  //textView->viewDimensionsChanged();
  delayTimer->start(1000);
}

/* main.c */

/*
 * The select() mainloop has been replaced by QTimer and
 * getUserInput/getUserBind, and getRemoteInput in order to
 * allow portability between system types.
 */
void Wrapper::mainLoop() {
  int sleeptime = computeDelaySleeptime();

  now_updated = 0;
  exec_delays();

  tcp_flush();
  redrawEverything();
  prompt_reset_iac();

  delayTimer->start(sleeptime);
}

/*
 * Calculate the Delayed Label Sleeptime
 */
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

/*
 * Redraw the Prompt if Necessary
 */
void Wrapper::redrawEverything() {
  if (prompt_status == 1 && line_status == 0)
    line_status = 1;
  if (prompt_status == 1)
    draw_prompt();
  else if (prompt_status == -1) {
    promptzero();
    col0 = surely_isprompt = '\0';
  }
  if (line_status == 1) {  // InputBar needs to updated to the internal buffer
      //qDebug("updating inputbar: %s %d", edbuf, edlen);
      //emit inputClear();
      //emit inputInsertText(edbuf);
      //edlen = 0;
    line_status = 0;
  }
}

/*
 * A socket has gotten input, signal the main loop
 */
void Wrapper::getRemoteInput(int fd) {
  // Set the current connection
  tcp_fd = fd;

  char buffer[BUFSIZE + 2];        /* allow for a terminating \0 later */
  char *buf = buffer, *newline;
  int got, otcp_fd, i;

  if (CONN_LIST(tcp_fd).fragment) {
    if ((i = strlen(CONN_LIST(tcp_fd).fragment)) >= BUFSIZE-1) {
      i = 0;
      //common_clear(promptlen && !opt_compact);
      tty_printf("#error: ##%s : line too long, discarded\n", CONN_LIST(tcp_fd).id);
    } else {
      buf += i;
      memcpy(buffer, CONN_LIST(tcp_fd).fragment, i);
    }
    free(CONN_LIST(tcp_fd).fragment);
    CONN_LIST(tcp_fd).fragment = 0;
  }
  else i = 0;

  got = tcp_read(tcp_fd, buf, BUFSIZE - i);
  if (!got)
    return;

  buf[got]='\0';  /* Safe, there is space. Do it now not to forget it later */
  received += got;

  if (!(CONN_LIST(tcp_fd).flags & ACTIVE))
    return; /* process only active connections */

  got += (buf - buffer);
  buf = buffer;

  emit analyzeMudStream(buf, got);

  mainLoop();
}

void Wrapper::sendToUser(const QByteArray& ba) {
  if (linemode & LM_CHAR) {
    /* char-by-char mode: just display output, no fuss */
    clear_input_line(0);
    tty_puts(ba.data());
    return;
  }

  /* line-at-a-time mode: process input in a number of ways */
  if (tcp_fd == tcp_main_fd && promptlen) {
    process_first_fragment((char*)ba.data(), ba.size());
  } else {
      //common_clear(promptlen && !opt_compact);
  }

  if (ba.size() > 0)
    process_remote_input((char*)ba.data(), ba.size());

}

void Wrapper::sendToMud(const QByteArray& ba) {
  writeToSocket(tcp_main_fd, ba.data(), ba.size());
}

/*
 * Rewritten from the Powwow Sources. Takes only a string now
 * and doesn't handle key binds anymore.
 */
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
    memmove(edbuf, input.toAscii().data(), n);
    edlen = n;
    pos = n;
    edbuf[edlen] = '\0';
    last_edit_cmd = (function_any)enter_line;
    if (line_status == 0)
      tty_printf("%s", edbuf);
    enter_line("dummy");
  }

  mainLoop();
}

/*
 * Separated from getUserInput. TODO: Pass in the function instead since
 * we check if the keybind exists in InputBar?
 */
void Wrapper::getUserBind(QString input) {
  keynode *p;

  for (p = keydefs; (p && (p->seqlen < input.length() || 
       memcmp(input.toAscii().constData(), p->sequence, input.length()))) ;
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


/*
 * Object Editor Helper Function
 * TODO: Rewrite this in a more intelligent fashion/location?
 * The Object Editor cannot access these variables due to scope
 */
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

/* Powwow C Functions */

extern "C" {
  /* main.c */
  void mainloop() {} // TODO: Change the powwow source instead?

  /* other */
  void wrapper_debug(const char *format, ...);

  /* 
  * C Wrapper Call to Toggle the Input Echo
  */
  void wrapper_toggle_echo() { Wrapper::self()->toggleEcho(); }

  /* utils.c */

  /*
  * Called when Powwow Quits (#quit, syserr)
  */
  void wrapper_quit() { Wrapper::self()->wrapperQuit(); }
}
