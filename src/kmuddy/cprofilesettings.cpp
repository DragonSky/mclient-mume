//
// C++ Implementation: cProfileSettings
//
// Description: profile settings
//
//
// Author: Tomas Mecir <kmuddy@kmuddy.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "cprofilesettings.h"
#include "cprofilemanager.h"

#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include <map>

using namespace std;

struct cProfileSettings::Private {
  QString profileId;
  map<QString, bool> boolVals;
  map<QString, int> intVals;
  map<QString, QString> strVals;
  map<QString, bool> defBoolVals;
  map<QString, int> defIntVals;
  map<QString, QString> defStrVals;
};

cProfileSettings::cProfileSettings (QString profileId)
{
  d = new Private;
  d->profileId = profileId;
  load ();
  fillDefaultValues ();
}

cProfileSettings::~cProfileSettings ()
{
  // we must NOT save here
  delete d;
}

void cProfileSettings::setBool (const QString &name, bool value)
{
  d->boolVals[name] = value;
}

void cProfileSettings::setInt (const QString &name, int value)
{
  d->intVals[name] = value;
}

void cProfileSettings::setString (const QString &name, const QString &value)
{
  d->strVals[name] = value;
}

bool cProfileSettings::getBool (const QString &name) const
{
  if (d->boolVals.count (name))
    return d->boolVals[name];
  if (d->defBoolVals.count (name))
    return d->defBoolVals[name];
  return false;
}

int cProfileSettings::getInt (const QString &name) const
{
  if (d->intVals.count (name))
    return d->intVals[name];
  if (d->defIntVals.count (name))
    return d->defIntVals[name];
  return 0;
}

QString cProfileSettings::getString (const QString &name) const
{
  if (d->strVals.count (name))
    return d->strVals[name];
  if (d->defStrVals.count (name))
    return d->defStrVals[name];
  return QString();
}

void cProfileSettings::setDefaultBool (const QString &name, bool value)
{
  d->defBoolVals[name] = value;
}

void cProfileSettings::setDefaultInt (const QString &name, int value)
{
  d->defIntVals[name] = value;
}

void cProfileSettings::setDefaultString (const QString &name, const QString &value)
{
  d->defStrVals[name] = value;
}

void cProfileSettings::fillDefaultValues ()
{
  setDefaultBool ("use-ansi", true);
  setDefaultBool ("limit-repeater", true);
  setDefaultString ("encoding", "ISO 8859-1");
  setDefaultBool ("startup-negotiate", true);
  setDefaultBool ("lpmud-style", false);
  setDefaultBool ("prompt-label", false);
  setDefaultBool ("prompt-status", true);
  setDefaultBool ("prompt-console", true);
  setDefaultBool ("auto-adv-transcript", false);

  QString movecmds[10];
  movecmds[0] = "n"; movecmds[1] = "ne";
  movecmds[2] = "e"; movecmds[3] = "se";
  movecmds[4] = "s"; movecmds[5] = "sw";
  movecmds[6] = "w"; movecmds[7] = "nw";
  movecmds[8] = "u"; movecmds[9] = "d";
  for (int i = 0; i < 10; i++)
    setDefaultString ("movement-command-"+QString::number(i), movecmds[i]);
  setDefaultString ("transcript-directory", QDir::homePath());
  // TODO: move these to the scripting plugin
  setDefaultString ("script-directory", QDir::homePath());
  setDefaultString ("script-working-directory", QDir::homePath());

  setDefaultInt ("sound-dir-count", 0);
  setDefaultBool ("use-msp", true);
  setDefaultBool ("always-msp", false);
  setDefaultBool ("midline-msp", false);

  setDefaultBool ("use-mxp", true);
  setDefaultString ("mxp-variable-prefix", QString());
}


void cProfileSettings::save ()
{
  cProfileManager *pm = cProfileManager::self();

  QString path = pm->profilePath (d->profileId);
  QDir dir = QDir (path);
  if (!dir.exists()) QDir::root().mkpath (dir.absolutePath());

  // backup the old profile file, if any
  dir.remove ("settings.backup");
  if (!QFile(path + "/settings.xml").copy (path + "/settings.backup")) {
    qDebug("Unable to backup settings.xml.");  // not fatal, may simply not exist
  }

  QFile f (path + "/settings.xml");
  if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
    qDebug("Unable to open settings.xml for writing.");
    return;  // problem
  }
  // save the profile file
  QXmlStreamWriter *writer = new QXmlStreamWriter (&f);

  writer->setAutoFormatting (true);  // make the generated XML more readable
  writer->writeStartDocument ();

  writer->writeStartElement ("profile");
  writer->writeAttribute ("version", "1.0");

  // integer values
  map<QString, int>::iterator it;
  for (it = d->intVals.begin(); it != d->intVals.end(); ++it) {
    writer->writeStartElement ("setting");
    writer->writeAttribute ("type", "integer");
    writer->writeAttribute ("name", it->first);
    writer->writeAttribute ("value", QString::number (it->second));
    writer->writeEndElement ();
  }

  // string values
  map<QString, QString>::iterator it2;
  for (it2 = d->strVals.begin(); it2 != d->strVals.end(); ++it2) {
    writer->writeStartElement ("setting");
    writer->writeAttribute ("type", "string");
    writer->writeAttribute ("name", it2->first);
    writer->writeAttribute ("value", it2->second);
    writer->writeEndElement ();
  }

  writer->writeEndElement ();
  writer->writeEndDocument ();

  f.close ();
  delete writer;
}

void cProfileSettings::load ()
{
  cProfileManager *pm = cProfileManager::self();
  
  QString path = pm->profilePath (d->profileId);
  QDir dir = QDir (path);
  if (!dir.exists()) QDir::root().mkpath (dir.absolutePath());

  QFile f (path + "/settings.xml");
  if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug("No settings file - nothing to do.");
    return;  // no profiles - nothing to do
  }
  QXmlStreamReader *reader = new QXmlStreamReader (&f);
  
  reader->readNext ();  // read the document start
  reader->readNext ();
  if (reader->isStartElement ())
    if (reader->name() == "profile")
      if (reader->attributes().value ("version") == "1.0") {
        // okay, read the list
        while (!reader->atEnd()) {
          reader->readNext ();
          if (reader->isStartElement () && (reader->name() == "setting")) {
            QString type = reader->attributes().value ("type").toString();
            QString name = reader->attributes().value ("name").toString();
            QString value = reader->attributes().value ("value").toString();
            if (type == "integer")
              setInt (name, value.toInt());
            else if (type == "string")
              setString (name, value);
            else
              qDebug("Unrecognized setting type %s", type.toAscii().data());
          }
        }
      } else reader->raiseError ("Unknown profile file version.");
    else reader->raiseError ("This is not a valid profile list file.");
  else reader->raiseError ("This file is corrupted.");

  if (reader->hasError()) {
    qDebug("Error in settings.xml at line %s, column %d: %s", QString::number (reader->lineNumber()).toAscii().constData(), QString::number (reader->columnNumber()).toAscii().constData(), reader->errorString().toAscii().constData() );
  }

  // close the file
  f.close ();
  delete reader;
}



