#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <SFML/Audio.hpp>  


class ResourceManager {
public:
// Delete copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    static ResourceManager& getInstance();
    
    // Existing methods
    sf::Texture& getTexture(const std::string& path);
    sf::Font& getFont(const std::string& path);
    
    // New method for sounds
    sf::SoundBuffer& getSoundBuffer(const std::string& path);
private:
    ResourceManager() = default;
    sf::Font createMinimalFont();
    
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;  // Add this line

};