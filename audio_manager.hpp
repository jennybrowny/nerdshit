#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <memory>

class AudioManager {
public:
    static AudioManager& getInstance();
    
    // Music methods
    void loadMusic(const std::string& key, const std::string& path);
    void playMusic(const std::string& key, bool loop = false);
    sf::SoundSource::Status getMusicStatus(const std::string& key) const; // Declaration only
    void stopMusic(const std::string& key = "");
    void setMusicLoop(const std::string& key, bool loop);
    void setMusicVolume(float volume);
    float getMusicVolume() const;

    // Sound methods
    void loadSound(const std::string& key, const std::string& path);
    void playSound(const std::string& key);

private:
    AudioManager() = default;
    
    std::unordered_map<std::string, sf::Music> musicMap;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, std::unique_ptr<sf::Sound>> sounds;
    
    sf::Music* currentMusic = nullptr;
    std::string currentMusicKey;
};