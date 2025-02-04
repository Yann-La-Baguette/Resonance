#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <QtNetwork/QUdpSocket>
#include <QCoreApplication>
#include <QTimer>

class UDPSender : public QObject {
    Q_OBJECT
public:
    UDPSender(QObject *parent = nullptr);

    void sendData();

private:
    QUdpSocket *socket;
    QTimer *timer;
};

#endif // UDPSENDER_H
