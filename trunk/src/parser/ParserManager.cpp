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

#include "ParserManager.h"

#include "configuration.h"
#include "MainWindow.h"
#include "MapperManager.h"
#include "GroupManager.h"
#include "PowwowWrapper.h"

#include "connectionlistener.h"
#include "proxy.h"
#include "telnetfilter.h"
#include "parser.h"
#include "mumexmlparser.h"

#include "CGroup.h"
#include "mmapper2pathmachine.h"
#include "prespammedpath.h"

ParserManager *ParserManager::_self = 0;

ParserManager *ParserManager::self(MainWindow *parent)
{
  if (_self == 0) {
    _self = new ParserManager(parent);
    qDebug("Parser Manager created.");
  }
  return _self;
}

ParserManager::ParserManager(MainWindow *parent) : QObject(parent) {
  _mainWindow = parent;
  PowwowWrapper *wrapper = PowwowWrapper::self();
  MapperManager *mapMgr = MapperManager::self();
  GroupManager *grpMgr = GroupManager::self();

  ConnectionListener *_server = new ConnectionListener(this);
  _server->setMaxPendingConnections (1);
  _server->setRemoteHost(Config().m_remoteServerName);
  _server->setRemotePort(Config().m_remotePort);

  _filter = new TelnetFilter(this);
  _xmlParser = new MumeXmlParser(mapMgr->getMapData(), this);
  _parser = new Parser(mapMgr->getMapData(), this);
  qDebug("Telnet Filter and Parsers Created");

  // PowwowWrapper <--> TelnetFilter
  connect(wrapper, SIGNAL(analyzeUserStream( const char*, const int& )),
          _filter, SLOT(analyzeUserStream( const char*, int )));
  connect(wrapper, SIGNAL(analyzeMudStream( const char*, const int& )),
          _filter, SLOT(analyzeMudStream( const char*, int )));
  connect(_filter, SIGNAL(sendToMud(const QByteArray&)),
          wrapper, SLOT(sendToMud(const QByteArray&)));
  connect(_filter, SIGNAL(sendToUser(const QByteArray&)),
          wrapper, SLOT(sendToUser(const QByteArray&)));
  qDebug("Wrapper connected with TelnetFilter");

  // TelnetFilter <--> Parser, MumeXmlParser
  connect(_filter, SIGNAL(parseNewMudInput(IncomingData&)), _parser, SLOT(parseNewMudInput(IncomingData&)));
  connect(_filter, SIGNAL(parseNewUserInput(IncomingData&)), _parser, SLOT(parseNewUserInput(IncomingData&)));
  connect(_filter, SIGNAL(parseNewMudInputXml(IncomingData&)), _xmlParser, SLOT(parseNewMudInput(IncomingData&)));
  connect(_filter, SIGNAL(parseNewUserInputXml(IncomingData&)), _xmlParser, SLOT(parseNewUserInput(IncomingData&)));
  connect(_parser, SIGNAL(setXmlMode()), _filter, SLOT(setXmlMode()));
  connect(_xmlParser, SIGNAL(setNormalMode()), _filter, SLOT(setNormalMode()));
  qDebug("TelnetFilter <--> Parser, MumeXmlParser");

  // Parser, MumeXmlParser --> Proxy, PowwowWrapper
  connect(_parser, SIGNAL(sendToMud(const QByteArray&)), wrapper, SLOT(sendToMud(const QByteArray&)));
  connect(_parser, SIGNAL(sendToUser(const QByteArray&)), wrapper, SLOT(sendToUser(const QByteArray&)));
  connect(_xmlParser, SIGNAL(sendToMud(const QByteArray&)), wrapper, SLOT(sendToMud(const QByteArray&)));
  connect(_xmlParser, SIGNAL(sendToUser(const QByteArray&)), wrapper, SLOT(sendToUser(const QByteArray&)));
  qDebug("Parser, MumeXmlParser --> Proxy, PowwowWrapper");
  
  // Parser, MumeXmlParser --> MapperManager
  connect(_parser, SIGNAL(event(ParseEvent* )),
          mapMgr->getPathMachine(), SLOT(event(ParseEvent* )), Qt::QueuedConnection);
  connect(_parser, SIGNAL(releaseAllPaths()),
          mapMgr->getPathMachine(), SLOT(releaseAllPaths()), Qt::QueuedConnection);
  connect(_parser, SIGNAL(showPath(CommandQueue, bool)),
          mapMgr->getPrespammedPath(), SLOT(setPath(CommandQueue, bool)), Qt::QueuedConnection);
  connect(_xmlParser, SIGNAL(event(ParseEvent* )),
          mapMgr->getPathMachine(), SLOT(event(ParseEvent* )), Qt::QueuedConnection);
  connect(_xmlParser, SIGNAL(releaseAllPaths()),
          mapMgr->getPathMachine(), SLOT(releaseAllPaths()), Qt::QueuedConnection);
  connect(_xmlParser, SIGNAL(showPath(CommandQueue, bool)),
          mapMgr->getPrespammedPath(), SLOT(setPath(CommandQueue, bool)), Qt::QueuedConnection);
  qDebug("Parser, MumeXmlParser --> MapperManager");

  // MumeXmlParser --> GroupManager
  connect(_xmlParser, SIGNAL(sendScoreLineEvent(QByteArray)),
          grpMgr->getGroup(), SLOT(parseScoreInformation(QByteArray)), Qt::QueuedConnection);
  connect(_xmlParser, SIGNAL(sendPromptLineEvent(QByteArray)),
          grpMgr->getGroup(), SLOT(parsePromptInformation(QByteArray)), Qt::QueuedConnection);
  connect(_xmlParser, SIGNAL(sendGroupTellEvent(QByteArray)),
          grpMgr->getGroup(), SLOT(sendGTell(QByteArray)), Qt::QueuedConnection);
  connect(grpMgr->getGroup(), SIGNAL(displayGroupTellEvent(const QByteArray&)),
          _xmlParser, SLOT(sendToUser(const QByteArray&)), Qt::QueuedConnection);

  qDebug("MumeXmlParser --> GroupManager");

  qDebug("Parser Manager Created");
}

ParserManager::~ParserManager() {
  _self = 0;
}
