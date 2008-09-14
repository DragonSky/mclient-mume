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

#include <QMainWindow>
#include <QCloseEvent>

class QAction;
class QMenu;
class InputBar;
class TextView;
class Wrapper;
class ObjectEditor;
class ConfigDialog;
class ProfileDialog;

class MainWindow:public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow(int argc, char **argv);
    ~MainWindow();

  protected:
    void closeEvent(QCloseEvent *event);

  private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void aboutmClient();
    void aboutPowwow();
    void documentWasModified();
    void help();

    void editObjects();
    void changeConfiguration();
    void sendUserInput();
    void sendUserBind(const QString&);

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
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    void selectProfile();

    ProfileDialog *profileDialog;
    ObjectEditor *objectEditor;
    Wrapper *wrapper;
    TextView *textView;
    InputBar *inputBar;
    QString curFile;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *settingsMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *helpAct;
    QAction *aboutmClientAct;
    QAction *aboutPowwowAct;
    QAction *aboutQtAct;

    QAction *objectAct;
    QAction *settingsAct;
};

#endif
