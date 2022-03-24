#include "pauseState.hpp"

PauseState::PauseState(sf::RenderWindow* win) : State(4,win) {
    
    //Luo Tarvittavat textuurit spritet yms
    this->resumeGameButt = new Button((TILESIZE*ROOM_WIDTH+SIDEBAR)/2 - 200 -100, (TILESIZE*ROOM_WIDTH+BELOWBAR)/2 -100, 200, 50, "Resume Game", 16, sf::Color::Red, sf::Color::Green, sf::Color::Green);
    this->mainMenuButt = new Button((TILESIZE*ROOM_WIDTH+SIDEBAR)/2 + 200 -100, (TILESIZE*ROOM_WIDTH+BELOWBAR)/2 -100, 200, 50, "Quit To Menu", 16, sf::Color::Red, sf::Color::Green, sf::Color::Green);
    this->paused = new Text((TILESIZE*ROOM_WIDTH+SIDEBAR)/2, 200,  "Paused", "fonts/Montserrat-Regular.ttf", sf::Color::White, sf::Color::White, 50, 0, true);
    
}

PauseState::~PauseState(){
    delete this->resumeGameButt;
    delete this->mainMenuButt;
    delete this->paused;
}

void PauseState::reset(){
    this->goNextState = false;
    this->mainMenuButt->activated = false;
    this->resumeGameButt->activated = false;
}

void PauseState::updateState(){

    this->updateMousePos();
    this->mainMenuButt->updateButton(mousePos);
    this->resumeGameButt->updateButton(mousePos);

    if(this->mainMenuButt->activated){
        this->nextStateNum = 0;
        this->goNextState = true;
    }

    if(this->resumeGameButt->activated){
        this->nextStateNum = 1;
        this->goNextState = true;
    }
    
}

void PauseState::drawState(sf::RenderTarget* target){
    //std::cout << "Pause " << std::endl ;

    this->mainMenuButt->drawButton(target);
    this->resumeGameButt->drawButton(target);
    this->paused->drawText(target);
}

