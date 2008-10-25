#ifndef SOCKETDATAFILTER_H
#define SOCKETDATAFILTER_H

#include "MClientFilterPlugin.h"

class MClientEvent;

class QEvent;


class SocketDataFilter : public MClientFilterPlugin {
    Q_OBJECT
//    Q_INTERFACES(MClientFilterInterface MClientPluginInterface)
    
    public:
        SocketDataFilter(QObject* parent=0);
        ~SocketDataFilter();

        // From MClientFilterPlugin
        void customEvent(QEvent* e);
        void configure();
        const bool loadSettings();
        const bool saveSettings() const;


    private:
        const int determineType(const QByteArray& data) const;

};


#endif /* SOCKETDATAFILTER_H */
