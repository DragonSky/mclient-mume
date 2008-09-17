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
#include <QFileDialog>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "configuration.h"

#include "inputbar.h"
#include "textview.h"
#include "wrapper.h"
#include "objecteditor.h"
#include "configdialog.h"
#include "profiledialog.h"
#include "profilemanagerdialog.h"

MainWindow::MainWindow(int argc, char **argv)
{
  setWindowIcon(QIcon(":/icons/m.png"));

  QVBoxLayout *vbox = new QVBoxLayout();
  vbox->setSpacing(0);
  vbox->setContentsMargins(0, 0, 0, 0);

  inputBar = new InputBar(this);
  textView = new TextView(inputBar, this);
  wrapper = new Wrapper(inputBar, textView, this);

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

void MainWindow::aboutmClient()
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

void MainWindow::help()
{
  if (!QDesktopServices::openUrl(QUrl::fromEncoded("http://mume.org/wiki/index.php/mClient_Help")))
    qDebug("Failed to open web browser");
}

void MainWindow::documentWasModified()
{
  setWindowModified(true);
}

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
  newAct = new QAction(QIcon(":/filenew.xpm"), tr("&New"), this);
  newAct->setStatusTip(tr("Create a new file"));
  connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

  openAct = new QAction(QIcon(":/fileopen.xpm"), tr("&Open..."), this);
  openAct->setShortcut(tr("Ctrl+O"));
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  saveAct = new QAction(QIcon(":/filesave.xpm"), tr("&Save"), this);
  saveAct->setShortcut(tr("Ctrl+S"));
  saveAct->setStatusTip(tr("Save the document to disk"));
  connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

  saveAsAct = new QAction(tr("Save &As..."), this);
  saveAsAct->setStatusTip(tr("Save the document under a new name"));
  connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcut(tr("Ctrl+Q"));
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  cutAct = new QAction(QIcon(":/editcut.xpm"), tr("Cu&t"), this);
  cutAct->setShortcut(tr("Ctrl+X"));
  cutAct->setStatusTip(tr("Cut the current selection's contents to the "
      "clipboard"));
  connect(cutAct, SIGNAL(triggered()), textView, SLOT(cut()));

  copyAct = new QAction(QIcon(":/editcopy.xpm"), tr("&Copy"), this);
  copyAct->setShortcut(tr("Ctrl+C"));
  copyAct->setStatusTip(tr("Copy the current selection's contents to the "
      "clipboard"));
  connect(copyAct, SIGNAL(triggered()), textView, SLOT(copy()));

  pasteAct = new QAction(QIcon(":/editpaste.xpm"), tr("&Paste"), this);
  pasteAct->setShortcut(tr("Ctrl+V"));
  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
      "selection"));
  connect(pasteAct, SIGNAL(triggered()), inputBar, SLOT(paste()));

  objectAct = new QAction(tr("&Object Editor"), this);
  objectAct->setStatusTip(tr("Edit Powwow objects such as aliases and actions"));
  connect(objectAct, SIGNAL(triggered()), this, SLOT(editObjects()));
  
  profileAct = new QAction(tr("Profile &Manager"), this);
  profileAct->setStatusTip(tr("Manage mClient profile settings"));
  connect(profileAct, SIGNAL(triggered()), this, SLOT(manageProfiles()));

  settingsAct = new QAction(tr("&Preferences"), this);
  settingsAct->setStatusTip(tr("Change mClient settings"));
  connect(settingsAct, SIGNAL(triggered()), this, SLOT(changeConfiguration()) );

  helpAct = new QAction(tr("&Help Contents"), this);
  helpAct->setStatusTip(tr("View the mClient/Powwow help file"));
  connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));

  aboutmClientAct = new QAction(tr("&About mClient"), this);
  aboutmClientAct->setStatusTip(tr("Show the application's About box"));
  connect(aboutmClientAct, SIGNAL(triggered()), this, SLOT(aboutmClient()));

  aboutQtAct = new QAction(tr("About &Qt"), this);
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
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  fileMenu->addAction(saveAct);
  fileMenu->addAction(saveAsAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(cutAct);
  editMenu->addAction(copyAct);
  editMenu->addAction(pasteAct);

  settingsMenu = menuBar()->addMenu(tr("&Settings"));
  settingsMenu->addAction(objectAct);
  settingsMenu->addAction(profileAct);
  settingsMenu->addAction(settingsAct);

  menuBar()->addSeparator();

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(helpAct);
  helpMenu->addSeparator();
  helpMenu->addAction(aboutmClientAct);
  helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
  fileToolBar = addToolBar(tr("File"));
  fileToolBar->addAction(newAct);
  fileToolBar->addAction(openAct);
  fileToolBar->addAction(saveAct);

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
}

void MainWindow::writeSettings()
{
  Config().setWindowPosition(pos() );
  Config().setWindowSize(size() );
}

bool MainWindow::maybeSave()
{
  if (textView->document()->isModified()) {
    int ret = QMessageBox::warning(this, tr("mClient"),
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

void MainWindow::setCurrentProfile(const QString &profile)
{
  currentProfile = profile;
  //textView->document()->setModified(false);
  setWindowModified(false);

  QString shownName;
  if (currentProfile.isEmpty())
    shownName = "Undefined";
  else
    shownName = strippedName(currentProfile);

  setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("mClient")));
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
  //create cSession object and related stuff
  //int s = cSessionManager::self()->addSession (true);   //TODO
  wrapper->loadProfile(name);
}
