// FIXED audio_manager.cpp - Remove ambiguous function
#include "audio_manager.hpp"
#include <iostream>

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

void AudioManager::loadMusic(const std::string& key, const std::string& path) {
    if (!musicMap[key].openFromFile(path)) {
        throw std::runtime_error("Failed to load music: " + path);
    }
    std::cout << "Loaded music: " << key << " from " << path << std::endl;
}

void AudioManager::playMusic(const std::string& key, bool loop) {
    if (currentMusic) {
        currentMusic->stop();
    }
    
    auto it = musicMap.find(key);
    if (it != musicMap.end()) {
        currentMusic = &it->second;
        currentMusicKey = key;
        currentMusic->setLooping(loop);
        currentMusic->play();
        std::cout << "Playing music: " << key << " (loop: " << (loop ? "true" : "false") << ")" << std::endl;
    } else {
        std::cout << "Warning: Music key '" << key << "' not found" << std::endl;
    }
}

float AudioManager::getMusicVolume() const {
    return currentMusic ? currentMusic->getVolume() : 0.f;
}

void AudioManager::stopMusic(const std::string& key) {
    if (key.empty()) {
        if (currentMusic) {
            currentMusic->stop();
            currentMusic = nullptr;
            currentMusicKey.clear();
            std::cout << "Stopped current music" << std::endl;
        }
    } else {
        auto it = musicMap.find(key);
        if (it != musicMap.end()) {
            it->second.stop();
            std::cout << "Stopped music: " << key << std::endl;
        }
    }
}

// REMOVED: The parameterless stopMusic() function to fix ambiguity

void AudioManager::loadSound(const std::string& key, const std::string& path) {
    if (!soundBuffers[key].loadFromFile(path)) {
        throw std::runtime_error("Failed to load sound: " + path);
    }
    sounds[key] = std::make_unique<sf::Sound>(soundBuffers[key]);
    std::cout << "Loaded sound: " << key << " from " << path << std::endl;
}

sf::SoundSource::Status AudioManager::getMusicStatus(const std::string& key) const {
    if (key.empty() && currentMusic) {
        return currentMusic->getStatus();
    }
    
    auto it = musicMap.find(key);
    if (it != musicMap.end()) {
        return it->second.getStatus();
    }
    return sf::SoundSource::Status::Stopped;
}

void AudioManager::playSound(const std::string& key) {
    auto it = sounds.find(key);
    if (it != sounds.end()) {
        it->second->play();
        std::cout << "Playing sound: " << key << std::endl;
    } else {
        std::cout << "Warning: Sound key '" << key << "' not found" << std::endl;
    }
}

void AudioManager::setMusicLoop(const std::string& key, bool loop) {
    if (key.empty() && currentMusic) {
        currentMusic->setLooping(loop);
        return;
    }
    
    auto it = musicMap.find(key);
    if (it != musicMap.end()) {
        it->second.setLooping(loop);
    }
}

void AudioManager::setMusicVolume(float volume) {
    // Clamp volume to valid range
    volume = std::max(0.0f, std::min(100.0f, volume));
    
    for (auto& pair : musicMap) {
        pair.second.setVolume(volume);
    }
    if (currentMusic) {
        currentMusic->setVolume(volume);
    }
}

// NEW: Set volume for specific sound
void AudioManager::setSoundVolume(const std::string& key, float volume) {
    volume = std::max(0.0f, std::min(100.0f, volume));
    
    auto it = sounds.find(key);
    if (it != sounds.end()) {
        it->second->setVolume(volume);
    }
}

// NEW: Set global sound volume
void AudioManager::setGlobalSoundVolume(float volume) {
    volume = std::max(0.0f, std::min(100.0f, volume));
    
    for (auto& pair : sounds) {
        pair.second->setVolume(volume);
    }
}

// NEW: Get current playing music key
std::string AudioManager::getCurrentMusicKey() const {
    return currentMusicKey;
}

// NEW: Check if specific music is playing
bool AudioManager::isMusicPlaying(const std::string& key) const {
    return getMusicStatus(key) == sf::SoundSource::Status::Playing;
}

// NEW: Check if any music is playing
bool AudioManager::isAnyMusicPlaying() const {
    for (const auto& pair : musicMap) {
        if (pair.second.getStatus() == sf::SoundSource::Status::Playing) {
            return true;
        }
    }
    return false;
}