/***************************************************************************
 *   Copyright (C) 2008 by Nils Schimmelmann   *
 *   nschimme@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 ***************************************************************************/

#include <QMessageBox>
#include <QFileDialog>

#include "profileeditdialog.h"

QString ProfileEditDialog::name ()
{
  return profileName->text ();
}

QString ProfileEditDialog::server ()
{
  return profileServer->text();
}

int ProfileEditDialog::port ()
{
  return profilePort->value();
}

QString ProfileEditDialog::definitions ()
{
  return profileDefinitions->text ();
}

void ProfileEditDialog::setName (QString name)
{
  profileName->setText (name);
}

void ProfileEditDialog::setServer (QString server)
{
  profileServer->setText (server);
}

void ProfileEditDialog::setPort (int port)
{
  profilePort->setValue(port);
}

void ProfileEditDialog::setDefinitions (QString path)
{
  profileDefinitions->setText (path);
}

void ProfileEditDialog::slotOk ()
{
  QString s = name ().simplified ();
  bool failed = false;
  if (s == "")
  {
    failed = true;
    QMessageBox::critical(this, tr("Edit profile"), tr("Profile name cannot be empty."));
  }
  //looks like filenames can contain '/' char, but who knows?
  if (s.contains ('/'))
  {
    QMessageBox::critical(this, tr("Edit profile"), tr("Profile name cannot contain '/' character."));
    failed = true;
  }
    
  //if name is okay, accept() the dialog...
  if (!failed)
  {
    accept ();
    emit loadClicked ();
  }
}

void ProfileEditDialog::slotBrowse ()
{
  QString fileName = QFileDialog::getOpenFileName(this);
  if (!fileName.isEmpty()) {
    profileDefinitions->setText(fileName);
  }
}

/* Objects */

ProfileEditDialog::ProfileEditDialog(QWidget*parent) : QDialog(parent)
{
  setupUi(this);

  connect(browseButton, SIGNAL(clicked() ), this, SLOT(slotBrowse() ));
  connect(buttonBox, SIGNAL(rejected() ), this, SLOT(close() ));
  connect(buttonBox, SIGNAL(accepted() ), this, SLOT(slotOk() ));
}

ProfileEditDialog::~ProfileEditDialog() {}

