#include "states/ACT1_state.hpp"
#include "game.hpp"
#include "resource_manager.hpp"
#include "states/start_state.hpp"
#include "states/credits_state.hpp"
#include "states/start_state.hpp" 
#include "audio_manager.hpp"
#include <iostream>
#include <filesystem>
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <chrono>

ACT1_state::ACT1_state(sf::Font& font, int startIndex)
    : font(font), 
      currentIndex(startIndex), 
      showSaveButton(startIndex == 0),
      currentSprite(dummyTexture),  // Default constructed
      saveConfirmationText(font),
      saveTimestampText(font),
      examPopupText(font),
      nameConfirmationText(font),
      // Initialize other members
      nameEntered(false),
      examCompleted(false),
      examScore(0),
      showNameConfirmation(false),
      showExamPopup(false),
      justSaved(false),
      saveMessageTimer(0.0f),
      popupTimer(0.0f),
      examScoreLocked(false),
      sprite4AudioPlayed(false),
      sprite4LoopCount(0)
{
    loadSprites();
    
    // Initialize sprite with fallback texture
    if (!sprites.empty() && currentIndex < static_cast<int>(sprites.size())) {
        currentSprite.setTexture(sprites[currentIndex], true);
    } else {
        // Create fallback texture
        unsigned char pixels[] = {255, 255, 255, 255};
        if (dummyTexture.loadFromMemory(pixels, sizeof(pixels))) {
            currentSprite.setTexture(dummyTexture, true);
        }
    }
    
    initializeUI();
    updateUIForCurrentSprite();
    updateButtons();
    loadAudioResources();
}
// Loading singleton 
void ACT1_state::loadAudioResources() {
    auto& audio = AudioManager::getInstance();
    try {
        audio.loadSound("choi", "assets/sounds/choi.ogg");
        audio.loadMusic("mangoman", "assets/sounds/mangoman.mp3");
        audio.loadSound("sparkle", "assets/sounds/sparkle.mp3");
        std::cout << "ACT1 audio resources loaded successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Warning: Could not load ACT1 audio files - " << e.what() << std::endl;
    }
}

void ACT1_state::initializeUI() {
    // Initialize buttons
    prevButton = std::make_unique<Button>(
        sf::Vector2f(100, 45), sf::Vector2f(50, 540),
        sf::Color(45, 45, 50), font, "BACK"
    );
    
    menuButton = std::make_unique<Button>(
        sf::Vector2f(120, 45), sf::Vector2f(180, 540),
        sf::Color(60, 120, 180), font, "MENU"
    );
    
    saveButton = std::make_unique<Button>(
        sf::Vector2f(140, 45), sf::Vector2f(330, 540),
        sf::Color(150, 80, 200), font, "SAVE"
    );
    
    nextButton = std::make_unique<Button>(
        sf::Vector2f(100, 45), sf::Vector2f(650, 540),
        sf::Color(50, 150, 80), font, "NEXT"
    );
    
    creditsButton = std::make_unique<Button>(
        sf::Vector2f(120, 45), sf::Vector2f(500, 540),
        sf::Color(180, 100, 50), font, "CREDITS"
    );

    // Initialize input boxes
    playerNameInput = std::make_unique<InputTextBox>(
        sf::Vector2f(300, 40), sf::Vector2f(250, 200), font, 10
    );
    playerNameInput->setPlaceholderText("Enter your name...");
    playerNameInput->setVisible(false);
    
    // Exam input box positioned at from Mangoman 
    examInput = std::make_unique<InputTextBox>(
        sf::Vector2f(100, 40), sf::Vector2f(500, 350), font, 2
    );
    examInput->setPlaceholderText("??");
    examInput->setVisible(false);
    examInput->setNumericOnly(true);
    examInput->setRightAligned(true);
    
    // Initialize text elements
    nameConfirmationBackground.setSize(sf::Vector2f(400, 70));
    nameConfirmationBackground.setFillColor(sf::Color(25, 35, 50, 240));
    nameConfirmationBackground.setOutlineThickness(2);
    nameConfirmationBackground.setOutlineColor(sf::Color(80, 120, 160));
    nameConfirmationBackground.setPosition(sf::Vector2f(200, 160));
    
    nameConfirmationText.setCharacterSize(20);
    nameConfirmationText.setFillColor(sf::Color(200, 220, 255));
    nameConfirmationText.setStyle(sf::Text::Bold);
    
    saveConfirmationText.setCharacterSize(32);
    saveConfirmationText.setFillColor(sf::Color(100, 255, 100));
    saveConfirmationText.setStyle(sf::Text::Bold);
    saveConfirmationText.setString("GAME SAVED!");
    
    saveTimestampText.setCharacterSize(18);
    saveTimestampText.setFillColor(sf::Color(150, 200, 150));
    
    popupBackground.setSize(sf::Vector2f(400, 120));
    popupBackground.setFillColor(sf::Color(20, 30, 50, 230));
    popupBackground.setOutlineThickness(3);
    popupBackground.setOutlineColor(sf::Color(100, 150, 200));
    popupBackground.setPosition(sf::Vector2f(200, 240));
    
    // [MODIFIED] - Big red exam score text for dramatic effect
    examPopupText.setCharacterSize(42); // Bigger!
    examPopupText.setFillColor(sf::Color::Red); // Red color for impact
    examPopupText.setStyle(sf::Text::Bold);
}

void ACT1_state::updateUIForCurrentSprite() {
    // Reset all UI visibility
    if (playerNameInput) playerNameInput->setVisible(false);
    if (examInput) examInput->setVisible(false);
    creditsButton->setVisible(false);
    showNameConfirmation = false;
    
    // Sprite-specific UI setup
    switch (currentIndex) {
        case 1:  // ACT1_2 - Player name input
            if (playerNameInput) {
                playerNameInput->setVisible(true);
                playerNameInput->setFocused(true);
            }
            break;
            
        case 3:  // ACT1_4 - Exam input and player name display
            if (examInput) {
                examInput->setVisible(true);
                examInput->setFocused(true);
            }
            
            if (!sprite4AudioPlayed) {
                try {
                    AudioManager::getInstance().playMusic("mangoman", false);
                    sprite4AudioPlayed = true;
                    sprite4LoopCount = 0;
                    std::cout << "ACT1_4: Playing mangoman.mp3 (will play exactly 2 times)\n";
                } catch (...) {
                    std::cout << "Warning: Could not play mangoman.mp3\n";
                }
            }
            break;
            
        case 10: // ACT1_11 - Credits button and final message
            creditsButton->setVisible(true);
            
            try {
                AudioManager::getInstance().playSound("sparkle");
                std::cout << "ACT1_11: Playing sparkle.mp3 on entry\n";
            } catch (...) {
                std::cout << "Warning: Could not play sparkle.mp3\n";
            }
            break;
            
        default:
            // Reset audio state when leaving sprite 4
            if (currentIndex != 3 && sprite4AudioPlayed) {
                AudioManager::getInstance().stopMusic("mangoman");
                sprite4AudioPlayed = false;
                sprite4LoopCount = 0;
            }
            break;
    }
}

void ACT1_state::loadSprites() {
    auto& resMgr = ResourceManager::getInstance();
    sprites.clear();
    
    for (int i = 1; i <= 11; i++) {
        std::string path = "assets/ACT1/ACT1_" + std::to_string(i) + ".png";
        try {
            sf::Texture texture = resMgr.getTexture(path);
            if (texture.getSize().x == 0 || texture.getSize().y == 0) {
                std::cerr << "Warning: Empty texture loaded for " << path << "\n";
                continue;
            }
            sprites.push_back(texture);
            std::cout << "Loaded ACT1 sprite " << i << " successfully\n";
        } catch (...) {
            std::cerr << "Failed to load texture: " << path << "\n";
        }
    }
    
    if (sprites.empty()) {
        std::cerr << "ERROR: No ACT1 sprites loaded!\n";
    } else {
        std::cout << "Successfully loaded " << sprites.size() << " ACT1 sprites\n";
    }
}

void ACT1_state::updateButtons() {
    bool hasNext = (currentIndex < static_cast<int>(sprites.size()) - 1);
    bool hasPrev = (currentIndex > 0);
    bool showNext = hasNext;
    
    // ACT1_2: Hide Next until name is entered
    if (currentIndex == 1) {
        showNext = hasNext && nameEntered;
    }
    
    // UPDATED - ACT1_4: Show Next immediately after exam is completed (score displayed)
    if (currentIndex == 3) {
        showNext = hasNext && examCompleted;
    }
    
    nextButton->setVisible(showNext);
    prevButton->setVisible(hasPrev);
    menuButton->setVisible(true);
    saveButton->setVisible(currentIndex == 0);
}

void ACT1_state::handleTextInput(Game& game) {
    // ACT1_2: Handle player name input
    if (currentIndex == 1 && playerNameInput && playerNameInput->getVisible()) {
        if (!playerNameInput->getText().empty() && !nameEntered) {
            playerName = playerNameInput->getText();
            nameEntered = true;
            
            showNameConfirmation = true;
            std::string confirmText = "Your player name is " + playerName + ".";
            nameConfirmationText.setString(confirmText);
            
            sf::FloatRect textBounds = nameConfirmationText.getLocalBounds();
            nameConfirmationText.setOrigin(sf::Vector2f(
                textBounds.position.x + textBounds.size.x / 2.0f,
                textBounds.position.y + textBounds.size.y / 2.0f
            ));
            nameConfirmationText.setPosition(sf::Vector2f(400, 195));
            
            updateButtons();
            std::cout << "Player name entered: " << playerName << " (confirmation shown)\n";
        }
    }
    
    // UPDATED - ACT1_4: Handle numeric exam input with immediate Next button availability
    if (currentIndex == 3 && examInput && examInput->getVisible()) {
        std::string examText = examInput->getText();
        if (examText.length() >= 2 && !examCompleted) {
            examCompleted = true;
            showExamScorePopup();
            updateButtons(); // Show Next button immediately after score is displayed
            std::cout << "Exam input completed: " << examText << " (Next button now available)\n";
        }
    }
}

void ACT1_state::showExamScorePopup() {
    // Generate random score between 0-60
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 60);
    examScore = dis(gen);
    
    examPopupText.setString("EXAM SCORE: " + std::to_string(examScore) + "%");
    
    // Center text in popup
    sf::FloatRect textBounds = examPopupText.getLocalBounds();
    examPopupText.setOrigin(sf::Vector2f(
        textBounds.position.x + textBounds.size.x / 2.0f,
        textBounds.position.y + textBounds.size.y / 2.0f
    ));
    examPopupText.setPosition(sf::Vector2f(400, 300));
    
    showExamPopup = true;
    popupTimer = 3.0f;
    
    // Play dramatic sound effect when exam score is shown
    try {
        AudioManager::getInstance().playSound("sparkle");
        std::cout << "Playing dramatic sparkle sound for exam score reveal\n";
    } catch (...) {
        std::cout << "Warning: Could not play sparkle sound\n";
    }
}

std::string ACT1_state::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << "Saved on " << std::put_time(std::localtime(&time_t), "%B %d, %Y at %I:%M %p");
    return ss.str();
}

void ACT1_state::handleEvents(Game& game) {
    while (auto event = game.getWindow().pollEvent()) {
        // Handle text input events
        if (playerNameInput && playerNameInput->getVisible()) {
            playerNameInput->handleEvent(*event);
        }
        if (examInput && examInput->getVisible()) {
            examInput->handleEvent(*event);
        }
        
        if (event->is<sf::Event::Closed>()) {
            game.getWindow().close();
        }
        else if (event->is<sf::Event::KeyPressed>()) {
            const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();
            
            // Handle Enter key for text inputs
            if (keyEvent->code == sf::Keyboard::Key::Enter) {
                if (currentIndex == 1 && playerNameInput && playerNameInput->isFocusedBox()) {
                    if (!playerNameInput->getText().empty()) {
                        playerName = playerNameInput->getText();
                        nameEntered = true;
                        showNameConfirmation = true;
                        
                        std::string confirmText = "Your player name is " + playerName + ".";
                        nameConfirmationText.setString(confirmText);
                        
                        sf::FloatRect textBounds = nameConfirmationText.getLocalBounds();
                        nameConfirmationText.setOrigin(sf::Vector2f(
                            textBounds.position.x + textBounds.size.x / 2.0f,
                            textBounds.position.y + textBounds.size.y / 2.0f
                        ));
                        nameConfirmationText.setPosition(sf::Vector2f(400, 195));
                        
                        updateButtons();
                        AudioManager::getInstance().playSound("click");
                        std::cout << "Player name submitted: " << playerName << "\n";
                    }
                }
                // UPDATED - Enhanced exam submission with immediate Next button availability
                else if (currentIndex == 3 && examInput && examInput->isFocusedBox()) {
                    if (!examInput->getText().empty() && examInput->getText().length() >= 2) {
                        examCompleted = true;
                        showExamScorePopup();
                        updateButtons(); // Show Next button immediately
                        std::cout << "Exam submitted via Enter: " << examInput->getText() << "\n";
                    }
                }
            }
            
            // Keyboard navigation
            if (keyEvent->code == sf::Keyboard::Key::Left) {
                if (currentIndex > 0) {
                    navigateToPrevious();
                }
            }
            else if (keyEvent->code == sf::Keyboard::Key::Right) {
                if (currentIndex < static_cast<int>(sprites.size()) - 1) {
                    navigateToNext();
                }
            }
        }
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            auto mousePos = game.getWindow().mapPixelToCoords(
                event->getIf<sf::Event::MouseButtonPressed>()->position);

            handleButtonClicks(game, mousePos);
        }
    }
}

void ACT1_state::handleButtonClicks(Game& game, const sf::Vector2f& mousePos) {
    // Save button with enhanced feedback
    if (saveButton->isVisible() && saveButton->isClicked(mousePos)) {
        game.saveGame(1, currentIndex + 1);
        
        // Play save sound
        try {
            AudioManager::getInstance().playSound("choi");
            std::cout << "Save button: Playing choi.ogg\n";
        } catch (...) {
            std::cout << "Warning: Could not play choi.ogg\n";
        }
        
        // Show save confirmation with timestamp for 15 full seconds
        justSaved = true;
        saveMessageTimer = 15.0f; // 15 seconds visibility
        saveTimestampText.setString(getCurrentTimestamp());
        
        // Center both texts
        sf::FloatRect saveBounds = saveConfirmationText.getLocalBounds();
        saveConfirmationText.setOrigin(sf::Vector2f(
            saveBounds.position.x + saveBounds.size.x / 2.0f,
            saveBounds.position.y + saveBounds.size.y / 2.0f
        ));
        saveConfirmationText.setPosition(sf::Vector2f(400, 220));
        
        sf::FloatRect timeBounds = saveTimestampText.getLocalBounds();
        saveTimestampText.setOrigin(sf::Vector2f(
            timeBounds.position.x + timeBounds.size.x / 2.0f,
            timeBounds.position.y + timeBounds.size.y / 2.0f
        ));
        saveTimestampText.setPosition(sf::Vector2f(400, 250));
        
        return;
    }

    // Navigation buttons
    if (nextButton->isVisible() && nextButton->isClicked(mousePos)) {
        navigateToNext();
    }
    else if (prevButton->isVisible() && prevButton->isClicked(mousePos)) {
        navigateToPrevious();
    }
    else if (menuButton->isClicked(mousePos)) {
        AudioManager::getInstance().stopMusic();
        game.changeState(std::make_unique<StartState>(game.getFont()));
        AudioManager::getInstance().playSound("click");
    }
    else if (creditsButton->isVisible() && creditsButton->isClicked(mousePos)) {
        game.changeState(std::make_unique<CreditsState>(game.getFont()));
        AudioManager::getInstance().playSound("click");
    }
}

void ACT1_state::navigateToNext() {
    if (currentIndex < static_cast<int>(sprites.size()) - 1) {
        // Hide name confirmation when leaving ACT1_2
        if (currentIndex == 1) {
            showNameConfirmation = false;
        }
        
        // UPDATED - Clean up exam UI when leaving sprite 4
        if (currentIndex == 3) {
            showExamPopup = false;
            examScoreLocked = false;
            std::cout << "Leaving ACT1_4: Cleaned up exam UI\n";
        }
        
        currentIndex++;
        currentSprite.setTexture(sprites[currentIndex], true);
        updateUIForCurrentSprite();
        updateButtons();
        AudioManager::getInstance().playSound("click");
    }
}

void ACT1_state::navigateToPrevious() {
    if (currentIndex > 0) {
        currentIndex--;
        currentSprite.setTexture(sprites[currentIndex], true);
        updateUIForCurrentSprite();
        updateButtons();
        AudioManager::getInstance().playSound("click");
    }
}

void ACT1_state::update(Game& game) {
    // Get actual delta time for accurate timing
    static sf::Clock deltaClock;
    float deltaTime = deltaClock.restart().asSeconds();
    
    // FIXED - Enhanced save message timer with 15-second visibility using real delta time
    if (justSaved && saveMessageTimer > 0.0f) {
        saveMessageTimer -= deltaTime; // Use actual frame time instead of assumed 0.016f
        if (saveMessageTimer <= 0.0f) {
            justSaved = false;
        }
    }
    
    // UPDATED - Handle exam popup timer using real delta time
    if (showExamPopup && popupTimer > 0.0f) {
        popupTimer -= deltaTime; // Use real delta time
        if (popupTimer <= 0.0f) {
            showExamPopup = false;
        }
    }
    
    // Handle mangoman.mp3 loop counting for sprite 4
    if (currentIndex == 3 && sprite4AudioPlayed) {
        auto& audio = AudioManager::getInstance();
        if (audio.getMusicStatus("mangoman") == sf::SoundSource::Status::Stopped) {
            sprite4LoopCount++;
            std::cout << "Mangoman play completed: " << sprite4LoopCount << "/2\n";
            
            if (sprite4LoopCount < 2) {
                audio.playMusic("mangoman", false);
            } else {
                sprite4AudioPlayed = false;
                std::cout << "Mangoman.mp3 completed exactly 2 plays\n";
            }
        }
    }
    
    // Update button hover states
    sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(
        sf::Mouse::getPosition(game.getWindow()));
    
    if (nextButton->isVisible()) nextButton->update(mousePos);
    if (prevButton->isVisible()) prevButton->update(mousePos);
    if (saveButton->isVisible()) saveButton->update(mousePos);
    if (creditsButton->isVisible()) creditsButton->update(mousePos);
    menuButton->update(mousePos);
    
    // Update input text boxes with real delta time
    if (playerNameInput && playerNameInput->getVisible()) {
        playerNameInput->update(deltaTime); // Use real delta time
    }
    if (examInput && examInput->getVisible()) {
        examInput->update(deltaTime); // Use real delta time
    }
    
    handleTextInput(game);
}

void ACT1_state::render(Game& game) {
    game.getWindow().clear(sf::Color(20, 25, 35));
    
    // Draw main sprite with proper scaling
    if (currentIndex < static_cast<int>(sprites.size())) {
        sf::Vector2u windowSize = game.getWindow().getSize();
        sf::Vector2u textureSize = currentSprite.getTexture().getSize();
        
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y - 80) / textureSize.y;
        float scale = std::min(scaleX, scaleY);
        currentSprite.setScale(sf::Vector2f(scale, scale));
        
        currentSprite.setOrigin(sf::Vector2f(
            textureSize.x/2.f, 
            textureSize.y/2.f
        ));
        currentSprite.setPosition(sf::Vector2f(
            windowSize.x/2.f, 
            (windowSize.y - 80)/2.f
        ));
        
        game.getWindow().draw(currentSprite);
    }
    
    // Draw sprite-specific content overlays
    renderSpriteSpecificContent(game);
    
    // Draw input text boxes when visible
    if (playerNameInput && playerNameInput->getVisible()) {
        playerNameInput->draw(game.getWindow());
    }
    
    if (examInput && examInput->getVisible()) {
        // [MODIFIED] - Updated exam instruction position for new input box location
        sf::Text examInstruction(font);
        examInstruction.setString("Enter exam answer (2 digits):");
        examInstruction.setCharacterSize(18);
        examInstruction.setFillColor(sf::Color::White);
        examInstruction.setPosition(sf::Vector2f(450, 320)); // Adjusted for new input position
        game.getWindow().draw(examInstruction);
        
        examInput->draw(game.getWindow());
    }
    
    // Enhanced name confirmation display
    if (showNameConfirmation && currentIndex == 1) {
        game.getWindow().draw(nameConfirmationBackground);
        game.getWindow().draw(nameConfirmationText);
    }
    
    // UPDATED - Draw exam score popup with fade effect, but always show persistent score when completed
    if (showExamPopup && popupTimer > 0.0f) {
        float alpha = std::min(1.0f, popupTimer / 1.0f) * 255;
        sf::Color fadeColor = popupBackground.getFillColor();
        fadeColor.a = static_cast<uint8_t>(alpha);
        popupBackground.setFillColor(fadeColor);
        
        sf::Color textFadeColor = examPopupText.getFillColor();
        textFadeColor.a = static_cast<uint8_t>(alpha);
        examPopupText.setFillColor(textFadeColor);
        
        game.getWindow().draw(popupBackground);
        game.getWindow().draw(examPopupText);
    }
    
    // UPDATED - Draw persistent exam score when exam is completed (always visible after completion)
    if (currentIndex == 3 && examCompleted) {
        // Draw big red exam score text that stays visible
        sf::Text persistentScore(font);
        persistentScore.setString("EXAM SCORE: " + std::to_string(examScore) + "%");
        persistentScore.setCharacterSize(42);
        persistentScore.setFillColor(sf::Color::Red);
        persistentScore.setStyle(sf::Text::Bold);
        
        sf::FloatRect scoreBounds = persistentScore.getLocalBounds();
        persistentScore.setOrigin(sf::Vector2f(
            scoreBounds.position.x + scoreBounds.size.x / 2.0f,
            scoreBounds.position.y + scoreBounds.size.y / 2.0f
        ));
        persistentScore.setPosition(sf::Vector2f(400, 300));
        
        game.getWindow().draw(persistentScore);
    }
    
    // Draw UI elements
    renderUI(game);
    
    game.getWindow().display();
}

void ACT1_state::renderSpriteSpecificContent(Game& game) {
    // ACT1_4 - Display player name 
    if (currentIndex == 3 && !playerName.empty()) {
        sf::Text playerNameDisplay(font);
        playerNameDisplay.setString(playerName);
        playerNameDisplay.setCharacterSize(20);
        playerNameDisplay.setFillColor(sf::Color::Black);
        playerNameDisplay.setStyle(sf::Text::Bold);
        
        sf::FloatRect nameBounds = playerNameDisplay.getLocalBounds();
        playerNameDisplay.setOrigin(sf::Vector2f(
            nameBounds.position.x + nameBounds.size.x / 2.0f,
            nameBounds.position.y + nameBounds.size.y / 2.0f
        ));
        playerNameDisplay.setPosition(sf::Vector2f(400, 250)); // Moved up from 272 to 250
        
        game.getWindow().draw(playerNameDisplay);
    }
    
    // ACT1_11 - Display "[playerName] IS A NERD"
    if (currentIndex == 10 && !playerName.empty()) {
        sf::Text finalMessage(font);
        finalMessage.setString(playerName + " IS A NERD");
        finalMessage.setCharacterSize(32);
        finalMessage.setFillColor(sf::Color(150, 255, 150));
        finalMessage.setStyle(sf::Text::Bold);
        
        sf::FloatRect finalBounds = finalMessage.getLocalBounds();
        finalMessage.setOrigin(sf::Vector2f(
            finalBounds.position.x + finalBounds.size.x / 2.0f,
            finalBounds.position.y + finalBounds.size.y / 2.0f
        ));
        finalMessage.setPosition(sf::Vector2f(400, 400));
        
        game.getWindow().draw(finalMessage);
    }
}

void ACT1_state::renderUI(Game& game) {
    // Draw bottom UI bar
    sf::RectangleShape uiBar(sf::Vector2f(800, 70));
    uiBar.setPosition(sf::Vector2f(0, 530));
    uiBar.setFillColor(sf::Color(15, 20, 30, 200));
    game.getWindow().draw(uiBar);
    
    // Draw page indicator
    sf::Text pageIndicator(font);
    pageIndicator.setString(std::to_string(currentIndex + 1) + " / " + std::to_string(sprites.size()));
    pageIndicator.setCharacterSize(20);
    pageIndicator.setFillColor(sf::Color::White);
    pageIndicator.setPosition(sf::Vector2f(360, 550));
    game.getWindow().draw(pageIndicator);
    
    // Draw buttons
    if (prevButton->isVisible()) prevButton->draw(game.getWindow());
    if (nextButton->isVisible()) nextButton->draw(game.getWindow());
    menuButton->draw(game.getWindow());
    
    if (creditsButton->isVisible()) {
        creditsButton->draw(game.getWindow());
    }
    
    // Enhanced save button and confirmation with 5-second timer and fade effect
    if (saveButton->isVisible()) {
        saveButton->draw(game.getWindow());
        
        if (justSaved && saveMessageTimer > 0.0f) {
            // Smooth fade effect over the last 3 seconds
            float alpha = 1.0f;
            if (saveMessageTimer <= 3.0f) {
                alpha = saveMessageTimer / 3.0f; // Fade out over last 3 seconds
            }
            
            sf::Color saveColor(100, 255, 100, static_cast<uint8_t>(alpha * 255));
            saveConfirmationText.setFillColor(saveColor);
            
            sf::Color timeColor(150, 200, 150, static_cast<uint8_t>(alpha * 0.9f * 255));
            saveTimestampText.setFillColor(timeColor);
            
            // Draw enhanced background for better readability
            sf::RectangleShape saveBackground(sf::Vector2f(520, 90));
            saveBackground.setPosition(sf::Vector2f(140, 190));
            saveBackground.setFillColor(sf::Color(20, 30, 40, static_cast<uint8_t>(alpha * 0.8f * 255)));
            saveBackground.setOutlineThickness(2);
            saveBackground.setOutlineColor(sf::Color(80, 120, 80, static_cast<uint8_t>(alpha * 255)));
            
            game.getWindow().draw(saveBackground);
            game.getWindow().draw(saveConfirmationText);
            game.getWindow().draw(saveTimestampText);
        }
    }
}