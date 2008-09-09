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

#include <QtGui>
#include "colorfontpage.h"
#include "configuration.h"

ColorFontPage::ColorFontPage(QWidget *parent)
  : QWidget(parent)
{
  setupUi(this);

  serverOutputButton->setFont(Config().serverOutputFont); 
  serverOutputButton->setText(QString("%1, %2").arg(Config().serverOutputFont.family()).arg(Config().serverOutputFont.pointSize()));
  inputLineButton->setFont(Config().inputLineFont);
  inputLineButton->setText(QString("%1, %2").arg(Config().inputLineFont.family()).arg(Config().inputLineFont.pointSize()));

  connect(serverOutputButton, SIGNAL(clicked()), this, SLOT(serverOutputButtonClicked()) );
  connect(inputLineButton, SIGNAL(clicked()), this, SLOT(inputLineButtonClicked()) );

  blackButton->setPalette(QPalette(Config().blackColor));
  redButton->setPalette(QPalette(Config().redColor));
  greenButton->setPalette(QPalette(Config().greenColor));
  yellowButton->setPalette(QPalette(Config().yellowColor));
  blueButton->setPalette(QPalette(Config().blueColor));
  magentaButton->setPalette(QPalette(Config().magentaColor));
  cyanButton->setPalette(QPalette(Config().cyanColor));
  grayButton->setPalette(QPalette(Config().grayColor));
  darkGrayButton->setPalette(QPalette(Config().darkGrayColor));
  brightRedButton->setPalette(QPalette(Config().brightRedColor));
  brightGreenButton->setPalette(QPalette(Config().brightGreenColor));
  brightYellowButton->setPalette(QPalette(Config().brightYellowColor));
  brightBlueButton->setPalette(QPalette(Config().brightBlueColor));
  brightMagentaButton->setPalette(QPalette(Config().brightMagentaColor));
  brightCyanButton->setPalette(QPalette(Config().brightCyanColor));
  whiteButton->setPalette(QPalette(Config().whiteColor));
  foregroundButton->setPalette(QPalette(Config().foregroundColor));
  backgroundButton->setPalette(QPalette(Config().backgroundColor));

  connect(blackButton, SIGNAL(clicked()), this, SLOT(blackButtonClicked()) );
  connect(redButton, SIGNAL(clicked()), this, SLOT(redButtonClicked()) );
  connect(greenButton, SIGNAL(clicked()), this, SLOT(greenButtonClicked()) );
  connect(yellowButton, SIGNAL(clicked()), this, SLOT(yellowButtonClicked()) );
  connect(blueButton, SIGNAL(clicked()), this, SLOT(blueButtonClicked()) );
  connect(magentaButton, SIGNAL(clicked()), this, SLOT(magentaButtonClicked()) );
  connect(cyanButton, SIGNAL(clicked()), this, SLOT(cyanButtonClicked()) );
  connect(grayButton, SIGNAL(clicked()), this, SLOT(grayButtonClicked()) );
  connect(darkGrayButton, SIGNAL(clicked()), this, SLOT(darkGrayButtonClicked()) );
  connect(brightRedButton, SIGNAL(clicked()), this, SLOT(brightRedButtonClicked()) );
  connect(brightGreenButton, SIGNAL(clicked()), this, SLOT(brightGreenButtonClicked()) );
  connect(brightYellowButton, SIGNAL(clicked()), this, SLOT(brightYellowButtonClicked()) );
  connect(brightBlueButton, SIGNAL(clicked()), this, SLOT(brightBlueButtonClicked()) );
  connect(brightMagentaButton, SIGNAL(clicked()), this, SLOT(brightMagentaButtonClicked()) );
  connect(brightCyanButton, SIGNAL(clicked()), this, SLOT(brightCyanButtonClicked()) );
  connect(whiteButton, SIGNAL(clicked()), this, SLOT(whiteButtonClicked()) );
  connect(foregroundButton, SIGNAL(clicked()), this, SLOT(foregroundButtonClicked()) );
  connect(backgroundButton, SIGNAL(clicked()), this, SLOT(backgroundButtonClicked()) );
}

void ColorFontPage::serverOutputButtonClicked() {
  QFont newFont, oldFont = Config().serverOutputFont;
  bool isValid;
  newFont = QFontDialog::getFont(&isValid, oldFont, this);
  if (isValid && newFont.toString().compare(oldFont.toString()) != 0) {
    Config().setServerOutputFont(newFont);
    serverOutputButton->setFont(newFont); 
    serverOutputButton->setText(QString("%1, %2").arg(newFont.family()).arg(newFont.pointSize()));
  }
}

void ColorFontPage::inputLineButtonClicked() {
  QFont newFont, oldFont = Config().inputLineFont;
  bool isValid;
  newFont = QFontDialog::getFont(&isValid, oldFont, this);
  if (isValid && newFont.toString().compare(oldFont.toString()) != 0) {
    Config().setInputLineFont(newFont);
    inputLineButton->setFont(newFont);
    inputLineButton->setText(QString("%1, %2").arg(newFont.family()).arg(newFont.pointSize()));
  }
}

void ColorFontPage::blackButtonClicked() {
  QColor newColor, oldColor = Config().blackColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setBlackColor(newColor);
    blackButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::redButtonClicked() {
  QColor newColor, oldColor = Config().redColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setRedColor(newColor);
    redButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::greenButtonClicked() {
  QColor newColor, oldColor = Config().greenColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setGreenColor(newColor);
    greenButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::yellowButtonClicked() {
  QColor newColor, oldColor = Config().yellowColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setYellowColor(newColor);
    yellowButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::blueButtonClicked() {
  QColor newColor, oldColor = Config().blueColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setBlueColor(newColor);
    blueButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::magentaButtonClicked() {
  QColor newColor, oldColor = Config().magentaColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setMagentaColor(newColor);
    magentaButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::cyanButtonClicked() {
  QColor newColor, oldColor = Config().cyanColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setCyanColor(newColor);
    cyanButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::grayButtonClicked() {
  QColor newColor, oldColor = Config().grayColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setGrayColor(newColor);
    grayButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::darkGrayButtonClicked() {
  QColor newColor, oldColor = Config().darkGrayColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setDarkGrayColor(newColor);
    darkGrayButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::brightRedButtonClicked() {
  QColor newColor, oldColor = Config().brightRedColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setBrightRedColor(newColor);
    brightRedButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::brightGreenButtonClicked() {
  QColor newColor, oldColor = Config().brightGreenColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setBrightGreenColor(newColor);
    brightGreenButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::brightYellowButtonClicked() {
  QColor newColor, oldColor = Config().brightYellowColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setBrightYellowColor(newColor);
    brightYellowButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::brightBlueButtonClicked() {
  QColor newColor, oldColor = Config().brightBlueColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setBrightBlueColor(newColor);
    brightBlueButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::brightMagentaButtonClicked() {
  QColor newColor, oldColor = Config().brightMagentaColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setBrightMagentaColor(newColor);
    brightMagentaButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::brightCyanButtonClicked() {
  QColor newColor, oldColor = Config().brightCyanColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setBrightCyanColor(newColor);
    brightCyanButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::whiteButtonClicked() {
  QColor newColor, oldColor = Config().whiteColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setWhiteColor(newColor);
    whiteButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::foregroundButtonClicked() {
  QColor newColor, oldColor = Config().foregroundColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setForegroundColor(newColor);
    foregroundButton->setPalette(QPalette(newColor));
  }
}

void ColorFontPage::backgroundButtonClicked() {
  QColor newColor, oldColor = Config().backgroundColor;
  newColor = QColorDialog::getColor(oldColor, this);
  if (newColor.isValid() && newColor != oldColor) {
    Config().setBackgroundColor(newColor);
    backgroundButton->setPalette(QPalette(newColor));
  }
}
