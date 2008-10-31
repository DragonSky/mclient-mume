#ifndef SOCKETDATAFILTER_H
#define SOCKETDATAFILTER_H

#include "MClientFilterPlugin.h"

class MClientEvent;

class QEvent;


class SocketDataFilter : public MClientFilterPlugin {
    Q_OBJECT
    
    public:
        SocketDataFilter(QObject* parent=0);
        ~SocketDataFilter();

        // From MClientFilterPlugin
        void customEvent(QEvent* e);
        void configure();
        const bool loadSettings();
        const bool saveSettings() const;
        const bool startSession();
        const bool stopSession();


    private:

};


#endif /* SOCKETDATAFILTER_H */
