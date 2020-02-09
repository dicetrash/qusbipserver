// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef GROUPLISTENER_H
#define GROUPLISTENER_H

#include <QHostAddress>
#include <QUdpSocket>

class GroupListener: public QObject
{
 Q_OBJECT
 public:
   GroupListener(QString groupIpv4Host, qint16 hostPort);
 public slots:
   void dataRecieved();
 private:
   QUdpSocket listener {};
   QHostAddress groupAddress;
   quint16 hostPort;
};

#endif // GROUPLISTENER_H
