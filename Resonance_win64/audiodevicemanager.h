#ifndef AUDIODEVICEMANAGER_H
#define AUDIODEVICEMANAGER_H

#include <QAudioDevice>

class AudioDeviceManager {
public:
    static AudioDeviceManager& instance();

    void setAudioDevice(const QAudioDevice& device);

    QAudioDevice getAudioDevice() const;

private:
    QAudioDevice selectedDevice;

    // Empêcher la création de plusieurs instances
    AudioDeviceManager() = default;
    ~AudioDeviceManager() = default;
    AudioDeviceManager(const AudioDeviceManager&) = delete;
    AudioDeviceManager& operator=(const AudioDeviceManager&) = delete;
};

#endif // AUDIODEVICEMANAGER_H


