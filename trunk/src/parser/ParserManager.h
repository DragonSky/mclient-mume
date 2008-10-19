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

#ifndef _PARSERMANAGER_H_
#define _PARSERMANAGER_H_

#include <QObject>

class MainWindow;
class ConnectionListener;
class TelnetFilter;
class Parser;
class MumeXmlParser;
class Proxy;

class ParserManager: public QObject {
  Q_OBJECT

  public:
    static ParserManager *self (MainWindow *parent = 0);
    ~ParserManager ();

    ConnectionListener *getListener() { return _server; }
    Proxy *getProxy() { return _proxy; }
    TelnetFilter *getTelnetFilter() { return _filter; }
    Parser *getParser() { return _parser; }
    MumeXmlParser *getXmlParser() { return _xmlParser; }

  private:
    ParserManager (MainWindow *);
    static ParserManager *_self;

    bool _inUse;
    ConnectionListener *_server;
    Proxy *_proxy;
    TelnetFilter *_filter;
    Parser *_parser;
    MumeXmlParser *_xmlParser;
    MainWindow *_mainWindow;
};

#endif /* _PARSERMANAGER_H_ */
