/* SPDX-License-Identifier: GPL-2.0-or-later */
#include "grouplistener.h"

#define FIND_MESSAGE "usbip-find"
#define BIND_PORT_ADM "usbip-bind-adm"
#define IM_A_HOST "usbip-host"
#define LIST_OUT "usbip-list"
#define USB_CHANGE "usb-host-change"

#include <QNetworkDatagram>
#include <QJsonDocument>

extern "C" {
    #include "usbip_bind.c"
    #include "usbip_list.c"
}

GroupListener::GroupListener(QString groupIpv4Host, qint16 hostPort): QObject(nullptr), groupAddress(groupIpv4Host), hostPort(hostPort)
{
  listener.bind(QHostAddress::AnyIPv4, hostPort, QUdpSocket::ShareAddress);
  listener.joinMulticastGroup(groupAddress);
  connect(&listener, &QUdpSocket::readyRead, this, &GroupListener::dataRecieved);
  connect(&monitor, &UdevMonitor::update, this, &GroupListener::monitorUpdate);
  listener.writeDatagram(IM_A_HOST, groupAddress, hostPort);
  usbip_names_init((char*)USBIDS_FILE);
}

void GroupListener::dataRecieved()
{
  while(listener.hasPendingDatagrams()) {
    QNetworkDatagram dgram(listener.receiveDatagram());
    if (dgram.data().contains(IM_A_HOST)) {
      continue;
    }
    if (dgram.data().contains(FIND_MESSAGE)) {
      listener.writeDatagram(dgram.makeReply(IM_A_HOST));
    }
    else if (dgram.data().contains(BIND_PORT_ADM)) {
      char* bus = QJsonDocument::fromJson(dgram.data())["bus"].toString().toLocal8Bit().data();
      auto bindRes = usbip_bind(bus);
      auto result = QJsonDocument::fromVariant(QVariantMap({
         {"process", "usbip-bind-ack"},
         {"recieved", bindRes}
      }));
      listener.writeDatagram(dgram.makeReply(result.toJson()));
    }
    else if (dgram.data().contains(LIST_OUT)) {
      QVariantList devices;
      usbip_external_list* linked_device = list_devices();
      usbip_external_list* current_device = nullptr;
      while(linked_device != nullptr) {
          devices.push_back(QVariantMap({
              {"product_name", linked_device->product_name},
              {"busid", linked_device->busid},
              {"vendor", linked_device->idVendor},
              {"product", linked_device->idProduct}
          }));
          current_device = linked_device;
          linked_device = linked_device->next;
          usbip_external_list_free(current_device);
      }

      listener.writeDatagram(dgram.makeReply(QJsonDocument::fromVariant(devices).toJson()));
    }
  }
}

void GroupListener::monitorUpdate(UdevMonitor::UpdateEvent updateEvent)
{
  auto broadcast = QJsonDocument::fromVariant(QVariantMap({
     {"message", USB_CHANGE},
     {"action", updateEvent.action},
     {"device", updateEvent.device}
   })).toJson();
  listener.writeDatagram(broadcast, groupAddress, hostPort);
}
