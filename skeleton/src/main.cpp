//#include "mainwindow.h"

#include "ClientWidget.h"
#include "PowwowWrapper.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char** argv) {
 
    QApplication app(argc, argv);
    QMainWindow mw;

    // Create a few cool things: powwow, the client, (the mapper), ... 
    PowwowWrapper* pw = PowwowWrapper::Instance();
    ClientWidget* cw = new ClientWidget();
    
    // Add dockwidgets to the mainwindow here, after everything's ready
    mw.setCentralWidget(cw);
    mw.show();
    
    // Time for some magic
    pw->connectToHost("mume.org", 4242);

    int retval = app.exec();
    
    // Cleanup
    PowwowWrapper::Instance()->Delete();
    return retval;
}
