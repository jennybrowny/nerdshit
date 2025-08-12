#include "resource_manager.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cstdint>
#include <SFML/Audio.hpp>  


ResourceManager::ResourceManager() {
    // Constructor implementation (can be empty if no initialization needed)
}


ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

std::vector<std::string> ResourceManager::getSystemFontPaths() {
    return {
        // System fonts
        "/System/Library/Fonts/Supplemental/Arial.ttf",       // macOS
        "C:/Windows/Fonts/arial.ttf",                        // Windows
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",   // Linux
        
        // Emoji fonts
        "/System/Library/Fonts/Apple Color Emoji.ttc",        // macOS
        "C:/Windows/Fonts/seguiemj.ttf",                     // Windows
        "/usr/share/fonts/noto/NotoColorEmoji.ttf",          // Linux
        
        // Project fonts
        "assets/fonts/arial.ttf",
        "assets/fonts/NotoEmoji-Regular.ttf"
    };
}

bool ResourceManager::tryLoadFont(sf::Font& font, const std::string& path) {
    try {
        return font.openFromFile(path);
    } catch (...) {
        return false;
    }
}

sf::Font& ResourceManager::getFont(const std::string& path, bool isEmojiFont) {
    if (fonts.find(path) == fonts.end()) {
        // Try loading the requested font first
        if (!tryLoadFont(fonts[path], path)) {
            if (isEmojiFont) {
                // Try emoji-specific fallbacks
                for (const auto& fallback : getSystemFontPaths()) {
                    if (fallback.find("Emoji") != std::string::npos || 
                        fallback.find("emoji") != std::string::npos) {
                        if (tryLoadFont(fonts[path], fallback)) {
                            return fonts[path];
                        }
                    }
                }
            }
            
            // Try all system fonts as final fallback
            for (const auto& fallback : getSystemFontPaths()) {
                if (tryLoadFont(fonts[path], fallback)) {
                    return fonts[path];
                }
            }
            
            throw std::runtime_error("Failed to load font: " + path);
        }
    }
    return fonts[path];
}

sf::Texture& ResourceManager::getTexture(const std::string& path) {
    if (textures.find(path) == textures.end()) {
        if (!textures[path].loadFromFile(path)) {
            std::vector<uint8_t> pixels(32 * 32 * 4, 255);
            for (size_t i = 0; i < pixels.size(); i += 4) {
                pixels[i+1] = 0;
                pixels[i+2] = 0;
            }
            if (!textures[path].loadFromMemory(pixels.data(), pixels.size())) {
                throw std::runtime_error("Failed to create error texture for: " + path);
            }
        }
    }
    return textures[path];
}

sf::SoundBuffer& ResourceManager::getSoundBuffer(const std::string& path) {
    if (soundBuffers.find(path) == soundBuffers.end()) {
        if (!soundBuffers[path].loadFromFile(path)) {
            throw std::runtime_error("Failed to load sound: " + path);
        }
    }
    return soundBuffers[path];
}