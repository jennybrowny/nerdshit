
#include "Game.h"
#include <iostream>

// CONSTRUCTOR - Sets up the game window and initializes everything
Game::Game() {
    // Create window (800 pixels wide, 600 pixels tall)
    window.create(sf::VideoMode(800, 600), "nerdshit: Engineering Survival Game");
    currentState = LOADING;  // Start with loading screen
    
    // Try to load a font (you'll need to add a font file to your project)
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Warning: Could not load arial.ttf. Using default font." << std::endl;
        // Try to use default font
    }
    
    // Initialize mouse state
    mousePosition = sf::Vector2i(0, 0);
    mouseClicked = false;
    
    // Create all the buttons for different screens
    createMenuButtons();
    createActionButtons();
    createChoiceButtons();
}

// MAIN GAME LOOP - This runs continuously while game is open
void Game::run() {
    while (window.isOpen()) {
        handleEvents();    // Check for user input (clicks, key presses)
        update();         // Update game logic
        render();         // Draw everything to screen
    }
}

// HANDLE EVENTS - Responds to user input
void Game::handleEvents() {
    sf::Event event;
    mouseClicked = false;  // Reset each frame
    
    while (window.pollEvent(event)) {
        // If user clicks the X button, close game
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        // Handle mouse clicks
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                mouseClicked = true;
                handleMouseClick(event.mouseButton.x, event.mouseButton.y);
            }
        }
        
        // Handle keyboard input
        if (event.type == sf::Event::KeyPressed) {
            handleKeyPress(event.key.code);
        }
    }
    
    // Update mouse position every frame
    mousePosition = sf::Mouse::getPosition(window);
}

// HANDLE MOUSE CLICKS - What happens when user clicks
void Game::handleMouseClick(int x, int y) {
    std::cout << "Mouse clicked at: " << x << ", " << y << std::endl;
    
    // Let buttons handle their own clicks
    updateButtons();
}

// HANDLE KEY PRESSES - What happens when user presses keys
void Game::handleKeyPress(sf::Keyboard::Key key) {
    // ESC key always goes back to main menu
    if (key == sf::Keyboard::Escape) {
        currentState = MAIN_MENU;
        std::cout << "Returning to main menu..." << std::endl;
    }
    
    // Space bar progresses through story scenes
    if (key == sf::Keyboard::Space) {
        switch (currentState) {
            case SCENE_1:
                currentState = SCENE_2_ECE160;
                std::cout << "Moving to ECE160 challenge..." << std::endl;
                break;
            case SCENE_2_ECE160:
                currentState = SCENE_2_ECE323;
                std::cout << "Moving to ECE323 challenge..." << std::endl;
                break;
            case CREDITS:
                currentState = MAIN_MENU;
                std::cout << "Back to main menu..." << std::endl;
                break;
        }
    }
}

// CREATE MENU BUTTONS - Set up main menu buttons
void Game::createMenuButtons() {
    menuButtons.clear();
    
    // Start Game button
    auto startButton = std::make_unique<Button>(300, 200, 200, 50, "Start Game", &font);
    startButton->setColors(sf::Color::Green, sf::Color::Yellow, sf::Color::Red);
    menuButtons.push_back(std::move(startButton));
    
    // Credits button  
    auto creditsButton = std::make_unique<Button>(300, 280, 200, 50, "Credits", &font);
    creditsButton->setColors(sf::Color::Blue, sf::Color::Cyan, sf::Color::Magenta);
    menuButtons.push_back(std::move(creditsButton));
    
    // Exit button
    auto exitButton = std::make_unique<Button>(300, 360, 200, 50, "Exit", &font);
    exitButton->setColors(sf::Color::Red, sf::Color::Yellow, sf::Color::Green);
    menuButtons.push_back(std::move(exitButton));
}

// CREATE ACTION BUTTONS - Set up study/caffeine/chatgpt buttons
void Game::createActionButtons() {
    actionButtons.clear();
    
    // Study button
    auto studyButton = std::make_unique<Button>(100, 450, 120, 50, "Study", &font);
    studyButton->setColors(sf::Color::Yellow, sf::Color::White, sf::Color::Green);
    actionButtons.push_back(std::move(studyButton));
    
    // Caffeine button
    auto caffeineButton = std::make_unique<Button>(250, 450, 120, 50, "Caffeine", &font);
    caffeineButton->setColors(sf::Color::Red, sf::Color::Yellow, sf::Color::Green);
    actionButtons.push_back(std::move(caffeineButton));
    
    // ChatGPT button
    auto chatgptButton = std::make_unique<Button>(400, 450, 120, 50, "ChatGPT", &font);
    chatgptButton->setColors(sf::Color::Magenta, sf::Color::Cyan, sf::Color::Green);
    actionButtons.push_back(std::move(chatgptButton));
}

// CREATE CHOICE BUTTONS - Set up A/B/C/D quiz buttons
void Game::createChoiceButtons() {
    choiceButtons.clear();
    
    // A button
    auto buttonA = std::make_unique<Button>(100, 350, 60, 40, "A", &font);
    buttonA->setColors(sf::Color::White, sf::Color::Yellow, sf::Color::Green);
    choiceButtons.push_back(std::move(buttonA));
    
    // B button
    auto buttonB = std::make_unique<Button>(200, 350, 60, 40, "B", &font);
    buttonB->setColors(sf::Color::White, sf::Color::Yellow, sf::Color::Green);
    choiceButtons.push_back(std::move(buttonB));
    
    // C button
    auto buttonC = std::make_unique<Button>(300, 350, 60, 40, "C", &font);
    buttonC->setColors(sf::Color::White, sf::Color::Yellow, sf::Color::Green);
    choiceButtons.push_back(std::move(buttonC));
    
    // D button
    auto buttonD = std::make_unique<Button>(400, 350, 60, 40, "D", &font);
    buttonD->setColors(sf::Color::White, sf::Color::Yellow, sf::Color::Green);
    choiceButtons.push_back(std::move(buttonD));
}

// UPDATE BUTTONS - Handle button interactions
void Game::updateButtons() {
    // Update main menu buttons
    if (currentState == MAIN_MENU) {
        for (int i = 0; i < menuButtons.size(); i++) {
            menuButtons[i]->update(mousePosition, mouseClicked);
            
            if (menuButtons[i]->wasClicked()) {
                switch (i) {
                    case 0:  // Start Game
                        currentState = SCENE_1;
                        std::cout << "Starting new game..." << std::endl;
                        break;
                    case 1:  // Credits
                        currentState = CREDITS;
                        std::cout << "Showing credits..." << std::endl;
                        break;
                    case 2:  // Exit
                        window.close();
                        std::cout << "Exiting game..." << std::endl;
                        break;
                }
            }
        }
    }
    
    // Update action buttons (study/caffeine/chatgpt)
    if (currentState == SCENE_2_ECE160 || currentState == SCENE_2_ECE323) {
        for (int i = 0; i < actionButtons.size(); i++) {
            actionButtons[i]->update(mousePosition, mouseClicked);
            
            if (actionButtons[i]->wasClicked()) {
                switch (i) {
                    case 0:  // Study
                        player.study();
                        break;
                    case 1:  // Caffeine
                        player.drinkCaffeine();
                        break;
                    case 2:  // ChatGPT
                        player.useChatGPT();
                        break;
                }
            }
        }
        
        // Update quiz choice buttons
        for (int i = 0; i < choiceButtons.size(); i++) {
            choiceButtons[i]->update(mousePosition, mouseClicked);
            
            if (choiceButtons[i]->wasClicked()) {
                std::string answer = (i == 0) ? "A" : (i == 1) ? "B" : (i == 2) ? "C" : "D";
                int questionIndex = (currentState == SCENE_2_ECE160) ? 0 : 1;
                
                bool correct = quizManager.presentQuiz(questionIndex, answer);
                if (correct) {
                    std::cout << "You got it right! Moving to next challenge..." << std::endl;
                } else {
                    std::cout << "Wrong answer, but you still pass!" << std::endl;
                }
            }
        }
    }
}

// UPDATE - Game logic that runs every frame
void Game::update() {
    // Update buttons every frame
    updateButtons();
    
    switch (currentState) {
        case LOADING:
            // Loading screen shows for 3 seconds, then goes to main menu
            if (loadingClock.getElapsedTime().asSeconds() > 3.0f) {
                currentState = MAIN_MENU;
                std::cout << "Loading complete! Welcome to nerdshit!" << std::endl;
            }
            break;
            
        case SCENE_2_ECE160:
        case SCENE_2_ECE323:
            // Check if player is still alive during challenges
            if (!player.isAlive()) {
                currentState = GAME_OVER;
                std::cout << "Game Over! You didn't survive engineering school!" << std::endl;
            }
            break;
    }
}

// RENDER - Draws everything to the screen
void Game::render() {
    window.clear(sf::Color::Black);  // Clear screen with black color
    
    // Draw different things based on current game state
    switch (currentState) {
        case LOADING:
            drawLoadingScreen();
            break;
        case MAIN_MENU:
            drawMainMenu();
            break;
        case SCENE_1:
            drawScene1();
            break;
        case SCENE_2_ECE160:
            drawScene2ECE160();
            break;
        case SCENE_2_ECE323:
            drawScene2ECE323();
            break;
        case CREDITS:
            drawCredits();
            break;
        case GAME_OVER:
            drawGameOver();
            break;
    }
    
    window.display();  // Actually show everything we drew
}

// DRAW LOADING SCREEN
void Game::drawLoadingScreen() {
    sf::Text loadingText;
    loadingText.setFont(font);
    loadingText.setString("Loading nerdshit...");
    loadingText.setCharacterSize(36);
    loadingText.setFillColor(sf::Color::White);
    loadingText.setPosition(250, 250);
    window.draw(loadingText);
    
    // Simple loading animation - rotating dots
    std::string dots = "";
    int dotCount = (int)(loadingClock.getElapsedTime().asSeconds()) % 4;
    for (int i = 0; i < dotCount; i++) {
        dots += ".";
    }
    
    sf::Text dotsText;
    dotsText.setFont(font);
    dotsText.setString(dots);
    dotsText.setCharacterSize(36);
    dotsText.setFillColor(sf::Color::White);
    dotsText.setPosition(500, 250);
    window.draw(dotsText);
}

// DRAW MAIN MENU
void Game::drawMainMenu() {
    // Title
    sf::Text title;
    title.setFont(font);
    title.setString("nerdshit: Engineering Survival");
    title.setCharacterSize(28);
    title.setFillColor(sf::Color::White);
    title.setPosition(200, 100);
    window.draw(title);
    
    // Draw all menu buttons
    for (auto& button : menuButtons) {
        button->draw(window);
    }
}

// DRAW SCENE 1 - Dr. Song Choi introduction
void Game::drawScene1() {
    sf::Text sceneText;
    sceneText.setFont(font);
    sceneText.setString("Welcome to the College of Engineering!\n\nDr. Song Choi: \"Survive 4 years by studying,\ncaffeine, or ChatGPT. Good luck!\"\n\nPress SPACE to continue...");
    sceneText.setCharacterSize(18);
    sceneText.setFillColor(sf::Color::White);
    sceneText.setPosition(50, 150);
    window.draw(sceneText);
    
    drawPlayerStats();
}

// DRAW SCENE 2 ECE160 - First coding challenge
void Game::drawScene2ECE160() {
    sf::Text sceneText;
    sceneText.setFont(font);
    sceneText.setString("ECE160 with Professor Mangoman\n\n\"Solve this Python problem!\"\n" + quizManager.getQuestion(0));
    sceneText.setCharacterSize(16);
    sceneText.setFillColor(sf::Color::White);
    sceneText.setPosition(50, 50);
    window.draw(sceneText);
    
    // Draw multiple choice options
    std::vector<std::string> choices = quizManager.getChoices(0);
    for (int i = 0; i < choices.size(); i++) {
        sf::Text choiceText;
        choiceText.setFont(font);
        choiceText.setString(choices[i]);
        choiceText.setCharacterSize(14);
        choiceText.setFillColor(sf::Color::Yellow);
        choiceText.setPosition(50, 150 + i * 25);
        window.draw(choiceText);
    }
    
    // Draw choice buttons (A, B, C, D)
    for (auto& button : choiceButtons) {
        button->draw(window);
    }
    
    // Draw action buttons
    for (auto& button : actionButtons) {
        button->draw(window);
    }
    
    drawPlayerStats();
}

// DRAW SCENE 2 ECE323 - Second year challenge  
void Game::drawScene2ECE323() {
    sf::Text sceneText;
    sceneText.setFont(font);
    sceneText.setString("ECE323 with Professor Jeffrey Weldon\n\n\"Welcome to year 2! Here's your final:\"\n" + quizManager.getQuestion(1));
    sceneText.setCharacterSize(16);
    sceneText.setFillColor(sf::Color::White);
    sceneText.setPosition(50, 50);
    window.draw(sceneText);
    
    // Draw multiple choice options
    std::vector<std::string> choices = quizManager.getChoices(1);
    for (int i = 0; i < choices.size(); i++) {
        sf::Text choiceText;
        choiceText.setFont(font);
        choiceText.setString(choices[i]);
        choiceText.setCharacterSize(14);
        choiceText.setFillColor(sf::Color::Yellow);
        choiceText.setPosition(50, 150 + i * 25);
        window.draw(choiceText);
    }
    
    // Draw choice buttons (A, B, C, D)
    for (auto& button : choiceButtons) {
        button->draw(window);
    }
    
    // Draw action buttons
    for (auto& button : actionButtons) {
        button->draw(window);
    }
    
    drawPlayerStats();
}

// DRAW PLAYER STATS - Shows health and stamina
void Game::drawPlayerStats() {
    sf::Text statsText;
    statsText.setFont(font);
    statsText.setString("Health: " + std::to_string(player.getHealth()) + "/100\nStamina: " + std::to_string(player.getStamina()) + "/100");
    statsText.setCharacterSize(16);
    statsText.setFillColor(sf::Color::Green);
    statsText.setPosition(550, 500);
    window.draw(statsText);
}

// DRAW CREDITS
void Game::drawCredits() {
    sf::Text creditsText;
    creditsText.setFont(font);
    creditsText.setString("nerdshit: Engineering Survival Game\n\nCreated by: [Your Name]\n\nFeaturing:\nDr. Song Choi\nProfessor Mangoman  \nProfessor Jeffrey Weldon\n\nPress SPACE to return to main menu");
    creditsText.setCharacterSize(18);
    creditsText.setFillColor(sf::Color::White);
    creditsText.setPosition(200, 150);
    window.draw(creditsText);
}

// DRAW GAME OVER
void Game::drawGameOver() {
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER\n\nYou didn't survive engineering school!\n\nPress ESC to return to main menu");
    gameOverText.setCharacterSize(24);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(200, 200);
    window.draw(gameOverText);
}
