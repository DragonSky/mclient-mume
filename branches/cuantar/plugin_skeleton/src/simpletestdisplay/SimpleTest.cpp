#include "SimpleTest.h"

#include <QDebug>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>


SimpleTest::SimpleTest(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* _vbox = new QVBoxLayout();
    QGroupBox* _vg = new QGroupBox(this);
    _vg->setLayout(_vbox);
    
    QTextEdit* _ta = new QTextEdit(this);  
    _vbox->addWidget(_ta);

    QHBoxLayout* _hbox = new QHBoxLayout();
    QGroupBox* _hg = new QGroupBox(this);
    _hg->setLayout(_hbox);
    _vbox->addWidget(_hg);

    QPushButton* _start = new QPushButton(this);
    _hbox->addWidget(_start);
    _start->setText("start");
    QPushButton* _stop = new QPushButton(_hg);
    _hbox->addWidget(_stop);
    _stop->setText("stop");
   
    this->setMinimumSize(400,400);
    this->show();
}


SimpleTest::~SimpleTest() {
    // everything has 'this' as a parent, so no cleanup needed
}
