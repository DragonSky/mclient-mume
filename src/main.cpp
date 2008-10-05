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

#include "mainwindow.h"
#include "configuration.h"

#include <QApplication>


int main(int argc, char *argv[])
{
      Q_INIT_RESOURCE(application);
      QApplication app(argc, argv);
      Config().read();
      mainWindow = new MainWindow;
      mainWindow->start(argc, argv);
      /*
      if (Config().m_autoLoadWorld && Config().m_autoLoadFileName!="")
      {
        mw.loadFile(Config().m_autoLoadFileName);
      }
      */
      //mw->show();
      int ret = app.exec();
      return ret;
}
