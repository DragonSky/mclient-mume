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

#ifndef _GROUPMANAGER_H_
#define _GROUPMANAGER_H_

#include <QObject>

class CGroup;
class CGroupCommunicator;
class MainWindow;

class GroupManager: public QObject {
  Q_OBJECT

  public:
    static GroupManager *self (MainWindow *parent = 0);
    ~GroupManager ();

    CGroup *getGroup() { return _groupManager; }
    CGroupCommunicator *getCommunicator();

  public slots:
    void groupOff(bool);
    void groupClient(bool);
    void groupServer(bool);

  private:
    GroupManager (MainWindow *);
    static GroupManager *_self;

    CGroup *_groupManager;
    MainWindow *_mainWindow;
};

#endif /* _GROUPMANAGER_H_ */
