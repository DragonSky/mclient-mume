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

class ClientSocket;
class ClientProcess;
class InternalEditor;
class TelnetFilter;
class MumeXmlParser;

/* This is the interacting parent object */
class PowwowWrapper: public QObject {
  Q_OBJECT

  public:
    static PowwowWrapper *self();
    ~PowwowWrapper();

    void start(int, char**); // from MainWindow

    // for Object Editor
    const actionnode* getActions() const;
    QHash<QString, QString> getNUMVARs(const int&);

    /* main.h */
    void getUserInput(const QString& input);
    void getUserBind(const QString& input);

    /* tty.h */
    void getRemoteInput(const int& fd);
    void writeToStdout(const QString& str) const;
    void toggleEcho() const;
    void emitMoveCursor(const int fromcol, const int fromline, 
        const int tocol, const int toline) const;
    void emitInputInsertText(const QString& str) { emit inputInsertText(str); }
    void emitInputMoveTo(const int& new_pos) { emit inputMoveTo(new_pos); }
    void emitInputDeleteChars(const int& n)  { emit inputDeleteChars(n); }
    void emitInputSet(const char* const str);

    /* tcp.h */
    void createSocket(char *addr, int port, char *initstr, int i);
    void createSocketContinued(ClientSocket *socket, bool connected);
    const int readFromSocket(const int& fd, char* const buffer, 
            const int& maxsize) const;
    const int writeToSocket(const int& fd, const char* data, 
            const int& len) const;

    /* beam.h, cmd.h */
    int internalEditor(editsess*);
    void killProcess(int);
    int createProcess(char *arg, bool displayStdout = false);
    void detectFinishedBeam(int pid);
    void finishBeamEdit(editsess **sp);
    void finishedInternalEditor(editsess *s);
    QHash<int, ClientProcess*> m_processes;
    QHash<int, ClientSocket*> socketHash;

    const int getKeyBind(char* const) const;

    /* other */
    void wrapperQuit() { emit close(); }
    void loadProfile(const QString &profile);


  signals:
    void addText(const QString&) const;  // to ClientTextEdit
    void moveCursor(const int&) const;
    void toggleEchoMode() const;   // To ClientLineEdit
    void inputInsertText(const QString&);
    void inputMoveTo(int);
    void inputDeleteChars(int);
    void inputClear() const;
    void close();            // to MainWindow
    void setCurrentProfile(const QString&) const;

    // to TelnetFilter
    void analyzeUserStream( const char*, const int& ) const;
    void analyzeMudStream( const char*, const int& ) const;

  private slots:
    void delayTimerExpired() { mainLoop(); } // timer for delayed labels (exec_delays)
    //void reconnectProfile(QString &profile);

  public slots:
    void mergeInputWrapper(const QString&, const int&); // from ClientLineEdit
    void connectSession();
    void disconnectSession() const;
    void clearPowwowMemory();
    void sendToMud(const QByteArray&) const;
    void sendToUser(const QByteArray&) const;

  private:
    PowwowWrapper(QObject *);
    static PowwowWrapper *_self;

    void mainLoop() const;
    void redrawEverything() const;
    const int computeDelaySleeptime() const;

    QHash<editsess*, InternalEditor*> editorHash;

    TelnetFilter *filter;
    MumeXmlParser *parserXml;
    QTimer *delayTimer;
    QObject *parent;         // MainWindow
};

#endif /* __cplusplus */

#endif /* _WRAPPER_H_ */
