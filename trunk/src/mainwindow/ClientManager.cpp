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

#include "ClientManager.h"

#include "MainWindow.h"
#include "ClientLineEdit.h"
#include "ClientTextEdit.h"
#include "PowwowWrapper.h"

ClientManager *ClientManager::_self = 0;

ClientManager *ClientManager::self(MainWindow *parent)
{
  if (_self == 0) {
    _self = new ClientManager(parent);
    qDebug("Client Manager created.");
  }
  return _self;
}

ClientManager::ClientManager(MainWindow *parent) {
  _mainWindow = parent;

  _lineEdit = new ClientLineEdit(parent);
  _textEdit = new ClientTextEdit(parent);
  _wrapper = PowwowWrapper::self();

  _textEdit->setFocusProxy(_lineEdit);

  // PowwowWrapper <--> MainWindow
  connect(_wrapper, SIGNAL(log(const QString&, const QString&)), parent, SLOT(log(const QString&, const QString&)) );

  // PowwowWrapper --> ClientTextEdit
  connect(_wrapper, SIGNAL(addText(const QString&)), _textEdit, SLOT(addText(const QString&)) );
  connect(_wrapper, SIGNAL(moveCursor(int)), _textEdit, SLOT(moveCursor(int)) );

  // PowwowWrapper <--> ClientLineEdit
  connect(_wrapper, SIGNAL(toggleEchoMode()), _lineEdit, SLOT(toggleEchoMode()) );
  connect(_wrapper, SIGNAL(inputInsertText(QString)), _lineEdit, SLOT(inputInsertText(QString)) );
  connect(_wrapper, SIGNAL(inputMoveTo(int)), _lineEdit, SLOT(inputMoveTo(int)) );
  connect(_wrapper, SIGNAL(inputDeleteChars(int)), _lineEdit, SLOT(inputDeleteChars(int)) );
  connect(_wrapper, SIGNAL(inputClear()), _lineEdit, SLOT(inputClear()) );
  connect(_lineEdit, SIGNAL(mergeInputWrapper(QString, int)), _wrapper, SLOT(mergeInputWrapper(QString, int)) );

  // PowwowWrapper --> MainWindow
  connect(_wrapper, SIGNAL(close()), parent, SLOT(close()) );
  connect(_wrapper, SIGNAL(setCurrentProfile(const QString&)), parent, SLOT(setCurrentProfile(const QString&)) );

  // ClientLineEdit --> ClientManager --> PowwowWrapper
  connect(_lineEdit, SIGNAL(returnPressed()), this, SLOT(sendUserInput()) );
  connect(_lineEdit, SIGNAL(keyPressed(const QString&)), this, SLOT(sendUserBind(const QString&)) ); 
}

ClientManager::~ClientManager() {
  _self = 0;
}

void ClientManager::sendUserInput()
{
  _lineEdit->selectAll();
  _wrapper->getUserInput(_lineEdit->selectedText());
}

void ClientManager::sendUserBind(const QString& input)
{
  _wrapper->getUserBind(input);
}
