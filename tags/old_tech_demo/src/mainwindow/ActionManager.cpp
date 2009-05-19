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

#include "ActionManager.h"

#include "mainwindow.h"
#include "configuration.h"
#include "ClientManager.h"
#include "MapperManager.h"
#include "GroupManager.h"
#include "PowwowWrapper.h"

#include "mapwindow.h"
#include "mapcanvas.h"
#include "ClientTextEdit.h"
#include "ClientLineEdit.h"
#include "mmapper2pathmachine.h"
#include "pathmachine.h"
#include "CGroup.h"

ActionManager *ActionManager::_self = 0;

ActionManager *ActionManager::self(MainWindow *parent)
{
  if (_self == 0) {
    _self = new ActionManager(parent);
    qDebug("Action Manager created.");
  }
  return _self;
}

ActionManager::ActionManager(MainWindow *parent) {
  _mainWindow = parent;
  createActions();
  createGroupActions();
  createMapperActions();

  // Group Manager Type --> ActionManager
  connect(GroupManager::self()->getGroup()->getCommunicator(), SIGNAL(typeChanged(int)),
          SLOT(groupManagerTypeChanged(int)), Qt::QueuedConnection);

  // Load Configuration Settings
  groupManagerTypeChanged( Config().m_groupManagerState );
  if (Config().m_mapMode == 0)
  {
    playModeAct->setChecked(true);
    MapperManager::self()->onPlayMode();
  }
  else
    if (Config().m_mapMode == 1)
  {
    mapModeAct->setChecked(true);
    MapperManager::self()->onMapMode();
  }
  else
    if (Config().m_mapMode == 2)
  {
    offlineModeAct->setChecked(true);
    MapperManager::self()->onOfflineMode();
  }
}

ActionManager::~ActionManager() {
  _self = 0;
}

void ActionManager::createActions() {
  ClientManager *cliMgr = ClientManager::self();

  connectAct = new QAction(QIcon(":/crystal/connect.png"), tr("&Connect..."), this);
  connectAct->setStatusTip(tr("Load a new session and connect to the remote host"));
  connect(connectAct, SIGNAL(triggered()), _mainWindow, SLOT(selectProfile() ));

  disconnectAct = new QAction(QIcon(":/crystal/disconnect.png"), tr("&Disconnect"), this);
  disconnectAct->setStatusTip(tr("Disconnect from the current session"));
  connect(disconnectAct, SIGNAL(triggered()), PowwowWrapper::self(), SLOT(disconnectSession()));

  reconnectAct = new QAction(QIcon(":/crystal/reconnect.png"), tr("&Reconnect"), this);
  reconnectAct->setStatusTip(tr("Reconnect to the current session's remote host"));
  connect(reconnectAct, SIGNAL(triggered()), PowwowWrapper::self(), SLOT(connectSession()) );

  exitAct = new QAction(QIcon(":/crystal/exit.png"), tr("E&xit"), this);
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct, SIGNAL(triggered()), _mainWindow, SLOT(close()));

  cutAct = new QAction(QIcon(":/editcut.png"), tr("Cu&t"), this);
  cutAct->setShortcut(tr("Ctrl+X"));
  cutAct->setStatusTip(tr("Cut the current selection's contents to the "
      "clipboard"));
  connect(cutAct, SIGNAL(triggered()), cliMgr->getTextEdit(), SLOT(cut()));

  copyAct = new QAction(QIcon(":/editcopy.png"), tr("&Copy"), this);
  copyAct->setShortcut(tr("Ctrl+C"));
  copyAct->setStatusTip(tr("Copy the current selection's contents to the "
      "clipboard"));
  connect(copyAct, SIGNAL(triggered()), cliMgr->getTextEdit(), SLOT(copy()));

  pasteAct = new QAction(QIcon(":/editpaste.png"), tr("&Paste"), this);
  pasteAct->setShortcut(tr("Ctrl+V"));
  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
      "selection"));
  connect(pasteAct, SIGNAL(triggered()), cliMgr->getLineEdit(), SLOT(paste()));

  alwaysOnTopAct = new QAction(tr("&Always on Top"), this);
  alwaysOnTopAct->setCheckable(true);
  alwaysOnTopAct->setStatusTip(tr("Toggle the window to always stay on the top"));
  connect(alwaysOnTopAct, SIGNAL(triggered()), this, SLOT(alwaysOnTop()));

  objectAct = new QAction(tr("&Object Editor..."), this);
  objectAct->setStatusTip(tr("Edit Powwow objects such as aliases and actions"));
  connect(objectAct, SIGNAL(triggered()), _mainWindow, SLOT(editObjects()));
  
  profileAct = new QAction(QIcon(":/crystal/profile.png"), tr("Profile &Manager..."), this);
  profileAct->setStatusTip(tr("Manage mClient profile settings"));
  connect(profileAct, SIGNAL(triggered()), _mainWindow, SLOT(manageProfiles()));

  settingsAct = new QAction(QIcon(":/crystal/settings.png"), tr("&Preferences..."), this);
  settingsAct->setStatusTip(tr("Change mClient settings"));
  connect(settingsAct, SIGNAL(triggered()), _mainWindow, SLOT(changeConfiguration()) );

  mumeHelpAct = new QAction(tr("M&ume"), this);
  mumeHelpAct->setStatusTip(tr("MUME Website"));

  wikiAct = new QAction(tr("&Wiki"), this);
  wikiAct->setStatusTip(tr("Visit the MUME Wiki"));

  forumAct = new QAction(tr("&Forum"), this);
  forumAct->setStatusTip(tr("Visit the MUME Forum"));

  clientHelpAct = new QAction(QIcon(":/crystal/help.png"), tr("mClient &Help"), this);
  clientHelpAct->setStatusTip(tr("View the mClient/Powwow help file"));
  connect(clientHelpAct, SIGNAL(triggered()), this, SLOT(clientHelp()));

  aboutAct = new QAction(QIcon(":/icons/m.png"), tr("About &mClient"), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  aboutQtAct = new QAction(QIcon(":/crystal/qt.png"), tr("About &Qt"), this);
  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  cutAct->setEnabled(false);
  copyAct->setEnabled(false);
  connect(ClientManager::self()->getTextEdit(), SIGNAL(copyAvailable(bool)),
          cutAct, SLOT(setEnabled(bool)));
  connect(ClientManager::self()->getTextEdit(), SIGNAL(copyAvailable(bool)),
          copyAct, SLOT(setEnabled(bool)));
}

void ActionManager::createGroupActions() {
  GroupManager *grpMgr = GroupManager::self();

  groupOffAct = new QAction(QIcon(":/icons/groupoff.png"), tr("Switch to &Off mode"), this );
  groupOffAct->setShortcut(tr("Ctrl+G"));
  groupOffAct->setCheckable(true);
  connect(groupOffAct, SIGNAL(toggled(bool)), grpMgr, SLOT(groupOff(bool)), Qt::QueuedConnection);

  groupClientAct = new QAction(QIcon(":/icons/groupclient.png"),tr("Switch to &Client mode"), this );
  groupClientAct->setCheckable(true);
  connect(groupClientAct, SIGNAL(toggled(bool)), grpMgr, SLOT(groupClient(bool)), Qt::QueuedConnection);

  groupServerAct = new QAction(QIcon(":/icons/groupserver.png"),tr("Switch to &Server mode"), this );
  groupServerAct->setCheckable(true);
  connect(groupServerAct, SIGNAL(toggled(bool)), grpMgr, SLOT(groupServer(bool)), Qt::QueuedConnection);

  groupManagerGroup = new QActionGroup(this);
  groupManagerGroup->addAction(groupOffAct);
  groupManagerGroup->addAction(groupClientAct);
  groupManagerGroup->addAction(groupServerAct);
}

void ActionManager::createMapperActions() {
  MapperManager *mapMgr = MapperManager::self();
  newAct = new QAction(QIcon(":/icons/new.png"), tr("&New"), this);
  newAct->setShortcut(tr("Ctrl+N"));
  newAct->setStatusTip(tr("Create a new file"));
  connect(newAct, SIGNAL(triggered()), mapMgr, SLOT(newFile()));

  openAct = new QAction(QIcon(":/icons/open.png"), tr("&Open..."), this);
  openAct->setShortcut(tr("Ctrl+O"));
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, SIGNAL(triggered()), mapMgr, SLOT(open()));

  reloadAct = new QAction(QIcon(":/icons/reload.png"), tr("&Reload"), this);
  reloadAct->setShortcut(tr("Ctrl+R"));
  reloadAct->setStatusTip(tr("Reload the current map"));
  connect(reloadAct, SIGNAL(triggered()), mapMgr, SLOT(reload()));

  saveAct = new QAction(QIcon(":/icons/save.png"), tr("&Save"), this);
  saveAct->setShortcut(tr("Ctrl+S"));
  saveAct->setStatusTip(tr("Save the document to disk"));
  connect(saveAct, SIGNAL(triggered()), mapMgr, SLOT(save()));

  saveAsAct = new QAction(tr("Save &As..."), this);
  saveAsAct->setStatusTip(tr("Save the document under a new name"));
  connect(saveAsAct, SIGNAL(triggered()), mapMgr, SLOT(saveAs()));

  mergeAct = new QAction(QIcon(":/icons/merge.png"), tr("&Merge..."), this);
  //mergeAct->setShortcut(tr("Ctrl+M"));
  mergeAct->setStatusTip(tr("Merge an existing file into current map"));
  connect(mergeAct, SIGNAL(triggered()), mapMgr, SLOT(merge()));

  zoomInAct = new QAction(QIcon(":/icons/viewmag+.png"), tr("Zoom In"), this);

  zoomInAct->setStatusTip(tr("Zooms In current map"));
  zoomOutAct = new QAction(QIcon(":/icons/viewmag-.png"), tr("Zoom Out"), this);
  zoomOutAct->setStatusTip(tr("Zooms Out current map"));

  layerUpAct = new QAction(QIcon(":/icons/layerup.png"), tr("Layer Up"), this);
  layerUpAct->setStatusTip(tr("Layer Up"));
  connect(layerUpAct, SIGNAL(triggered()), mapMgr, SLOT(onLayerUp()));
  layerDownAct = new QAction(QIcon(":/icons/layerdown.png"), tr("Layer Down"), this);
  layerDownAct->setStatusTip(tr("Layer Down"));
  connect(layerDownAct, SIGNAL(triggered()), mapMgr, SLOT(onLayerDown()));

  modeConnectionSelectAct = new QAction(QIcon(":/icons/connectionselection.png"), tr("Select Connection"), this);
  modeConnectionSelectAct->setStatusTip(tr("Select Connection"));
  modeConnectionSelectAct->setCheckable(true);
  connect(modeConnectionSelectAct, SIGNAL(triggered()), mapMgr, SLOT(onModeConnectionSelect()));
  modeRoomSelectAct = new QAction(QIcon(":/icons/roomselection.png"), tr("Select Rooms"), this);
  modeRoomSelectAct->setStatusTip(tr("Select Rooms"));
  modeRoomSelectAct->setCheckable(true);
  connect(modeRoomSelectAct, SIGNAL(triggered()), mapMgr, SLOT(onModeRoomSelect()));
  modeMoveSelectAct = new QAction(QIcon(":/icons/mapmove.png"), tr("Move map"), this);
  modeMoveSelectAct->setStatusTip(tr("Move Map"));
  modeMoveSelectAct->setCheckable(true);
  connect(modeMoveSelectAct, SIGNAL(triggered()), mapMgr, SLOT(onModeMoveSelect()));
  modeInfoMarkEditAct = new QAction(QIcon(":/icons/infomarksedit.png"), tr("Edit Info Marks"), this);
  modeInfoMarkEditAct->setStatusTip(tr("Edit Info Marks"));
  modeInfoMarkEditAct->setCheckable(true);
  connect(modeInfoMarkEditAct, SIGNAL(triggered()), mapMgr, SLOT(onModeInfoMarkEdit()));

  createRoomAct = new QAction(QIcon(":/icons/roomcreate.png"), tr("Create New Rooms"), this);
  createRoomAct->setStatusTip(tr("Create New Rooms"));
  createRoomAct->setCheckable(true);
  connect(createRoomAct, SIGNAL(triggered()), mapMgr, SLOT(onModeCreateRoomSelect()));

  createConnectionAct = new QAction(QIcon(":/icons/connectioncreate.png"), tr("Create New Connection"), this);
  createConnectionAct->setStatusTip(tr("Create New Connection"));
  createConnectionAct->setCheckable(true);
  connect(createConnectionAct, SIGNAL(triggered()), mapMgr, SLOT(onModeCreateConnectionSelect()));

  createOnewayConnectionAct = new QAction(QIcon(":/icons/onewayconnectioncreate.png"), tr("Create New Oneway Connection"), this);
  createOnewayConnectionAct->setStatusTip(tr("Create New Oneway Connection"));
  createOnewayConnectionAct->setCheckable(true);
  connect(createOnewayConnectionAct, SIGNAL(triggered()), mapMgr, SLOT(onModeCreateOnewayConnectionSelect()));

  modeActGroup = new QActionGroup(this);
  modeActGroup->setExclusive(true);
  modeActGroup->addAction(modeMoveSelectAct);
  modeActGroup->addAction(modeRoomSelectAct);
  modeActGroup->addAction(modeConnectionSelectAct);
  modeActGroup->addAction(createRoomAct);
  modeActGroup->addAction(createConnectionAct);
  modeActGroup->addAction(createOnewayConnectionAct);
  modeActGroup->addAction(modeInfoMarkEditAct);
  modeMoveSelectAct->setChecked(true);

  editRoomSelectionAct = new QAction(QIcon(":/icons/roomedit.png"), tr("Edit Selected Rooms"), this);
  editRoomSelectionAct->setStatusTip(tr("Edit Selected Rooms"));
  connect(editRoomSelectionAct, SIGNAL(triggered()), mapMgr, SLOT(onEditRoomSelection()));

  deleteRoomSelectionAct = new QAction(QIcon(":/icons/roomdelete.png"), tr("Delete Selected Rooms"), this);
  deleteRoomSelectionAct->setStatusTip(tr("Delete Selected Rooms"));
  connect(deleteRoomSelectionAct, SIGNAL(triggered()), mapMgr, SLOT(onDeleteRoomSelection()));

  moveUpRoomSelectionAct = new QAction(QIcon(":/icons/roommoveup.png"), tr("Move Up Selected Rooms"), this);
  moveUpRoomSelectionAct->setStatusTip(tr("Move Up Selected Rooms"));
  connect(moveUpRoomSelectionAct, SIGNAL(triggered()), mapMgr, SLOT(onMoveUpRoomSelection()));
  moveDownRoomSelectionAct = new QAction(QIcon(":/icons/roommovedown.png"), tr("Move Down Selected Rooms"), this);
  moveDownRoomSelectionAct->setStatusTip(tr("Move Down Selected Rooms"));
  connect(moveDownRoomSelectionAct, SIGNAL(triggered()), mapMgr, SLOT(onMoveDownRoomSelection()));
  mergeUpRoomSelectionAct = new QAction(QIcon(":/icons/roommergeup.png"), tr("Merge Up Selected Rooms"), this);
  mergeUpRoomSelectionAct->setStatusTip(tr("Merge Up Selected Rooms"));
  connect(mergeUpRoomSelectionAct, SIGNAL(triggered()), mapMgr, SLOT(onMergeUpRoomSelection()));
  mergeDownRoomSelectionAct = new QAction(QIcon(":/icons/roommergedown.png"), tr("Merge Down Selected Rooms"), this);
  mergeDownRoomSelectionAct->setStatusTip(tr("Merge Down Selected Rooms"));
  connect(mergeDownRoomSelectionAct, SIGNAL(triggered()), mapMgr, SLOT(onMergeDownRoomSelection()));
  connectToNeighboursRoomSelectionAct = new QAction(QIcon(":/icons/roomconnecttoneighbours.png"), tr("Connect room(s) to its neighbour rooms"), this);
  connectToNeighboursRoomSelectionAct->setStatusTip(tr("Connect room(s) to its neighbour rooms"));
  connect(connectToNeighboursRoomSelectionAct, SIGNAL(triggered()), mapMgr, SLOT(onConnectToNeighboursRoomSelection()));

  findRoomsAct = new QAction(QIcon(":/icons/roomfind.png"), tr("&Find Rooms"), this);
  findRoomsAct->setStatusTip(tr("Find Matching Rooms"));
  findRoomsAct->setShortcut(tr("Ctrl+F"));
  connect(findRoomsAct, SIGNAL(triggered()), mapMgr, SLOT(onFindRoom()));

  releaseAllPathsAct = new QAction(QIcon(":/icons/cancel.png"), tr("Release All Paths"), this);
  releaseAllPathsAct->setStatusTip(tr("Release All Paths"));
  releaseAllPathsAct->setCheckable(false);
  connect(releaseAllPathsAct, SIGNAL(triggered()), mapMgr->getPathMachine(), SLOT(releaseAllPaths()));

  forceRoomAct = new QAction(QIcon(":/icons/force.png"), tr("Force Path Machine to selected Room"), this);
  forceRoomAct->setStatusTip(tr("Force Path Machine to selected Room"));
  forceRoomAct->setCheckable(false);
  forceRoomAct->setEnabled(FALSE);
  connect(forceRoomAct, SIGNAL(triggered()), mapMgr->getMapWindow()->getCanvas(), SLOT(forceMapperToRoom()));

  roomActGroup = new QActionGroup(this);
  roomActGroup->setExclusive(false);
  roomActGroup->addAction(editRoomSelectionAct);
  roomActGroup->addAction(deleteRoomSelectionAct);
  roomActGroup->addAction(moveUpRoomSelectionAct);
  roomActGroup->addAction(moveDownRoomSelectionAct);
  roomActGroup->addAction(mergeUpRoomSelectionAct);
  roomActGroup->addAction(mergeDownRoomSelectionAct);
  roomActGroup->addAction(connectToNeighboursRoomSelectionAct);
  roomActGroup->setEnabled(FALSE);

  deleteConnectionSelectionAct = new QAction(QIcon(":/icons/connectiondelete.png"), tr("Delete Selected Connection"), this);
  deleteConnectionSelectionAct->setStatusTip(tr("Delete Selected Connection"));
  connect(deleteConnectionSelectionAct, SIGNAL(triggered()), mapMgr, SLOT(onDeleteConnectionSelection()));

  connectionActGroup = new QActionGroup(this);
  connectionActGroup->setExclusive(false);
  //connectionActGroup->addAction(editConnectionSelectionAct);
  connectionActGroup->addAction(deleteConnectionSelectionAct);
  connectionActGroup->setEnabled(FALSE);

  playModeAct = new QAction(QIcon(":/icons/online.png"), tr("Switch to play mode"), this);
  playModeAct->setStatusTip(tr("Switch to play mode - no new rooms are created"));
  playModeAct->setCheckable(true);
  connect(playModeAct, SIGNAL(triggered()), mapMgr, SLOT(onPlayMode()));

  mapModeAct = new QAction(QIcon(":/icons/map.png"), tr("Switch to mapping mode"), this);
  mapModeAct->setStatusTip(tr("Switch to mapping mode - new rooms are created when moving"));
  mapModeAct->setCheckable(true);
  connect(mapModeAct, SIGNAL(triggered()), mapMgr, SLOT(onMapMode()));

  offlineModeAct = new QAction(QIcon(":/icons/play.png"), tr("Switch to offline emulation mode"), this);
  offlineModeAct->setStatusTip(tr("Switch to offline emulation mode - you can learn areas offline"));
  offlineModeAct->setCheckable(true);
  connect(offlineModeAct, SIGNAL(triggered()), mapMgr, SLOT(onOfflineMode()));

  mapModeActGroup = new QActionGroup(this);
  mapModeActGroup->setExclusive(true);
  mapModeActGroup->addAction(playModeAct);
  mapModeActGroup->addAction(mapModeAct);
  mapModeActGroup->addAction(offlineModeAct);
  mapModeActGroup->setEnabled(true);
}

void ActionManager::disableActions(bool value)
{
  newAct->setDisabled(value);
  openAct->setDisabled(value);
  mergeAct->setDisabled(value);
  reloadAct->setDisabled(value);
  saveAct->setDisabled(value);
  saveAsAct->setDisabled(value);
  exitAct->setDisabled(value);
  cutAct->setDisabled(value);
  copyAct->setDisabled(value);
  pasteAct->setDisabled(value);
  aboutAct->setDisabled(value);
  aboutQtAct->setDisabled(value);
  //    nextWindowAct->setDisabled(value);
  //    prevWindowAct->setDisabled(value);
  zoomInAct->setDisabled(value);
  zoomOutAct->setDisabled(value);
  playModeAct->setDisabled(value);
  mapModeAct->setDisabled(value);
  modeRoomSelectAct->setDisabled(value);
  modeConnectionSelectAct->setDisabled(value);
  modeMoveSelectAct->setDisabled(value);
  modeInfoMarkEditAct->setDisabled(value);
  layerUpAct->setDisabled(value);
  layerDownAct->setDisabled(value);
  createRoomAct->setDisabled(value);
  createConnectionAct->setDisabled(value);
  createOnewayConnectionAct->setDisabled(value);
  releaseAllPathsAct->setDisabled(value);
  alwaysOnTopAct->setDisabled(value);
}

void ActionManager::groupManagerTypeChanged(int type)
{
  Config().m_groupManagerState = type;
  if (type == CGroupCommunicator::Server)
    groupServerAct->setChecked(true);
  else if (type == CGroupCommunicator::Client)
    groupClientAct->setChecked(true);
  else if (type == CGroupCommunicator::Off)
    groupOffAct->setChecked(true);
}

void ActionManager::alwaysOnTop()
{
  _mainWindow->setWindowFlags(_mainWindow->windowFlags() ^ Qt::WindowStaysOnTopHint);
  _mainWindow->show();
}

void ActionManager::about()
{
#ifdef SVN_REVISION
  QString version = tr("Subversion Revision ") + QString::number(SVN_REVISION));
#else
  QString version = tr("Alpha Release 0.2.3");
#endif
  QMessageBox::about(_mainWindow, tr("About mClient"),
                     tr("<FONT SIZE=\"+1\"><B>mClient ") + version + tr("</B></FONT><P>"
                         "Copyright \251 2008 Jahara<P>"
                         "mClient is a modern, fully functional, and highly portable fork of the mud client "
                         "powwow<P>"
                         "mClient has incorporated code and ideas from the mud clients "
                         "<A HREF=\"http://www.kmuddy.com\">KMuddy</A> and JMC respectively.<P>"
                         "Powwow is a console-based mud client written for Unix machines which prevented "
                         "its portability to other systems and ease of use for new-comers. Its main engine "
                         "has been rewritten with Qt libraries allowing it to run on any operating system "
                         "supported by Trolltech's Qt framework.<P>"
                         "Contributors: Cuantar, Kalev, Mint, Yants<BR>"
                         "Visit the <A HREF=\"http://code.google.com/p/mclient-mume/\">mClient website</A> "
                         "for more information."));
}

void ActionManager::clientHelp()
{
  if (!QDesktopServices::openUrl(QUrl::fromEncoded("http://mume.org/wiki/index.php/mClient_Help")))
    qDebug("Failed to open web browser");
}
