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
#include <QMenuBar>
//#include <QToolBar>

#include "internaleditor.h"

InternalEditor::InternalEditor(QString file, editsess* s, QWidget *parent) : QDialog(parent, Qt::Window)
{
  editSession = s;
  fileName = file;

  qDebug("Internal Editor running, PID %d", s->pid);

  resize(QSize(400, 400));
  setWindowTitle("mClient Local Editor");

  QVBoxLayout *vbox = new QVBoxLayout(this);
  vbox->setSpacing(0);
  vbox->setContentsMargins(0, 0, 0, 0);

  QHBoxLayout *hbox = new QHBoxLayout;
  hbox->setSpacing(0);
  hbox->setContentsMargins(0, 0, 0, 0);

  editToolBar = new QToolBar(tr("Edit"), this);
  fileToolBar = new QToolBar(tr("File"), this);
  textEdit = new QTextEdit;
  buttonBox = new QDialogButtonBox();
  statusBar = new QStatusBar(this);
  menuBar = new QMenuBar(this);

  /*
  boldButton = new QPushButton(QIcon(":/editor/bold.png"), tr(""));
  boldButton->setToolTip(tr("Bold"));
  //boldButton->setFlat(true);
  //boldButton->setToggleButton(true);
  buttonBox->addButton(boldButton, QDialogButtonBox::ActionRole);
  */

  vbox->addLayout(hbox);
  vbox->setMenuBar(menuBar);
  //vbox->addWidget(buttonBox);
  vbox->addWidget(textEdit);
  vbox->addWidget(statusBar);

  hbox->addWidget(editToolBar);
  hbox->addWidget(fileToolBar);


  createActions();
  createMenus();
  createToolBars();
  createStatusBar();

  connect(buttonBox, SIGNAL(rejected() ), this, SLOT(close() ));
  connect(buttonBox, SIGNAL(accepted() ), this, SLOT(close() ));
}

InternalEditor::~InternalEditor() {}

void InternalEditor::createActions()
{
  exitAct = new QAction(QIcon(":/crystal/exit.png"), tr("E&xit"), this);
  exitAct->setStatusTip(tr("Exit the editor"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  cutAct = new QAction(QIcon(":/editcut.png"), tr("Cu&t"), this);
  cutAct->setShortcut(tr("Ctrl+X"));
  cutAct->setStatusTip(tr("Cut the current selection's contents to the "
      "clipboard"));
  connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

  copyAct = new QAction(QIcon(":/editcopy.png"), tr("&Copy"), this);
  copyAct->setShortcut(tr("Ctrl+C"));
  copyAct->setStatusTip(tr("Copy the current selection's contents to the "
      "clipboard"));
  connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

  pasteAct = new QAction(QIcon(":/editpaste.png"), tr("&Paste"), this);
  pasteAct->setShortcut(tr("Ctrl+V"));
  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
      "selection"));
  connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

  helpAct = new QAction(QIcon(":/crystal/help.png"), tr("&Help"), this);
  helpAct->setStatusTip(tr("View the mClient/Powwow help file"));
  connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));

  aboutAct = new QAction(QIcon(":/icons/m.png"), tr("About &mClient"), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  aboutQtAct = new QAction(QIcon(":/crystal/qt.png"), tr("About &Qt"), this);
  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  cutAct->setEnabled(false);
  copyAct->setEnabled(false);
  connect(textEdit, SIGNAL(copyAvailable(bool)),
          cutAct, SLOT(setEnabled(bool)));
  connect(textEdit, SIGNAL(copyAvailable(bool)),
          copyAct, SLOT(setEnabled(bool)));
}

void InternalEditor::createMenus()
{
  fileMenu = menuBar->addMenu(tr("&File"));
  /*
  fileMenu->addAction(connectAct);
  fileMenu->addAction(disconnectAct);
  fileMenu->addAction(reconnectAct);
  */
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  editMenu = menuBar->addMenu(tr("&Edit"));
  editMenu->addAction(cutAct);
  editMenu->addAction(copyAct);
  editMenu->addAction(pasteAct);

  helpMenu = menuBar->addMenu(tr("&Help"));
  helpMenu->addAction(helpAct);
  helpMenu->addSeparator();
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);
}

void InternalEditor::createToolBars()
{
  fileToolBar->addAction(exitAct);

  editToolBar->addAction(cutAct);
  editToolBar->addAction(copyAct);
  editToolBar->addAction(pasteAct);
}

void InternalEditor::createStatusBar()
{
  statusBar->showMessage(tr("Ready"));
}

bool InternalEditor::maybeSave()
{
  if (textEdit->document()->isModified()) {
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
  return false;
}

/*
void InternalEditor::loadFile(const QString &fileName)
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
  //textEdit->setText(in.readAll());
  QString input = in.readAll();
  textEdit->addText(input);
  QApplication::restoreOverrideCursor();

 setCurrentProfile(fileName);
  statusBar()->showMessage(tr("File loaded"), 2000);
}
*/

bool InternalEditor::saveFile(const QString &fileName)
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
  out << textEdit->toPlainText();
  QApplication::restoreOverrideCursor();

  //setCurrentProfile(fileName);
  statusBar->showMessage(tr("File saved"), 2000);
  return true;
}

void InternalEditor::closeEvent(QCloseEvent *event)
{
  if (maybeSave()) {
    //writeSettings();
    event->accept();
  } else {
    event->ignore();
  }
}

bool InternalEditor::save()
{
  if (fileName.isEmpty()) {
    return saveAs();
  } else {
    return saveFile(fileName);
  }
}

bool InternalEditor::saveAs()
{
  QString fileName = QFileDialog::getSaveFileName(this);
  if (fileName.isEmpty())
    return false;

  return saveFile(fileName);
}

void InternalEditor::help()
{
  if (!QDesktopServices::openUrl(QUrl::fromEncoded("http://mume.org/wiki/index.php/mClient_Help")))
    qDebug("Failed to open web browser");
}

void InternalEditor::about()
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
