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
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef MainWindow_H
#define MainWindow_H

#include <QtGui>

#ifdef MMAPPER
class MapWindow;
class MainWindowCtrl;
class Mmapper2PathMachine;
class CommandEvaluator;
class PrespammedPath;
class MapData;
class RoomSelection;
class ConnectionSelection;
class RoomPropertySetter;
class FindRoomsDlg;
class CGroup;
class CGroupCommunicator;
#endif

class QAction;
class QMenu;
class InputBar;
class TextView;
class Wrapper;

class ObjectEditor;
class ConfigDialog;
class ProfileDialog;
class ProfileManagerDialog;

class MainWindow:public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow();
    ~MainWindow();

    void start(int argc, char **argv);

#ifdef MMAPPER
    MapWindow *getCurrentMapWindow() { return mapWindow; }
    Mmapper2PathMachine *getPathMachine(){return m_pathMachine;}
    CGroup *getGroupManager() {return m_groupManager;}
    MapData *getMapData() {return m_mapData;}
    PrespammedPath *getPrespammedPath() { return m_prespammedPath; }
#endif

  protected:
    void closeEvent(QCloseEvent *event);

  public slots:
    void log(const QString&, const QString&);

  private slots:
    void about();
    void clientHelp();
    void alwaysOnTop();

    void selectProfile();
    void editObjects();
    void manageProfiles();
    void changeConfiguration();

    void sendUserInput();
    void sendUserBind(const QString&);
    void profileSelected();
    void setCurrentProfile(const QString &profile);

#ifdef MMAPPER
    void newFile();
    void open();
    void reload();
    void merge();
    bool save();
    bool saveAs();

    void setCurrentFile(const QString &fileName);

    void percentageChanged(quint32);

    void currentMapWindowChanged();

    void onModeConnectionSelect();
    void onModeRoomSelect();
    void onModeMoveSelect();
    void onModeInfoMarkEdit();
    void onModeCreateRoomSelect();
    void onModeCreateConnectionSelect();
    void onModeCreateOnewayConnectionSelect();
    void onLayerUp();
    void onLayerDown();
    void onEditRoomSelection();
    void onEditConnectionSelection();
    void onDeleteRoomSelection();
    void onDeleteConnectionSelection();
    void onMoveUpRoomSelection();
    void onMoveDownRoomSelection();
    void onMergeUpRoomSelection();
    void onMergeDownRoomSelection();
    void onConnectToNeighboursRoomSelection();
    void onFindRoom();
    void onPlayMode();
    void onMapMode();
    void onOfflineMode();

    void newRoomSelection(const RoomSelection*);
    void newConnectionSelection(ConnectionSelection*);

    void groupOff(bool);
    void groupClient(bool);
    void groupServer(bool);
    void groupManagerTypeChanged(int);
#endif

  private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    void startMapper();

    QTextEdit *logWindow;
    QDockWidget *dockDialogLog;
    ProfileManagerDialog *profileManager;
    ProfileDialog *profileDialog;
    ObjectEditor *objectEditor;
    Wrapper *wrapper;
    TextView *textView;
    InputBar *inputBar;
    QString currentProfile;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *viewDockMenu;
    QMenu *viewToolbarMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *settingsMenu;

    QToolBar *editToolBar;
    QToolBar *connectToolBar;

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

#ifdef MMAPPER
    QDockWidget *dockDialogMapper;
    QDockWidget *m_dockDialogGroup;

    Mmapper2PathMachine *m_pathMachine;
    MapData *m_mapData;
    RoomPropertySetter * m_propertySetter;
    CommandEvaluator *m_commandEvaluator;
    PrespammedPath *m_prespammedPath;

    FindRoomsDlg *m_findRoomsDlg;
    CGroup *m_groupManager;
    CGroupCommunicator *m_groupCommunicator;

    const RoomSelection* m_roomSelection;
    ConnectionSelection* m_connectionSelection;
    
    QAction               *groupOffAct;
    QAction               *groupClientAct;
    QAction               *groupServerAct;
    QAction               *groupShowHideAct;
    QAction               *groupSettingsAct;

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
    QAction *preferencesAct;

    QMenu *mapperMenu;
    QMenu *roomMenu;
    QMenu *connectionMenu;
    QMenu *searchMenu;
    QMenu *groupMenu;

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
    bool maybeSaveMap();

    MapWindow *mapWindow;
    QProgressDialog *progressDlg;
#endif
};

extern class MainWindow *mainWindow;

#endif
