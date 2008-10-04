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
//
// Author: Tomas Mecir <kmuddy@kmuddy.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <QMessageBox>

#include "profiledialog.h"
#include "profilemanagerdialog.h"
#include "cprofilemanager.h"
#include "cprofilesettings.h"

// Currently mClient only allows one session
#define DEFAULT_SESSION 0

QString ProfileDialog::selectedProfile ()
{
  QItemSelection sel = profileView->selectionModel()->selection();
  if (sel.empty()) return QString();
  int idx = sel.indexes().first().row();
  return cProfileManager::self()->profileList()[idx];
}

void ProfileDialog::playClicked() {
  cProfileManager *mgr = cProfileManager::self();
  QString profile = selectedProfile();
  cProfileSettings *sett = mgr->settings (profile);
  if (!sett) return;   // no profile selected

  // Check if there is a loaded profile
  if (mgr->sessionAssigned(DEFAULT_SESSION)) {
    if (QMessageBox::warning (this, tr("Load profile"),
        tr("The profile %1 is already loaded.\nAre you sure you want to continue?").arg(mgr->visibleProfileName(mgr->profileName(DEFAULT_SESSION))),
           QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
      return;
    else
    {
      emit clearPowwowMemory();
    }
  }

  qDebug("running with settings...");
  hide();
  emit profileSelected();
}

void ProfileDialog::relayLoadProfile(const QString& profile)
{
  //hide the dialog, so that it doesn't look like we've crashed
  dialog->hide();
  //click play!
  playClicked();
}

void ProfileDialog::profileClicked() {
  dialog = new ProfileManagerDialog(profileView->selectionModel(), this);

  connect(dialog, SIGNAL(loadProfile(const QString&)), this, SLOT(relayLoadProfile(const QString&) ));

  dialog->exec();

  delete dialog;
  dialog = NULL;
}

void ProfileDialog::selectionChanged (const QItemSelection &index)
{
  //enable/disable Connect button
  playButton->setEnabled(index.indexes().empty() ? false : true);
}

void ProfileDialog::doubleClicked(const QModelIndex &index)
{
  if (index.isValid ())
    playButton->click();
}


void ProfileDialog::createDefaultProfile()
{
  qDebug("Creating Default profile");
  cProfileManager *mgr = cProfileManager::self();
  QString profile = mgr->newProfile("Default");
  cProfileSettings *sett = mgr->settings (profile);
  if (!sett) return;
  sett->setString ("server", "mume.org");
  sett->setInt ("port", 4242);
  sett->setString ("definitions", "default.pow");
  sett->save();
}

/* Objects */

ProfileDialog::ProfileDialog(QWidget*parent) : QDialog(parent)
{
  setupUi(this);

  QGraphicsScene *scene = new QGraphicsScene();
  scene->addPixmap(QPixmap(":/images/mclient.png"));
  profilePicture->setScene(scene);
  profilePicture->show();

  playButton->setFocus();

  profileView->setModel (cProfileManager::self()->model());
  profileView->hideColumn(1);
  profileView->hideColumn(2);
  profileView->hideColumn(3);
  profileView->setUniformRowHeights (true);
  profileView->setRootIsDecorated (false);
  profileView->setItemsExpandable (false);
  profileView->setWhatsThis(
                   tr("This list shows currently defined profiles.<p><b>Profiles</b> "
      "allow you to speed up connecting to your MUD, as well as to use "
      "more advanced features like <i>aliases</i> or <i>triggers</i>."));

  // Create Default Profile if it is Empty
  if (cProfileManager::self()->profileList().count() == 0) {
    createDefaultProfile();
  }

  // Automatically select the last profile   TODO
  QModelIndex topLeft = profileView->model()->index(0, 0), bottomRight = profileView->model()->index(0, 3);
  QItemSelection selection(topLeft, bottomRight);
  profileView->selectionModel()->select(selection, QItemSelectionModel::Select);
  selectionChanged(profileView->selectionModel()->selection());


  connect(playButton, SIGNAL(clicked() ), this, SLOT(playClicked() ));
  connect(profileButton, SIGNAL(clicked() ), this, SLOT(profileClicked() ));

  connect (profileView->selectionModel(), SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
           this, SLOT(selectionChanged (const QItemSelection &)));
  connect (profileView, SIGNAL(doubleClicked (const QModelIndex &)),
           this, SLOT(doubleClicked (const QModelIndex &)));
}

ProfileDialog::~ProfileDialog() {}

