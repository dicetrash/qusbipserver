// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef GROUPLISTENER_H
#define GROUPLISTENER_H

#include <QHostAddress>
#include <QUdpSocket>
#include "udevmonitor.h"

class GroupListener: public QObject
{
 Q_OBJECT
 public:
   GroupListener(QString groupIpv4Host, qint16 hostPort);
 public slots:
   void dataRecieved();
   void monitorUpdate();
 private:
   QUdpSocket listener {};
   UdevMonitor monitor {};
   QHostAddress groupAddress;
   quint16 hostPort;
};

#endif // GROUPLISTENER_H
