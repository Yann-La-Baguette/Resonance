#include "audiocapture.h"


AudioCapture::AudioCapture(QObject *parent = nullptr) : QObject(parent) {
    socket = new QUdpSocket(this);

    // Sélectionner le périphérique de boucle locale
    QAudioDevice audioDevice = QMediaDevices::defaultAudioOutput(); // Assurez-vous que c'est le périphérique de boucle locale

    // Définir le format audio
    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Float);

    // Vérifier et ajuster le format si nécessaire
    if (!audioDevice.isFormatSupported(format)) {
        qWarning() << "Format non supporté, sélection d'un format compatible...";
        format = audioDevice.preferredFormat();
    }

    // Afficher le format choisi
    qDebug() << "Utilisation du format: " << format.sampleRate()
             << "Hz, " << format.channelCount()
             << "canaux, type:" << format.sampleFormat();

    // Initialiser la capture audio
    audioSource = new QAudioSource(audioDevice, format, this);
    audioInput = audioSource->start(); // Initialiser audioInput ici

    if (audioInput) {
        // Connecter l'entrée audio au slot captureAndSendAudio
        connect(audioInput, &QIODevice::readyRead, this, &AudioCapture::captureAndSendAudio);
    } else {
        qWarning() << "Échec du démarrage de l'entrée audio";
    }
}

void AudioCapture::captureAndSendAudio() {
    QByteArray audioData = audioInput->readAll();
    if (!audioData.isEmpty()) {
        socket->writeDatagram(audioData, QHostAddress("127.0.0.1"), 5000); // Remplacez par l'adresse IP et le port appropriés
        qDebug() << "Audio envoyé: " << audioData.size() << " octets";
    }
}
