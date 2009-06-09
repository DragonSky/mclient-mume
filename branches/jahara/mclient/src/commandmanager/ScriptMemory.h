#ifndef SCRIPTMEMORY_H
#define SCRIPTMEMORY_H

#include <QObject>

class ScriptMemory : public QObject {
    Q_OBJECT

    public:
        ScriptMemory(QObject* parent=0);
        ~ScriptMemory();	

	int number(int index);
	QByteArray& string(int index);

	int number(QString name);
	QByteArray& string(QString name);

    private:
	QList<int> _numberedIntegerVariables;
	QList<QByteArray> _numberedStringVariables;
	QHash<QString, int> _namedIntegerVariables;
	QHash<QString, QByteArray> _namedStringVariables;
	//QVector< QPair<QVariant, QVariant> > _parameterStack;
};

#endif /* SCRIPTMEMORY_H */

