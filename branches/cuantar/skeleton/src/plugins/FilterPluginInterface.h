#ifndef FILTERPLUGININTERFACE_H
#define FILTERPLUGININTERFACE_H


class QString;

class FilterPluginInterface {
    public:
        virtual ~FilterPluginInterface() { }
        
        virtual const QString& pluginName() const=0;
        virtual const bool& addListener(const QObject& listener)=0;
        virtual const bool& listenTo(const QObject& source)=0;
        virtual const QObject& source() const=0;
};


Q_DECLARE_INTERFACE(FilterInterface,
"mclient.simple_client.FilterPluginInterface/1.0")


#endif /* FILTERPLUGININTERFACE_H */
