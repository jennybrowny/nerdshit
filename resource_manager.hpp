#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

class ResourceManager {
public:
    static ResourceManager& getInstance();
    
    sf::Texture& getTexture(const std::string& path);
    sf::Font& getFont(const std::string& path);

private:
    ResourceManager() = default;
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
};