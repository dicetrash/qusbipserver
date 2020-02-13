/* SPDX-License-Identifier: GPL-2.0-or-later */
#include <QCoreApplication>
#include <QProcess>
#include "grouplistener.h"

// TODO make these settings
const qint16 hostPort = 5191; // AIM aint usin it
QString groupIPV4Addr = "239.255.22.71";

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  GroupListener g(groupIPV4Addr, hostPort);

  return a.exec();
}
