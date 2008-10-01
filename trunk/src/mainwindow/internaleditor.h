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

#ifndef _INTERNALEDITOR_H
#define _INTERNALEDITOR_H

#include <QtGui>

#include "defines.h"

class QAction;
class QMenu;
class QMenuBar;
class QTextEdit;
class QCloseEvent;

class Wrapper;

class InternalEditor : public QDialog
{
  Q_OBJECT

  public:
    InternalEditor(editsess*, Wrapper*, QWidget* parent);
    //virtual ~InternalEditor();
    ~InternalEditor();

  protected:
    void closeEvent(QCloseEvent *event);

  private slots:
    void about();
    void help();

  private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    bool save();
    bool saveAs();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *helpAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QToolBar *editToolBar;
    QToolBar *fileToolBar;

    editsess* editSession;
    QString fileName;

    QPushButton *boldButton, *blinkButton, *underlineButton, *inverseButton;

    QTextEdit *textEdit;
    QDialogButtonBox *buttonBox;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    Wrapper *wrapper;
};


#endif /* _INTERNALEDITOR_H */
