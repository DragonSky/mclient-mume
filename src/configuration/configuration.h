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


#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <QtGui>
#include <QtCore>

class Configuration {
  public:
    void read();
    void write() const;
    bool isChanged() { return configurationChanged; }

    bool firstRun;
    QPoint windowPosition;
    QSize windowSize;
    QByteArray windowState;
    bool alwaysOnTop;
    int scrollbackSize;
    void setFirstRun(bool value) { firstRun = value; change(); }
    void setWindowPosition(QPoint pos) {windowPosition = pos; change(); }
    void setWindowSize(QSize size) { windowSize = size; change(); }
    void setWindowState(QByteArray state) { windowState = state; change(); }
    void setAlwaysOnTop(bool b) { alwaysOnTop = b; change(); }
    
    bool useInternalEditor;

    QString profilePath;
    void setProfilePath(QString str) { profilePath = str; change(); }

    QColor blackColor, redColor, greenColor, yellowColor, blueColor, magentaColor;
    QColor cyanColor, grayColor, darkGrayColor, brightRedColor, brightGreenColor;
    QColor brightYellowColor, brightBlueColor, brightMagentaColor, brightCyanColor;
    QColor whiteColor, foregroundColor, backgroundColor;
    void setBlackColor(QColor color) { blackColor = color; change(); }
    void setRedColor(QColor color) { redColor = color; change(); }
    void setGreenColor(QColor color) { greenColor = color; change(); }
    void setYellowColor(QColor color) { yellowColor = color; change(); }
    void setBlueColor(QColor color) { blueColor = color; change(); }
    void setMagentaColor(QColor color) { magentaColor = color; change(); }
    void setCyanColor(QColor color) { cyanColor = color; change(); }
    void setGrayColor(QColor color) { grayColor = color; change(); }
    void setDarkGrayColor(QColor color) { darkGrayColor = color; change(); }
    void setBrightRedColor(QColor color) { brightRedColor = color; change(); }
    void setBrightGreenColor(QColor color) { brightGreenColor = color; change(); }
    void setBrightYellowColor(QColor color) { brightYellowColor = color; change(); }
    void setBrightBlueColor(QColor color) { brightBlueColor = color; change(); }
    void setBrightMagentaColor(QColor color) { brightMagentaColor = color; change(); }
    void setBrightCyanColor(QColor color) { brightCyanColor = color; change(); }
    void setWhiteColor(QColor color) { whiteColor = color; change(); }
    void setForegroundColor(QColor color) { foregroundColor = color; change(); }
    void setBackgroundColor(QColor color) { backgroundColor = color; change(); }

    QFont serverOutputFont, inputLineFont;
    void setServerOutputFont(QFont font) { serverOutputFont = font; change(); }
    void setInputLineFont(QFont font) { inputLineFont = font; change(); }

    bool IAC_prompt_parser, removeXmlTags;
    QStringList moveCancelPatternsList, moveForcePatternsList, noDescriptionPatternsList, dynamicDescriptionPatternsList;
    QString exitsPattern, scoutPattern;
    QByteArray promptPattern, loginPattern, passwordPattern, menuPromptPattern;

    int m_mapMode; //0 play, 1 map

    QString   m_remoteServerName;         /// Remote host and port settings
    quint32   m_remotePort;
    quint32   m_localPort;         /// Port to bind to on local machine

    bool m_autoLog;         // enables log to file
    QString m_logFileName;  // file name to log
    bool m_autoLoadWorld;
    QString m_autoLoadFileName;

    QString m_roomNameColor; // ANSI room name color
    QString m_roomDescColor; // ANSI room descriptions color
    bool m_brief;
    bool m_emulatedExits;
    bool m_showUpdated;
    bool m_drawNotMappedExits;
    bool m_drawUpperLayersTextured;
    
    enum RoomDescriptionsParserType {RDPT_COLOR, RDPT_PARSER, RDPT_LINEBREAK};
    RoomDescriptionsParserType           m_roomDescriptionsParserType;
    quint16  m_minimumStaticLines;
    
    bool m_IAC_prompt_parser;
    bool m_removeXmlTags;
    
    qreal m_acceptBestRelative;
    qreal m_acceptBestAbsolute;
    qreal m_newRoomPenalty;
    qreal m_multipleConnectionsPenalty;
    qreal m_correctPositionBonus;
    quint32 m_maxPaths;
    quint32 m_matchingTolerance;

    int m_groupManagerState;
    int m_groupManagerLocalPort;
    int m_groupManagerRemotePort;
    QByteArray m_groupManagerHost;
    QByteArray m_groupManagerCharName;
    bool m_showGroupManager;
    QRect m_groupManagerRect;
    QColor m_groupManagerColor;
    bool m_groupManagerRulesWarning;

  private:
    Configuration();
    Configuration(const Configuration&);

    bool configurationChanged;
    void change() { configurationChanged = true; }

  friend Configuration& Config();

};

// Returns a reference to the application configuration object.
Configuration& Config();

#endif
