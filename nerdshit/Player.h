
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>

// PLAYER CLASS - Represents the student character
// This tracks the player's stats like health and energy
class Player {
private:
    int stamina;    // Energy level (0-100) - gets drained by studying
    int health;     // Physical wellness (0-100) - gets drained by caffeine
    
public:
    // Constructor - creates a new player with full stats
    Player();
    
    // ACTION METHODS - Different things the player can do
    void study();           // Study hard (costs stamina, helps with answers)
    void drinkCaffeine();   // Drink coffee (costs health, gives hints)
    void useChatGPT();      // Use AI (risky but free)
    
    // GETTER METHODS - Let other code check player stats
    int getStamina() const;
    int getHealth() const;
    
    // STATUS METHODS
    bool isAlive() const;   // Check if player can still continue
};

#endif
