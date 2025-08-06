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

void AudioManager::playMusic(const std::string& key, bool loop) {
    if (currentMusic) {
        currentMusic->stop();
    }
    
    auto it = musicMap.find(key);
    if (it != musicMap.end()) {
        currentMusic = &it->second;
        currentMusicKey = key;
        currentMusic->setLooping(loop);  // Changed from setLoop to setLooping
        currentMusic->play();
    }
}

float AudioManager::getMusicVolume() const {
    return currentMusic ? currentMusic->getVolume() : 0.f;
}

void AudioManager::stopMusic(const std::string& key) {
    if (key.empty()) {
        if (currentMusic) {
            currentMusic->stop();
        }
    } else {
        auto it = musicMap.find(key);
        if (it != musicMap.end()) {
            it->second.stop();
        }
    }
}

void AudioManager::loadSound(const std::string& key, const std::string& path) {
    if (!soundBuffers[key].loadFromFile(path)) {
        throw std::runtime_error("Failed to load sound: " + path);
    }
    sounds[key] = std::make_unique<sf::Sound>(soundBuffers[key]);
}

sf::SoundSource::Status AudioManager::getMusicStatus(const std::string& key) const {
    auto it = musicMap.find(key);
    if (it != musicMap.end()) {
        return it->second.getStatus();
    }
    return sf::SoundSource::Status::Stopped;
}

void AudioManager::playSound(const std::string& key) {
    if (sounds.find(key) != sounds.end()) {
        sounds[key]->play();
    }
}

void AudioManager::setMusicLoop(const std::string& key, bool loop) {
    if (musicMap.find(key) != musicMap.end()) {
        musicMap[key].setLooping(loop);  // Changed from setLoop to setLooping
    }
}

void AudioManager::setMusicVolume(float volume) {
    for (auto& pair : musicMap) {
        pair.second.setVolume(volume);
    }
    if (currentMusic) {
        currentMusic->setVolume(volume);
    }
}