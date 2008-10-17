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

#ifndef _CLIENTMANAGER_H_
#define _CLIENTMANAGER_H_

#include <QObject>

class MainWindow;
class ClientTextEdit;
class ClientLineEdit;
class PowwowWrapper;

class ClientManager: public QObject {
  Q_OBJECT

  public:
    static ClientManager *self (MainWindow *parent = 0);
    ~ClientManager ();

    PowwowWrapper *getPowwowWrapper() { return _wrapper; }
    ClientTextEdit *getTextEdit() { return _textEdit; }
    ClientLineEdit *getLineEdit() { return _lineEdit; }

  private slots:
    void sendUserInput();
    void sendUserBind(const QString& input);

  private:
    ClientManager (MainWindow *);
    static ClientManager *_self;

    PowwowWrapper *_wrapper;
    ClientTextEdit *_textEdit;
    ClientLineEdit *_lineEdit;
    MainWindow *_mainWindow;
};

#endif /* _CLIENTMANAGER_H_ */