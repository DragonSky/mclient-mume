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
  windowState = conf.value("Window State", "").toByteArray();
  scrollbackSize = conf.value("Scrollback Size", 20000).toInt();
  alwaysOnTop = conf.value("Always On Top", FALSE).toBool();
  useInternalEditor = conf.value("Use Internal Editor", TRUE).toBool();
  m_mapMode = conf.value("Map Mode", 0).toInt(); //0 play, 1 map
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

  conf.beginGroup("Fonts"); // TODO: System specific fonts
  serverOutputFont.fromString( conf.value("Server Output", QApplication::font()).toString() );
  inputLineFont.fromString( conf.value("Input Line", QApplication::font()).toString() );
  conf.endGroup();
  
  conf.beginGroup("Parser");
  IAC_prompt_parser = conf.value("Use IAC-GA prompt", TRUE).toBool();
  m_removeXmlTags = conf.value("Remove XML tags", TRUE).toBool();

  moveForcePatternsList = conf.value("Move force patterns").toStringList();
  moveCancelPatternsList = conf.value("Move cancel patterns").toStringList();
  noDescriptionPatternsList = conf.value("Non-standard room description patterns").toStringList();
  dynamicDescriptionPatternsList = conf.value("Dynamic room description patterns").toStringList();
  scoutPattern = conf.value("Scout pattern", "#<You quietly scout").toString();
  promptPattern = conf.value("Prompt pattern", "#>>").toByteArray();
  exitsPattern = conf.value("Exits pattern", "#<Exits:").toString();
  loginPattern = conf.value("Login pattern", "#>known? ").toByteArray();
  passwordPattern = conf.value("Password pattern", "#>pass phrase: ").toByteArray();
  menuPromptPattern = conf.value("Menu prompt pattern", "#>> ").toByteArray();

  /*
  roomDescriptionsParserType = (RoomDescriptionsParserType) conf.value("Static room description parser type", (int)(RDPT_COLOR)).toInt();
  minimumStaticLines = conf.value("Minimum static lines in room description", 1).toInt();
  */
  conf.endGroup();

  /*
  conf.beginGroup("Mume Native");
  brief = conf.value("Brief mode", FALSE).toBool();
  emulatedExits = conf.value("Emulated Exits", TRUE).toBool();
  conf.endGroup();
  */

  if (moveForcePatternsList.isEmpty())
  {
    moveForcePatternsList.append("#=You are borne along by a strong current.");
    moveForcePatternsList.append("#?leads you out");
    moveForcePatternsList.append("#<Your feet slip, making you fall to the");
    moveForcePatternsList.append("#<Suddenly an explosion of ancient rhymes");
  }

  if (moveCancelPatternsList.isEmpty())
  {
    moveCancelPatternsList.append("#=Your boat cannot enter this place.");
    moveCancelPatternsList.append("#>steps in front of you.");
    moveCancelPatternsList.append("#>bars your way.");
    moveCancelPatternsList.append("#=Maybe you should get on your feet first?");
    moveCancelPatternsList.append("#<Alas, you cannot go that way...");
    moveCancelPatternsList.append("#<You need to swim");
    moveCancelPatternsList.append("#=You failed swimming there.");
    moveCancelPatternsList.append("#<You failed to climb there");
    moveCancelPatternsList.append("#=No way! You are fighting for your life!");
    moveCancelPatternsList.append("#<Nah...");
    moveCancelPatternsList.append("#<You are too exhausted.");
    moveCancelPatternsList.append("#>is too exhausted.");
    moveCancelPatternsList.append("#=PANIC! You couldn't escape!");
    moveCancelPatternsList.append("#=PANIC! You can't quit the fight!");
    moveCancelPatternsList.append("#<ZBLAM");
    moveCancelPatternsList.append("#<Oops!");
    moveCancelPatternsList.append("#>seems to be closed.");
    moveCancelPatternsList.append("#>ou need to climb to go there.");
    moveCancelPatternsList.append("#=In your dreams, or what?");
    moveCancelPatternsList.append("#<You'd better be swimming");
    moveCancelPatternsList.append("#=You unsuccessfully try to break through the ice.");
    moveCancelPatternsList.append("#=Your mount refuses to follow your orders!");
    moveCancelPatternsList.append("#<You are too exhausted to ride");
    moveCancelPatternsList.append("#=You can't go into deep water!");
    moveCancelPatternsList.append("#=You don't control your mount!");
    moveCancelPatternsList.append("#=Your mount is too sensible to attempt such a feat.");
    moveCancelPatternsList.append("#?* prevents you from going *.");
    moveCancelPatternsList.append("#=Scouting in that direction is impossible.");
    moveCancelPatternsList.append("#<You stop moving towards");
    moveCancelPatternsList.append("#>is too difficult.");
  }

  if (noDescriptionPatternsList.isEmpty())
  {
    noDescriptionPatternsList.append("#=It is pitch black...");
    noDescriptionPatternsList.append("#=You just see a dense fog around you...");
  }

  if (dynamicDescriptionPatternsList.isEmpty())
  {
    dynamicDescriptionPatternsList.append("#!(?:A|An|The)[^.]*\\.");
    dynamicDescriptionPatternsList.append("#![^.]*(?:sit(?:s|ting)?|rest(?:s|ing)?|stand(?:s|ing)|sleep(?:s|ing)?|swim(?:s|ming)?|walk(?:s|ing)?|wander(?:s|ing)?|grow(?:s|ing)?|lies?|lying)[^.]*");
    dynamicDescriptionPatternsList.append("#!.*(?:\\(glowing\\)|\\(shrouded\\)|\\(hidden\\))\\.?");
    dynamicDescriptionPatternsList.append("#![^.]*(in|on) the ground\\.");
    dynamicDescriptionPatternsList.append("#?*");
    dynamicDescriptionPatternsList.append("#<You have found");
    dynamicDescriptionPatternsList.append("#<You have revealed");
    dynamicDescriptionPatternsList.append("#>whip all around you.");
    dynamicDescriptionPatternsList.append("#<Clusters of");
    dynamicDescriptionPatternsList.append("#<Prickly");
    dynamicDescriptionPatternsList.append("#!.*arrived from.*\\.");
  }

  conf.beginGroup("Connection");
  m_remoteServerName = conf.value("Server name", "fire.pvv.org").toString();
  m_remotePort = conf.value("Remote port number", 4242).toInt();
  m_localPort = conf.value("Local port number", 4242).toInt();
  conf.endGroup();

  conf.beginGroup("Canvas");
  m_showUpdated = conf.value("Show updated rooms", TRUE).toBool();
  m_drawNotMappedExits = conf.value("Draw not mapped exits", TRUE).toBool();
  m_drawUpperLayersTextured = conf.value("Draw upper layers textured", FALSE).toBool();
  conf.endGroup();

  conf.beginGroup("Debug");
  m_autoLog = conf.value("Enable log", FALSE).toBool();
  m_logFileName = conf.value("Log file name", "~/mmapper.log").toString();
  conf.endGroup();

  conf.beginGroup("Auto load world");
  m_autoLoadWorld = conf.value("Auto load", FALSE ).toBool();
  m_autoLoadFileName = conf.value("File name", "arda.mm2").toString();
  conf.endGroup();
  
  conf.beginGroup("Path Machine");
  m_acceptBestRelative = conf.value("relative path acceptance", 25).toDouble();
  m_acceptBestAbsolute = conf.value("absolute path acceptance", 6).toDouble();
  m_newRoomPenalty = conf.value("room creation penalty", 5).toDouble();
  m_correctPositionBonus = conf.value("correct position bonus", 5).toDouble();
  m_multipleConnectionsPenalty = conf.value("multiple connections penalty", 2.0).toDouble();
  m_maxPaths = conf.value("maximum number of paths", 1000).toUInt();
  m_matchingTolerance = conf.value("room matching tolerance", 8).toUInt();
  conf.endGroup();

  conf.beginGroup("Group Manager");
  m_groupManagerState = conf.value("group manager state", 2).toInt(); // OFF
  m_groupManagerLocalPort = conf.value("group manager local port", 4243).toInt();
  m_groupManagerRemotePort = conf.value("group manager remote port", 4243).toInt();
  m_groupManagerHost = conf.value("group manager host", "localhost").toByteArray();
  m_groupManagerCharName = conf.value("group manager character name", "MMapper").toByteArray();
  //m_showGroupManager = conf.value("group manager show", false).toBool();
  m_showGroupManager = false;
  m_groupManagerColor = (QColor) conf.value("group manager color", "#ffff00").toString();
  m_groupManagerRect.setRect(conf.value("group manager rectangle x", 0).toInt(),
                             conf.value("group manager rectangle y", 0).toInt(),
                                        conf.value("group manager rectangle width", 0).toInt(),
                                            conf.value("group manager rectangle height", 0).toInt());
  m_groupManagerRulesWarning = conf.value("group manager rules warning", true).toBool();
  conf.endGroup();

};

void Configuration::write() const {

  QSettings conf("MUME", "mClient");

  conf.beginGroup("General");
  conf.setValue("Run first time", FALSE);
  conf.setValue("Window Position", windowPosition);
  conf.setValue("Window Size", windowSize);
  conf.setValue("Window State", windowState);
  conf.setValue("Scrollback Size", scrollbackSize);
  conf.setValue("Always On Top", alwaysOnTop);
  conf.setValue("Use Internal Editor", useInternalEditor);
  conf.setValue("Map Mode", m_mapMode);
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
