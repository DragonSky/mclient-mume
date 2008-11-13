#include "SocketDataFilter.h"

#include "MClientEvent.h"
#include "PluginManager.h"

#include <QApplication>
#include <QDebug>
#include <QStringList>
#include <QVariant>

Q_EXPORT_PLUGIN2(socketdatafilter, SocketDataFilter)


SocketDataFilter::SocketDataFilter(QObject* parent) 
        : MClientFilterPlugin(parent) {
//    _libName = "libsocketdatafilter.so";
    _shortName = "socketdatafilter";
    _longName = "Socket Data Filter";
    _description = "A socket data filter.";
    _dependencies.insert("some_stupid_api", 10);
    _implemented.insert("some_other_api",1);
    _dataTypes << "SocketData";
}


SocketDataFilter::~SocketDataFilter() {
    stopAllSessions();
}


void SocketDataFilter::customEvent(QEvent* e) {
    qDebug() << "SocketDataFilter got an event";

    if(e->type() != 10001) {
        qDebug() << "Somehow received the wrong kind of event...";
    } else {
        qDebug() << "Doing some crap with the event";
        MClientEvent* me = static_cast<MClientEvent*>(e);
        qDebug() << "Doing some crap with the event";

        // Shouldn't have to do this test
        bool found = false;
        QString s;
        QStringList types = me->dataTypes();
        foreach(s,types) {
            if(_dataTypes.contains(s)) {
                found = true;
                break;
            }
        }
        //me->shared()->refs();
      
        // Send out a FilteredData
        QByteArray ba = me->payload()->toByteArray();
        ba.append("monkey!"); //filtering ;)
        
        QVariant* qv = new QVariant(ba);
        QStringList sl("FilteredData");
        
        postEvent(qv, sl, me->session());
        qDebug() << "posted FilteredData!";
    }
}


void SocketDataFilter::configure() {
}


const bool SocketDataFilter::loadSettings() {
    return true;
}


const bool SocketDataFilter::saveSettings() const {
    return true;
}


const bool SocketDataFilter::startSession(QString s) {
    _runningSessions << s;
    return true;
}


const bool SocketDataFilter::stopSession(QString s) {
    int removed =  _runningSessions.removeAll(s);
    return removed!=0?true:false;
}
