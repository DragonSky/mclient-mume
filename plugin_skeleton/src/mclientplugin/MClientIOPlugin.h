#ifndef MCLIENTIOPLUGIN_H
#define MCLIENTIOPLUGIN_H

#include "MClientPlugin.h"
#include "MClientIOInterface.h"

class QByteArray;
class QEvent;


class MClientIOPlugin : public MClientPlugin, public MClientIOInterface {
    Q_OBJECT
    Q_INTERFACES(MClientIOInterface)
    
    public:
        MClientIOPlugin(QObject* parent=0);
        ~MClientIOPlugin();

        // Plugin members
        virtual void customEvent(QEvent* e);

        // IO members
        virtual void sendData(const QByteArray data);
        virtual void connectDevice();
        virtual void disconnectDevice();

    //protected:
        // This will get subclassed as needed by the developer
        // QIODevice* _device;
        
        // Bad idea: we don't want to limit ourselves to a single socket,
        // file, whatever.  Maybe we want a plugin that reads from a few
        // places and combines the data before spitting it out.
};


#endif /* MCLIENTIOPLUGIN_H */
