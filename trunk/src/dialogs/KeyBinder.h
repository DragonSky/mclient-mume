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

#ifndef _KEYBINDER_CMD_H
#define _KEYBINDER_CMD_H

#include <QDialog>
#include "ui_KeyBinder.h"

bool isAcceptableKey(QKeyEvent* ke);
QString generateKeyCodeSequence(QKeyEvent *ke);
QString generateSequenceText(QString sequence);

class KeyBinder : public QDialog, public Ui::KeyBinder
{
  Q_OBJECT

  public:
    KeyBinder(QString, QString&, QWidget* parent);
    virtual ~KeyBinder();

    QString *seq;

    bool event(QEvent *event);
};


#endif /* _KEYBINDER_CMD_H */
