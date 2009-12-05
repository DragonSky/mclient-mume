#!/usr/bin/env python

from PyQt4 import QtCore, QtNetwork


class ProxyServer(QtCore.QThread):
    def __init__(self, parent=None):
        super(ProxyServer, self).__init__(parent)

        self.clients = []
        self.serverPort = 23
        self.serverAddress = 'mume.org'

        self.tcpServer = QtNetwork.QTcpServer()
        self.tcpClient = QtNetwork.QTcpSocket()

        self.tcpServer.listen(\
                QtNetwork.QHostAddress(QtNetwork.QHostAddress.Any), 4242)

        self.tcpServer.newConnection.connect(self.clientConnected)
        self.tcpClient.readyRead.connect(self.dataFromServer)

        self.connected = 0

    def clientConnected(self):
        if self.tcpServer.hasPendingConnections():
            sock = self.tcpServer.nextPendingConnection()
            if not self.connected:
                self.tcpClient.connectToHost(self.serverAddress,\
                        self.serverPort)
                self.connected = 1

            sock.readyRead.connect(self.dataFromClient)
            self.clients.append(sock)

    def dataFromServer(self):
        len = self.tcpClient.bytesAvailable()
        if len > 0:
            bytes = self.tcpClient.read(len)
            for sock in self.clients:
                sock.write(bytes)

    def run(self):
        print "running!" 

    def dataFromClient(self):
        for sock in self.clients:
            len = sock.bytesAvailable()
            if len > 0:
                bytes = sock.read(len)
                self.tcpClient.write(bytes)

if __name__ == '__main__':
    
    import sys
    app = QtCore.QCoreApplication(sys.argv)

    server = ProxyServer()
    sys.exit(app.exec_())
