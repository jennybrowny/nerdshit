#pragma once
#include "game_state.hpp"
#include "../button.hpp"
#include <vector>
#include <ctime>
#include <memory>

struct SaveSlotDisplay {
    int slotNumber;
    bool exists;
    bool isSelected;
    int act;
    int scene;
    std::time_t saveTime;
    std::string displayText;
    std::string timeText;
    sf::RectangleShape backgroundShape;
    sf::RectangleShape selectionOutline;
    
    // FIXED: sf::Text needs font reference in SFML 3
    std::unique_ptr<sf::Text> mainText;
    std::unique_ptr<sf::Text> timeDisplay;
    
    // NEW: Individual Clear button for each slot
    std::unique_ptr<Button> clearButton;
    
    // Default constructor
    SaveSlotDisplay() 
        : slotNumber(0), exists(false), isSelected(false), 
          act(0), scene(0), saveTime(0) {}
          
    // FIXED: Add move constructor and assignment operator
    SaveSlotDisplay(SaveSlotDisplay&& other) noexcept
        : slotNumber(other.slotNumber), exists(other.exists), isSelected(other.isSelected),
          act(other.act), scene(other.scene), saveTime(other.saveTime),
          displayText(std::move(other.displayText)), timeText(std::move(other.timeText)),
          backgroundShape(std::move(other.backgroundShape)), 
          selectionOutline(std::move(other.selectionOutline)),
          mainText(std::move(other.mainText)), timeDisplay(std::move(other.timeDisplay)),
          clearButton(std::move(other.clearButton)) {}  // NEW: Move clearButton too
          
    SaveSlotDisplay& operator=(SaveSlotDisplay&& other) noexcept {
        if (this != &other) {
            slotNumber = other.slotNumber;
            exists = other.exists;
            isSelected = other.isSelected;
            act = other.act;
            scene = other.scene;
            saveTime = other.saveTime;
            displayText = std::move(other.displayText);
            timeText = std::move(other.timeText);
            backgroundShape = std::move(other.backgroundShape);
            selectionOutline = std::move(other.selectionOutline);
            mainText = std::move(other.mainText);
            timeDisplay = std::move(other.timeDisplay);
            clearButton = std::move(other.clearButton);  // NEW: Move clearButton too
        }
        return *this;
    }
    
    // Delete copy constructor and copy assignment to be explicit
    SaveSlotDisplay(const SaveSlotDisplay&) = delete;
    SaveSlotDisplay& operator=(const SaveSlotDisplay&) = delete;
};

class SaveState : public GameState {
public:
    explicit SaveState(sf::Font& font);
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
    
private:
    void loadSaveSlots();
    void createSlotVisuals();
    void updateSlotSelection(const sf::Vector2f& mousePos);
    void loadSelectedSave(Game& game);
    void deleteSelectedSave();
    
    // NEW: Clear save functionality
    void clearAllSaves();
    void clearSpecificSave(int slotIndex);
    
    bool hasAnySaves() const;
    
    sf::Font& font;
    std::vector<SaveSlotDisplay> saveSlots;
    int selectedSlot;
    int hoveredSlot;
    
    // UI Elements
    std::unique_ptr<Button> backButton;
    std::unique_ptr<Button> loadButton;
    std::unique_ptr<Button> deleteButton;
    std::unique_ptr<Button> newGameButton;
    
    // NEW: Clear All button
    std::unique_ptr<Button> clearAllButton;
    
    // Background elements
    sf::RectangleShape titleBackground;
    sf::Text titleText;
    sf::Text instructionText;
    sf::RectangleShape mainPanel;
    
    // Colors
    static const sf::Color backgroundColor;
    static const sf::Color panelColor;
    static const sf::Color selectedColor;
    static const sf::Color hoveredColor;
    static const sf::Color textColor;
    static const sf::Color emptySlotColor;
};