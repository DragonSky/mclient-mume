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

#include "connectionlistener.h"
#include "configuration.h"
#include "proxy.h"
#include "ParserManager.h"

ConnectionListener::ConnectionListener(QObject *parent): QTcpServer(parent)
{
  _acceptNewConnection = true;

  // ConnectionListener --> MainWindow
  connect(this, SIGNAL(log(const QString&, const QString&)),
          parent->parent(), SLOT(log(const QString&, const QString&)));
  qDebug("ConnectionListener Connected");
}

void ConnectionListener::incomingConnection(int socketDescriptor)
{
  if (_acceptNewConnection)
  {
    emit log ("Listener", "New connection: accepted.");
    doNotAcceptNewConnections();
    ParserManager *prsMgr = ParserManager::self();
    prsMgr->disableWrapper();
    Proxy *proxy = prsMgr->getProxy();
    QString remoteServer = Config().m_remoteServerName;
    int remotePort = Config().m_remotePort;
    proxy = new Proxy(socketDescriptor, remoteServer, remotePort, true, this);
    proxy->start();
  }
  else
  {
    emit log ("Listener", "New connection: rejected.");
    QTcpSocket tcpSocket;
    if (tcpSocket.setSocketDescriptor(socketDescriptor)) {
      tcpSocket.write("You can't connect more than once!!!\r\n",37);
      tcpSocket.flush();
      tcpSocket.disconnectFromHost();
      tcpSocket.waitForDisconnected();
    }
  }
}

void ConnectionListener::doNotAcceptNewConnections()
{
  _acceptNewConnection = false;
}

void ConnectionListener::doAcceptNewConnections()
{
  _acceptNewConnection = true;
}

void ConnectionListener::start() {
  if (!listen(QHostAddress::Any, Config().m_localPort))
  {
    QMessageBox::critical(0, tr("mClient"),
                          tr("Unable to start the server: %1.")
                              .arg(errorString()));
    return;
  }

  //emit log("ConnectionListener", tr("Server bound on localhost to port: %2.").arg(Config().m_localPort));
}
