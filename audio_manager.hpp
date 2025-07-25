#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <memory>


class AudioManager {
public:
    static AudioManager& getInstance();
    
    void loadMusic(const std::string& key, const std::string& path);
    void playMusic(const std::string& key);
    void loadSound(const std::string& key, const std::string& path);
    void playSound(const std::string& key);
    
private:
    AudioManager() = default;
    std::unordered_map<std::string, sf::Music> musicMap;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, sf::Sound> sounds;
};