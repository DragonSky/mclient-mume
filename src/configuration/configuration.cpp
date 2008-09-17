/************************************************************************
**
** Authors:   Ulf Hermann <ulfonk_mennhar@gmx.de> (Alve),
**            Marek Krejza <krejza@gmail.com> (Caligor)
**
** This file is part of the MMapper2 project.
** Maintained by Marek Krejza <krejza@gmail.com>
**
** Copyright: See COPYING file that comes with this distribution
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file COPYING included in the packaging of
** this file.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
*************************************************************************/

#include <configuration.h>

#ifdef _WIN32
#include <stdlib.h> // for getenv()
#endif

Configuration::Configuration() {
    read();
    configurationChanged = false;
};

void Configuration::read()
{
  QSettings conf("MUME", "mClient");

  conf.beginGroup("General");
  firstRun = conf.value("Run first time", TRUE).toBool();
  windowPosition = conf.value("Window Position", QPoint(200, 200)).toPoint();
  windowSize = conf.value("Window Size", QSize(400, 400)).toSize();
  scrollbackSize = conf.value("Scrollback Size", 20000).toInt();
  conf.endGroup();

  conf.beginGroup("Profiles");
#ifdef _WIN32
  // for Windows
  profilePath = conf.value("Profile Path", QString(getenv("APPDATA")) + "\\mClient").toString();
#else
  // for Linux/Mac
  profilePath = conf.value("Profile Path", QDir::homePath() + "/.mclient" ).toString();
#endif
  conf.endGroup();

  conf.beginGroup("Colors");
  foregroundColor = (QColor) conf.value("Foreground", Qt::lightGray).toString();
  backgroundColor = (QColor) conf.value("Background", Qt::black).toString();
  blackColor = (QColor) conf.value("Black", Qt::darkGray).toString();
  redColor = (QColor) conf.value("Red", Qt::darkRed).toString();
  greenColor = (QColor) conf.value("Green", Qt::darkGreen).toString();
  yellowColor = (QColor) conf.value("Yellow", Qt::darkYellow).toString();
  blueColor = (QColor) conf.value("Blue", Qt::darkBlue).toString();
  magentaColor = (QColor) conf.value("Magenta", Qt::darkMagenta).toString();
  cyanColor = (QColor) conf.value("Cyan", Qt::darkCyan).toString();
  grayColor = (QColor) conf.value("Gray", Qt::lightGray).toString();
  darkGrayColor = (QColor) conf.value("Dark Gray", Qt::gray).toString();
  brightRedColor = (QColor) conf.value("Bright Red", Qt::red).toString();
  brightGreenColor = (QColor) conf.value("Bright Green", Qt::green).toString();
  brightYellowColor = (QColor) conf.value("Bright Yellow", Qt::yellow).toString();
  brightBlueColor = (QColor) conf.value("Bright Blue", Qt::blue).toString();
  brightMagentaColor = (QColor) conf.value("Bright Magenta", Qt::magenta).toString();
  brightCyanColor = (QColor) conf.value("Bright Cyan", Qt::cyan).toString();
  whiteColor = (QColor) conf.value("White", Qt::white).toString();
  conf.endGroup();

  conf.beginGroup("Fonts");
  serverOutputFont.fromString( conf.value("Server Output", QApplication::font()).toString() );
  inputLineFont.fromString( conf.value("Input Line", QApplication::font()).toString() );
  conf.endGroup();
};

void Configuration::write() const {

  QSettings conf("MUME", "mClient");

  conf.beginGroup("General");
  conf.setValue("Run first time", FALSE);
  conf.setValue("Window Position", windowPosition);
  conf.setValue("Window Size", windowSize);
  conf.setValue("Scrollback Size", scrollbackSize);
  conf.endGroup();

  conf.beginGroup("Profiles");
  conf.setValue("Profile Path", profilePath);
  conf.endGroup();

  conf.beginGroup("Colors");
  conf.setValue("Foreground", foregroundColor.name());
  conf.setValue("Background", backgroundColor.name());
  conf.setValue("Black", blackColor.name());
  conf.setValue("Red", redColor.name());
  conf.setValue("Green", greenColor.name());
  conf.setValue("Yellow", yellowColor.name());
  conf.setValue("Blue", blueColor.name());
  conf.setValue("Magenta", magentaColor.name());
  conf.setValue("Cyan", cyanColor.name());
  conf.setValue("Gray", grayColor.name());
  conf.setValue("Dark Gray", darkGrayColor.name());
  conf.setValue("Bright Red", brightRedColor.name());
  conf.setValue("Bright Green", brightGreenColor.name());
  conf.setValue("Bright Yellow", brightYellowColor.name());
  conf.setValue("Bright Blue", brightBlueColor.name());
  conf.setValue("Bright Magenta", brightMagentaColor.name());
  conf.setValue("Bright Cyan", brightCyanColor.name());
  conf.setValue("White", whiteColor.name());
  conf.endGroup();

  conf.beginGroup("Fonts");
  conf.setValue("Server Output", serverOutputFont.toString());
  conf.setValue("Input Line", inputLineFont.toString());
  conf.endGroup();


};

Configuration& Config() {
  static Configuration conf;
  return conf;
};
