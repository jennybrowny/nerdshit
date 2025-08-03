#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>
#include <memory>

class AudioManager {
public:
    static AudioManager& getInstance();
    
    // Music management
    void loadMusic(const std::string& key, const std::string& path);
    void playMusic(const std::string& key, bool loop = true);
    void stopMusic(const std::string& key = "");
    float getMusicVolume() const;
    void setMusicVolume(float volume);
    void setMusicLoop(const std::string& key, bool loop);
    
    // Sound effects
    void loadSound(const std::string& key, const std::string& path);
    void playSound(const std::string& key);

private:
    AudioManager() = default;
    
    // Music storage
    std::unordered_map<std::string, sf::Music> musicMap;
    sf::Music* currentMusic = nullptr;
    std::string currentMusicKey;
    
    // Sound effects storage
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, std::unique_ptr<sf::Sound>> sounds;
};