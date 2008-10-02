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

#ifndef _PROFILEDIALOG_H
#define _PROFILEDIALOG_H

#include <QDialog>
#include "ui_profiledialog.h"

class ProfileManagerDialog;

class ProfileDialog : public QDialog, public Ui::ProfileDialog
{
  Q_OBJECT

  public:
    ProfileDialog(QWidget* parent);
    virtual ~ProfileDialog();

    QString selectedProfile();

  private:
    ProfileManagerDialog *dialog;

    /** Create the Default "Quick Connect" MUME Profile */
    void createDefaultProfile();

  signals:
    void profileSelected();

  private slots:
    void playClicked();
    void profileClicked();
    void relayLoadProfile(const QString&);
    void doubleClicked(const QModelIndex &index);
    void selectionChanged(const QItemSelection &index);

};

#endif /* _PROFILEDIALOG_H */
