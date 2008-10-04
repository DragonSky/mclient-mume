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

#ifndef _WRAPPER_H_
#define _WRAPPER_H_

/*   This file is loaded by Powwow C and the mClient C++
 *   As such we need to make sure its cross-compatible.
 */

#ifdef __cplusplus

#include <QObject>
#include <QHash>
#include <QTimer>

#include "defines.h"

class Wrapper;
class WrapperSocket;
class WrapperProcess;
class InternalEditor;
class TelnetFilter;
class MumeXmlParser;

/* This is the interacting parent object */
class Wrapper: public QObject {
  Q_OBJECT

  public:
    static Wrapper *self ();
    ~Wrapper();

    void start(int, char**); // from MainWindow

    // for Object Editor
    actionnode* getActions();
    QHash<QString, QString> getNUMVARs(int);

    /* main.h */
    void getUserInput(QString input);
    void getUserBind(QString input);

    /* tty.h */
    void getRemoteInput(int fd);
    void writeToStdout(QString str) { emit addText(str); } // to TextView
    void toggleEcho() { emit toggleEchoMode(); }
    void emitMoveCursor(int fromcol, int fromline, int tocol, int toline);
    void emitInputInsertText(QString str) { emit inputInsertText(str); }
    void emitInputMoveTo(int new_pos) { emit inputMoveTo(new_pos); }
    void emitInputDeleteChars(int n) { emit inputDeleteChars(n); }
    void emitInputSet(char *str);

    /* tcp.h */
    void createSocket(char *addr, int port, char *initstr, int i);
    void createSocketContinued(WrapperSocket *socket, bool connected);
    int readFromSocket(int fd, char *buffer, int maxsize);
    int writeToSocket(int fd, const char *data, int len);

    /* beam.h, cmd.h */
    int internalEditor(editsess*);
    void killProcess(int);
    int createProcess(char *arg, bool displayStdout);
    void detectFinishedBeam(int pid);
    void finishBeamEdit(editsess **sp);
    void finishedInternalEditor(editsess *s);
    QHash<int, WrapperProcess*> m_processes;
    QHash<int, WrapperSocket*> socketHash;

    int getKeyBind(char *);

    /* other */
    void wrapperQuit() { emit close(); }
    void loadProfile(const QString &profile);


  signals:
    void addText(QString&);  // to TextView
    void moveCursor(int);
    void toggleEchoMode();   // To InputBar
    void inputInsertText(QString);
    void inputMoveTo(int);
    void inputDeleteChars(int);
    void inputClear();
    void close();            // to MainWindow
    void setCurrentProfile(const QString&);
    
    // to TelnetFilter
    void analyzeUserStream( const char*, int );
    void analyzeMudStream( const char*, int );

  private slots:
    void delayTimerExpired() { mainLoop(); } // timer for delayed labels (exec_delays)
    //void reconnectProfile(QString &profile);

  public slots:
    void mergeInputWrapper(QString, int); // from InputBar
    void connectSession();
    void disconnectSession();
    void clearPowwowMemory();
    void sendToMud(const QByteArray&);
    void sendToUser(const QByteArray&);

  private:
    Wrapper(QObject *);
    static Wrapper *_self;

    void mainLoop();
    void redrawEverything();
    int computeDelaySleeptime();

    //TextView *textView;
    //InputBar *inputBar;

    QHash<editsess*, InternalEditor*> editorHash;

    TelnetFilter *filter;
    MumeXmlParser *parserXml;
    QTimer *delayTimer;
    QObject *parent;         // MainWindow
};

#else

/* Interacting parent object as seen in C/Powwow */
   typedef
       struct Wrapper
       Wrapper;
#endif

#endif /* _WRAPPER_H_ */
