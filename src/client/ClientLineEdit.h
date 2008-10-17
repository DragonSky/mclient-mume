/***************************************************************************
 *   Copyright (C) 2008 by Nils Schimmelmann                               *
 *   nschimme@gmail.com                                                    *
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

#ifndef _CLIENTLINEEDIT_H_
#define _CLIENTLINEEDIT_H_

#include <QLineEdit>
#include <QStringListModel>

class ClientLineEdit : public QLineEdit
{
  Q_OBJECT

  public:
    ClientLineEdit(QWidget *parent);

    bool event(QEvent *event);

  signals:
    void keyPressed(const QString&);
    void minimizeParent();
    void mergeInputWrapper(QString, int);

  private:
    bool emitFakeKey(QKeyEvent *);

  public slots:
    void toggleEchoMode();
    void inputInsertText(const QString&);
    void inputMoveTo(const int& new_pos);
    void inputDeleteChars(const int& num);
    void inputClear();
};

#endif /* _CLIENTLINEEDIT_H_ */
