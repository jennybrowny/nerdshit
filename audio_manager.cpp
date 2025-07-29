#include "audio_manager.hpp"
// Include necessary SFML headers
AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}
// Load music from file and associate with key
// Throws std::runtime_error if loading fails
void AudioManager::loadMusic(const std::string& key, const std::string& path) {
    if (!musicMap[key].openFromFile(path)) {
        throw std::runtime_error("Failed to load music: " + path);
    }
}
// Play music associated with key
// If music is not loaded, this will do nothing
void AudioManager::playMusic(const std::string& key) {
    musicMap[key].play();
    
}
// Load sound effect from file and associate with key
// Throws std::runtime_error if loading fails
void AudioManager::loadSound(const std::string& key, const std::string& path) {
    if (!soundBuffers[key].loadFromFile(path)) {
        throw std::runtime_error("Failed to load sound: " + path);
    }
    sounds[key] = std::make_unique<sf::Sound>(soundBuffers[key]);
}
// Play sound effect associated with key
// If sound is not loaded, this will do nothing
void AudioManager::playSound(const std::string& key) {
    if (sounds.find(key) != sounds.end()) {
        sounds[key]->play();
    }
}