// FIXED audio_manager.hpp - Remove ambiguous overload
#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <memory>
#include <string>

class AudioManager {
private:
    std::unordered_map<std::string, sf::Music> musicMap;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, std::unique_ptr<sf::Sound>> sounds;
    
    sf::Music* currentMusic = nullptr;
    std::string currentMusicKey;
    
    AudioManager() = default;

public:
    static AudioManager& getInstance();
    
    // Music management
    void loadMusic(const std::string& key, const std::string& path);
    void playMusic(const std::string& key, bool loop = true);
    void stopMusic(const std::string& key = "");  // Empty string stops current
    // REMOVED: void stopMusic(); // This was causing ambiguity
    
    // Music properties
    void setMusicLoop(const std::string& key, bool loop);
    void setMusicVolume(float volume);
    float getMusicVolume() const;
    sf::SoundSource::Status getMusicStatus(const std::string& key = "") const;
    
    // NEW: Enhanced music control
    std::string getCurrentMusicKey() const;
    bool isMusicPlaying(const std::string& key) const;
    bool isAnyMusicPlaying() const;
    
    // Sound management
    void loadSound(const std::string& key, const std::string& path);
    void playSound(const std::string& key);
    
    // NEW: Sound volume control
    void setSoundVolume(const std::string& key, float volume);
    void setGlobalSoundVolume(float volume);
    
    // Prevent copying
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
};