#include "audio_manager.hpp"

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

void AudioManager::loadMusic(const std::string& key, const std::string& path) {
    if (!musicMap[key].openFromFile(path)) {
        throw std::runtime_error("Failed to load music: " + path);
    }
}

void AudioManager::playMusic(const std::string& key) {
    musicMap[key].play();
}

// Add other method implementations...