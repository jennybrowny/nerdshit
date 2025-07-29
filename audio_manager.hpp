#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <memory>


class AudioManager {
public:
    // Singleton pattern to ensure only one instance of AudioManager exists
    static AudioManager& getInstance();
    
    /**
     * Loads music from a file and associates it with a key.
     * Throws std::runtime_error if loading fails.
     */
    void loadMusic(const std::string& key, const std::string& path);



    /**
     * Plays the music associated with the given key.
     * If the music is not loaded, this will do nothing.
     */
    void playMusic(const std::string& key);


    /**
     * Loads a sound effect from a file and associates it with a key.
     * Throws std::runtime_error if loading fails.
     */
    void loadSound(const std::string& key, const std::string& path);

    /**
     * Plays the sound effect associated with the given key.
     * If the sound is not loaded, this will do nothing.
     */

    void playSound(const std::string& key);
    
private:
    // Private constructor prevents direct instantiation (Singleton pattern)
    AudioManager() = default;
     // AUDIO STORAGE ====================
    /**
     * musicMap - Stores streaming music objects
     * Key-value pairs where the key is a string identifier and 
     * value is an sf::Music object that streams from file
     */
    std::unordered_map<std::string, sf::Music> musicMap;
     /**
     * soundBuffers - Stores loaded sound data in memory
     * Contains the actual audio data for sound effects.
     * SoundBuffer objects hold the raw audio in RAM for instant access.
     */
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    /**
     * sounds - Stores sound playback objects
     * Each sf::Sound object can play audio from a SoundBuffer.
     * Using unique_ptr for automatic memory management and to allow
     * multiple simultaneous playbacks of the same sound effect.
     */
    std::unordered_map<std::string, std::unique_ptr<sf::Sound>> sounds;
};