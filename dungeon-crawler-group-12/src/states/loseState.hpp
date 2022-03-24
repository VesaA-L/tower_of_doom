#pragma once

#include "state.hpp"

class LoseState : public State {

public:

    LoseState(sf::RenderWindow*);
    ~LoseState();

    void reset();

    void updateState();
    void drawState(sf::RenderTarget*);

private:

    Button* mainMenuButt;
    Button* exitButt;
    Button* newGameButt;

    Text* loseTitle;

    //textures for button, font yms.

};