#include "MMapperPluginParser.h"

#include "MMapperPlugin.h"
#include "MapperManager.h"

#include "mapdata.h"
#include "mmapper2pathmachine.h"
#include "prespammedpath.h"

#include "patterns.h" // fog, etc

MMapperPluginParser::MMapperPluginParser(QString s, MMapperPlugin *plugin,
					 MapperManager *mapMgr,
					 QObject *parent)
  : AbstractParser(mapMgr->getMapData(), parent) {
  _session = s;

  connect(this, SIGNAL(event(ParseEvent* )),
          mapMgr->getPathMachine(),
	  SLOT(event(ParseEvent* )), Qt::QueuedConnection);
  connect(this, SIGNAL(releaseAllPaths()),
          mapMgr->getPathMachine(),
	  SLOT(releaseAllPaths()), Qt::QueuedConnection);
  connect(this, SIGNAL(showPath(CommandQueue, bool)),
          mapMgr->getPrespammedPath(),
	  SLOT(setPath(CommandQueue, bool)), Qt::QueuedConnection);

  connect(plugin, SIGNAL(name(const QString&)),
	  SLOT(name(const QString&)));
  connect(plugin, SIGNAL(description(const QString&)),
	  SLOT(description(const QString&)));
  connect(plugin, SIGNAL(dynamicDescription(const QString&)),
	  SLOT(dynamicDescription(const QString&)));
  connect(plugin, SIGNAL(exits(const QString&)),
	  SLOT(exits(const QString&)));
  connect(plugin, SIGNAL(prompt(const QString&)),
	  SLOT(prompt(const QString&)));
  connect(plugin, SIGNAL(move(const QString&)),
	  SLOT(move(const QString&)));
  
  connect(plugin, SIGNAL(userInput(QString)),
	  SLOT(userInput(QString)));
  connect(plugin, SIGNAL(mudOutput(const QString&)),
	  SLOT(mudOutput(const QString&)));

  connect(this, SIGNAL(sendToUser(const QByteArray&)),
	  SLOT(sendToUserWrapper(const QByteArray&)));

  connect(this, SIGNAL(sendToUser(const QString&)),
 	  plugin, SLOT(displayMessage(const QString&)));

  _move = CID_NONE;

  qDebug() << "MMapperPluginParser loaded";
}


MMapperPluginParser::~MMapperPluginParser() {
}


void MMapperPluginParser::name(QString text) {
  removeAnsiMarks(text);
  m_roomName = text.trimmed();
}


void MMapperPluginParser::description(const QString &text) {
  if (m_descriptionReady) submit();

  m_staticRoomDesc = text;
  m_descriptionReady = true;
  m_readingRoomDesc = true;
}


void MMapperPluginParser::dynamicDescription(const QString &text) {
  m_dynamicRoomDesc = text;
}


void MMapperPluginParser::exits(QString text) {
  m_readingRoomDesc = false; // to identify if we saw exits or not
  parseExits(text);
}


void MMapperPluginParser::prompt(QString text) {
  if (m_readingRoomDesc) {
    emulateExits();
    m_readingRoomDesc = false;
  }

  parsePrompt(text);

  if (m_descriptionReady) submit();
}


void MMapperPluginParser::move(QString text) {
  switch (text.at(0).toAscii()) {
    case 'n':
      _move = CID_NORTH;
      break;
    case 's':
      _move = CID_SOUTH;
      break;
    case 'e':
      _move = CID_EAST;
      break;
    case 'w':
      _move = CID_WEST;
      break;
    case 'u':
      switch (text.at(1).toAscii()) {
      case 'p':
	_move = CID_UP;
	break;
      case 'n':
      default:
	_move = CID_NONE;
	break;
      };
      break;
    case 'd':
      _move = CID_DOWN;
      break;
    default:
      _move = CID_NONE;
    };
}


void MMapperPluginParser::submit() {
  m_descriptionReady = false;
  
  // non standard end of description parsed (fog, dark or so ...)
  if (Patterns::matchNoDescriptionPatterns(m_roomName)) {
    qDebug() << "* MMapperPluginParser detected something wrong";
    m_roomName = nullString;
    m_dynamicRoomDesc = nullString;
    m_staticRoomDesc = nullString;
  }
  
  if (!queue.isEmpty()) {
    CommandIdType c = queue.dequeue();
    if ( c != CID_SCOUT ) {
      emit showPath(queue, false);
      characterMoved(_move, m_roomName, m_dynamicRoomDesc, m_staticRoomDesc, m_exitsFlags, m_promptFlags);
      if (c != _move)
        queue.clear();
      _move = CID_LOOK;
    }

  } else {
    //emit showPath(queue, false);
    characterMoved(_move, m_roomName, m_dynamicRoomDesc, m_staticRoomDesc, m_exitsFlags, m_promptFlags);
    _move = CID_LOOK;
  }
}


void MMapperPluginParser::userInput(QString text) {
  qDebug() << "* MMapperPluginParser got input" << text << thread();
  parseUserCommands(text);
}


void MMapperPluginParser::mudOutput(const QString &text) {
  if (text.startsWith('Y')) {
    qDebug() << "* MMapperPluginParser detected some text" << text;

    if (text.startsWith("You are dead!")) {
      queue.clear();
      emit showPath(queue, true);
      emit releaseAllPaths();
      return;
    }
    else if (text.startsWith("You failed to climb")) {
      if(!queue.isEmpty()) queue.dequeue();
      queue.prepend(CID_NONE);
      emit showPath(queue, true);
      return;
    }
    else if (text.startsWith("You flee head")) {
      queue.enqueue(_move);
    }
    else if (text.startsWith("You follow")) {
      queue.enqueue(_move);
      return;
    }
    else if (text.startsWith("You quietly scout")) {
      queue.prepend(CID_SCOUT);
      return;
    }
  }
}


void MMapperPluginParser::sendToUserWrapper(const QByteArray &ba) {
  QString text(ba);
  emit sendToUser(text);
}


void MMapperPluginParser::sendToMudWrapper(const QByteArray &ba) {
  QString text(ba);
  emit sendToMud(text);
}