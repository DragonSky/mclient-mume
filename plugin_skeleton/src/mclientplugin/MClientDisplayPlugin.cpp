#include "MClientDisplayPlugin.h"

#include <QEvent>


MClientDisplayPlugin::MClientDisplayPlugin(QWidget* parent) : MClientPlugin(parent) {
}


MClientDisplayPlugin::~MClientDisplayPlugin() {
}


void MClientDisplayPlugin::customEvent(QEvent* e) {
}


const bool MClientDisplayPlugin::initDisplay() {
}
