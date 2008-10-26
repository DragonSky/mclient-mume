#ifndef MCLIENTDISPLAYINTERFACE_H
#define MCLIENTDISPLAYINTERFACE_H

#include <QtPlugin>


class MClientDisplayInterface {
    
    public:
        // called from the main thread; initialises all QWidgets needed.
        // The plugin developer MUST do this, or the plugin will cause a
        // segfault.  (actually I'm not positive that's such a strong 'must')
        const bool initDisplay();

};

Q_DECLARE_INTERFACE(MClientDisplayInterface,
        "mume.mclient.MClientDisplayInterface/1.0")

#endif /* MCLIENTDISPLAYINTERFACE_H */
