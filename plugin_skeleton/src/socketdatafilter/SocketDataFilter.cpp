#include "SocketDataFilter.h"

#include "MClientEvent.h"
#include "PluginManager.h"
#include "SocketData.h"

#include <QApplication>
#include <QDebug>
#include <QStringList>
#include <QVariant>

#include <algorithm>
using std::copy;

Q_EXPORT_PLUGIN2(socketdatafilter, SocketDataFilter)


SocketDataFilter::SocketDataFilter(QObject* parent) 
        : MClientFilterPlugin(parent) {
    _libName = "libsocketdatafilter.so";
    _shortName = "socketdatafilter";
    _longName = "Socket Data Filter";
    _description = "A socket data filter.";
    _dependencies.insert("some_stupid_api", 10);
    _implemented.insert("some_other_api",1);
    _dataTypes << "SocketData";
}


SocketDataFilter::~SocketDataFilter() {
}


void SocketDataFilter::customEvent(QEvent* e) {
    qDebug() << "SocketDataFilter got an event";

    if(e->type() != 10001) {
        qDebug() << "Somehow received the wrong kind of event...";
    } else {
        qDebug() << "Doing some crap with the event";
        MClientEvent* me = static_cast<MClientEvent*>(e);

        bool found = false;
        QString s;
        QStringList types = me->dataTypes();
//        qDebug() << types.size();
        foreach(s,types) {
//            qDebug() << "string s" << s;
            if(_dataTypes.contains(s)) {
                found = true;
//                qDebug() << "found" << s;
                break;
            }
        }
//        if(found == false) { qDebug() << "found == false"; return; }

        // Do processing of the QObject* stored on the event
        //SocketData* o = qobject_cast<SocketData*>(me->payload());
        //if(!o) return;
//        qDebug() << "Doing EVEN MORE crap with the event";
        me->shared()->refs();
      
        // Send out a FilteredData
            QByteArray ba = me->payload()->toByteArray();
            ba.append("monkey!");
            QVariant* qv = new QVariant(ba);
            QStringList sl;
            sl << "FilteredData";
            MClientEvent* e2 =
                new MClientEvent(new MClientEventData(qv), sl);
            e2->session(me->session());
            QApplication::postEvent(PluginManager::instance(), e2);
            qDebug() << "posted FilteredData!";
    }
}




void SocketDataFilter::configure() {
}


const bool SocketDataFilter::loadSettings() {
}


const bool SocketDataFilter::saveSettings() const {
}
