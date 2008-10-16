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

#include "clientmanager.h"

#include "mainwindow.h"
#include "inputbar.h"
#include "textview.h"
#include "wrapper.h"

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

  _inputBar = new InputBar(parent);
  _textView = new TextView(_inputBar, parent);
  _wrapper = Wrapper::self();

  // Wrapper --> TextView
  connect(_wrapper, SIGNAL(addText(const QString&)), _textView, SLOT(addText(const QString&)) );
  connect(_wrapper, SIGNAL(moveCursor(int)), _textView, SLOT(moveCursor(int)) );

  // Wrapper <--> InputBar
  connect(_wrapper, SIGNAL(toggleEchoMode()), _inputBar, SLOT(toggleEchoMode()) );
  connect(_wrapper, SIGNAL(inputInsertText(QString)), _inputBar, SLOT(inputInsertText(QString)) );
  connect(_wrapper, SIGNAL(inputMoveTo(int)), _inputBar, SLOT(inputMoveTo(int)) );
  connect(_wrapper, SIGNAL(inputDeleteChars(int)), _inputBar, SLOT(inputDeleteChars(int)) );
  connect(_wrapper, SIGNAL(inputClear()), _inputBar, SLOT(inputClear()) );
  connect(_inputBar, SIGNAL(mergeInputWrapper(QString, int)), _wrapper, SLOT(mergeInputWrapper(QString, int)) );

  // Wrapper --> MainWindow
  connect(_wrapper, SIGNAL(close()), parent, SLOT(close()) );
  connect(_wrapper, SIGNAL(setCurrentProfile(const QString&)), parent, SLOT(setCurrentProfile(const QString&)) );

  // InputBar --> ClientManager --> Wrapper
  connect(_inputBar, SIGNAL(returnPressed()), this, SLOT(sendUserInput()) );
  connect(_inputBar, SIGNAL(keyPressed(const QString&)), this, SLOT(sendUserBind(const QString&)) ); 
}

ClientManager::~ClientManager() {
  _self = 0;
}

void ClientManager::sendUserInput()
{
  _inputBar->selectAll();
  _wrapper->getUserInput(_inputBar->selectedText());
}

void ClientManager::sendUserBind(const QString& input)
{
  _wrapper->getUserBind(input);
}
