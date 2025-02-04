#include "udpsender.h"

UDPSender::UDPSender(QObject *parent) : QObject(parent) {
    socket = new QUdpSocket(this);
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &UDPSender::sendData);
    timer->start(1000); // Envoi toutes les secondes (exemple)
}

void UDPSender::sendData() {
    QByteArray data = "Test UDP Message"; // Remplace par des données audio
    QHostAddress recipient("192.168.1.100"); // IP du smartphone
    quint16 port = 5000; // Port d'écoute du smartphone

    socket->writeDatagram(data, recipient, port);
    qDebug() << "Message envoyé";
}
