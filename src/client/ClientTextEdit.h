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

#ifndef _CLIENTTEXTEDIT_H_
#define _CLIENTTEXTEDIT_H_

#include <QLineEdit>
#include <QTextEdit>
#include <QTextFrame>
#include <QTextCursor>

class ClientTextEdit : public QTextEdit
{
  Q_OBJECT

  public:
    ClientTextEdit(QWidget *parent);

  public slots:
    void addText(const QString& str);
    void moveCursor(int pos);
    void autoCopySelectedText(bool);
    //void resizeDocument(int);
    void viewDimensionsChanged();

  private:
    QTextDocument* doc;
    QTextCursor cursor;
    QTextCharFormat defaultFormat, format;

    int previous;

    void setDefaultFormat(QTextCharFormat& format);
    void updateFormat(QTextCharFormat& format, int ansiCode);
    void updateFormatBoldColor(QTextCharFormat& format);
};

#endif /* _CLIENTTEXTEDIT_H_ */
