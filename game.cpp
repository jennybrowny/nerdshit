#include "game.hpp"
#include "button.hpp"
#include "audio_manager.hpp"
#include "resource_manager.hpp"
#include "states/start_state.hpp"
#include "states/tutorial_state.hpp"
#include "states/credits_state.hpp"
#include "states/ACT1_state.hpp"
#include "states/start_state.hpp" 
#include <sstream>
#include <iostream>
#include <filesystem>
#include <fstream>

/* 
███╗   ██╗███████╗██████╗ ██████╗ ███████╗██╗  ██╗██╗████████╗
████╗  ██║██╔════╝██╔══██╗██╔══██╗██╔════╝██║  ██║██║╚══██╔══╝
██╔██╗ ██║█████╗  ██████╔╝██║  ██║███████╗███████║██║   ██║   
██║╚██╗██║██╔══╝  ██╔══██╗██║  ██║╚════██║██╔══██║██║   ██║   
██║ ╚████║███████╗██║  ██║██████╔╝███████║██║  ██║██║   ██║  Jenny Brown 
╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝   ╚═╝  ECE205, Summer 2025 
*/

const std::vector<std::string> Game::fontPaths = {
    "assets/fonts/times.ttf",
    "assets/fonts/arial.ttf"
};

Game::Game() : 
    window(sf::VideoMode({800, 600}), "NerdShit"),
    pageText(font, "", 24),
    bgColor(sf::Color::Green),
    currentState(nullptr),
    isMusicPlaying(false),
    currentTutorialIndex(0),
    fontLoaded(false),
    lastSaveTime(0)
{
    loadResources();
    Button::preloadButtonSounds();
}

Game::~Game() {
    currentState.reset();
    std::cout << "Game resources cleaned up\n";
}

void Game::loadResources() {
    loadAudioResources();
    loadVisualResources();
    
    // Load tutorial textures
    auto& resMgr = ResourceManager::getInstance();
    for (const auto& path : tutorialPaths) {
        try {
            tutorialTextures.push_back(resMgr.getTexture(path));
        } catch (const std::exception& e) {
            std::cerr << "Failed to load tutorial texture: " << path << "\n";
        }
    }
    
    // Create first tutorial sprite
    if (!tutorialTextures.empty()) {
        currentTutorialSprite = std::make_unique<sf::Sprite>(tutorialTextures[0]);
        scaleSpriteToWindow(*currentTutorialSprite);
    }
}

void Game::loadAudioResources() {
    auto& audio = AudioManager::getInstance();
    const std::vector<std::pair<std::string, std::string>> audioFiles = {
        {"background", "assets/sounds/Intro.mp3"},
        {"act0", "assets/sounds/ACT0.mp3"},
        {"click", "assets/sounds/button_click.mp3"},
        {"sparkle", "assets/sounds/sparkle.mp3"}
    };

    try {
        for (const auto& [name, path] : audioFiles) {
            audio.loadMusic(name, path);
        }
    } catch (const std::runtime_error& e) {
        handleAudioLoadError(e);
    }
}

void Game::loadVisualResources() {
    try {
        loadTitleScreen();
        loadFont();
    } catch (const std::exception& e) {
        handleVisualLoadError(e);
    }
}

void Game::loadTitleScreen() {
    auto& resMgr = ResourceManager::getInstance();
    titleTexture = resMgr.getTexture("assets/background/title_screen.png");
    titleSprite = std::make_unique<sf::Sprite>(titleTexture);
    scaleSpriteToWindow(*titleSprite);
}

void Game::loadFont() {
    auto& resMgr = ResourceManager::getInstance();
    fontLoaded = false;
    
    for (const auto& path : fontPaths) {
        if (tryLoadFont(path)) {
            font = resMgr.getFont(path);
            fontLoaded = true;
            logFontSuccess(path);
            break;
        }
    }
}

bool Game::tryLoadFont(const std::string& path) {
    try {
        auto& resMgr = ResourceManager::getInstance();
        font = resMgr.getFont(path);
        return true;
    } catch (...) {
        return false;
    }
}

void Game::handleAudioLoadError(const std::runtime_error& e) {
    std::cerr << "Audio load failed: " << e.what() << "\n";
    isMusicPlaying = false;
}

void Game::handleVisualLoadError(const std::exception& e) {
    std::cout << "WARNING: " << e.what() << std::endl;
    bgColor = sf::Color(50, 100, 150);
}

void Game::logFontSuccess(const std::string& path) {
    std::cout << "Successfully loaded font from: " << path << std::endl;
}

void Game::run() {
    std::cout << "=== STARTING GAME LOOP ===" << std::endl;
    std::cout << "Font loaded: " << (fontLoaded ? "YES" : "NO") << std::endl;
    
    // Start background music
    auto& audio = AudioManager::getInstance();
    audio.setMusicVolume(50.f);
    audio.playMusic("background");
    isMusicPlaying = true;
    
    // Initialize first state
    currentState = std::make_unique<StartState>(font);

    while (window.isOpen()) {
        if (currentState) {
            currentState->handleEvents(*this);
            currentState->update(*this);
            currentState->render(*this);
        }
    }
}

void Game::scaleSpriteToWindow(sf::Sprite& sprite) {
    const sf::Texture& texture = sprite.getTexture();
    sf::Vector2u textureSize = texture.getSize();
    sf::Vector2u windowSize = window.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    float scale = std::min(scaleX, scaleY);

    sprite.setScale(sf::Vector2f(scale, scale));
    sprite.setOrigin(sf::Vector2f(textureSize.x / 2.0f, textureSize.y / 2.0f));
    sprite.setPosition(sf::Vector2f(windowSize.x / 2.0f, windowSize.y / 2.0f));
}

void Game::autoResizeSprite(sf::Sprite& sprite, const sf::Vector2u& targetSize) {
    const sf::Texture& texture = sprite.getTexture();
    sf::Vector2u textureSize = texture.getSize();
    
    float scaleX = static_cast<float>(targetSize.x) / textureSize.x;
    float scaleY = static_cast<float>(targetSize.y) / textureSize.y;
    float scale = std::min(scaleX, scaleY);
    
    sprite.setScale(sf::Vector2f(scale, scale));
    sprite.setOrigin(sf::Vector2f(textureSize.x / 2.0f, textureSize.y / 2.0f));
}

void Game::scaleSpriteToContentArea(sf::Sprite& sprite, float uiReservedHeight) {
    const sf::Texture& texture = sprite.getTexture();
    sf::Vector2u textureSize = texture.getSize();
    sf::Vector2u windowSize = window.getSize();
    
    float contentWidth = static_cast<float>(windowSize.x);
    float contentHeight = static_cast<float>(windowSize.y) - uiReservedHeight;
    
    float scaleX = contentWidth / textureSize.x;
    float scaleY = contentHeight / textureSize.y;
    float scale = std::min(scaleX, scaleY);
    
    sprite.setScale(sf::Vector2f(scale, scale));
    sprite.setOrigin(sf::Vector2f(textureSize.x / 2.0f, textureSize.y / 2.0f));
    sprite.setPosition(sf::Vector2f(windowSize.x / 2.0f, contentHeight / 2.0f));
}

sf::Vector2u Game::getCurrentResolution() const {
    return window.getSize();
}

bool Game::wasWindowResized() {
    static sf::Vector2u lastSize = window.getSize();
    sf::Vector2u currentSize = window.getSize();
    
    if (currentSize != lastSize) {
        lastSize = currentSize;
        return true;
    }
    return false;
}

void Game::updatePageIndicator() {
    std::stringstream ss;
    ss << (currentTutorialIndex + 1) << "/" << tutorialTextures.size();
    pageText.setString(ss.str());
}

void Game::goToNextPage() {
    if (!tutorialTextures.empty()) {
        currentTutorialIndex = (currentTutorialIndex + 1) % tutorialTextures.size();
        currentTutorialSprite->setTexture(tutorialTextures[currentTutorialIndex]);
        scaleSpriteToWindow(*currentTutorialSprite);
        updatePageIndicator();
        AudioManager::getInstance().playSound("click");
    }
}

void Game::goToPreviousPage() {
    if (!tutorialTextures.empty()) {
        currentTutorialIndex = (currentTutorialIndex - 1 + tutorialTextures.size()) % tutorialTextures.size();
        currentTutorialSprite->setTexture(tutorialTextures[currentTutorialIndex]);
        scaleSpriteToWindow(*currentTutorialSprite);
        updatePageIndicator();
        AudioManager::getInstance().playSound("click");
    }
}

void Game::changeState(std::unique_ptr<GameState> newState) {
    // Handle music transitions
    if (dynamic_cast<TutorialState*>(newState.get())) {
        AudioManager::getInstance().playMusic("act0", true);
    }
    currentState = std::move(newState);
}

void Game::saveGame(int act, int scene) {
    int slotToUse = 1;
    
    // Find first available slot
    for (int i = 1; i <= 4; i++) {
        std::string saveFile = "savegame_slot" + std::to_string(i) + ".dat";
        std::ifstream checkFile(saveFile);
        if (!checkFile.good()) {
            slotToUse = i;
            break;
        }
    }
    
    std::string saveFile = "savegame_slot" + std::to_string(slotToUse) + ".dat";
    std::ofstream file(saveFile);
    if (file) {
        lastSaveTime = std::time(nullptr);
        file << act << " " << scene << " " << lastSaveTime;
        std::cout << "Game saved to slot " << slotToUse << " at ACT" << act << "_" << scene << std::endl;
    }
}

bool Game::hasSavedGame() const {
    for (int i = 1; i <= 4; i++) {
        std::string saveFile = "savegame_slot" + std::to_string(i) + ".dat";
        std::ifstream file(saveFile);
        if (file.good()) return true;
    }
    return false;
}

std::pair<int, int> Game::loadSavedGame() {
    std::time_t mostRecent = 0;
    int bestAct = 0, bestScene = 0;
    
    for (int i = 1; i <= 4; i++) {
        std::string saveFile = "savegame_slot" + std::to_string(i) + ".dat";
        std::ifstream file(saveFile);
        if (file.good()) {
            int act, scene;
            std::time_t saveTime;
            if (file >> act >> scene >> saveTime) {
                if (saveTime > mostRecent) {
                    mostRecent = saveTime;
                    bestAct = act;
                    bestScene = scene;
                    lastSaveTime = saveTime;
                }
            }
        }
    }
    
    return {bestAct, bestScene};
}

std::string Game::getSaveTime() const {
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&lastSaveTime));
    return buffer;
}

