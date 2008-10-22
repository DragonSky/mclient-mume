#ifndef OUTPUTPLUGININTERFACE_H
#define OUTPUTPLUGININTERFACE_H


class QString;

class OutputPluginInterface {
    public:
        virtual ~OutputPluginInterface() { }
        
        virtual const QString& pluginName() const=0;
        virtual const bool& listenTo(const QObject& source)=0;
        virtual const QObject& source() const=0;
};


Q_DECLARE_INTERFACE(FilterInterface,
"mclient.simple_client.OutputPluginInterface/1.0")


#endif /* OUTPUTPLUGININTERFACE_H */
