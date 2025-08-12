#pragma once
#include "game_state.hpp"  
#include "button.hpp"
#include "input_textbox.hpp"
#include <memory>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "start_state.hpp" 

class ACT1_state : public GameState {
public:
    explicit ACT1_state(sf::Font& font, int startIndex = 0); 
    
    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;

private:
    // audio resources
    void loadAudioResources();
    // Core systems
    sf::Font& font;
    std::vector<sf::Texture> sprites;
    sf::Texture dummyTexture;
    sf::Sprite currentSprite;
    int currentIndex;
    
    // UI elements - streamlined
    std::unique_ptr<Button> prevButton;
    std::unique_ptr<Button> nextButton;
    std::unique_ptr<Button> menuButton;
    std::unique_ptr<Button> saveButton;
    std::unique_ptr<Button> creditsButton;
    
    // Input systems
    std::unique_ptr<InputTextBox> playerNameInput;
    std::unique_ptr<InputTextBox> examInput;
    
    // Player data
    std::string playerName;
    bool nameEntered = false;
    bool examCompleted = false;
    int examScore = 0;
    bool examScoreLocked = true;
    
    // UI state
    bool showSaveButton;
    bool justSaved = false;
    float saveMessageTimer = 0.0f;
    bool showExamPopup = false;
    float popupTimer = 0.0f;
    bool showNameConfirmation = false;
    
    // Audio state
    bool sprite4AudioPlayed = false;
    int sprite4LoopCount = 0;
    
    // UI elements
    sf::Text saveConfirmationText;
    sf::Text saveTimestampText;
    sf::Text examPopupText;
    sf::Text nameConfirmationText;
    sf::RectangleShape nameConfirmationBackground;
    sf::RectangleShape popupBackground;
    
    // Streamlined helper methods
    void loadSprites();
    void initializeUI();
    void updateUIForCurrentSprite();
    void updateButtons();
    void handleTextInput(Game& game);
    void showExamScorePopup();
    void handleButtonClicks(Game& game, const sf::Vector2f& mousePos);
    void navigateToNext();
    void navigateToPrevious();
    void renderSpriteSpecificContent(Game& game);
    void renderUI(Game& game);
    std::string getCurrentTimestamp();
};