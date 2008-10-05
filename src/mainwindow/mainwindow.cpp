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

#include <QtGui>
#include <QTextStream>
#include <QCloseEvent>
//#include <QFileDialog>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "configuration.h"

#include "inputbar.h"
#include "textview.h"
#include "wrapper.h"
#include "objecteditor.h"
#include "configdialog.h"

#include "cprofilemanager.h"
#include "cprofilesettings.h"
#include "profiledialog.h"
#include "profilemanagerdialog.h"

// Currently mClient only allows one session
#define DEFAULT_SESSION 0

#ifdef MMAPPER
#include "mapwindow.h"
#include "mapcanvas.h"
#include "mapdata.h"
#include "roomeditattrdlg.h"
#include "mapstorage.h"
#include "connectionlistener.h"
#include "configuration.h"
#include "mmapper2pathmachine.h"
#include "roomselection.h"
#include "connectionselection.h"
#include "customaction.h"
#include "prespammedpath.h"
#include "roompropertysetter.h"
#include "findroomsdlg.h"
#include "CGroup.h"
#endif

class MainWindow *mainWindow;

MainWindow::MainWindow()
{
  setWindowIcon(QIcon(":/icons/m.png"));

  QVBoxLayout *vbox = new QVBoxLayout();
  vbox->setSpacing(0);
  vbox->setContentsMargins(0, 0, 0, 0);

  inputBar = new InputBar(this);
  textView = new TextView(inputBar, this);
  //wrapper = new Wrapper(inputBar, textView, this);
  wrapper = Wrapper::self();

  vbox->addWidget(textView);
  vbox->addWidget(inputBar);

  QWidget *mainWidget = new QWidget();
  mainWidget->setLayout(vbox);

  setCentralWidget(mainWidget);

  dockDialogLog = new QDockWidget(tr("Log"), this);
  dockDialogLog->setObjectName("DockWidgetLog");
  dockDialogLog->setAllowedAreas(Qt::AllDockWidgetAreas);
  dockDialogLog->setFeatures(QDockWidget::AllDockWidgetFeatures);
  addDockWidget(Qt::TopDockWidgetArea, dockDialogLog);

  logWindow = new QTextBrowser(dockDialogLog);
  logWindow->setObjectName("LogWindow");
  dockDialogLog->setWidget(logWindow);

  startMapper();

  createActions();
  createMenus();
  createToolBars();
  createStatusBar();

  readSettings();

  objectEditor = NULL;
  profileDialog = NULL;
  profileManager = NULL;

  /*connect(textView->document(), SIGNAL(contentsChanged()),
          this, SLOT(documentWasModified()));*/

  connect(wrapper, SIGNAL(addText(const QString&)), textView, SLOT(addText(const QString&)) );
  connect(wrapper, SIGNAL(moveCursor(int)), textView, SLOT(moveCursor(int)) );
  connect(wrapper, SIGNAL(toggleEchoMode()), inputBar, SLOT(toggleEchoMode()) );
  connect(wrapper, SIGNAL(close()), this, SLOT(close()) );
  connect(wrapper, SIGNAL(inputInsertText(QString)), inputBar, SLOT(inputInsertText(QString)) );
  connect(wrapper, SIGNAL(inputMoveTo(int)), inputBar, SLOT(inputMoveTo(int)) );
  connect(wrapper, SIGNAL(inputDeleteChars(int)), inputBar, SLOT(inputDeleteChars(int)) );
  connect(wrapper, SIGNAL(inputClear()), inputBar, SLOT(inputClear()) );
  connect(wrapper, SIGNAL(setCurrentProfile(const QString&)), this, SLOT(setCurrentProfile(const QString&)) );

  connect(inputBar, SIGNAL(returnPressed()), this, SLOT(sendUserInput()) );
  connect(inputBar, SIGNAL(keyPressed(const QString&)), this, SLOT(sendUserBind(const QString&)) );
  connect(inputBar, SIGNAL(mergeInputWrapper(QString, int)), wrapper, SLOT(mergeInputWrapper(QString, int)) );

  //connect(inputBar, SIGNAL(textEdited(const QString &)), this, SLOT(signalStdin(const QString &)));

 setCurrentProfile("");
  qDebug("MainWindow created.");
}

void MainWindow::start(int argc, char **argv) {
  if (Config().m_mapMode == 0)
  {
    playModeAct->setChecked(true);
    onPlayMode();
  }
  else
    if (Config().m_mapMode == 1)
  {
    mapModeAct->setChecked(true);
    onMapMode();
  }
  else
    if (Config().m_mapMode == 2)
  {
    offlineModeAct->setChecked(true);
    onOfflineMode();
  }
  groupManagerTypeChanged( Config().m_groupManagerState );

    // Initialize Powwow
  wrapper->start(argc, argv);

  // Display Main Window
  show();

  if (argc == 1)
    selectProfile();

  inputBar->setFocus();
}

void MainWindow::startMapper() {
#ifdef MMAPPER
  qRegisterMetaType<IncomingData>("IncomingData");
  qRegisterMetaType<CommandQueue>("CommandQueue");
  qRegisterMetaType<DirectionType>("DirectionType");
  qRegisterMetaType<DoorActionType>("DoorActionType");

  m_roomSelection = NULL;
  m_connectionSelection = NULL;

  m_mapData = new MapData();
  m_mapData->setObjectName("MapData");
  m_mapData->start();
  qDebug("MapData loaded");

  m_prespammedPath = new PrespammedPath(this);
  qDebug("PrespammedPath loaded");

  m_groupManager = new CGroup(Config().m_groupManagerCharName, m_mapData, this);
  m_groupManager->setObjectName("GroupManager");
  qDebug("GroupManager loaded");

  mapWindow = new MapWindow(m_mapData, m_prespammedPath, m_groupManager);
  dockDialogMapper = new QDockWidget(tr("Map"), this);
  dockDialogMapper->setObjectName("Mapper DockWidget");
  dockDialogMapper->setAllowedAreas(Qt::AllDockWidgetAreas);
  dockDialogMapper->setFeatures(QDockWidget::AllDockWidgetFeatures);
  addDockWidget(Qt::LeftDockWidgetArea, dockDialogMapper);
  dockDialogMapper->setWidget(mapWindow);
  qDebug("MapWindow/DockWidget loaded");

  m_pathMachine = new Mmapper2PathMachine();
  m_pathMachine->setObjectName("Mmapper2PathMachine");
  m_pathMachine->start();
  qDebug("PathMachine loaded");

  m_propertySetter = new RoomPropertySetter();
  m_propertySetter->setObjectName("RoomPropertySetter");
  m_propertySetter->start();
  qDebug("RoomPropertySetter loaded");

  m_dockDialogGroup = new QDockWidget(tr("Group Manager"), this);
  m_dockDialogGroup->setObjectName("DockWidgetGroup");
  m_dockDialogGroup->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
  m_dockDialogGroup->setFeatures(QDockWidget::AllDockWidgetFeatures);
  addDockWidget(Qt::TopDockWidgetArea, m_dockDialogGroup);
  m_dockDialogGroup->setWidget(m_groupManager);
  qDebug("GroupManagerView/DockWidget loaded");

  m_groupCommunicator = m_groupManager->getCommunicator();
  m_groupCommunicator->setObjectName("CCommunicator");
  qDebug("GroupCommunicator loaded");

  m_findRoomsDlg = new FindRoomsDlg(m_mapData, this);
  m_findRoomsDlg->setObjectName("FindRoomsDlg");
  qDebug("FindRooms loaded");

  currentMapWindowChanged();

#endif
}

#ifdef MMAPPER
void MainWindow::currentMapWindowChanged() {
  QObject::connect(m_pathMachine, SIGNAL(log( const QString&, const QString& )), this, SLOT(log( const QString&, const QString& )));

  QObject::connect(m_pathMachine, SIGNAL(lookingForRooms(RoomRecipient*, const Coordinate & )), m_mapData, SLOT(lookingForRooms(RoomRecipient*, const Coordinate & )));
  QObject::connect(m_pathMachine, SIGNAL(lookingForRooms(RoomRecipient*, ParseEvent* )), m_mapData, SLOT(lookingForRooms(RoomRecipient*, ParseEvent* )));
  QObject::connect(m_pathMachine, SIGNAL(lookingForRooms(RoomRecipient*, uint )), m_mapData, SLOT(lookingForRooms(RoomRecipient*, uint )));
  QObject::connect(m_mapData, SIGNAL(clearingMap()), m_pathMachine, SLOT(releaseAllPaths()));
  QObject::connect(m_pathMachine, SIGNAL(playerMoved(const Coordinate & )),getCurrentMapWindow()->getCanvas(), SLOT(moveMarker(const Coordinate &)));

  QObject::connect(getCurrentMapWindow()->getCanvas(), SIGNAL(setCurrentRoom(uint)), m_pathMachine, SLOT(setCurrentRoom(uint)));
  QObject::connect(getCurrentMapWindow()->getCanvas(), SIGNAL(charMovedEvent(ParseEvent*)), m_pathMachine, SLOT(event(ParseEvent*)));

  //moved to mapwindow
  QObject::connect(m_mapData, SIGNAL(mapSizeChanged(const Coordinate &, const Coordinate &)), getCurrentMapWindow(), SLOT(setScrollBars(const Coordinate &, const Coordinate &)));
  QObject::connect(getCurrentMapWindow()->getCanvas(), SIGNAL(roomPositionChanged()), m_pathMachine, SLOT(retry()));

  connect(m_prespammedPath, SIGNAL(update()), getCurrentMapWindow()->getCanvas(), SLOT(update()));

  connect (m_mapData, SIGNAL(log( const QString&, const QString& )), this, SLOT(log( const QString&, const QString& )));
  connect (getCurrentMapWindow()->getCanvas(), SIGNAL(log( const QString&, const QString& )), this, SLOT(log( const QString&, const QString& )));

  connect (m_mapData, SIGNAL(onDataLoaded()), getCurrentMapWindow()->getCanvas(), SLOT(dataLoaded()) );

  //connect(zoomInAct, SIGNAL(triggered()), getCurrentMapWindow()->getCanvas(), SLOT(zoomIn()));
  //connect(zoomOutAct, SIGNAL(triggered()), getCurrentMapWindow()->getCanvas(), SLOT(zoomOut()));

  connect(getCurrentMapWindow()->getCanvas(), SIGNAL(newRoomSelection(const RoomSelection*)), this, SLOT(newRoomSelection(const RoomSelection*)));
  connect(getCurrentMapWindow()->getCanvas(), SIGNAL(newConnectionSelection(ConnectionSelection*)), this, SLOT(newConnectionSelection(ConnectionSelection*)));

  // Group
  connect(m_groupManager, SIGNAL(log(const QString&, const QString&)), this, SLOT(log(const QString&, const QString&)));
  connect(m_pathMachine, SIGNAL(setCharPosition(uint)), m_groupManager, SLOT(setCharPosition(uint)));
  connect(m_groupManager, SIGNAL(drawCharacters()), getCurrentMapWindow()->getCanvas(), SLOT(update()));
  
  qDebug("Finished connecting MMapper Actions");
}
#endif

void MainWindow::closeEvent(QCloseEvent *event)
{
  writeSettings();
  if (maybeSave()) {
    event->accept();
  } else {
    event->ignore();
  }
}

/*
void MainWindow::newFile()
{
  if (maybeSave()) {
    textView->clear();
   setCurrentProfile("");
  }
}

void MainWindow::open()
{
  if (maybeSave()) {
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
      loadFile(fileName);
  }
}

bool MainWindow::save()
{
  if (currentProfile.isEmpty()) {
    return saveAs();
  } else {
    return saveFile(currentProfile);
  }
}

bool MainWindow::saveAs()
{
  QString fileName = QFileDialog::getSaveFileName(this);
  if (fileName.isEmpty())
    return false;

  return saveFile(fileName);
}
*/

void MainWindow::about()
{
#ifdef SVN_REVISION
  QString version = tr("Subversion Revision ") + QString::number(SVN_REVISION));
#else
  QString version = tr("Alpha Release 0.2.0");
#endif
  QMessageBox::about(this, tr("About mClient"),
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
                         "Contributors: Mint, Yants, Kalev<BR>"
                         "Visit the <A HREF=\"http://code.google.com/p/mclient-mume/\">mClient website</A> "
                         "for more information."));
}

void MainWindow::clientHelp()
{
  if (!QDesktopServices::openUrl(QUrl::fromEncoded("http://mume.org/wiki/index.php/mClient_Help")))
    qDebug("Failed to open web browser");
}

/*
void MainWindow::documentWasModified()
{
  setWindowModified(true);
}
*/

void MainWindow::sendUserInput()
{
  inputBar->selectAll();
  wrapper->getUserInput(inputBar->selectedText());
}

void MainWindow::sendUserBind(const QString& input)
{
  wrapper->getUserBind(input);
}

void MainWindow::createActions()
{
  connectAct = new QAction(QIcon(":/crystal/connect.png"), tr("&Connect..."), this);
  connectAct->setStatusTip(tr("Load a new session and connect to the remote host"));
  connect(connectAct, SIGNAL(triggered()), this, SLOT(selectProfile() ));

  disconnectAct = new QAction(QIcon(":/crystal/disconnect.png"), tr("&Disconnect"), this);
  disconnectAct->setStatusTip(tr("Disconnect from the current session"));
  connect(disconnectAct, SIGNAL(triggered()), wrapper, SLOT(disconnectSession()));

  reconnectAct = new QAction(QIcon(":/crystal/reconnect.png"), tr("&Reconnect"), this);
  reconnectAct->setStatusTip(tr("Reconnect to the current session's remote host"));
  connect(reconnectAct, SIGNAL(triggered()), wrapper, SLOT(connectSession()) );

  exitAct = new QAction(QIcon(":/crystal/exit.png"), tr("E&xit"), this);
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  cutAct = new QAction(QIcon(":/editcut.png"), tr("Cu&t"), this);
  cutAct->setShortcut(tr("Ctrl+X"));
  cutAct->setStatusTip(tr("Cut the current selection's contents to the "
      "clipboard"));
  connect(cutAct, SIGNAL(triggered()), textView, SLOT(cut()));

  copyAct = new QAction(QIcon(":/editcopy.png"), tr("&Copy"), this);
  copyAct->setShortcut(tr("Ctrl+C"));
  copyAct->setStatusTip(tr("Copy the current selection's contents to the "
      "clipboard"));
  connect(copyAct, SIGNAL(triggered()), textView, SLOT(copy()));

  pasteAct = new QAction(QIcon(":/editpaste.png"), tr("&Paste"), this);
  pasteAct->setShortcut(tr("Ctrl+V"));
  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
      "selection"));
  connect(pasteAct, SIGNAL(triggered()), inputBar, SLOT(paste()));

  alwaysOnTopAct = new QAction(tr("&Always on Top"), this);
  alwaysOnTopAct->setCheckable(true);
  alwaysOnTopAct->setStatusTip(tr("Toggle the window to always stay on the top"));
  connect(alwaysOnTopAct, SIGNAL(triggered()), this, SLOT(alwaysOnTop()));

  objectAct = new QAction(tr("&Object Editor..."), this);
  objectAct->setStatusTip(tr("Edit Powwow objects such as aliases and actions"));
  connect(objectAct, SIGNAL(triggered()), this, SLOT(editObjects()));
  
  profileAct = new QAction(QIcon(":/crystal/profile.png"), tr("Profile &Manager..."), this);
  profileAct->setStatusTip(tr("Manage mClient profile settings"));
  connect(profileAct, SIGNAL(triggered()), this, SLOT(manageProfiles()));

  settingsAct = new QAction(QIcon(":/crystal/settings.png"), tr("&Preferences..."), this);
  settingsAct->setStatusTip(tr("Change mClient settings"));
  connect(settingsAct, SIGNAL(triggered()), this, SLOT(changeConfiguration()) );

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
  connect(textView, SIGNAL(copyAvailable(bool)),
          cutAct, SLOT(setEnabled(bool)));
  connect(textView, SIGNAL(copyAvailable(bool)),
          copyAct, SLOT(setEnabled(bool)));
  
#ifdef MMAPPER
  newAct = new QAction(QIcon(":/icons/new.png"), tr("&New"), this);
  newAct->setShortcut(tr("Ctrl+N"));
  newAct->setStatusTip(tr("Create a new file"));
  connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

  openAct = new QAction(QIcon(":/icons/open.png"), tr("&Open..."), this);
  openAct->setShortcut(tr("Ctrl+O"));
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  reloadAct = new QAction(QIcon(":/icons/reload.png"), tr("&Reload"), this);
  reloadAct->setShortcut(tr("Ctrl+R"));
  reloadAct->setStatusTip(tr("Reload the current map"));
  connect(reloadAct, SIGNAL(triggered()), this, SLOT(reload()));

  saveAct = new QAction(QIcon(":/icons/save.png"), tr("&Save"), this);
  saveAct->setShortcut(tr("Ctrl+S"));
  saveAct->setStatusTip(tr("Save the document to disk"));
  connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

  saveAsAct = new QAction(tr("Save &As..."), this);
  saveAsAct->setStatusTip(tr("Save the document under a new name"));
  connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

  mergeAct = new QAction(QIcon(":/icons/merge.png"), tr("&Merge..."), this);
  //mergeAct->setShortcut(tr("Ctrl+M"));
  mergeAct->setStatusTip(tr("Merge an existing file into current map"));
  connect(mergeAct, SIGNAL(triggered()), this, SLOT(merge()));

  preferencesAct = new QAction(QIcon(":/icons/preferences.png"), tr("MMapper2 configuration"), this);
  preferencesAct->setShortcut(tr("Ctrl+P"));
  preferencesAct->setStatusTip(tr("MMapper2 configuration"));
  connect(preferencesAct, SIGNAL(triggered()), this, SLOT(onPreferences()));

  zoomInAct = new QAction(QIcon(":/icons/viewmag+.png"), tr("Zoom In"), this);

  alwaysOnTopAct = new QAction(tr("Always on top"), this);
  alwaysOnTopAct->setCheckable(true);
  connect(alwaysOnTopAct, SIGNAL(triggered()), this, SLOT(alwaysOnTop()));

  zoomInAct->setStatusTip(tr("Zooms In current map"));
  zoomOutAct = new QAction(QIcon(":/icons/viewmag-.png"), tr("Zoom Out"), this);
  zoomOutAct->setStatusTip(tr("Zooms Out current map"));

  layerUpAct = new QAction(QIcon(":/icons/layerup.png"), tr("Layer Up"), this);
  layerUpAct->setStatusTip(tr("Layer Up"));
  connect(layerUpAct, SIGNAL(triggered()), this, SLOT(onLayerUp()));
  layerDownAct = new QAction(QIcon(":/icons/layerdown.png"), tr("Layer Down"), this);
  layerDownAct->setStatusTip(tr("Layer Down"));
  connect(layerDownAct, SIGNAL(triggered()), this, SLOT(onLayerDown()));

  modeConnectionSelectAct = new QAction(QIcon(":/icons/connectionselection.png"), tr("Select Connection"), this);
  modeConnectionSelectAct->setStatusTip(tr("Select Connection"));
  modeConnectionSelectAct->setCheckable(true);
  connect(modeConnectionSelectAct, SIGNAL(triggered()), this, SLOT(onModeConnectionSelect()));
  modeRoomSelectAct = new QAction(QIcon(":/icons/roomselection.png"), tr("Select Rooms"), this);
  modeRoomSelectAct->setStatusTip(tr("Select Rooms"));
  modeRoomSelectAct->setCheckable(true);
  connect(modeRoomSelectAct, SIGNAL(triggered()), this, SLOT(onModeRoomSelect()));
  modeMoveSelectAct = new QAction(QIcon(":/icons/mapmove.png"), tr("Move map"), this);
  modeMoveSelectAct->setStatusTip(tr("Move Map"));
  modeMoveSelectAct->setCheckable(true);
  connect(modeMoveSelectAct, SIGNAL(triggered()), this, SLOT(onModeMoveSelect()));
  modeInfoMarkEditAct = new QAction(QIcon(":/icons/infomarksedit.png"), tr("Edit Info Marks"), this);
  modeInfoMarkEditAct->setStatusTip(tr("Edit Info Marks"));
  modeInfoMarkEditAct->setCheckable(true);
  connect(modeInfoMarkEditAct, SIGNAL(triggered()), this, SLOT(onModeInfoMarkEdit()));

  createRoomAct = new QAction(QIcon(":/icons/roomcreate.png"), tr("Create New Rooms"), this);
  createRoomAct->setStatusTip(tr("Create New Rooms"));
  createRoomAct->setCheckable(true);
  connect(createRoomAct, SIGNAL(triggered()), this, SLOT(onModeCreateRoomSelect()));

  createConnectionAct = new QAction(QIcon(":/icons/connectioncreate.png"), tr("Create New Connection"), this);
  createConnectionAct->setStatusTip(tr("Create New Connection"));
  createConnectionAct->setCheckable(true);
  connect(createConnectionAct, SIGNAL(triggered()), this, SLOT(onModeCreateConnectionSelect()));

  createOnewayConnectionAct = new QAction(QIcon(":/icons/onewayconnectioncreate.png"), tr("Create New Oneway Connection"), this);
  createOnewayConnectionAct->setStatusTip(tr("Create New Oneway Connection"));
  createOnewayConnectionAct->setCheckable(true);
  connect(createOnewayConnectionAct, SIGNAL(triggered()), this, SLOT(onModeCreateOnewayConnectionSelect()));

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
  connect(editRoomSelectionAct, SIGNAL(triggered()), this, SLOT(onEditRoomSelection()));

  deleteRoomSelectionAct = new QAction(QIcon(":/icons/roomdelete.png"), tr("Delete Selected Rooms"), this);
  deleteRoomSelectionAct->setStatusTip(tr("Delete Selected Rooms"));
  connect(deleteRoomSelectionAct, SIGNAL(triggered()), this, SLOT(onDeleteRoomSelection()));

  moveUpRoomSelectionAct = new QAction(QIcon(":/icons/roommoveup.png"), tr("Move Up Selected Rooms"), this);
  moveUpRoomSelectionAct->setStatusTip(tr("Move Up Selected Rooms"));
  connect(moveUpRoomSelectionAct, SIGNAL(triggered()), this, SLOT(onMoveUpRoomSelection()));
  moveDownRoomSelectionAct = new QAction(QIcon(":/icons/roommovedown.png"), tr("Move Down Selected Rooms"), this);
  moveDownRoomSelectionAct->setStatusTip(tr("Move Down Selected Rooms"));
  connect(moveDownRoomSelectionAct, SIGNAL(triggered()), this, SLOT(onMoveDownRoomSelection()));
  mergeUpRoomSelectionAct = new QAction(QIcon(":/icons/roommergeup.png"), tr("Merge Up Selected Rooms"), this);
  mergeUpRoomSelectionAct->setStatusTip(tr("Merge Up Selected Rooms"));
  connect(mergeUpRoomSelectionAct, SIGNAL(triggered()), this, SLOT(onMergeUpRoomSelection()));
  mergeDownRoomSelectionAct = new QAction(QIcon(":/icons/roommergedown.png"), tr("Merge Down Selected Rooms"), this);
  mergeDownRoomSelectionAct->setStatusTip(tr("Merge Down Selected Rooms"));
  connect(mergeDownRoomSelectionAct, SIGNAL(triggered()), this, SLOT(onMergeDownRoomSelection()));
  connectToNeighboursRoomSelectionAct = new QAction(QIcon(":/icons/roomconnecttoneighbours.png"), tr("Connect room(s) to its neighbour rooms"), this);
  connectToNeighboursRoomSelectionAct->setStatusTip(tr("Connect room(s) to its neighbour rooms"));
  connect(connectToNeighboursRoomSelectionAct, SIGNAL(triggered()), this, SLOT(onConnectToNeighboursRoomSelection()));

  findRoomsAct = new QAction(QIcon(":/icons/roomfind.png"), tr("&Find Rooms"), this);
  findRoomsAct->setStatusTip(tr("Find Matching Rooms"));
  findRoomsAct->setShortcut(tr("Ctrl+F"));
  connect(findRoomsAct, SIGNAL(triggered()), this, SLOT(onFindRoom()));

  releaseAllPathsAct = new QAction(QIcon(":/icons/cancel.png"), tr("Release All Paths"), this);
  releaseAllPathsAct->setStatusTip(tr("Release All Paths"));
  releaseAllPathsAct->setCheckable(false);
  connect(releaseAllPathsAct, SIGNAL(triggered()), m_pathMachine, SLOT(releaseAllPaths()));

  forceRoomAct = new QAction(QIcon(":/icons/force.png"), tr("Force Path Machine to selected Room"), this);
  forceRoomAct->setStatusTip(tr("Force Path Machine to selected Room"));
  forceRoomAct->setCheckable(false);
  forceRoomAct->setEnabled(FALSE);
  connect(forceRoomAct, SIGNAL(triggered()), getCurrentMapWindow()->getCanvas(), SLOT(forceMapperToRoom()));

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
  connect(deleteConnectionSelectionAct, SIGNAL(triggered()), this, SLOT(onDeleteConnectionSelection()));

  connectionActGroup = new QActionGroup(this);
  connectionActGroup->setExclusive(false);
  //connectionActGroup->addAction(editConnectionSelectionAct);
  connectionActGroup->addAction(deleteConnectionSelectionAct);
  connectionActGroup->setEnabled(FALSE);

  playModeAct = new QAction(QIcon(":/icons/online.png"), tr("Switch to play mode"), this);
  playModeAct->setStatusTip(tr("Switch to play mode - no new rooms are created"));
  playModeAct->setCheckable(true);
  connect(playModeAct, SIGNAL(triggered()), this, SLOT(onPlayMode()));

  mapModeAct = new QAction(QIcon(":/icons/map.png"), tr("Switch to mapping mode"), this);
  mapModeAct->setStatusTip(tr("Switch to mapping mode - new rooms are created when moving"));
  mapModeAct->setCheckable(true);
  connect(mapModeAct, SIGNAL(triggered()), this, SLOT(onMapMode()));

  offlineModeAct = new QAction(QIcon(":/icons/play.png"), tr("Switch to offline emulation mode"), this);
  offlineModeAct->setStatusTip(tr("Switch to offline emulation mode - you can learn areas offline"));
  offlineModeAct->setCheckable(true);
  connect(offlineModeAct, SIGNAL(triggered()), this, SLOT(onOfflineMode()));

  mapModeActGroup = new QActionGroup(this);
  mapModeActGroup->setExclusive(true);
  mapModeActGroup->addAction(playModeAct);
  mapModeActGroup->addAction(mapModeAct);
  mapModeActGroup->addAction(offlineModeAct);
  mapModeActGroup->setEnabled(true);

    // Find Room Dialog Connections
  connect(m_findRoomsDlg, SIGNAL(center(qint32, qint32)), getCurrentMapWindow(), SLOT(center(qint32, qint32)));
  connect(m_findRoomsDlg, SIGNAL(log( const QString&, const QString& )), this, SLOT(log( const QString&, const QString& )));

      // group Manager
  groupOffAct = new QAction(QIcon(":/icons/groupoff.png"), tr("Switch to &Off mode"), this );
  groupOffAct->setShortcut(tr("Ctrl+G"));
  groupOffAct->setCheckable(true);
  connect(groupOffAct, SIGNAL(toggled(bool)), this, SLOT(groupOff(bool)), Qt::QueuedConnection);

  groupClientAct = new QAction(QIcon(":/icons/groupclient.png"),tr("Switch to &Client mode"), this );
  groupClientAct->setCheckable(true);
  connect(groupClientAct, SIGNAL(toggled(bool)), this, SLOT(groupClient(bool)), Qt::QueuedConnection);

  groupServerAct = new QAction(QIcon(":/icons/groupserver.png"),tr("Switch to &Server mode"), this );
  groupServerAct->setCheckable(true);
  connect(groupServerAct, SIGNAL(toggled(bool)), this, SLOT(groupServer(bool)), Qt::QueuedConnection);

  groupManagerGroup = new QActionGroup(this);
  groupManagerGroup->addAction(groupOffAct);
  groupManagerGroup->addAction(groupClientAct);
  groupManagerGroup->addAction(groupServerAct);

  // Group Manager/Communicator
  connect(m_groupCommunicator, SIGNAL(typeChanged(int)), this, SLOT(groupManagerTypeChanged(int)), Qt::QueuedConnection);
#endif
}

void MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(connectAct);
  fileMenu->addAction(disconnectAct);
  fileMenu->addAction(reconnectAct);
  fileMenu->addSeparator();
  fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  //fileMenu->addAction(reloadAct);
  fileMenu->addAction(saveAct);
  fileMenu->addAction(saveAsAct);
  //fileMenu->addAction(mergeAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(cutAct);
  editMenu->addAction(copyAct);
  editMenu->addAction(pasteAct);

  viewMenu = menuBar()->addMenu(tr("&View"));
  viewMenu->addAction(alwaysOnTopAct);

  mapperMenu = menuBar()->addMenu(tr("&Mapper"));
  groupMenu = menuBar()->addMenu(tr("&Group") );

  settingsMenu = menuBar()->addMenu(tr("&Settings"));
  viewDockMenu = settingsMenu->addMenu(tr("&Dock"));
  viewDockMenu->addAction(dockDialogLog->toggleViewAction());
  viewDockMenu->addAction(m_dockDialogGroup->toggleViewAction());
  settingsMenu->addSeparator();
  settingsMenu->addAction(objectAct);
  settingsMenu->addAction(profileAct);
  settingsMenu->addAction(settingsAct);

  menuBar()->addSeparator();

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(mumeHelpAct);
  helpMenu->addAction(forumAct);
  helpMenu->addAction(wikiAct);
  helpMenu->addSeparator();
  helpMenu->addAction(clientHelpAct);
  helpMenu->addSeparator();
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);
  
#ifdef MMAPPER
  roomMenu = mapperMenu->addMenu(tr("&Rooms"));
  connectionMenu = mapperMenu->addMenu(tr("&Connections"));

  mapperMenu->addSeparator();
  mapperMenu->addAction(playModeAct);
  mapperMenu->addAction(mapModeAct);
  mapperMenu->addAction(offlineModeAct);
  mapperMenu->addSeparator();
  mapperMenu->addAction(modeRoomSelectAct);
  mapperMenu->addAction(modeConnectionSelectAct);
  mapperMenu->addAction(modeMoveSelectAct);
  mapperMenu->addAction(modeInfoMarkEditAct);

  roomMenu->addAction(findRoomsAct);
  roomMenu->addSeparator();
  roomMenu->addAction(createRoomAct);
  roomMenu->addAction(editRoomSelectionAct);
  roomMenu->addAction(deleteRoomSelectionAct);
  roomMenu->addAction(moveUpRoomSelectionAct);
  roomMenu->addAction(moveDownRoomSelectionAct);
  roomMenu->addAction(mergeUpRoomSelectionAct);
  roomMenu->addAction(mergeDownRoomSelectionAct);
  roomMenu->addAction(connectToNeighboursRoomSelectionAct);

  connectionMenu->addAction(createConnectionAct);
  connectionMenu->addAction(createOnewayConnectionAct);
  //connectionMenu->addAction(editConnectionSelectionAct);
  connectionMenu->addAction(deleteConnectionSelectionAct);

  viewMenu->addSeparator();
  viewMenu->addAction(zoomInAct);
  viewMenu->addAction(zoomOutAct);
  viewMenu->addSeparator();
  viewMenu->addAction(layerUpAct);
  viewMenu->addAction(layerDownAct);
  viewMenu->addAction(releaseAllPathsAct);
  //    viewMenu->addAction(nextWindowAct);
  //    viewMenu->addAction(prevWindowAct);
  //    viewMenu->addSeparator();

  groupMenu->addAction(groupOffAct);
  groupMenu->addAction(groupClientAct);
  groupMenu->addAction(groupServerAct);
#endif
}

void MainWindow::createToolBars()
{
  /*
  connectToolBar = addToolBar(tr("Connection"));
  connectToolBar->addAction(connectAct);
  connectToolBar->addAction(disconnectAct);
  //connectToolBar->addAction(reconnectAct);
  */

  editToolBar = addToolBar(tr("Edit"));
  editToolBar->addAction(cutAct);
  editToolBar->addAction(copyAct);
  editToolBar->addAction(pasteAct);
}

void MainWindow::createStatusBar()
{
  statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
  resize(Config().windowSize);
  move(Config().windowPosition);
  restoreState(Config().windowState);
  alwaysOnTopAct->setChecked(Config().alwaysOnTop);
  if (Config().alwaysOnTop) {
    alwaysOnTop();
  }
  else
  {
    if (pos().x() < 0) pos().setX(0);
    if (pos().y() < 0) pos().setY(0);
    move(pos());
  }
}

void MainWindow::writeSettings()
{
  Config().setWindowPosition(pos() );
  Config().setWindowSize(size() );
  Config().setWindowState(saveState() );
  Config().setAlwaysOnTop((bool)(windowFlags() & Qt::WindowStaysOnTopHint));
}

void MainWindow::alwaysOnTop()
{
  setWindowFlags(windowFlags() ^ Qt::WindowStaysOnTopHint);
  if (pos().x() < 0) pos().setX(0);
  if (pos().y() < 0) pos().setY(0);
  move(pos());
  show();
}

bool MainWindow::maybeSave()
{
  if (Config().isChanged()) {
    int ret = QMessageBox::warning(this, tr("mClient"),
                                   tr("Your client settings have been modified.\n"
                                       "Do you want to save your changes?"),
                                       QMessageBox::Yes | QMessageBox::Default,
                                       QMessageBox::No,
                                       QMessageBox::Cancel | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
      Config().write();
    else if (ret == QMessageBox::Cancel)
      return false;
  }
  return true;
}

/*
void MainWindow::loadFile(const QString &fileName)
{
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("mClient"),
                         tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
    return;
  }

  QTextStream in(&file);
  QApplication::setOverrideCursor(Qt::WaitCursor);
  //textView->setText(in.readAll());
  QString input = in.readAll();
  textView->addText(input);
  QApplication::restoreOverrideCursor();

 setCurrentProfile(fileName);
  statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
  QFile file(fileName);
  if (!file.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("mClient"),
                         tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
    return false;
  }

  QTextStream out(&file);
  QApplication::setOverrideCursor(Qt::WaitCursor);
  out << textView->toPlainText();
  QApplication::restoreOverrideCursor();

 setCurrentProfile(fileName);
  statusBar()->showMessage(tr("File saved"), 2000);
  return true;
}
*/

void MainWindow::setCurrentProfile(const QString &profile)
{
  currentProfile = profile;
  //textView->document()->setModified(false);
  setWindowModified(false);

  QString shownName;
  if (currentProfile.isEmpty())
    shownName = "No connection";
  else
    shownName = strippedName(currentProfile);

  setWindowTitle(tr("%1 - mClient").arg(shownName));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
  return QFileInfo(fullFileName).fileName();
}

MainWindow::~MainWindow()
{}

void MainWindow::editObjects() {
  if (!objectEditor) {
    objectEditor = new ObjectEditor(wrapper, this);
  }
  objectEditor->show();
  objectEditor->activateWindow();
}

void MainWindow::manageProfiles() {
  if (!profileManager) {
    profileManager = new ProfileManagerDialog(0, this);
    //connect(profileManager, SIGNAL(loadClicked()), this, SLOT(relayConnect() ));
  }
  profileManager->show();
  profileManager->activateWindow();
}

void MainWindow::changeConfiguration() {
  ConfigDialog dialog;
  dialog.exec();
}

void MainWindow::selectProfile() {
  if (!profileDialog) {
  profileDialog = new ProfileDialog(this);
  }
  /*
  profileDialog->show();
  profileDialog->activateWindow();
  */
  connect (profileDialog, SIGNAL(profileSelected() ), this, SLOT(profileSelected() ));

  profileDialog->exec();

  delete profileDialog;
  profileDialog = NULL;
}

void MainWindow::profileSelected() {
  QString profile = profileDialog->selectedProfile();
  qDebug("Got signal to start new session %s", profile.toAscii().constData());
  cProfileManager *mgr = cProfileManager::self();
  cProfileSettings *sett = mgr->settings(profile);

  // Check if there is a loaded profile
  if (mgr->sessionAssigned(DEFAULT_SESSION)) {
    Wrapper::self()->disconnectSession();
    Wrapper::self()->clearPowwowMemory();
  }

  QString fileName = sett->getString("map");
  if (!fileName.isEmpty() && maybeSaveMap()) {
    loadFile(fileName);
  }

  wrapper->loadProfile(profile);
}

void MainWindow::log(const QString& module, const QString& message)
{
  logWindow->append("[" + module + "] " + message);
  logWindow->update();
}

#ifdef MMAPPER
void MainWindow::newRoomSelection(const RoomSelection* rs)
{
  forceRoomAct->setEnabled(FALSE);
  m_roomSelection = rs;
  if (m_roomSelection)
  {
    roomActGroup->setEnabled(TRUE);
    if (m_roomSelection->size() == 1) {
      forceRoomAct->setEnabled(TRUE);
    }
  }
  else
  {
    roomActGroup->setEnabled(FALSE);
  }
}

void MainWindow::newConnectionSelection(ConnectionSelection* cs)
{
  m_connectionSelection = cs;
  if (m_connectionSelection)
  {
    connectionActGroup->setEnabled(TRUE);
  }
  else
  {
    connectionActGroup->setEnabled(FALSE);
  }
}

void MainWindow::newFile()
{
  getCurrentMapWindow()->getCanvas()->clearRoomSelection();
  getCurrentMapWindow()->getCanvas()->clearConnectionSelection();

  if (maybeSave())
  {
    AbstractMapStorage *storage = (AbstractMapStorage*) new MapStorage(*m_mapData , "");
    connect(storage, SIGNAL(onNewData()), getCurrentMapWindow()->getCanvas(), SLOT(dataLoaded()));
    connect(storage, SIGNAL(log(const QString&, const QString&)), this, SLOT(log(const QString&, const QString&)));
    storage->newData();
    delete(storage);
  }
}

void MainWindow::disableActions(bool value)
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

void MainWindow::merge()
{
  QString fileName = QFileDialog::getOpenFileName(this,"Choose map file ...","","MMapper2 (*.mm2);;MMapper (*.map)");
  if (!fileName.isEmpty())
  {
    QFile *file = new QFile(fileName);

    if (!file->open(QFile::ReadOnly))
    {
      QMessageBox::warning(this, tr("Application"),
                           tr("Cannot read file %1:\n%2.")
                               .arg(fileName)
                               .arg(file->errorString()));

      getCurrentMapWindow()->getCanvas()->setEnabled(true);
      delete file;
      return;
    }

    //MERGE
    progressDlg = new QProgressDialog(this);
    QPushButton *cb = new QPushButton("Abort ...");
    cb->setEnabled(false);
    progressDlg->setCancelButton ( cb );
    progressDlg->setLabelText("Importing map...");
    progressDlg->setCancelButtonText("Abort");
    progressDlg->setMinimum(0);
    progressDlg->setMaximum(100);
    progressDlg->setValue(0);
    progressDlg->show();

    getCurrentMapWindow()->getCanvas()->clearRoomSelection();
    getCurrentMapWindow()->getCanvas()->clearConnectionSelection();

    AbstractMapStorage *storage = (AbstractMapStorage*) new MapStorage(*m_mapData , fileName, file);
    connect(storage, SIGNAL(onDataLoaded()), getCurrentMapWindow()->getCanvas(), SLOT(dataLoaded()));
    connect(storage, SIGNAL(onPercentageChanged(quint32)), this, SLOT(percentageChanged(quint32)));
    connect(storage, SIGNAL(log(const QString&, const QString&)), this, SLOT(log(const QString&, const QString&)));

    disableActions(true);
    getCurrentMapWindow()->getCanvas()->hide();
    if (storage->canLoad()) storage->mergeData();
    getCurrentMapWindow()->getCanvas()->show();
    disableActions(false);
    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    pasteAct->setEnabled(false);

    delete(storage);
    delete progressDlg;

    statusBar()->showMessage(tr("File merged"), 2000);
    delete file;
  }
}

void MainWindow::open()
{
  if (maybeSave())
  {
    QString fileName = QFileDialog::getOpenFileName(this,"Choose map file ...","","MMapper2 (*.mm2);;MMapper (*.map)");
    if (!fileName.isEmpty())
      loadFile(fileName);
  }
}

void MainWindow::reload()
{
  if (maybeSave())
  {
    loadFile(m_mapData->getFileName());
  }
}

bool MainWindow::save()
{
  if (m_mapData->getFileName().isEmpty())
  {
    return saveAs();
  }
  else
  {
    return saveFile(m_mapData->getFileName());
  }
}

bool MainWindow::saveAs()
{
  QFileDialog save(this, "Choose map file name ...");
  save.setFileMode(QFileDialog::AnyFile);
  save.setDirectory(QDir::current());
  save.setFilter("MMapper2 (*.mm2)");
  save.setDefaultSuffix("mm2");
  save.setAcceptMode(QFileDialog::AcceptSave);
  QStringList fileNames;
  if (save.exec()) {
    fileNames = save.selectedFiles();
  }

  if (fileNames.isEmpty()) {
    statusBar()->showMessage(tr("No filename provided"), 2000);
    return false;
  }

  return saveFile(fileNames[0]);
}


bool MainWindow::maybeSaveMap()
{
  if ( m_mapData->dataChanged() )
  {
    int ret = QMessageBox::warning(this, tr("Application"),
                                   tr("The document has been modified.\n"
                                       "Do you want to save your changes?"),
                                       QMessageBox::Yes | QMessageBox::Default,
                                       QMessageBox::No,
                                       QMessageBox::Cancel | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
      return save();
    else if (ret == QMessageBox::Cancel)
      return false;
  }
  return true;
}

void MainWindow::loadFile(const QString &fileName)
{
  //getCurrentMapWindow()->getCanvas()->setEnabled(false);

  QFile *file = new QFile(fileName);
  //QIODevice *file = KFilterDev::deviceForFile( filename, "application/x-gzip", TRUE );

  if (!file->open(QFile::ReadOnly))
  {
    QMessageBox::warning(this, tr("Application"),
                         tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file->errorString()));

    getCurrentMapWindow()->getCanvas()->setEnabled(true);
    delete file;
    return;
  }

  //LOAD
  progressDlg = new QProgressDialog(this);
  QPushButton *cb = new QPushButton("Abort ...");
  cb->setEnabled(false);
  progressDlg->setCancelButton ( cb );
  progressDlg->setLabelText("Loading map...");
  progressDlg->setCancelButtonText("Abort");
  progressDlg->setMinimum(0);
  progressDlg->setMaximum(100);
  progressDlg->setValue(0);
  progressDlg->show();

  getCurrentMapWindow()->getCanvas()->clearRoomSelection();
  getCurrentMapWindow()->getCanvas()->clearConnectionSelection();

  AbstractMapStorage *storage = (AbstractMapStorage*) new MapStorage(*m_mapData , fileName, file);
  connect(storage, SIGNAL(onDataLoaded()), getCurrentMapWindow()->getCanvas(), SLOT(dataLoaded()));
  connect(storage, SIGNAL(onPercentageChanged(quint32)), this, SLOT(percentageChanged(quint32)));
  connect(storage, SIGNAL(log(const QString&, const QString&)), this, SLOT(log(const QString&, const QString&)));

  disableActions(true);
  getCurrentMapWindow()->getCanvas()->hide();
  if (storage->canLoad()) storage->loadData();
  getCurrentMapWindow()->getCanvas()->show();
  disableActions(false);
  cutAct->setEnabled(false);
  copyAct->setEnabled(false);
  pasteAct->setEnabled(false);

  delete(storage);
  delete progressDlg;

  setCurrentFile(fileName);
  statusBar()->showMessage(tr("File loaded"), 2000);
  delete file;
  //getCurrentMapWindow()->getCanvas()->setEnabled(true);
}

void MainWindow::percentageChanged(quint32 p)
{
  if(!progressDlg) return;
  progressDlg->setValue(p);

  qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
  //qApp->processEvents();
}

bool MainWindow::saveFile(const QString &fileName)
{
  getCurrentMapWindow()->getCanvas()->setEnabled(false);

  //QIODevice *file = KFilterDev::deviceForFile( filename, "application/x-gzip", TRUE );
  QFile *file = new QFile( fileName );
  if (!file->open(QFile::WriteOnly))
  {
    QMessageBox::warning(NULL, tr("Application"),
                         tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file->errorString()));
    delete file;
    getCurrentMapWindow()->getCanvas()->setEnabled(true);
    return false;
  }


  //SAVE
  progressDlg = new QProgressDialog(this);
  QPushButton *cb = new QPushButton("Abort ...");
  cb->setEnabled(false);
  progressDlg->setCancelButton ( cb );
  progressDlg->setLabelText("Saving map...");
  progressDlg->setCancelButtonText("Abort");
  progressDlg->setMinimum(0);
  progressDlg->setMaximum(100);
  progressDlg->setValue(0);
  progressDlg->show();

  AbstractMapStorage *storage = (AbstractMapStorage*) new MapStorage(*m_mapData , fileName, file);
  connect(storage, SIGNAL(onPercentageChanged(quint32)), this, SLOT(percentageChanged(quint32)));
  connect(storage, SIGNAL(log(const QString&, const QString&)), this, SLOT(log(const QString&, const QString&)));

  disableActions(true);
  //getCurrentMapWindow()->getCanvas()->hide();
  if (storage->canSave()) storage->saveData();
  //getCurrentMapWindow()->getCanvas()->show();
  disableActions(false);
  cutAct->setEnabled(false);
  copyAct->setEnabled(false);
  pasteAct->setEnabled(false);

  delete(storage);
  delete progressDlg;

  setCurrentFile(fileName);
  statusBar()->showMessage(tr("File saved"), 2000);
  delete file;
  getCurrentMapWindow()->getCanvas()->setEnabled(true);

  return true;
}

void MainWindow::onFindRoom()
{
  m_findRoomsDlg->show();
}

void MainWindow::groupManagerTypeChanged(int type)
{
  //qDebug("Action: signal received groupManager type changed");
  Config().m_groupManagerState = type;
  if (type == CGroupCommunicator::Server)
  {
    groupServerAct->setChecked(true);
    //m_groupManager->show();
  }
  if (type == CGroupCommunicator::Client)
  {
    groupClientAct->setChecked(true);
    //m_groupManager->show();
  }
  if (type == CGroupCommunicator::Off)
  {
    groupOffAct->setChecked(true);
    //m_groupManager->hide();
  }
}


void MainWindow::groupOff(bool b)
{
  if (b)
    m_groupManager->setType(CGroupCommunicator::Off);
  //qDebug("Done.");
}


void MainWindow::groupClient(bool b)
{
  if (b)
    m_groupManager->setType(CGroupCommunicator::Client);
  //qDebug("Done.");
}


void MainWindow::groupServer(bool b)
{
  if (b)
    m_groupManager->setType(CGroupCommunicator::Server);
  //qDebug("Done.");
}

void MainWindow::setCurrentFile(const QString &fileName)
{
  QString shownName;
  if (fileName.isEmpty())
    shownName = "untitled.mm2";
  else
    shownName = strippedName(fileName);

  setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("MMapper2")));
}

void MainWindow::onLayerUp()
{
  getCurrentMapWindow()->getCanvas()->layerUp();
}

void MainWindow::onLayerDown()
{
  getCurrentMapWindow()->getCanvas()->layerDown();
}

void MainWindow::onModeConnectionSelect()
{
  getCurrentMapWindow()->getCanvas()->setCanvasMouseMode(MapCanvas::CMM_SELECT_CONNECTIONS);
}

void MainWindow::onModeRoomSelect()
{
  getCurrentMapWindow()->getCanvas()->setCanvasMouseMode(MapCanvas::CMM_SELECT_ROOMS);
}

void MainWindow::onModeMoveSelect()
{
  getCurrentMapWindow()->getCanvas()->setCanvasMouseMode(MapCanvas::CMM_MOVE);
}

void MainWindow::onModeCreateRoomSelect()
{
  getCurrentMapWindow()->getCanvas()->setCanvasMouseMode(MapCanvas::CMM_CREATE_ROOMS);
}

void MainWindow::onModeCreateConnectionSelect()
{
  getCurrentMapWindow()->getCanvas()->setCanvasMouseMode(MapCanvas::CMM_CREATE_CONNECTIONS);
}

void MainWindow::onModeCreateOnewayConnectionSelect()
{
  getCurrentMapWindow()->getCanvas()->setCanvasMouseMode(MapCanvas::CMM_CREATE_ONEWAY_CONNECTIONS);
}

void MainWindow::onModeInfoMarkEdit()
{
  getCurrentMapWindow()->getCanvas()->setCanvasMouseMode(MapCanvas::CMM_EDIT_INFOMARKS);
}

void MainWindow::onEditRoomSelection()
{
  if (m_roomSelection)
  {
    RoomEditAttrDlg m_roomEditDialog;
    m_roomEditDialog.setRoomSelection(m_roomSelection, m_mapData, getCurrentMapWindow()->getCanvas());
    m_roomEditDialog.exec();
  }
}

void MainWindow::onEditConnectionSelection()
{

  if (m_connectionSelection)
  {
    /*RoomConnectionsDlg connectionsDlg;
    connectionsDlg.setRoom(static_cast<Room*>(m_connectionSelection->getFirst().room),
    m_mapData,
    static_cast<Room*>(m_connectionSelection->getSecond().room),
    m_connectionSelection->getFirst().direction,
    m_connectionSelection->getSecond().direction);
    connect(&connectionsDlg, SIGNAL(connectionChanged()), getCurrentMapWindow()->getCanvas(), SLOT(update()));

    connectionsDlg.exec();
    */
  }
}

void MainWindow::onDeleteRoomSelection()
{
  if (m_roomSelection)
  {
    m_mapData->execute(new GroupAction(new Remove(), m_roomSelection), m_roomSelection);
    getCurrentMapWindow()->getCanvas()->clearRoomSelection();
    getCurrentMapWindow()->getCanvas()->update();
  }
}

void MainWindow::onDeleteConnectionSelection()
{
  if (m_connectionSelection)
  {
    const Room *r1 = m_connectionSelection->getFirst().room;
    ExitDirection dir1 = m_connectionSelection->getFirst().direction;
    const Room *r2 = m_connectionSelection->getSecond().room;
    ExitDirection dir2 = m_connectionSelection->getSecond().direction;

    if (r2)
    {
      const RoomSelection *tmpSel = m_mapData->select();
      m_mapData->getRoom(r1->getId(), tmpSel);
      m_mapData->getRoom(r2->getId(), tmpSel);

      getCurrentMapWindow()->getCanvas()->clearConnectionSelection();

      m_mapData->execute(new RemoveTwoWayExit(r1->getId(), r2->getId(), dir1, dir2), tmpSel);
      //m_mapData->execute(new RemoveExit(r2->getId(), r1->getId(), dir2), tmpSel);

      m_mapData->unselect(tmpSel);
    }
  }

  getCurrentMapWindow()->getCanvas()->update();
}

void MainWindow::onMoveUpRoomSelection()
{
  if (!m_roomSelection) return;
  Coordinate moverel(0, 0, 1);
  m_mapData->execute(new GroupAction(new MoveRelative(moverel), m_roomSelection), m_roomSelection);
  onLayerUp();
  getCurrentMapWindow()->getCanvas()->update();
}

void MainWindow::onMoveDownRoomSelection()
{
  if (!m_roomSelection) return;
  Coordinate moverel(0, 0, -1);
  m_mapData->execute(new GroupAction(new MoveRelative(moverel), m_roomSelection), m_roomSelection);
  onLayerDown();
  getCurrentMapWindow()->getCanvas()->update();
}

void MainWindow::onMergeUpRoomSelection()
{
  if (!m_roomSelection) return;
  Coordinate moverel(0, 0, 1);
  m_mapData->execute(new GroupAction(new MergeRelative(moverel), m_roomSelection), m_roomSelection);
  onLayerUp();
  onModeRoomSelect();
}

void MainWindow::onMergeDownRoomSelection()
{
  if (!m_roomSelection) return;
  Coordinate moverel(0, 0, -1);
  m_mapData->execute(new GroupAction(new MergeRelative(moverel), m_roomSelection), m_roomSelection);
  onLayerDown();
  onModeRoomSelect();
}

void MainWindow::onConnectToNeighboursRoomSelection()
{
  if (!m_roomSelection) return;
  m_mapData->execute(new GroupAction(new ConnectToNeighbours, m_roomSelection), m_roomSelection);
  getCurrentMapWindow()->getCanvas()->update();
}

void MainWindow::onPlayMode()
{
  QObject::disconnect(m_pathMachine, SIGNAL(createRoom(ParseEvent*, const Coordinate & )), m_mapData, SLOT(createRoom(ParseEvent*, const Coordinate & )));
  QObject::disconnect(m_pathMachine, SIGNAL(scheduleAction(MapAction *)), m_mapData, SLOT(scheduleAction(MapAction *)));
  Config().m_mapMode = 0;
}

void MainWindow::onMapMode()
{
  log("MainWindow","Map mode selected - new rooms are created when entering not mapped area.");
  QObject::connect(m_pathMachine, SIGNAL(createRoom(ParseEvent*, const Coordinate & )), m_mapData, SLOT(createRoom(ParseEvent*, const Coordinate & )));
  QObject::connect(m_pathMachine, SIGNAL(scheduleAction(MapAction *)), m_mapData, SLOT(scheduleAction(MapAction *)));
  Config().m_mapMode = 1;
}

void MainWindow::onOfflineMode()
{
  log("MainWindow","Offline emulation mode selected - you can learn areas 'safe' way.");
  QObject::disconnect(m_pathMachine, SIGNAL(createRoom(ParseEvent*, const Coordinate & )), m_mapData, SLOT(createRoom(ParseEvent*, const Coordinate & )));
  QObject::disconnect(m_pathMachine, SIGNAL(scheduleAction(MapAction *)), m_mapData, SLOT(scheduleAction(MapAction *)));
  Config().m_mapMode = 2;
}

#endif
