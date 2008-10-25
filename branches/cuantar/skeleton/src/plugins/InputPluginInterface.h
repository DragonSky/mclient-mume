#ifndef INPUTPLUGININTERFACE_H
#define INPUTPLUGININTERFACE_H


class QString;

class InputPluginInterface {
    public:
        virtual ~InputPluginInterface() { }
        
        virtual QString& pluginName() const=0;
        virtual const bool& addListener(const QObject& listener)=0;
        
};


Q_DECLARE_INTERFACE(FilterInterface,
"mclient.simple_client.OutputPluginInterface/1.0")


#endif /* INPUTPLUGININTERFACE_H */
