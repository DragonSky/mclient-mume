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

#ifndef COLORFONTPAGE_H
#define COLORFONT_H

#include <QWidget>
#include "ui_colorfontpage.h"

class ColorFontPage : public QWidget, private Ui::ColorFontPage
{
    Q_OBJECT

  public:
    ColorFontPage(QWidget *parent = 0);

  private slots:
    void serverOutputButtonClicked();
    void inputLineButtonClicked();

    void blackButtonClicked();
    void redButtonClicked();
    void greenButtonClicked();
    void yellowButtonClicked();
    void blueButtonClicked();
    void magentaButtonClicked();
    void cyanButtonClicked();
    void grayButtonClicked();
    void darkGrayButtonClicked();
    void brightRedButtonClicked();
    void brightGreenButtonClicked();
    void brightYellowButtonClicked();
    void brightBlueButtonClicked();
    void brightMagentaButtonClicked();
    void brightCyanButtonClicked();
    void whiteButtonClicked();
    void foregroundButtonClicked();
    void backgroundButtonClicked();
};

#endif

