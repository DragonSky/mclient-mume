#include <QDebug>
#include "ScriptMemory.h"

ScriptMemory::ScriptMemory(QObject* parent) : QObject(parent) {
  qDebug() << "* Initializing Script Memory";

  _numberedIntegerVariables.resize(60);
  _numberedStringVariables.resize(60);
}

int ScriptMemory::number(int index) {
  return _numberedIntegerVariables.at(index);
}

QByteArray& ScriptMemory::string(int index) {
  return _numberedStringVariables.at(index);
}

int ScriptMemory::string(QString name) {
  return _namedNumberVariables.value(name);
}

QByteArray& ScriptMemory::string(QString name) {
  return _namedStringVariables.value(name);
}

ScriptMemory::~ScriptMemory() {
}
