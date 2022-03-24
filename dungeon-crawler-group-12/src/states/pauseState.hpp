#pragma once

#include "constants"
#include "state.hpp"


class PauseState : public State {

public:

    PauseState(sf::RenderWindow*);
    ~PauseState();

    void reset();

    void updateState();
    void drawState(sf::RenderTarget*);

private:

    Button* resumeGameButt;
    Button* mainMenuButt;

    Text* paused;
};
