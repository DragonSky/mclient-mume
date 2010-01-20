#!/usr/bin/env python

from PyQt4 import QtCore, QtNetwork

from PyKDE4.kdeui import KApplication, KNotification, KIcon
from PyKDE4.kdecore import ki18n, KAboutData, KCmdLineArgs

import re

class AnsiStripper():
    def __init__(self):
        #super(AnsiStripper,self).__init(self)
        
        #self.esc = '%s['%chr(27)
        #self.reset = '%s0m'%esc
        #self.format = '1;%dm'
        #self.fgoffset, bgoffset = 30, 40
        
        #self.toStrip = re.compile('^[\[[0-9]\+m\?')
        self.toStrip = re.compile('\033\[[0-9][0-9]?m')
        #self.toStrip = re.compile('\e\[[\d;]+m')

    def stripAnsi(self,str):
        return self.toStrip.sub('',str)


class MUMETellNotifier(KApplication):
    def __init__(self, args=None):
        #super(MUMETellNotifier, self).__init__(self)
        KApplication.__init__(self)

        self.serverPort = 4244
        self.serverAddress = 'luthien.home'

        self.tcpClient = QtNetwork.QTcpSocket()
        self.tcpClient.readyRead.connect(self.dataFromServer)

        self.stripper = AnsiStripper()

        self.debug = 2

        self.connected = 0

        self.connectSocket()


    def connectSocket(self):
        if not self.connected:
            self.tcpClient.connectToHost(self.serverAddress,\
                self.serverPort)
            self.connected = 1


    def dataFromServer(self):
        len = self.tcpClient.bytesAvailable()
        if len > 0:
            bytes = self.tcpClient.read(len)
            # send bytes to a notification
            self.sendNotification(bytes)


    def sendNotification(self, msg):
        if self.debug >= 1: 
            print "* received tell:", msg

        strippedMsg = self.stripper.stripAnsi(msg)

        evt = KNotification.event("mume-tell-received", strippedMsg,\
                KIcon("terminal").pixmap(QtCore.QSize(48,48)))

        if self.debug >=2:
            print evt

    def dataFromClient(self):
        for sock in self.clients:
            len = sock.bytesAvailable()
            if len > 0:
                bytes = sock.read(len)
                self.tcpClient.write(bytes)

if __name__ == '__main__':
    import sys

    appName     = "mumetellnotifier"
    catalog     = ""
    programName = ki18n("MUME Tell Notifications")
    version     = "0.2"
    description = ki18n("desc")
    license     = KAboutData.License_GPL
    copyright   = ki18n("(c) 2009 Kevin Wilson")
    text        = ki18n("none")
    homePage    = ""
    bugEmail    = "kwilson@barney.physics.sc.edu"

    aboutData   = KAboutData(appName, catalog, programName, version,
        description, license, copyright, text, homePage, bugEmail)

    aboutData.setProgramIconName("terminal")

    KCmdLineArgs.init(sys.argv, aboutData)

     
    app = MUMETellNotifier()

    sys.exit(app.exec_())
