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

#ifndef _ACTIONMANAGER_H_
#define _ACTIONMANAGER_H_

#include <QMainWindow>

class MainWindow;
class QActionGroup;

class ActionManager: public QObject {
  Q_OBJECT

  public:
    static ActionManager* instance(MainWindow *parent = 0);
    void destroy();

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

public slots:
    void about();
    void clientHelp();
    void alwaysOnTop();

    void connectSession();
    void disconnectSession();
    void reconnectSession();

 protected:
    ActionManager(MainWindow*);
    ~ActionManager();
    
    static ActionManager* _pinstance;

  private:
    void disableActions(bool value);
    void postEvent(QVariant *payload, const QStringList& tags);

    MainWindow *_mainWindow;

    /* MainWindow (i.e. Normal) Actions */
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *clientHelpAct;
    QAction *mumeHelpAct;
    QAction *wikiAct;
    QAction *forumAct;

    QAction *aboutAct;
    QAction *aboutQtAct;

    QAction *settingsAct;

    QAction *connectAct;
    QAction *disconnectAct;
    QAction *reconnectAct;

    QAction *alwaysOnTopAct;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *settingsMenu;

    QToolBar *editToolBar;
    QToolBar *connectToolBar;
};

#endif /* _ACTIONMANAGER_H_ */
