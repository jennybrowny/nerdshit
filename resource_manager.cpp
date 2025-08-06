#include "resource_manager.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cstdint>
#include <SFML/Audio.hpp>  


ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}
// texture by path
sf::Texture& ResourceManager::getTexture(const std::string& path) {
    if (textures.find(path) == textures.end()) {
        // Try loading from file first
        if (!textures[path].loadFromFile(path)) {
            // Fallback: Create a red error texture
            std::vector<uint8_t> pixels(32 * 32 * 4, 255); // RGBA format
            
            // Make it red (R=255, G=0, B=0, A=255)
            for (size_t i = 0; i < pixels.size(); i += 4) {
                pixels[i+1] = 0; // Green
                pixels[i+2] = 0; // Blue
            }
            
            // Create texture from raw pixels with error checking
            if (!textures[path].loadFromMemory(pixels.data(), pixels.size())) {
                throw std::runtime_error("Failed to create error texture for: " + path);
            }
        }
    }
    return textures[path];
}

// font by path
sf::Font& ResourceManager::getFont(const std::string& path) {
    if (fonts.find(path) == fonts.end()) {
        // Try loading from file first
        if (!fonts[path].openFromFile(path)) {
            // Try fallback font
            if (!fonts[path].openFromFile("assets/fonts/times.ttf")) {
                // Create a basic font as fallback
                sf::Font fallbackFont;
                if (!fallbackFont.openFromFile("assets/fonts/arial.ttf")) {
                    throw std::runtime_error("Failed to load any font");
                }
                fonts[path] = fallbackFont;
            }
        }
    }
    return fonts[path];
}

sf::SoundBuffer& ResourceManager::getSoundBuffer(const std::string& path) {
    // Check if sound is already loaded
    if (soundBuffers.find(path) == soundBuffers.end()) {
        // Load from file (throws exception if fails)
        if (!soundBuffers[path].loadFromFile(path)) {
            throw std::runtime_error("Failed to load sound: " + path);
        }
    }
    return soundBuffers[path];
}