//#include "mainwindow.h"

#include "ClientWidget.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char** argv) {
 
    QApplication app(argc, argv);
    QMainWindow mw;

    ClientWidget* cw = new ClientWidget();
    cw->connectToHost("mume.org", 4242);
    mw.setCentralWidget(cw);
    mw.show();

    return app.exec();
}
