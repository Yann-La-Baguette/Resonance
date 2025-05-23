#include "audiodevicemanager.h"

AudioDeviceManager& AudioDeviceManager::instance() {
    static AudioDeviceManager instance;
    return instance;
}

void AudioDeviceManager::setAudioDevice(const QAudioDevice& device) {
    selectedDevice = device;
}

QAudioDevice AudioDeviceManager::getAudioDevice() const {
    return selectedDevice;
}
