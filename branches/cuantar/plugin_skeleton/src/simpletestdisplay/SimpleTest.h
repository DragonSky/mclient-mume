#ifndef SIMPLETEST_H
#define SIMPLETEST_H

#include <QWidget>

class QPushButton;


class SimpleTest : public QWidget {
    Q_OBJECT

    public:
        SimpleTest(QWidget* parent=0);
        ~SimpleTest();

};


#endif /* SIMPLETEST_H */
