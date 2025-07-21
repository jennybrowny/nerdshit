
#ifndef GAMESTATE_H
#define GAMESTATE_H

// GAME STATES - Different "modes" the game can be in
// Think of these like different screens in your phone
enum GameState {
    LOADING,        // Shows loading screen with spinning dots
    MAIN_MENU,      // Start screen with buttons to click
    SCENE_1,        // Dr. Song Choi introduces the game
    SCENE_2_ECE160, // First coding challenge with Professor Mangoman
    SCENE_2_ECE323, // Second year with Professor Jeffrey Weldon
    CREDITS,        // Shows who made the game
    GAME_OVER       // When player runs out of health/stamina
};

#endif
