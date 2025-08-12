#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

class ResourceManager {
public:
    // Delete copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    static ResourceManager& getInstance();
    
    // Enhanced methods
    sf::Texture& getTexture(const std::string& path);
    sf::Font& getFont(const std::string& path, bool isEmojiFont = false);
    sf::SoundBuffer& getSoundBuffer(const std::string& path);

    // Helper to get default system font paths
    static std::vector<std::string> getSystemFontPaths();
    ResourceManager();


private:    
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;

    // Internal font loading helper
    bool tryLoadFont(sf::Font& font, const std::string& path);
};