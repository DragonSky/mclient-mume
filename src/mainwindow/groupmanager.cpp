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

#include "groupmanager.h"

#include "CGroup.h"
#include "configuration.h"
#include "mainwindow.h"

#include "mappermanager.h"
#include "mmapper2pathmachine.h"
#include "mapwindow.h"
#include "mapcanvas.h"

GroupManager *GroupManager::_self = 0;

GroupManager *GroupManager::self(MainWindow *parent)
{
  if (_self == 0) {
    _self = new GroupManager(parent);
    qDebug("Group Manager created.");
  }
  return _self;
}

GroupManager::GroupManager(MainWindow *parent) {
  _mainWindow = parent;

  _groupManager = new CGroup(Config().m_groupManagerCharName, MapperManager::self()->getMapData(), parent);

  connect(_groupManager, SIGNAL(log(const QString&, const QString&)), parent, SLOT(log(const QString&, const QString&)));
  connect(MapperManager::self()->getPathMachine(), SIGNAL(setCharPosition(uint)),_groupManager, SLOT(setCharPosition(uint)));
  connect(_groupManager, SIGNAL(drawCharacters()), MapperManager::self()->getMapWindow()->getCanvas(), SLOT(update()));
}

GroupManager::~GroupManager() {
  _self = 0;
}

CGroupCommunicator *GroupManager::getCommunicator() {
  return _groupManager->getCommunicator();
}

void GroupManager::groupOff(bool b)
{
  if (b) _groupManager->setType(CGroupCommunicator::Off);
}


void GroupManager::groupClient(bool b)
{
  if (b) _groupManager->setType(CGroupCommunicator::Client);
}


void GroupManager::groupServer(bool b)
{
  if (b) _groupManager->setType(CGroupCommunicator::Server);
}
