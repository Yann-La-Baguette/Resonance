#ifndef AUDIOCAPTURE_H
#define AUDIOCAPTURE_H

#include <QAudioSource>
#include <QAudioDevice>
#include <QMediaDevices>
#include <QAudioFormat>
#include <QUdpSocket>
#include <QDebug>



class AudioCapture : public QObject {
    Q_OBJECT

public:
    AudioCapture(QObject *parent);
private slots:
    void captureAndSendAudio();

private:
    QAudioSource *audioSource;
    QIODevice *audioInput;
    QUdpSocket *socket;
};

#endif // AUDIOCAPTURE_H
