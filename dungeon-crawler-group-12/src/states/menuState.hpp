#pragma once

#include "state.hpp"

class MenuState : public State {

public:

    MenuState(sf::RenderWindow*);
    ~MenuState();

    void reset();

    void updateState();
    void drawState(sf::RenderTarget*);

private:

    Button* startGameButt;
    Button* exitButt;
    Button* controlButton;

    Text* title;

};