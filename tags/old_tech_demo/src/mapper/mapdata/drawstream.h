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

#ifndef DRAWSTREAM_H
#define DRAWSTREAM_H

#include "roomoutstream.h"
#include "mapcanvas.h"

class DrawStream : public RoomOutStream {
  public:
		DrawStream(MapCanvas & in, const std::vector<Room *> & in_rooms, const std::vector<std::set<RoomRecipient *> > & in_locks) : canvas(in), rooms(in_rooms), locks(in_locks) {}
    virtual RoomOutStream & operator<<(const Room * room) {
      canvas.drawRoom(room, rooms, locks);
      return *this;
    }
  private:
    MapCanvas & canvas;
		const std::vector<Room *> & rooms;
		const std::vector<std::set<RoomRecipient *> > & locks;
};
#endif