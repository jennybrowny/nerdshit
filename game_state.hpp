#pragma once
class Game; // Forward declaration
/*
Purpose of game state: 
Inherited state classes - StartState, TutorialState, ACT1_state all inherit from GameState
Polymorphic state management - Game class uses std::unique_ptr<GameState> for seamless transitions
Composition in UI - Button and InputTextBox objects composed within state classes
Consistent interface - All states implement the same three core methods
*/
class GameState {
public:
    virtual ~GameState() = default;
    virtual void handleEvents(Game& game) = 0;
    virtual void update(Game& game) = 0;
    virtual void render(Game& game) = 0;
};