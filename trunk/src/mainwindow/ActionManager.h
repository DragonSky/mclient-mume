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
    static ActionManager *self (MainWindow *parent = 0);
    ~ActionManager ();

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

    QAction *profileAct;
    QAction *objectAct;
    QAction *settingsAct;

    QAction *connectAct;
    QAction *disconnectAct;
    QAction *reconnectAct;

    QAction *alwaysOnTopAct;

    /* Group Actions */
    QAction *groupOffAct;
    QAction *groupClientAct;
    QAction *groupServerAct;
    QAction *groupShowHideAct;
    QAction *groupSettingsAct;

    /* Mapper Actions */
    QAction *newAct;
    QAction *openAct;
    QAction *mergeAct;
    QAction *reloadAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *prevWindowAct;
    QAction *nextWindowAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;

    QAction *layerUpAct;
    QAction *layerDownAct;

    QAction *modeConnectionSelectAct;
    QAction *modeRoomSelectAct;
    QAction *modeMoveSelectAct;
    QAction *modeInfoMarkEditAct;

    QAction *createRoomAct;
    QAction *createConnectionAct;
    QAction *createOnewayConnectionAct;

    QAction *playModeAct;
    QAction *mapModeAct;
    QAction *offlineModeAct;

    QActionGroup *mapModeActGroup;
    QActionGroup *modeActGroup;
    QActionGroup *roomActGroup;
    QActionGroup *connectionActGroup;
    QActionGroup *groupManagerGroup;

    QAction *editRoomSelectionAct;
    QAction *editConnectionSelectionAct;
    QAction *deleteRoomSelectionAct;
    QAction *deleteConnectionSelectionAct;

    QAction *moveUpRoomSelectionAct;
    QAction *moveDownRoomSelectionAct;
    QAction *mergeUpRoomSelectionAct;
    QAction *mergeDownRoomSelectionAct;
    QAction *connectToNeighboursRoomSelectionAct;

    QAction *findRoomsAct;

    QAction *forceRoomAct;
    QAction *releaseAllPathsAct;

    void disableActions(bool value);

  public slots:
    void about();
    void clientHelp();
    void alwaysOnTop();

    void groupManagerTypeChanged(int);

  private:
    ActionManager (MainWindow *);
    static ActionManager *_self;

    void createActions ();
    void createGroupActions ();
    void createMapperActions ();

    MainWindow *_mainWindow;
};

#endif /* _ACTIONMANAGER_H_ */
