#include "EventHandler.h"

#include <QEvent>
#include <QVariant>
#include <QDebug>
#include <QCoreApplication> // for sending command

#include "MClientEvent.h"

#include "PluginSession.h"
#include "CommandProcessor.h"

#include "MapperManager.h"
#include "mapwindow.h" // for grabbing the QWidget
#include "mmapper2pathmachine.h" // for menus
#include "mapcanvas.h" // for menus

// for menus
#include "SmartMenu.h"
#include <QAction>

EventHandler::EventHandler(PluginSession *ps, MClientPlugin *mp)
  : MClientDisplayHandler(ps, mp) {
  // Allowable Display Locations
  SET(_displayLocations, DL_FLOAT);
}


EventHandler::~EventHandler() {
  _mapper->getMapWindow()->close();
  _mapper->deleteLater();
  qDebug() << "* removed MMapperPlugin for session"
	   << _pluginSession->session();
}


void EventHandler::customEvent(QEvent *e) {
  if(e->type() == 10000)
    engineEvent(e);

  else {
    MClientEvent* me = static_cast<MClientEvent*>(e);
    
    if (me->dataTypes().contains("XMLAll")) {
      
      bool display = true;
      if (me->dataTypes().contains("XMLExits")) {
	emit exits(me->payload()->toString());

      } else if (me->dataTypes().contains("XMLNone")) {
	emit mudOutput(me->payload()->toString());
	  
      } else if (me->dataTypes().contains("XMLName")) {
	emit name(me->payload()->toString());

      } else if (me->dataTypes().contains("XMLDescription")) {
	emit description(me->payload()->toString());

      } else if (me->dataTypes().contains("XMLDynamicDescription")) {
	emit dynamicDescription(me->payload()->toString());
	  
      } else if (me->dataTypes().contains("XMLPrompt")) {
	emit prompt(me->payload()->toString());
	  	  
      } else if (me->dataTypes().contains("XMLTerrain")) {
	emit terrain(me->payload()->toString());
       
      }

      if (display) {
	// These tags get forwarded to the CommandProcessor
	MClientEvent* nme = new MClientEvent(*me);
	QCoreApplication::postEvent(_pluginSession->
				    getCommand()->
				    getAction(), nme);
	//qDebug() << "* forwarding to CommandProcessor";
	
      }

    }
    else if (me->dataTypes().contains("XMLMove")) {
      emit move(me->payload()->toString());
    
    }
    else if(me->dataTypes().contains("MMapperInput")) {
      emit userInput(me->payload()->toString());
      
    }
    else if (me->dataTypes().contains("MMapperLoadMap")) {
      QString arguments = me->payload()->toString();
      if (arguments.isEmpty())
	displayMessage("#no file specified\n");
      else if (arguments == "!") // hack
	emit loadFile("/mnt/games/powwow/z263-264.mm2");
      else
	emit loadFile(arguments);
      
    }
    else if (me->dataTypes().contains("SocketConnected")) {
      emit onPlayMode();
      
    }
    else if (me->dataTypes().contains("SocketDisconnected")) {
      emit onOfflineMode();
      
    } 
  }
}


QWidget* EventHandler::createWidget() {
  _mapper = new MapperManager(this);
  //_mapper->start();
  return _mapper->getMapWindow();
}


void EventHandler::displayMessage(const QByteArray& message) {
  QVariant* qv = new QVariant(message);
  QStringList sl("DisplayData");
  postSession(qv, sl);
}


void EventHandler::log(const QString& s, const QString& message) {
  qDebug() << "* MMapperPlugin[" << s << "]: " << message;
}


void EventHandler::postCommand(const QByteArray &input) {
  qDebug() << "* MmapperPlugin submitting command" << input;
  QVariant *payload = new QVariant(input);
  QStringList tags("UserInput");
  MClientEventData *med = new MClientEventData(payload, tags,
					       _pluginSession->session());
  MClientEvent* me = new MClientEvent(med);
  QCoreApplication::postEvent(_pluginSession->getCommand()->getUserInput(),
			      me);

}


const MenuData& EventHandler::createMenus() {
  // Mapper actions
  QAction *zoomInAct;
  QAction *zoomOutAct;

  zoomInAct = new QAction(QIcon(":/icons/viewmag+.png"), tr("Zoom &In"), 0);
  zoomInAct->setStatusTip(tr("Zooms In current map"));
  connect(zoomInAct, SIGNAL(triggered()), _mapper->getMapWindow()->getCanvas(), SLOT(zoomIn()));

  zoomOutAct = new QAction(QIcon(":/icons/viewmag-.png"), tr("Zoom &Out"), 0);  zoomOutAct->setStatusTip(tr("Zooms Out current map"));
  connect(zoomOutAct, SIGNAL(triggered()), _mapper->getMapWindow()->getCanvas(), SLOT(zoomOut()));

  SmartMenu *viewMenu = new SmartMenu(tr("&View"), 10, 2);
  
  viewMenu->addSeparator();
  viewMenu->addAction(zoomInAct);
  viewMenu->addAction(zoomOutAct);

  _menus.insert(viewMenu);

  return _menus;
}
