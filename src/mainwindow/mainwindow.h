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

//#include <tr1/memory>
//using std::tr1::shared_ptr;

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

  protected:
    void closeEvent(QCloseEvent *event);

  public slots:
    void log(const QString&, const QString&);

  private slots:
    void selectProfile();
    void editObjects();
    void manageProfiles();
    void changeConfiguration();

    void profileSelected();
    void setCurrentProfile(const QString &profile);

  private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();

    QTextEdit *logWindow;
    QDockWidget *dockDialogLog;
    ProfileManagerDialog *profileManager;
    ProfileDialog *profileDialog;
    ObjectEditor *objectEditor;
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

#ifdef MMAPPER
    QDockWidget *dockDialogMapper;
    QDockWidget *m_dockDialogGroup;

    QMenu *mapperMenu;
    QMenu *roomMenu;
    QMenu *connectionMenu;
    QMenu *searchMenu;
    QMenu *groupMenu;
#endif
};

#endif
