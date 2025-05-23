#ifndef UDPSENDER_H
#define UDPSENDER_H

// udpsender.h
#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QAudioSource>
#include <QAudioDevice>
#include <QAudioFormat>
#include <QIODevice>

class UDPSender : public QObject {
    Q_OBJECT
public:
    explicit UDPSender(QObject *parent = nullptr);
    void updateAudioDevice();
    void updateIpAddress(const QString &newIpAddress);
    void startSending();
    void stopSending();

private slots:
    void captureAndSendAudio();

private:
    QUdpSocket *socket = nullptr;
    QAudioSource *audioSource = nullptr;
    QIODevice *audioInput = nullptr;
    QString ipAddressSmartphone;
    const quint16 port = 5000;
    bool isSending = false;
};

#endif // UDPSENDER_H
