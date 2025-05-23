// udpsender.cpp
#include "udpsender.h"
#include "audiodevicemanager.h"
#include <QDebug>

UDPSender::UDPSender(QObject *parent) : QObject(parent) {
    socket = new QUdpSocket(this);
}

void UDPSender::updateAudioDevice() {
    if (audioSource) {
        audioSource->stop();
        delete audioSource;
        audioSource = nullptr;
    }

    QAudioDevice audioDevice = AudioDeviceManager::instance().getAudioDevice();
    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Float);

    if (!audioDevice.isFormatSupported(format)) {
        qWarning() << "Format not supported, selecting a compatible format...";
        format = audioDevice.preferredFormat();
    }

    qDebug() << "Using format: " << format.sampleRate()
             << "Hz, " << format.channelCount()
             << "channels, type:" << format.sampleFormat();

    audioSource = new QAudioSource(audioDevice, format, this);
    audioSource->setBufferSize(8192);

    if (isSending) {
        audioInput = audioSource->start();
        if (audioInput) {
            connect(audioInput, &QIODevice::readyRead, this, &UDPSender::captureAndSendAudio, Qt::UniqueConnection);
        } else {
            qWarning() << "Failed to start audio input";
        }
    }
}

void UDPSender::updateIpAddress(const QString &newIpAddress) {
    ipAddressSmartphone = newIpAddress;
    qDebug() << "IP address updated to:" << ipAddressSmartphone;
}

void UDPSender::startSending() {
    if (isSending) return;
    isSending = true;

    if (!audioSource) {
        updateAudioDevice();
    } else {
        audioInput = audioSource->start();
        if (audioInput) {
            connect(audioInput, &QIODevice::readyRead, this, &UDPSender::captureAndSendAudio, Qt::UniqueConnection);
        }
    }
    qDebug() << "Audio sending started";
}

void UDPSender::stopSending() {
    if (!isSending) return;
    isSending = false;

    if (audioSource) {
        audioSource->stop();
    }
    audioInput = nullptr;
    qDebug() << "Audio sending stopped";
}

void UDPSender::captureAndSendAudio() {
    if (!audioInput || !isSending) return;
    QByteArray audioData = audioInput->readAll();
    if (!audioData.isEmpty() && !ipAddressSmartphone.isEmpty()) {
        socket->writeDatagram(audioData, QHostAddress(ipAddressSmartphone), port);

        // Aperçu des 16 premiers octets
        QStringList preview;
        const int previewLen = qMin(16, audioData.size());
        for (int i = 0; i < previewLen; ++i) {
            preview << QString("%1").arg(static_cast<unsigned char>(audioData[i]), 2, 16, QLatin1Char('0'));
        }
        qDebug() << "Audio sent to" << ipAddressSmartphone << ":" << audioData.size()
                 << "bytes | Preview:" << preview.join(' ');
    }
}
