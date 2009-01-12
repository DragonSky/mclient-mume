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

class QAction;
class QMenu;

class MainWindow:public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow();
    ~MainWindow();

  protected:
    void closeEvent(QCloseEvent *event);

  public slots:
    void start();

  private slots:
    void changeConfiguration();

    void setCurrentProfile(const QString &profile);

  private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();

    QString currentProfile;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *settingsMenu;

    QToolBar *editToolBar;
    QToolBar *connectToolBar;
};

#endif
