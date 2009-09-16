/************************************************************************
**
** Authors:   Ulf Hermann <ulfonk_mennhar@gmx.de> (Alve),
**            Marek Krejza <krejza@gmail.com> (Caligor)
**
** This file is part of the MMapper2 project.
** Maintained by Marek Krejza <krejza@gmail.com>
**
** Copyright: See COPYING file that comes with this distributione
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


#include "abstractparser.h"
#include "mmapper2event.h"
#include "mmapper2room.h"
#include "mmapper2exit.h"
#include "configuration.h"

const QChar AbstractParser::escChar('\x1B');
const QString AbstractParser::nullString;
const QString AbstractParser::emptyString("");
const QByteArray AbstractParser::emptyByteArray("");

AbstractParser::AbstractParser(MapData* md, QObject *parent)
  : QObject(parent)
{
  m_readingRoomDesc = false;
  m_descriptionReady = false;
  m_examine = false;

  m_mapData = md;
   /*m_roomName = "";
  m_dynamicRoomDesc = "";
  m_staticRoomDesc = "";*/
  m_exitsFlags = 0;
  m_promptFlags = 0;
}

void AbstractParser::characterMoved(CommandIdType c, const QString& roomName, const QString& dynamicRoomDesc, const QString& staticRoomDesc, ExitsFlagsType exits, PromptFlagsType prompt)
{
  emit event(createEvent(c, roomName, dynamicRoomDesc, staticRoomDesc, exits, prompt));
}

void AbstractParser::emptyQueue()
{
  queue.clear();
}

QString& AbstractParser::removeAnsiMarks(QString& str) {
  static const QChar colorEndMark('m');
  QString out=emptyString;
  bool started=FALSE;

  for ( int i=0; i< str.length(); i++ ){
    if ( started && (str.at(i) == colorEndMark)){
      started = FALSE;
      continue;
    }
    if (str.at(i) == escChar){
      started = TRUE;
      continue;
    }
    if (started) continue;
    out.append((str.at(i).toAscii()));
  }
  str = out;
  return str;
}

void AbstractParser::parsePrompt(QString& prompt){
  m_promptFlags = 0;
  quint8 index = 0;
  int sv;

  emit sendPromptLineEvent(m_stringBuffer.toAscii());

  switch (sv=(int)((prompt[index]).toAscii()))
  {
    case 42: index++;m_promptFlags=SUN_ROOM;break; // *  // sunlight
    case 33: index++;break; // !  // artifical light
    case 41: index++;m_promptFlags=SUN_ROOM;break; // )  // moonlight
    case 111:index++;break; // o  // darkness
    default:;
  }

  switch ( sv = (int)(prompt[index]).toAscii() )
  {
    case 91: SET(m_promptFlags,RTT_INDOORS); break; // [  // indoors
    case 35: SET(m_promptFlags,RTT_CITY); break; // #  // city
    case 46: SET(m_promptFlags,RTT_FIELD); break; // .  // field
    case 102: SET(m_promptFlags,RTT_FOREST); break; // f  // forest
    case 40: SET(m_promptFlags,RTT_HILLS); break; // (  // hills
    case 60: SET(m_promptFlags,RTT_MOUNTAINS); break; // <  // mountains
    case 37: SET(m_promptFlags,RTT_SHALLOW); break; // %  // shallow
    case 126:SET(m_promptFlags,RTT_WATER); break; // ~w  // water
    case 87: SET(m_promptFlags,RTT_RAPIDS); break; // W  // rapids
    case 85: SET(m_promptFlags,RTT_UNDERWATER); break; // U  // underwater
    case 43: SET(m_promptFlags,RTT_ROAD); break; // +  // road
    case 61: SET(m_promptFlags,RTT_TUNNEL); break; // =  // tunnel
    case 79: SET(m_promptFlags,RTT_CAVERN); break; // O  // cavern
    case 58: SET(m_promptFlags,RTT_BRUSH); break; // :  // brush
    default:;
  }
  SET(m_promptFlags,PROMPT_FLAGS_VALID);
}

void AbstractParser::parseExits(QString& str)
{
  m_exitsFlags = EXITS_FLAGS_VALID;
  
  //const char* data = str.toAscii().data();
  bool doors=FALSE;
  bool road=FALSE;
  int length = str.length();
  
  for (int i=7; i<length; i++){
    switch ((int)(str.at(i).toAscii())){
    case 40: doors=true;break;    // (
    case 91: doors=true;break;    // [
    case 61: road=true;break;             // =
      
    case 110:  // n
      if ( (i+2)<length && (str.at(i+2).toAscii()) == 'r') //north
        {
          i+=5;
          if (doors){
            SET(m_exitsFlags,DOOR_N);
            SET(m_exitsFlags,EXIT_N);
            doors=false;
          }else
            SET(m_exitsFlags,EXIT_N);
	  if (road){
	    SET(m_exitsFlags,ROAD_N);
	    road=false;
	  }
        }
      else
	i+=4;  //none
      break;
      
    case 115:  // s
      i+=5;
      if (doors){
	SET(m_exitsFlags,DOOR_S);
	SET(m_exitsFlags,EXIT_S);
	doors=false;
      }else
	SET(m_exitsFlags,EXIT_S);
      if (road){
	SET(m_exitsFlags,ROAD_S);
	road=false;
      }
      break;
      
    case 101:  // e
      i+=4;
      if (doors){
	SET(m_exitsFlags,DOOR_E);
	SET(m_exitsFlags,EXIT_E);
	doors=false;
      }else
	SET(m_exitsFlags,EXIT_E);
      if (road){
	SET(m_exitsFlags,ROAD_E);
	road=false;
      }
      break;
      
    case 119:  // w
      i+=4;
      if (doors){
	SET(m_exitsFlags,DOOR_W);
	SET(m_exitsFlags,EXIT_W);
	doors=false;
      }else
	SET(m_exitsFlags,EXIT_W);
      if (road){
	SET(m_exitsFlags,ROAD_W);
	road=false;
      }
      break;
      
    case 117:  // u
      i+=2;
      if (doors){
	SET(m_exitsFlags,DOOR_U);
	SET(m_exitsFlags,EXIT_U);
	doors=false;
      }else
	SET(m_exitsFlags,EXIT_U);
      if (road){
	SET(m_exitsFlags,ROAD_U);
	road=false;
      }
      break;
      
    case 100:  // d
      i+=4;
      if (doors){
	SET(m_exitsFlags,DOOR_D);
	SET(m_exitsFlags,EXIT_D);
	doors=false;
      }else
	SET(m_exitsFlags,EXIT_D);
      if (road){
	SET(m_exitsFlags,ROAD_D);
	road=false;
      }
      break;
    default:;
    }
  }
  /*
  Coordinate c;
  QByteArray dn = emptyByteArray;
  QByteArray cn = " -";
  
  CommandQueue tmpqueue;
  bool noDoors = true;
  
  if (!queue.isEmpty())
    tmpqueue.enqueue(queue.head());
  
  QList<Coordinate> cl = m_mapData->getPath(tmpqueue);
  if (!cl.isEmpty())
    c = cl.at(cl.size()-1);
  else
    c = m_mapData->getPosition();

  for (uint i=0;i<6;i++)
    {
      dn = m_mapData->getDoorName(c, i).toAscii();
      if ( dn != emptyByteArray )
	{
	  noDoors = false;
	  switch (i)
	    {
	    case 0:
	      cn += " n:"+dn;
	      break;
	    case 1:
	      cn += " s:"+dn;
	      break;
	    case 2:
	      cn += " e:"+dn;
	      break;
	    case 3:
	      cn += " w:"+dn;
	      break;
	    case 4:
	      cn += " u:"+dn;
	      break;
	    case 5:
	      cn += " d:"+dn;
	      break;
	    default:
	      break;
	    }
	}
    }
  
  if (noDoors)
    {
      cn = "\r\n";
    }
  else
    {
      cn += ".\r\n";
      
    }

  qDebug() << "###displaying exit" << str.trimmed().toAscii() << cn;
  emit sendToUser(str.trimmed().toAscii()+cn);
  */
}

void AbstractParser::emulateExits()
{
  Coordinate c;

  CommandQueue tmpqueue;

  if (!queue.isEmpty())
    tmpqueue.enqueue(queue.head());

  QList<Coordinate> cl = m_mapData->getPath(tmpqueue);
  if (!cl.isEmpty())
    c = cl.at(cl.size()-1);
  else
    c = m_mapData->getPosition();

  const RoomSelection * rs = m_mapData->select();
  const Room *r = m_mapData->getRoom(c, rs);

  sendRoomExitsInfoToUser(r);

  m_mapData->unselect(rs);
}


// void AbstractParser::parseNewUserInput(IncomingData& data)
// {
//   switch (data.type)
//   {
//     case TDT_DELAY:
//     case TDT_PROMPT:
//     case TDT_MENU_PROMPT:
//     case TDT_LOGIN:
//     case TDT_LOGIN_PASSWORD:
//     case TDT_TELNET:
//     case TDT_SPLIT:
//     case TDT_UNKNOWN:
//       emit sendToMud(data.line);
//       break;
//     case TDT_CRLF:
//       m_stringBuffer = QString::fromAscii(data.line.constData(), data.line.size());
//       m_stringBuffer = m_stringBuffer.simplified();
//       if (parseUserCommands(m_stringBuffer))
//         emit sendToMud(data.line);
//                                 //else
//                                         //emit sendToMud(QByteArray("\r\n"));
//       break;
//     case TDT_LFCR:
//       m_stringBuffer = QString::fromAscii(data.line.constData(), data.line.size());
//       m_stringBuffer = m_stringBuffer.simplified();
//       if (parseUserCommands(m_stringBuffer))
//         emit sendToMud(data.line);
//                                 //else
//                                         //emit sendToMud(QByteArray("\r\n"));
//       break;
//     case TDT_LF:
//       m_stringBuffer = QString::fromAscii(data.line.constData(), data.line.size());
//       m_stringBuffer = m_stringBuffer.simplified();
//       if ( parseUserCommands(m_stringBuffer))
//         emit sendToMud(data.line);
//                                 //else
//                                         //emit sendToMud(QByteArray("\r\n"));
//       break;
//   }
// }


bool AbstractParser::parseUserCommands(QString& command)
{
  QString str = command;

  if (str=="n" || str=="north"){
    queue.enqueue(CID_NORTH);
    emit showPath(queue, true);
  }
  else if (str=="s" || str=="south"){
    queue.enqueue(CID_SOUTH);
    emit showPath(queue, true);
  }
  else if (str=="e" || str=="east"){
    queue.enqueue(CID_EAST);
    emit showPath(queue, true);
  }
  else if (str=="w" || str=="west"){
    queue.enqueue(CID_WEST);
    emit showPath(queue, true);
  }
  else if (str=="u" || str=="up"){
    queue.enqueue(CID_UP);
    emit showPath(queue, true);
  }
  else if (str=="d" || str=="down"){
    queue.enqueue(CID_DOWN);
    emit showPath(queue, true);
  }
  else if (str=="l" || str=="look"){
    queue.enqueue(CID_LOOK);
  }
  else if (str=="exa" || str=="examine"){
    queue.enqueue(CID_LOOK);
    m_examine = true;
  }

  // Always forward to the command processor
  return true;
}

void AbstractParser::offlineCharacterMove(CommandIdType direction)
{
  bool flee = false;
  if (direction == CID_FLEE)
  {
    flee = true;
    emit sendToUser((QByteArray)"You flee head over heels!\r\n");
    direction = (CommandIdType) (rand() % 6);
  }

  if (!flee) queue.dequeue();

  Coordinate c;
  c = m_mapData->getPosition();
  const RoomSelection * rs1 = m_mapData->select(c);
  const Room *rb = rs1->values().front();

  if (direction == CID_LOOK)
  {
    sendRoomInfoToUser(rb);
    sendRoomExitsInfoToUser(rb);
    sendPromptSimulationToUser();
  }
  else
  {
    const Exit &e = rb->exit((uint)direction);
    if ((getFlags(e) & EF_EXIT) && (e.outBegin() != e.outEnd()))
    {
      const RoomSelection * rs2 = m_mapData->select();
      const Room *r = m_mapData->getRoom(*e.outBegin(), rs2);

      sendRoomInfoToUser(r);
      sendRoomExitsInfoToUser(r);
      sendPromptSimulationToUser();
      characterMoved( direction, getName(r), getDynamicDescription(r), getDescription(r), 0, 0);
      emit showPath(queue, true);
      m_mapData->unselect(rs2);
    }
    else
    {
      if (!flee)
        emit sendToUser((QByteArray)"You cannot go that way...");
      else
        emit sendToUser((QByteArray)"You cannot flee!!!");
      emit sendToUser("\r\n>");
    }
  }
  m_mapData->unselect(rs1);
}

void AbstractParser::sendRoomInfoToUser(const Room* r)
{
  if (!r) return;
  emit sendToUser((QByteArray)"\r\n"+getName(r).toAscii()+(QByteArray)"\r\n");
  emit sendToUser(getDescription(r).toAscii().replace("\n","\r\n"));
  emit sendToUser(getDynamicDescription(r).toAscii().replace("\n","\r\n"));
}

void AbstractParser::sendRoomExitsInfoToUser(const Room* r)
{
  if (!r) return;
  QByteArray dn = emptyByteArray;
  QByteArray cn = " -";
  bool noDoors = true;


  QString etmp = "Exits/emulated:";
  int j;
  for (int jj = 0; jj < 7; jj++) {
    switch (jj)
    {
      case 1: j=2;break;
      case 2: j=1;break;
      default: j=jj;break;
    }

    bool door = false;
    bool exit = false;
    if (ISSET(getFlags(r->exit(j)),EF_DOOR))
    {
      door = true;
      etmp += " {";
    }

    if (ISSET(getFlags(r->exit(j)),EF_EXIT)) {
      exit = true;
      if (!door) etmp += " ";

      switch(j)
      {
        case 0: etmp += "north"; break;
        case 1: etmp += "south"; break;
        case 2: etmp += "east"; break;
        case 3: etmp += "west"; break;
        case 4: etmp += "up"; break;
        case 5: etmp += "down"; break;
        case 6: etmp += "unknown"; break;
      }
    }

    if (door)
    {
                /*if (getDoorName(r->exit(j))!="")
      etmp += "/"+getDoorName(r->exit(j))+"}";
      else*/
      etmp += "},";
    }
    else
    {
      if (exit)
        etmp += ",";
    }
  }
  etmp = etmp.left(etmp.length()-1);
  etmp += ".";

  for (uint i=0;i<6;i++)
  {
    dn = m_mapData->getDoorName(r->getPosition(), i).toAscii();
    if ( dn != emptyByteArray )
    {
      noDoors = false;
      switch (i)
      {
        case 0:
          cn += " n:"+dn;
          break;
        case 1:
          cn += " s:"+dn;
          break;
        case 2:
          cn += " e:"+dn;
          break;
        case 3:
          cn += " w:"+dn;
          break;
        case 4:
          cn += " u:"+dn;
          break;
        case 5:
          cn += " d:"+dn;
          break;
        default:
          break;
      }
    }
  }

  if (noDoors)
  {
    cn = "\r\n";
  }
  else
  {
    cn += ".\r\n";

  }

  qDebug() << "###displaying emulated exit" << etmp.toAscii() << cn;
  emit sendToUser(etmp.toAscii()+cn);

}

void AbstractParser::sendPromptSimulationToUser()
{
  emit sendToUser("\r\n>");
}

void AbstractParser::performDoorCommand(DirectionType direction, DoorActionType action)
{
  QByteArray cn = emptyByteArray;
  QByteArray dn = emptyByteArray;

  switch (action) {
    case DAT_OPEN:
      cn = "open ";
      break;
    case DAT_CLOSE:
      cn = "close ";
      break;
    case DAT_LOCK:
      cn = "lock ";
      break;
    case DAT_UNLOCK:
      cn = "unlock ";
      break;
    case DAT_PICK:
      cn = "pick ";
      break;
    case DAT_ROCK:
      cn = "throw rock ";
      break;
    case DAT_BASH:
      cn = "bash ";
      break;
    case DAT_BREAK:
      cn = "cast 'break door' ";
      break;
    case DAT_BLOCK:
      cn = "cast 'block door' ";
      break;
    default:
      break;
  }

  Coordinate c;
  QList<Coordinate> cl = m_mapData->getPath(queue);
  if (!cl.isEmpty())
    c = cl.at(cl.size()-1);
  else
    c = m_mapData->getPosition();

  dn = m_mapData->getDoorName(c, direction).toAscii();

  bool needdir = false;

  if (dn == emptyByteArray)
  {
    dn = "exit";
    needdir = true;
  }
  else
    for (int i=0; i<6; i++)
  {
    if ( (((DirectionType)i) != direction) && (m_mapData->getDoorName(c, (DirectionType)i).toAscii() == dn) )
      needdir = true;
  }

  switch (direction)
  {
    case NORTH:
      if (needdir)
        cn += dn+" n\r\n";
      else
        cn += dn+"\r\n";
      break;
    case SOUTH:
      if (needdir)
        cn += dn+" s\r\n";
      else
        cn += dn+"\r\n";
      break;
    case EAST:
      if (needdir)
        cn += dn+" e\r\n";
      else
        cn += dn+"\r\n";
      break;
    case WEST:
      if (needdir)
        cn += dn+" w\r\n";
      else
        cn += dn+"\r\n";
      break;
    case UP:
      if (needdir)
        cn += dn+" u\r\n";
      else
        cn += dn+"\r\n";
      break;
    case DOWN:
      if (needdir)
        cn += dn+" d\r\n";
      else
        cn += dn+"\r\n";
      break;
    default:
      cn += dn+"\r\n";
      break;
  }

  if (Config().m_mapMode != 2)  // online mode
  {
    emit sendToMud(cn);
    emit sendToUser("--->" + cn);
  }
  else
  {
    emit sendToUser("--->" + cn);
    emit sendToUser("OK.\r\n");
  }
}

void AbstractParser::genericDoorCommand(QString command, DirectionType direction)
{
  QByteArray cn = emptyByteArray;
  QByteArray dn = emptyByteArray;

  Coordinate c;
  QList<Coordinate> cl = m_mapData->getPath(queue);
  if (!cl.isEmpty())
    c = cl.at(cl.size()-1);
  else
    c = m_mapData->getPosition();

  dn = m_mapData->getDoorName(c, direction).toAscii();

  bool needdir = false;

  if (dn == emptyByteArray)
  {
    dn = "exit";
    needdir = true;
  }
  else
    for (int i=0; i<6; i++)
  {
    if ( (((DirectionType)i) != direction) && (m_mapData->getDoorName(c, (DirectionType)i).toAscii() == dn) )
      needdir = true;
  }

  switch (direction)
  {
    case NORTH:
      if (needdir)
        cn += dn+" n\r\n";
      else
        cn += dn+"\r\n";
      command = command.replace("$$DOOR_N$$",cn);
      break;
    case SOUTH:
      if (needdir)
        cn += dn+" s\r\n";
      else
        cn += dn+"\r\n";
      command = command.replace("$$DOOR_S$$",cn);
      break;
    case EAST:
      if (needdir)
        cn += dn+" e\r\n";
      else
        cn += dn+"\r\n";
      command = command.replace("$$DOOR_E$$",cn);
      break;
    case WEST:
      if (needdir)
        cn += dn+" w\r\n";
      else
        cn += dn+"\r\n";
      command = command.replace("$$DOOR_W$$",cn);
      break;
    case UP:
      if (needdir)
        cn += dn+" u\r\n";
      else
        cn += dn+"\r\n";
      command = command.replace("$$DOOR_U$$",cn);
      break;
    case DOWN:
      if (needdir)
        cn += dn+" d\r\n";
      else
        cn += dn+"\r\n";
      command = command.replace("$$DOOR_D$$",cn);
      break;
    default:
      cn += dn+"\r\n";
      command = cn;
      break;
  }

  if (Config().m_mapMode != 2)  // online mode
  {
    emit sendToMud(command.toAscii());
    emit sendToUser("--->" + command.toAscii());
  }
  else
  {
    emit sendToUser("--->" + command.toAscii());
    emit sendToUser("OK.\r\n");
  }
}

void AbstractParser::nameDoorCommand(QString doorname, DirectionType direction)
{
  Coordinate c;
  QList<Coordinate> cl = m_mapData->getPath(queue);
  if (!cl.isEmpty())
    c = cl.at(cl.size()-1);
  else
    c = m_mapData->getPosition();

  //if (doorname.isEmpty()) toggleExitFlagCommand(EF_DOOR, direction);
  m_mapData->setDoorName(c, doorname, direction);
  emit sendToUser("--->Doorname set to: " + doorname.toAscii() + "\n\r");
}

void AbstractParser::toggleExitFlagCommand(uint flag, DirectionType direction)
{
  Coordinate c;
  QList<Coordinate> cl = m_mapData->getPath(queue);
  if (!cl.isEmpty())
    c = cl.at(cl.size()-1);
  else
    c = m_mapData->getPosition();

  m_mapData->toggleExitFlag(c, flag, direction, E_FLAGS);

  QString toggle, flagname;
  if (m_mapData->getExitFlag(c, flag, direction, E_FLAGS))
    toggle = "enabled";
  else
    toggle = "disabled";

  switch(flag)
  {
    case EF_EXIT:
      flagname = "Possible";
      break;
    case EF_DOOR:
      flagname = "Door";
      break;
    case EF_ROAD:
      flagname = "Road";
      break;
    case EF_CLIMB:
      flagname = "Climbable";
      break;
    case EF_RANDOM:
      flagname = "Random";
      break;
    case EF_SPECIAL:
      flagname = "Special";
      break;
    default:
      flagname = "Unknown";
      break;
  }

  emit sendToUser("--->" + flagname.toAscii() + " exit " + toggle.toAscii() + "\n\r");
}

void AbstractParser::toggleDoorFlagCommand(uint flag, DirectionType direction)
{
  Coordinate c;
  QList<Coordinate> cl = m_mapData->getPath(queue);
  if (!cl.isEmpty())
    c = cl.at(cl.size()-1);
  else
    c = m_mapData->getPosition();

  m_mapData->toggleExitFlag(c, flag, direction, E_DOORFLAGS);

  QString toggle, flagname;
  if (m_mapData->getExitFlag(c, flag, direction, E_DOORFLAGS))
    toggle = "enabled";
  else
    toggle = "disabled";

  switch(flag)
  {
    case DF_HIDDEN:
      flagname = "Hidden";
      break;
    case DF_NEEDKEY:
      flagname = "Need key";
      break;
    case DF_NOBLOCK:
      flagname = "No block";
      break;
    case DF_NOBREAK:
      flagname = "No break";
      break;
    case DF_NOPICK:
      flagname = "No pick";
      break;
    case DF_DELAYED:
      flagname = "Delayed";
      break;
    default:
      flagname = "Unknown";
      break;
  }

  emit sendToUser("--->" + flagname.toAscii() + " door " + toggle.toAscii() + "\n\r");
}

void AbstractParser::setRoomFieldCommand(uint flag, uint field)
{
  Coordinate c;
  QList<Coordinate> cl = m_mapData->getPath(queue);
  if (!cl.isEmpty())
    c = cl.at(cl.size()-1);
  else
    c = m_mapData->getPosition();

  m_mapData->setRoomField(c, flag, field);

  emit sendToUser("--->Room field set\n\r");
}

void AbstractParser::toggleRoomFlagCommand(uint flag, uint field)
{
  Coordinate c;
  QList<Coordinate> cl = m_mapData->getPath(queue);
  if (!cl.isEmpty())
    c = cl.at(cl.size()-1);
  else
    c = m_mapData->getPosition();

  m_mapData->toggleRoomFlag(c, flag, field);

  QString toggle;
  if (m_mapData->getRoomFlag(c, flag, field))
    toggle = "enabled";
  else
    toggle = "disabled";

  emit sendToUser("--->Room flag " + toggle.toAscii() + "\n\r");
}
QString& AbstractParser::latinToAscii(QString& str) {
    // latin1 to 7-bit Ascii
    // taken from Pandora
  const unsigned char table[] = {
/*192*/   'A',
          'A',
          'A',
          'A',
          'A',
          'A',
          'A',
          'C',
          'E',
          'E',
          'E',
          'E',
          'I',
          'I',
          'I',
          'I',
          'D',
          'N',
          'O',
          'O',
          'O',
          'O',
          'O',
          'x',
          'O',
          'U',
          'U',
          'U',
          'U',
          'Y',
          'b',
          'B',
          'a',
          'a',
          'a',
          'a',
          'a',
          'a',
          'a',
          'c',
          'e',
          'e',
          'e',
          'e',
          'i',
          'i',
          'i',
          'i',
          'o',
          'n',
          'o',
          'o',
          'o',
          'o',
          'o',
          ':',
          'o',
          'u',
          'u',
          'u',
          'u',
          'y',
          'b',
          'y'
  };
  unsigned char ch;
  int pos;

  for (pos = 0; pos <= str.length(); pos++) {
    ch = str.at(pos).toLatin1();
    if (ch > 128) {
      if (ch < 192)
        ch = 'z';
      else
        ch = table[ ch - 192 ];
      str[pos] = ch;
    }
  }
  return str;
}

