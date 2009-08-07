#include "MClientEventHandler.h"

#include "PluginManager.h"
#include "PluginSession.h"
#include "MClientEvent.h"
#include "MClientEngineEvent.h"
#include "MClientEventData.h"

#include <QApplication>
#include <QEvent>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QDebug>

MClientEventHandler::MClientEventHandler(QObject* parent) : QObject(parent) {
}


MClientEventHandler::~MClientEventHandler() {
}


// Post an event
void MClientEventHandler::postSession(QVariant* payload, QStringList tags) {
  bool found = false;
  /*
  foreach (QString s, tags) {
    // Iterate through all the data types
    //qDebug() << "* finding data type" << s << "out of" << me->dataTypes();
    
    QMultiHash<QString, QObject*>::iterator it
      = _receivesTypes.find(s);
    while (it != _receivesTypes.end() && it.key() == s) {
      
      qDebug() << "# posting" << tags << "to" 
	       << it.value() << "with" << payload;
      
      // Post the event
      MClientEventData *med = new MClientEventData(payload, tags,
      _pluginSession->session());
      MClientEvent* me = new MClientEvent(med);
      QCoreApplication::postEvent(it.value(), me);
      found = true;
      
      ++it; // Iterate
    }
  }
  */
  if (!found) {
    qWarning() << "! No EventHandlers accepted data types" << tags;

    qWarning() << "! Rerouting event to session";
    MClientEventData *med = new MClientEventData(payload, tags,
						 _pluginSession->session());
    MClientEvent* me = new MClientEvent(med);
    QCoreApplication::postEvent(_pluginSession, me);

  }
}


void MClientEventHandler::postManager(QVariant* payload, QStringList tags, 
			      QString session) {
  MClientEvent* me = new MClientEvent(new MClientEventData(payload, tags,
							   session));
  
  QApplication::postEvent(_pluginSession->getManager(), me);
}


// Receive the PluginSession reference upon load
void MClientEventHandler::setPluginSession(PluginSession *ps) {
  _pluginSession = ps;
}


// Handles MClientEngineEvent
void MClientEventHandler::engineEvent(QEvent *e) {
  MClientEngineEvent* ee = static_cast<MClientEngineEvent*>(e);
  //qDebug() << "#" << _shortName << "got engineEvent" << ee->dataType();
  if (ee->dataType() == EE_MANAGER_POST) {
    // We should never get one of these
    
  }
  else if (ee->dataType() == EE_DATATYPE_UPDATE) {
    // Contains a hash of receiving EventHandlers we need to post to
    QHash<QString, QVariant> raw = ee->payload()->toHash();
    //qDebug() << raw;
    
    QHash<QString, QObject*> hash;
    QHash<QString, QVariant>::const_iterator i;
    for (i = raw.constBegin(); i != raw.constEnd(); ++i)
      hash.insertMulti(i.key(), i.value().value<QObject*>() );
    
    _receivesTypes.unite(hash);

  }
}