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

#include "PowwowWrapper.h"

#include "ClientTextEdit.h"
#include "telnetfilter.h"
#include "mumexmlparser.h"
#include "mainwindow.h"
#include "CGroup.h"
#include "mmapper2pathmachine.h"
#include "prespammedpath.h"

#include "KeyBinder.h"
#include "wrapper_cmd.h"
#include "wrapper_process.h"
#include "wrapper_profile.h"
#include "wrapper_tcp.h"
#include "wrapper_tty.h"
#include "ClientSocket.h"
#include "InternalEditor.h"
#include "ClientProcess.h"

#include <QTimer>

#include "main.h"
#include "tty.h"
#include "tcp.h"

#include "utils.h" //eval?
#include "list.h"

#include "edit.h" //insert_char

#include "beam.h" //edit_sess,finish_edit, edit_end

#include "MapperManager.h"
#include "ClientManager.h"
#include "GroupManager.h"
#include "mapdata.h"

PowwowWrapper *PowwowWrapper::_self = 0;

PowwowWrapper *PowwowWrapper::self()
{
  if (_self == 0) {
    _self = new PowwowWrapper(0); //MainWindow::self());
    qDebug() << "PowwowWrapper created.";
  }
  return _self;
}

PowwowWrapper::PowwowWrapper(QObject *parent) : QObject(parent) {
  /* Create the Delayed Label Timer */
  delayTimer = new QTimer;
  connect(delayTimer, SIGNAL(timeout()), this, SLOT(delayTimerExpired()) );

  /* Create the Telnet Filter */
  filter = new TelnetFilter(this);
  connect(this, SIGNAL(analyzeUserStream( const char*, const int& )), 
          filter, SLOT(analyzeUserStream( const char*, int )));
  connect(this, SIGNAL(analyzeMudStream( const char*, const int& )), 
          filter, SLOT(analyzeMudStream( const char*, int )));
  connect(filter, SIGNAL(sendToMud(const QByteArray&)), 
          this, SLOT(sendToMud(const QByteArray&)));
  connect(filter, SIGNAL(sendToUser(const QByteArray&)), 
          this, SLOT(sendToUser(const QByteArray&)));
}

PowwowWrapper::~PowwowWrapper() {
  _self = 0;
}

/* Powwow Initialization Function */
void PowwowWrapper::start(int argc, char** argv) {
  parserXml = new MumeXmlParser(MapperManager::self()->getMapData(), this);
  qDebug("Created XML Parser");

  connect(filter, SIGNAL(parseNewMudInputXml(IncomingData&)), parserXml, SLOT(parseNewMudInput(IncomingData&)));
  connect(filter, SIGNAL(parseNewUserInputXml(IncomingData&)), parserXml, SLOT(parseNewUserInput(IncomingData&)));

  connect(parserXml, SIGNAL(sendToMud(const QByteArray&)), this, SLOT(sendToMud(const QByteArray&)));
  connect(parserXml, SIGNAL(sendToUser(const QByteArray&)), this, SLOT(sendToUser(const QByteArray&)));
  connect(parserXml, SIGNAL(setNormalMode()), filter, SLOT(setNormalMode()));

  connect(parserXml, SIGNAL(event(ParseEvent* )), MapperManager::self()->getPathMachine(), SLOT(event(ParseEvent* )), Qt::QueuedConnection);
  connect(parserXml, SIGNAL(releaseAllPaths()), MapperManager::self()->getPathMachine(), SLOT(releaseAllPaths()), Qt::QueuedConnection);
  connect(parserXml, SIGNAL(showPath(CommandQueue, bool)), MapperManager::self()->getPrespammedPath(), SLOT(setPath(CommandQueue, bool)), Qt::QueuedConnection);

  //Group Manager Support
  connect(parserXml, SIGNAL(sendScoreLineEvent(QByteArray)), GroupManager::self()->getGroup(), SLOT(parseScoreInformation(QByteArray)), Qt::QueuedConnection);
  connect(parserXml, SIGNAL(sendPromptLineEvent(QByteArray)), GroupManager::self()->getGroup(), SLOT(parsePromptInformation(QByteArray)), Qt::QueuedConnection);
  connect(parserXml, SIGNAL(sendGroupTellEvent(QByteArray)), GroupManager::self()->getGroup(), SLOT(sendGTell(QByteArray)), Qt::QueuedConnection);

  startPowwow(argc, argv);
  qDebug("Started Powwow!");
  ClientManager::self()->getTextEdit()->viewDimensionsChanged();
  delayTimer->start(1000);
}

/* main.c */

/*
 * The select() mainloop has been replaced by QTimer and
 * getUserInput/getUserBind, and getRemoteInput in order to
 * allow portability between system types.
 */
void PowwowWrapper::mainLoop() const {
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
const int PowwowWrapper::computeDelaySleeptime() const {
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
void PowwowWrapper::redrawEverything() const {
  if (prompt_status == 1 && line_status == 0)
    line_status = 1;
  if (prompt_status == 1)
    draw_prompt();
  else if (prompt_status == -1) {
    promptzero();
    col0 = surely_isprompt = '\0';
  }
  if (line_status == 1) {  // ClientLineEdit needs to updated to the internal buffer
      //qDebug("updating clientlineedit: %s %d", edbuf, edlen);
      //emit inputClear();
      //emit inputInsertText(edbuf);
      //edlen = 0;
    line_status = 0;
  }
}

/*
 * A socket has gotten input, signal the main loop
 */
void PowwowWrapper::getRemoteInput(const int& fd) {
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

void PowwowWrapper::sendToUser(const QByteArray& ba) const {
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

void PowwowWrapper::sendToMud(const QByteArray& ba) const {
  writeToSocket(tcp_main_fd, ba.data(), ba.size());
}

/*
 * Rewritten from the Powwow Sources. Takes only a string now
 * and doesn't handle key binds anymore.
 */
void PowwowWrapper::getUserInput(const QString& input) {
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
 * we check if the keybind exists in ClientLineEdit?
 */
void PowwowWrapper::getUserBind(const QString& input) {
  keynode *p;

  p = keydefs; 
  while(p && (p->seqlen < input.length() || 
       memcmp(input.toAscii().constData(), p->sequence, input.length()))) {
      p = p->next;
  }

  if (!p) {
    // GH: type the first character and keep processing the rest in the 
    // input buffer
    last_edit_cmd = static_cast<function_any>(0);
    return;
  }
  else if (p->seqlen == input.length()) {
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
const  actionnode* PowwowWrapper::getActions() const { 
    return actions; 
}


QHash<QString, QString> PowwowWrapper::getNUMVARs(const int& type) {
  QHash<QString, QString> hash;
  int i;
  ptr p = static_cast<ptr>(0);
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


void PowwowWrapper::writeToStdout(const QString& str) const { 
    emit addText(str); // to ClientTextEdit
} 


void PowwowWrapper::toggleEcho() const { 
    emit toggleEchoMode(); 
}
// --------------------------------------------------------------------------


/* Powwow C Functions */

extern "C" {
  /* main.c */
  void mainloop() {} // TODO: Change the powwow source instead?

  /* other */
  void wrapper_debug(const char *format, ...);

  /* 
  * C PowwowWrapper Call to Toggle the Input Echo
  */
  void wrapper_toggle_echo() { PowwowWrapper::self()->toggleEcho(); }

  /* utils.c */

  /*
  * Called when Powwow Quits (#quit, syserr)
  */
  void wrapper_quit() { PowwowWrapper::self()->wrapperQuit(); }
}
// --------------------------------------------------------------------------


// From wrapper_tcp.cpp    
void PowwowWrapper::disconnectSession() const {
  if (socketHash[tcp_fd])
    socketHash[tcp_fd]->socketDisconnected();
  emit setCurrentProfile(QString());
}


const int PowwowWrapper::readFromSocket(const int& fd, char* const buffer, 
        const int& maxsize) const {
  ClientSocket* socket = socketHash[fd];
  int read;
  while(socket->bytesAvailable()) {
    read = socket->read(buffer, maxsize);
    if (read != -1)
      buffer[read] = 0;
  }
  return read;
}

const int PowwowWrapper::writeToSocket(const int& fd, const char* data, 
        const int& len) const {
  return socketHash[fd]->write(data, len);
}


// This socket needs to be managed, so it doesn't leak!  
// How about a vector of them?
void PowwowWrapper::createSocket(char *addr, int port, char *initstr, int i) {
  ClientSocket *socket = new ClientSocket(initstr, i, this);

  status(1);
  tty_printf("#trying %s... ", addr);

  socket->connectToHost(QString(addr), port, QIODevice::ReadWrite);
}


void PowwowWrapper::createSocketContinued(ClientSocket *socket, bool connected) {

  int newtcp_fd;
  if (connected) {
    newtcp_fd = socket->socketDescriptor();
    socketHash[newtcp_fd] = socket;
  } else {
    newtcp_fd = -1;
  }
  //send_command(newtcp_fd, C_DUMB, 1, 0);   // necessary?
  wrapper_tcp_connect_slot(socket->initstring, socket->peerPort(), 
          socket->i, newtcp_fd);
}
// --------------------------------------------------------------------------


// From wrapper_tty.cpp
void PowwowWrapper::emitInputSet(const char* const str) {
  emit inputClear();
  emit inputInsertText(str);
}

void PowwowWrapper::emitMoveCursor(const int fromcol, const int fromline, 
        const int tocol, const int toline) const {
  int diff = tocol - fromcol;
  qDebug("External: (%d, %d), (%d, %d), Diff %d", fromcol, fromline, tocol, toline, diff);
  emit moveCursor(diff);
}

void PowwowWrapper::mergeInputWrapper(const QString& inputBarText, const int& cursorPosition) {
  strcpy(edbuf, inputBarText.toAscii().data());
  edlen = inputBarText.length();
  edbuf[edlen] = '\0';
  pos = cursorPosition;
}
// --------------------------------------------------------------------------


// From wrapper_cmd.cpp
const int PowwowWrapper::getKeyBind(char* const seq) const {
  QString label("Blank"), sequence;
  KeyBinder *dlg = new KeyBinder(label, sequence, 0);
  if (dlg->exec()) {
    strcpy(seq, sequence.toAscii().constData());
  }
  delete dlg;
  return strlen(seq);
}

// --------------------------------------------------------------------------
// From wrapper_process.cpp

int PowwowWrapper::createProcess(char *arg, bool displayStdout)
{
  QString program(arg);
  if (program.trimmed().isEmpty())
    return -1;

  ClientProcess *process = new ClientProcess(this);

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

void PowwowWrapper::killProcess(int pid)
{
  detectFinishedBeam(pid);
  qDebug("killed %d", pid);
  if (m_processes[pid])
    m_processes[pid]->~ClientProcess();
  m_processes.remove(pid);
}
void PowwowWrapper::detectFinishedBeam(int pid)
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
int PowwowWrapper::internalEditor(editsess* s) {
  qDebug("Running with internal editor");

  InternalEditor *editor = new InternalEditor(&(*s), this, (QWidget*)parent);
  editor->show();
  editor->activateWindow();
  editorHash[s] = editor;

  return -1;
}


void PowwowWrapper::finishedInternalEditor(editsess *s)
{
  qDebug("done with internal editor");
  finishBeamEdit(&s);
  if (editorHash[s]) editorHash[s]->~InternalEditor();
  editorHash.remove(s);
}

void PowwowWrapper::finishBeamEdit(editsess **sp)
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