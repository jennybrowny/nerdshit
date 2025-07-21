
#include "Player.h"

// CONSTRUCTOR - Creates a new player with full health and stamina
Player::Player() {
    stamina = 100;  // Start with full energy
    health = 100;   // Start with perfect health
}

// STUDY METHOD - Player chooses to hit the books
void Player::study() {
    stamina -= 20;  // Studying is mentally exhausting
    if (stamina < 0) stamina = 0;  // Can't go below zero
    std::cout << "You studied hard! Stamina: " << stamina << "/100" << std::endl;
}

// DRINK CAFFEINE METHOD - Player drinks coffee/energy drinks
void Player::drinkCaffeine() {
    health -= 15;   // Caffeine damages your body over time
    if (health < 0) health = 0;    // Can't go below zero
    std::cout << "You drank caffeine! Health: " << health << "/100" << std::endl;
}

// USE CHATGPT METHOD - Player uses AI to help with work
void Player::useChatGPT() {
    // No immediate health/stamina cost, but there's risk of getting caught!
    std::cout << "You used ChatGPT... hope the professor doesn't notice!" << std::endl;
}

// GETTER METHODS - Allow other parts of code to read player stats
int Player::getStamina() const { return stamina; }
int Player::getHealth() const { return health; }

// IS ALIVE - Check if player has enough health and stamina to continue
bool Player::isAlive() const { 
    return health > 0 && stamina > 0; 
}
