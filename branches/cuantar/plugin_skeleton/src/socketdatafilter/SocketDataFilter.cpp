#include "SocketDataFilter.h"

#include "MClientEvent.h"
#include "PluginManager.h"
#include "SocketData.h"

#include <QApplication>
#include <QDebug>

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
    _dataTypes << "SocketDataEvent";
}


SocketDataFilter::~SocketDataFilter() {
}


void SocketDataFilter::customEvent(QEvent* e) {

    if(e->type() != 10001) {
        qDebug() << "Somehow received the wrong kind of event...";
    } else {
//        qDebug() << "Doing some crap with the event";
        MClientEvent* me = static_cast<MClientEvent*>(e);
        if(!_dataTypes.contains(me->dataType())) return;
      
        // Do processing of the QObject* stored on the event
        SocketData* o = qobject_cast<SocketData*>(me->payload());
        if(!o) return;
//        qDebug() << "Doing EVEN MORE crap with the event";
       
        // Do something with o
        uint8_t type = determineType(o->data());
    
        char* d = const_cast<char*>(o->data().data());
        char d2[sizeof(d)-sizeof(type)];
        copy(&d[sizeof(type)], &d[sizeof(d)], d2);
       
        QByteArray ba(const_cast<const char*>(d2));
       
        // Replace this type number with a type string corresponding to the
        // type string that's attached to the MClientEvent.  Do this in
        // mtbucket and we can eliminate this switch.
        if(type == 1) { 
            SocketData* sd = new SocketData(ba);
            MClientEvent* e2 =
                new MClientEvent(new MClientEventData(sd), "RateRulerEvent");
            QApplication::postEvent(PluginManager::instance(), e2);
            qDebug() << "posted RateRulerEvent!";
        }
    }
}


const int SocketDataFilter::determineType(const QByteArray& data) const {
    const char* d = data.constData();
    uint8_t type = static_cast<uint8_t>(d[0]);
    return type;
}


void SocketDataFilter::configure() {
}


const bool SocketDataFilter::loadSettings() {
}


const bool SocketDataFilter::saveSettings() const {
}
