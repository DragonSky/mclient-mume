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

#ifndef _PROFILEEDITDIALOG_H
#define _PROFILEEDITDIALOG_H

#include <QDialog>
#include "ui_ProfileEditDialog.h"

class ProfileEditDialog : public QDialog, public Ui::ProfileEditDialog
{
  Q_OBJECT

  public:
    ProfileEditDialog(QWidget* parent);
    virtual ~ProfileEditDialog();

  public:
    /** values in LineEdits */
    QString name();
    QString server();
    int port();
    QString definitions();
    QString map();

    /** set new values! */
    void setName(QString name);
    void setServer(QString server);
    void setPort(int port);
    void setDefinitions(QString path);
    void setMap(QString path);

  signals:
    void loadClicked();

  private slots:
    void slotOk();
    void definitionsBrowse();
    void mapBrowse();
};

#endif /* _PROFILEEDITDIALOG_H */
