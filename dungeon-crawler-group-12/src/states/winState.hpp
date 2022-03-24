#pragma once

#include "state.hpp"

class WinState : public State{

public:

    WinState(sf::RenderWindow*);
    ~WinState();

    void reset();

    void updateState();
    void drawState(sf::RenderTarget*);

private:

    Button* mainMenuButt;
    Button* exitButt;
    Button* newGameButt;

    Text* winTitle;

     //textures yms.

};