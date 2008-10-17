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

#include "MainWindow.h"
#include "configuration.h"

#include "ClientLineEdit.h"
#include "ClientTextEdit.h"
#include "ObjectEditor.h"
#include "configdialog.h"

#include "cprofilemanager.h"
#include "cprofilesettings.h"
#include "ConnectDialog.h"
#include "ProfileManagerDialog.h"

// Currently mClient only allows one session
#define DEFAULT_SESSION 0

#include "ClientManager.h"
#include "MapperManager.h"
#include "GroupManager.h"
#include "ActionManager.h"

#include "mapwindow.h"
#include "CGroup.h"

MainWindow::MainWindow() {
  setWindowIcon(QIcon(":/icons/m.png"));

  /* Generate Managers */
  ClientManager *clientManager = ClientManager::self(this);
  MapperManager *mapperManager = MapperManager::self(this);
  GroupManager *groupManager = GroupManager::self(this);

  /* Create Central Widget */
  QVBoxLayout *vbox = new QVBoxLayout();
  vbox->setSpacing(0);
  vbox->setContentsMargins(0, 0, 0, 0);
  vbox->addWidget(clientManager->getTextEdit());
  vbox->addWidget(clientManager->getLineEdit());
  QWidget *mainWidget = new QWidget();
  mainWidget->setLayout(vbox);
  setCentralWidget(mainWidget);

  /* Create Dock Widgets */
  dockDialogLog = new QDockWidget(tr("Log"), this);
  dockDialogLog->setObjectName("DockWidgetLog");
  dockDialogLog->setAllowedAreas(Qt::AllDockWidgetAreas);
  dockDialogLog->setFeatures(QDockWidget::AllDockWidgetFeatures);
  addDockWidget(Qt::TopDockWidgetArea, dockDialogLog);
  logWindow = new QTextBrowser(dockDialogLog);
  logWindow->setObjectName("LogWindow");
  dockDialogLog->setWidget(logWindow);

  dockDialogMapper = new QDockWidget(tr("Map"), this);
  dockDialogMapper->setObjectName("DockWidgetMapper");
  dockDialogMapper->setAllowedAreas(Qt::AllDockWidgetAreas);
  dockDialogMapper->setFeatures(QDockWidget::AllDockWidgetFeatures);
  addDockWidget(Qt::LeftDockWidgetArea, dockDialogMapper);
  dockDialogMapper->setWidget(mapperManager->getMapWindow());

  dockDialogGroup = new QDockWidget(tr("Group"), this);
  dockDialogGroup->setObjectName("DockWidgetGroup");
  dockDialogGroup->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
  dockDialogGroup->setFeatures(QDockWidget::AllDockWidgetFeatures);
  addDockWidget(Qt::TopDockWidgetArea, dockDialogGroup);
  dockDialogGroup->setWidget(groupManager->getGroup());

  /* Create Other Child Widgets */
  createActions();
  createMenus();
  createToolBars();
  createStatusBar();

  /* Read Settings */
  readSettings();
  setCurrentProfile("");

  /* Initialize Dialogs */
  objectEditor = NULL;
  profileDialog = NULL;
  profileManager = NULL;

  qDebug("MainWindow created.");
}

void MainWindow::start(int argc, char **argv) {
  // Initialize Powwow
  PowwowWrapper::self()->start(argc, argv);

  // Display Main Window
  show();

  if (argc == 1)
    selectProfile();

  ClientManager::self()->getLineEdit()->setFocus();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  writeSettings();
  if (maybeSave()) {
    event->accept();
  } else {
    event->ignore();
  }
}

void MainWindow::createActions() {
  ActionManager::self(this);
}

void MainWindow::createMenus() {
  ActionManager *actMgr = ActionManager::self();

  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(actMgr->connectAct);
  fileMenu->addAction(actMgr->disconnectAct);
  fileMenu->addAction(actMgr->reconnectAct);
  fileMenu->addSeparator();
  fileMenu->addAction(actMgr->newAct);
  fileMenu->addAction(actMgr->openAct);
  //fileMenu->addAction(reloadAct);
  fileMenu->addAction(actMgr->saveAct);
  fileMenu->addAction(actMgr->saveAsAct);
  //fileMenu->addAction(mergeAct);
  fileMenu->addSeparator();
  fileMenu->addAction(actMgr->exitAct);

  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(actMgr->cutAct);
  editMenu->addAction(actMgr->copyAct);
  editMenu->addAction(actMgr->pasteAct);

  viewMenu = menuBar()->addMenu(tr("&View"));
  viewMenu->addAction(actMgr->alwaysOnTopAct);

  mapperMenu = menuBar()->addMenu(tr("&Mapper"));
  groupMenu = menuBar()->addMenu(tr("&Group") );

  settingsMenu = menuBar()->addMenu(tr("&Settings"));
  viewDockMenu = settingsMenu->addMenu(tr("&Dock"));
  viewDockMenu->addAction(dockDialogLog->toggleViewAction());
  viewDockMenu->addAction(dockDialogGroup->toggleViewAction());
  settingsMenu->addSeparator();
  settingsMenu->addAction(actMgr->objectAct);
  settingsMenu->addAction(actMgr->profileAct);
  settingsMenu->addAction(actMgr->settingsAct);

  menuBar()->addSeparator();

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(actMgr->mumeHelpAct);
  helpMenu->addAction(actMgr->forumAct);
  helpMenu->addAction(actMgr->wikiAct);
  helpMenu->addSeparator();
  helpMenu->addAction(actMgr->clientHelpAct);
  helpMenu->addSeparator();
  helpMenu->addAction(actMgr->aboutAct);
  helpMenu->addAction(actMgr->aboutQtAct);

  roomMenu = mapperMenu->addMenu(tr("&Rooms"));
  connectionMenu = mapperMenu->addMenu(tr("&Connections"));

  mapperMenu->addSeparator();
  mapperMenu->addAction(actMgr->playModeAct);
  mapperMenu->addAction(actMgr->mapModeAct);
  mapperMenu->addAction(actMgr->offlineModeAct);
  mapperMenu->addSeparator();
  mapperMenu->addAction(actMgr->modeRoomSelectAct);
  mapperMenu->addAction(actMgr->modeConnectionSelectAct);
  mapperMenu->addAction(actMgr->modeMoveSelectAct);
  mapperMenu->addAction(actMgr->modeInfoMarkEditAct);

  roomMenu->addAction(actMgr->findRoomsAct);
  roomMenu->addSeparator();
  roomMenu->addAction(actMgr->createRoomAct);
  roomMenu->addAction(actMgr->editRoomSelectionAct);
  roomMenu->addAction(actMgr->deleteRoomSelectionAct);
  roomMenu->addAction(actMgr->moveUpRoomSelectionAct);
  roomMenu->addAction(actMgr->moveDownRoomSelectionAct);
  roomMenu->addAction(actMgr->mergeUpRoomSelectionAct);
  roomMenu->addAction(actMgr->mergeDownRoomSelectionAct);
  roomMenu->addAction(actMgr->connectToNeighboursRoomSelectionAct);

  connectionMenu->addAction(actMgr->createConnectionAct);
  connectionMenu->addAction(actMgr->createOnewayConnectionAct);
  //connectionMenu->addAction(editConnectionSelectionAct);
  connectionMenu->addAction(actMgr->deleteConnectionSelectionAct);

  viewMenu->addSeparator();
  viewMenu->addAction(actMgr->zoomInAct);
  viewMenu->addAction(actMgr->zoomOutAct);
  viewMenu->addSeparator();
  viewMenu->addAction(actMgr->layerUpAct);
  viewMenu->addAction(actMgr->layerDownAct);
  viewMenu->addAction(actMgr->releaseAllPathsAct);
  //    viewMenu->addAction(nextWindowAct);
  //    viewMenu->addAction(prevWindowAct);
  //    viewMenu->addSeparator();

  groupMenu->addAction(actMgr->groupOffAct);
  groupMenu->addAction(actMgr->groupClientAct);
  groupMenu->addAction(actMgr->groupServerAct);
}

void MainWindow::createToolBars() {
  ActionManager *actMgr = ActionManager::self();

  connectToolBar = addToolBar(tr("Connection"));
  connectToolBar->setObjectName("ToolBarConnect");
  connectToolBar->addAction(actMgr->connectAct);
  connectToolBar->addAction(actMgr->disconnectAct);
  connectToolBar->addAction(actMgr->reconnectAct);

  editToolBar = addToolBar(tr("Edit"));
  editToolBar->setObjectName("ToolBarEdit");
  editToolBar->addAction(actMgr->cutAct);
  editToolBar->addAction(actMgr->copyAct);
  editToolBar->addAction(actMgr->pasteAct);
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
  ActionManager::self()->alwaysOnTopAct->setChecked(Config().alwaysOnTop);
  if (Config().alwaysOnTop) {
    ActionManager::self()->alwaysOnTop();
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

void MainWindow::setCurrentProfile(const QString &profile)
{
  currentProfile = profile;
  //textView->document()->setModified(false);
  setWindowModified(false);

  QString shownName;
  if (currentProfile.isEmpty())
    shownName = "No connection";
  else
    shownName = currentProfile;

  setWindowTitle(tr("%1 - mClient").arg(shownName));
}

MainWindow::~MainWindow()
{}

void MainWindow::editObjects() {
  if (!objectEditor) {
    objectEditor = new ObjectEditor(this);
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
  profileDialog = new ConnectDialog(this);
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
    PowwowWrapper::self()->disconnectSession();
    PowwowWrapper::self()->clearPowwowMemory();
  }

  QString fileName = sett->getString("map");
  if (!fileName.isEmpty() && MapperManager::self()->maybeSave()) {
    MapperManager::self()->loadFile(fileName);
  }

  PowwowWrapper::self()->loadProfile(profile);
}

void MainWindow::log(const QString& module, const QString& message)
{
  logWindow->append("[" + module + "] " + message);
  logWindow->update();
}
