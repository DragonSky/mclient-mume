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
#include "profiledialog.h"
#include "profilemanagerdialog.h"

// Currently mClient only allows one session
#define DEFAULT_SESSION 0

MainWindow::MainWindow(int argc, char **argv)
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

  connect(wrapper, SIGNAL(addText(QString&)), textView, SLOT(addText(QString&)) );
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

  // Initialize Powwow
  wrapper->start(argc, argv);

  // Display Main Window
  show();

  if (argc == 1)
    selectProfile();

  inputBar->setFocus();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if (maybeSave()) {
    writeSettings();
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
}

void MainWindow::createMenus()
{
  connectMenu = menuBar()->addMenu(tr("&Connection"));
  connectMenu->addAction(connectAct);
  connectMenu->addAction(disconnectAct);
  connectMenu->addAction(reconnectAct);
  connectMenu->addSeparator();
  connectMenu->addAction(exitAct);

  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(cutAct);
  editMenu->addAction(copyAct);
  editMenu->addAction(pasteAct);

  viewMenu = menuBar()->addMenu(tr("&View"));
  viewMenu->addAction(alwaysOnTopAct);

  settingsMenu = menuBar()->addMenu(tr("&Settings"));
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
  Config().setAlwaysOnTop((bool)(windowFlags() & Qt::WindowStaysOnTopHint));
  Config().write();
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
      return true;
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
  QString name = profileDialog->selectedProfile();
  qDebug("Got signal to start new session %s", name.toAscii().constData());
  cProfileManager *mgr = cProfileManager::self();

  //create cSession object and related stuff
  //int s = cSessionManager::self()->addSession (true);   //TODO
  
  // Check if there is a loaded profile
  if (mgr->sessionAssigned(DEFAULT_SESSION)) {
    Wrapper::self()->disconnectSession();
    Wrapper::self()->clearPowwowMemory();
  }
  wrapper->loadProfile(name);
}
