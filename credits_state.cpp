#include "states/credits_state.hpp"
#include "game.hpp"
#include "states/start_state.hpp"
#include "audio_manager.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <cstdint>
/*

honestly this was optional but I was bored and wanted to see and get creative on what 
SFML can do...
*/

CreditsState::CreditsState(sf::Font& font) : 
// functions to provide crazy colorful animates
    creditsText(font),
    backgroundTexture(),
    backgroundSprite(backgroundTexture),
    rainbow(sf::PrimitiveType::TriangleStrip),
    gradientPosition(0.f),
    backgroundVariant(0),
    animationTimer(0.f)
{
    auto& audio = AudioManager::getInstance();
    try {
        audio.loadMusic("credits", "assets/sounds/credits.mp3");
        audio.playMusic("credits", true);
        std::cout << "Credits music started" << std::endl;
    } catch (...) {
        std::cout << "Warning: Could not load credits music" << std::endl;
    }
    
    // NRandomize background variant (3 variants total)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 4);
    backgroundVariant = dis(gen);
    
    std::cout << "Credits background variant: " << backgroundVariant << std::endl;
    
    rainbowColors = {
        sf::Color(255, 0, 0), sf::Color(255, 127, 0),
        sf::Color(255, 255, 0), sf::Color(0, 255, 0),
        sf::Color(0, 0, 255), sf::Color(75, 0, 130),
        sf::Color(148, 0, 211)
    };
    
    // Initialize background elements based on variant
    createDynamicBackground();
    
    // UPDATED: Button positioning to maintain 30px padding from dynamic elements
    sf::Vector2f safeButtonArea = calculateSafeButtonArea();
    
    backButton = std::make_unique<Button>(
        sf::Vector2f(180, 50),
        sf::Vector2f(safeButtonArea.x, 450),  // Dynamic positioning
        sf::Color(60, 120, 180),
        font,
        "MENU"
    );
    
    exitButton = std::make_unique<Button>(
        sf::Vector2f(180, 50),
        sf::Vector2f(safeButtonArea.x + 200, 450),  // Dynamic positioning
        sf::Color(150, 50, 50),
        font,
        "EXIT GAME"
    );
    
    // Credits text with dynamic positioning to avoid overlap
    creditsText.setString(
        "\n\nCreated by: Jenny Brown\n"
        "Music by: Illegal Pirating\n"
        "Special Thanks to:\n My soulmate and day 1 OG, Chatgpt,\n Deepseek, \n Claude Ai,\n Mangoman,\n Dr. Herve Collins\n\n"
        "I believe - Dr. Paul Schmitt Ph.D.\n"
    );
    creditsText.setCharacterSize(30);
    creditsText.setFillColor(sf::Color::White);
    
    // Center the credits text with safe area consideration
    sf::FloatRect creditsBounds = creditsText.getLocalBounds();
    creditsText.setOrigin(sf::Vector2f(
        creditsBounds.position.x + creditsBounds.size.x / 2.0f,
        creditsBounds.position.y + creditsBounds.size.y / 2.0f
    ));
    
    // Dynamic text positioning based on background variant
    sf::Vector2f safeTextPosition = calculateSafeTextPosition();
    creditsText.setPosition(safeTextPosition);
}

// NEW: Calculate safe area for buttons (30px padding from dynamic elements)
sf::Vector2f CreditsState::calculateSafeButtonArea() {
    switch (backgroundVariant) {
        case 1: // Side spirals - buttons in center
            return sf::Vector2f(210, 450);
        case 2: // Corner particles - buttons away from corners
            return sf::Vector2f(150, 450);
        case 3: // Top/bottom waves - buttons in middle
            return sf::Vector2f(210, 450);
        default:
            return sf::Vector2f(200, 450);
    }
}

sf::Vector2f CreditsState::calculateSafeTextPosition() {
    switch (backgroundVariant) {
        case 1: // Side spirals - text in center
            return sf::Vector2f(400, 200);
        case 2: // Corner particles - text well-centered
            return sf::Vector2f(400, 220);
        case 3: // Top/bottom waves - text in center-safe zone
            return sf::Vector2f(400, 200);
        default:
            return sf::Vector2f(400, 200);
    }
}

// Create dynamic background based on randomized variant
void CreditsState::createDynamicBackground() {
    switch (backgroundVariant) {
        case 1:
            createSpiralBackground();
            break;
        case 2:
            createParticleFieldBackground();
            break;
        case 3:
            createWaveBackground();
            break;
        default:
            createRainbowBackground(); // Fallback
            break;
    }
}

// Variant 1 - Spirals on left and right sides
void CreditsState::createSpiralBackground() {
    const int segments = 100;
    leftSpiral.clear();
    rightSpiral.clear();
    leftSpiral.resize(segments);
    rightSpiral.resize(segments);
    
    for (int i = 0; i < segments; i++) {
        float angle = (i / static_cast<float>(segments)) * 8 * M_PI; // 4 full rotations
        float radius = 20 + (i / static_cast<float>(segments)) * 80;
        
        // Left spiral
        float leftX = 100 + radius * std::cos(angle);
        float leftY = 300 + radius * std::sin(angle) * 0.5f;
        leftSpiral[i].position = sf::Vector2f(leftX, leftY);
        leftSpiral[i].color = rainbowColors[i % rainbowColors.size()];
        
        // Right spiral (mirror and offset)
        float rightX = 700 - radius * std::cos(angle);
        float rightY = 300 + radius * std::sin(angle) * 0.5f;
        rightSpiral[i].position = sf::Vector2f(rightX, rightY);
        rightSpiral[i].color = rainbowColors[(i + 3) % rainbowColors.size()];
    }
    std::cout << "Created spiral background with " << segments << " points per spiral" << std::endl;
}

//  Variant 2 - Particle field in corners
void CreditsState::createParticleFieldBackground() {
    const int particlesPerCorner = 25;
    cornerParticles.clear();
    cornerParticles.resize(particlesPerCorner * 4); // 4 corners
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> offsetDis(-50.0f, 50.0f);
    
    int particleIndex = 0;
    
    // Create particles in each corner with some randomness
    sf::Vector2f corners[4] = {
        sf::Vector2f(50, 50),    // Top-left
        sf::Vector2f(750, 50),   // Top-right  
        sf::Vector2f(50, 550),   // Bottom-left
        sf::Vector2f(750, 550)   // Bottom-right
    };
    
    for (int corner = 0; corner < 4; corner++) {
        for (int i = 0; i < particlesPerCorner; i++) {
            sf::Vector2f basePos = corners[corner];
            sf::Vector2f randomOffset(offsetDis(gen), offsetDis(gen));
            
            cornerParticles[particleIndex].position = basePos + randomOffset;
            cornerParticles[particleIndex].color = rainbowColors[(corner * 2 + i/5) % rainbowColors.size()];
            particleIndex++;
        }
    }
    std::cout << "Created particle field background with " << particleIndex << " particles" << std::endl;
}

// Variant 3 - Wave patterns at top and bottom
void CreditsState::createWaveBackground() {
    const int wavePoints = 80;
    topWave.clear();
    bottomWave.clear();
    topWave.resize(wavePoints);
    bottomWave.resize(wavePoints);
    
    for (int i = 0; i < wavePoints; i++) {
        float x = (i / static_cast<float>(wavePoints - 1)) * 800;
        float topY = 50 + 30 * std::sin((i / static_cast<float>(wavePoints)) * 6 * M_PI);
        float bottomY = 550 - 30 * std::cos((i / static_cast<float>(wavePoints)) * 6 * M_PI);
        
        topWave[i].position = sf::Vector2f(x, topY);
        topWave[i].color = rainbowColors[i % rainbowColors.size()];
        
        bottomWave[i].position = sf::Vector2f(x, bottomY);
        bottomWave[i].color = rainbowColors[(i + 4) % rainbowColors.size()];
    }
    std::cout << "Created wave background with " << wavePoints << " points per wave" << std::endl;
}

// Original rainbow background (fallback, my favorite)
void CreditsState::createRainbowBackground() {
    const int segments = 50;
    rainbow.clear();
    rainbow.resize(segments * 2);
    
    for (int i = 0; i < segments; i++) {
        float x = 800.f * i / (segments - 1);
        rainbow[i*2].position = sf::Vector2f(x, 0);
        rainbow[i*2+1].position = sf::Vector2f(x, 600);
    }
    std::cout << "Created rainbow background (fallback)" << std::endl;
}

void CreditsState::update(Game& game) {
    animationTimer += 0.016f; // ~60 FPS
    
    // Update dynamic backgrounds based on variant
    switch (backgroundVariant) {
        case 1:
            updateSpirals();
            break;
        case 2:
            updateParticleField();
            break;
        case 3:
            updateWaves();
            break;
        default:
            updateRainbow();
            break;
    }
}
// NEW: Update spiral rotation
void CreditsState::updateSpirals() {
    float rotationSpeed = animationTimer * 0.5f;
    const int segments = leftSpiral.getVertexCount();
    
    for (int i = 0; i < segments; i++) {
        float angle = (i / static_cast<float>(segments)) * 8 * M_PI + rotationSpeed;
        float radius = 20 + (i / static_cast<float>(segments)) * 80;
        
        // Left spiral
        float leftX = 100 + radius * std::cos(angle);
        float leftY = 300 + radius * std::sin(angle) * 0.5f;
        leftSpiral[i].position = sf::Vector2f(leftX, leftY);
        
        // Right spiral (counter-rotation)
        float rightX = 700 - radius * std::cos(-angle);
        float rightY = 300 + radius * std::sin(-angle) * 0.5f;
        rightSpiral[i].position = sf::Vector2f(rightX, rightY);
        
        // Color cycling
        float colorPhase = std::fmod(animationTimer * 0.2f + i * 0.1f, 1.0f);
        int colorIndex = static_cast<int>(colorPhase * rainbowColors.size()) % rainbowColors.size();
        leftSpiral[i].color = rainbowColors[colorIndex];
        rightSpiral[i].color = rainbowColors[(colorIndex + 3) % rainbowColors.size()];
    }
}

// Update particle field with gentle movement
void CreditsState::updateParticleField() {
    for (size_t i = 0; i < cornerParticles.size(); i++) {
        // Gentle floating motion
        float floatOffset = 5.0f * std::sin(animationTimer * 0.3f + i * 0.1f);
        sf::Vector2f originalPos = cornerParticles[i].position;
        
        // Color pulsing
        float pulse = (std::sin(animationTimer * 0.4f + i * 0.15f) + 1.0f) * 0.5f;
        sf::Color baseColor = rainbowColors[i % rainbowColors.size()];
        sf::Color pulsedColor(
            static_cast<std::uint8_t>(baseColor.r * pulse),
            static_cast<std::uint8_t>(baseColor.g * pulse),
            static_cast<std::uint8_t>(baseColor.b * pulse)
        );
        cornerParticles[i].color = pulsedColor;
    }
}

// Update wave animation
void CreditsState::updateWaves() {
    const int wavePoints = topWave.getVertexCount();
    float wavePhase = animationTimer * 0.8f;
    
    for (int i = 0; i < wavePoints; i++) {
        float x = (i / static_cast<float>(wavePoints - 1)) * 800;
        float topY = 50 + 30 * std::sin((i / static_cast<float>(wavePoints)) * 6 * M_PI + wavePhase);
        float bottomY = 550 - 30 * std::cos((i / static_cast<float>(wavePoints)) * 6 * M_PI + wavePhase);
        
        topWave[i].position = sf::Vector2f(x, topY);
        bottomWave[i].position = sf::Vector2f(x, bottomY);
        
        // Color wave
        float colorPhase = std::fmod(wavePhase * 0.1f + i * 0.05f, 1.0f);
        int colorIndex = static_cast<int>(colorPhase * rainbowColors.size()) % rainbowColors.size();
        topWave[i].color = rainbowColors[colorIndex];
        bottomWave[i].color = rainbowColors[(colorIndex + 4) % rainbowColors.size()];
    }
}

// Original rainbow update
void CreditsState::updateRainbow() {
    gradientPosition += 0.0001f;
    gradientPosition = std::fmod(gradientPosition, 1.f);
    
    for (size_t i = 0; i < rainbow.getVertexCount(); i++) {
        float pos = gradientPosition + (0.2f * i / rainbow.getVertexCount());
        pos = std::fmod(pos, 1.f);
        
        int colorIndex = static_cast<int>(pos * rainbowColors.size()) % rainbowColors.size();
        int nextColorIndex = (colorIndex + 1) % rainbowColors.size();
        float blend = (pos * rainbowColors.size()) - colorIndex;
        
        sf::Color blendedColor(
            rainbowColors[colorIndex].r + (rainbowColors[nextColorIndex].r - rainbowColors[colorIndex].r) * blend,
            rainbowColors[colorIndex].g + (rainbowColors[nextColorIndex].g - rainbowColors[colorIndex].g) * blend,
            rainbowColors[colorIndex].b + (rainbowColors[nextColorIndex].b - rainbowColors[colorIndex].b) * blend
        );
        
        rainbow[i].color = blendedColor;
    }
}
// Player opens credit page, crazy colorful background with text, they can click exit/menu

void CreditsState::handleEvents(Game& game) {
    sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(
        sf::Mouse::getPosition(game.getWindow()));
    
    while (auto event = game.getWindow().pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            game.getWindow().close();
        }
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            if (backButton && backButton->isClicked(mousePos)) {
                // Stop credits music and return to main menu
                AudioManager::getInstance().stopMusic("credits");
                AudioManager::getInstance().playMusic("background", true);  // Resume main menu music
                game.changeState(std::make_unique<StartState>(game.getFont()));
            }
            else if (exitButton && exitButton->isClicked(mousePos)) {
                // Close the game window
                game.getWindow().close();
            }
        }
        else if (event->is<sf::Event::KeyPressed>()) {
            const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();
            if (keyEvent->code == sf::Keyboard::Key::Escape) {
                // ESC key returns to menu
                AudioManager::getInstance().stopMusic("credits");
                AudioManager::getInstance().playMusic("background", true);
                game.changeState(std::make_unique<StartState>(game.getFont()));
            }
        }
    }
    
    // Update button hover states
    if (backButton) backButton->update(mousePos);
    if (exitButton) exitButton->update(mousePos);
}

// Rendering this bullshit

void CreditsState::render(Game& game) {
    game.getWindow().clear(sf::Color(10, 15, 25)); // Dark background
    
    // Render dynamic background based on variant
    switch (backgroundVariant) {
        case 1:
            renderSpirals(game);
            break;
        case 2:
            renderParticleField(game);
            break;
        case 3:
            renderWaves(game);
            break;
        default:
            game.getWindow().draw(rainbow);
            break;
    }
    
    // Render credits text (positioned to avoid overlap)
    game.getWindow().draw(creditsText);
    
    // Render buttons (positioned in safe areas with 30px padding)
    if (backButton) {
        backButton->draw(game.getWindow());
    }
    if (exitButton) {
        exitButton->draw(game.getWindow());
    }
    
    // Background variant indicator (debug info)
    sf::Text variantInfo(game.getFont());
    variantInfo.setString("Background Variant: " + std::to_string(backgroundVariant) + "/3");
    variantInfo.setCharacterSize(12);
    variantInfo.setFillColor(sf::Color(100, 100, 100, 150));
    variantInfo.setPosition(sf::Vector2f(10, 580));
    game.getWindow().draw(variantInfo);
    
    game.getWindow().display();
}

// Render color

// Spirals
void CreditsState::renderSpirals(Game& game) {
    // Draw spirals as point primitives for better performance
    if (leftSpiral.getVertexCount() > 0) {
        game.getWindow().draw(leftSpiral);
    }
    if (rightSpiral.getVertexCount() > 0) {
        game.getWindow().draw(rightSpiral);
    }
}

// Particle field
void CreditsState::renderParticleField(Game& game) {
    if (cornerParticles.size() > 0) {
        game.getWindow().draw(cornerParticles.data(), cornerParticles.size(), sf::PrimitiveType::Points);
    }
}

// Waves
void CreditsState::renderWaves(Game& game) {
    if (topWave.getVertexCount() > 0) {
       game.getWindow().draw(topWave);
    }
    if (bottomWave.getVertexCount() > 0) {
        game.getWindow().draw(bottomWave);
    }
    if (!cornerParticles.empty()) {
    game.getWindow().draw(cornerParticles.data(), cornerParticles.size(), sf::PrimitiveType::Points);
}
}