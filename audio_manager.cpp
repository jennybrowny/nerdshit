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
    // Stop currently playing music
    if (currentMusic) {
        currentMusic->stop();
    }
    
    // Find and play new music
    auto it = musicMap.find(key);
    if (it != musicMap.end()) {
        currentMusic = &it->second;
        currentMusicKey = key;
        currentMusic->setLooping(loop);
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

void AudioManager::playSound(const std::string& key) {
    if (sounds.find(key) != sounds.end()) {
        sounds[key]->play();
    }
}

void AudioManager::setMusicLoop(const std::string& key, bool loop) {
    if (musicMap.find(key) != musicMap.end()) {
        musicMap[key].setLooping(loop);
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