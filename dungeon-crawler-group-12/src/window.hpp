#pragma once

#include <iostream>
#include <ctime>
#include <stack>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "states/gameState.cpp"
#include "states/menuState.cpp"
#include "states/winState.cpp"
#include "states/loseState.cpp"
#include "states/pauseState.cpp"
#include "states/controlState.cpp"
#include "states/state.cpp"

class Game{

public:
    Game();
    virtual ~Game();

    void update();          //updates the window
    void updateSFML();      //only checks for window close
    void draw();            //draws the current state
    void run();             //calls update and draw continuosly
    
    void newCurrentState(); //updates the currentstate, called in update()

private:

    //Variables
    sf::RenderWindow *gameWindow;
    sf::Event eventSFML;

    //Variables for all the states the game has, and the current state, which is
    MenuState* menuState;   //0
    GameState* gameState;   //1
    LoseState* loseState;   //2
    WinState* winState;     //3
    PauseState* pauseState; //4
    ControlState* controlState; //5

    //Keeps track of the current state
    State* currentState;

    //Private initializer function
    void initializeWindow();
    void initializeStates();

};
