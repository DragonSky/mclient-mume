#ifndef SIMPLEINPUTPLUGIN_H
#define SIMPLEINPUTPLUGIN_H

#include "InputPluginInterface.h"

#include <QObject>


class QString;

class SimpleInputPlugin public QObject, public InputPluginInterface {
    Q_OBJECT
    Q_INTERFACES(InputPluginInterface)

    public:
        QString& pluginName() const;
        void (const QString& data) const;

};


#endif /* SIMPLEINPUTPLUGIN_H */
