#include "resource_manager.hpp"

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
} 

sf::Texture& ResourceManager::getTexture(const std::string& path) {
    if (textures.find(path) == textures.end()) {
        if (!textures[path].loadFromFile(path)) {
            throw std::runtime_error("Failed to load texture: " + path);
        }
    }
    return textures[path];
}

sf::Font& ResourceManager::getFont(const std::string& path) {
    // Similar implementation for fonts
}