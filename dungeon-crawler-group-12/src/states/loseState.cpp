#include "loseState.hpp"

LoseState::LoseState(sf::RenderWindow* win) : State(2, win){

    this->exitButt = new Button((TILESIZE*ROOM_WIDTH+SIDEBAR)/2-100, (TILESIZE*ROOM_HEIGHT+BELOWBAR)/2+100, 200, 50, "Quit Game", 16, sf::Color::Red, sf::Color::Green, sf::Color::Green);
    this->mainMenuButt = new Button((TILESIZE*ROOM_WIDTH+SIDEBAR)/2-300, (TILESIZE*ROOM_HEIGHT+BELOWBAR)/2-50, 200, 50, "Main Menu", 16, sf::Color::Red, sf::Color::Green, sf::Color::Green);
    this->newGameButt = new Button((TILESIZE*ROOM_WIDTH+SIDEBAR)/2+100, (TILESIZE*ROOM_HEIGHT+BELOWBAR)/2-50, 200, 50, "New Game", 16, sf::Color::Red, sf::Color::Green, sf::Color::Green);
    this->loseTitle = new Text((TILESIZE*ROOM_WIDTH+SIDEBAR)/2, (TILESIZE*ROOM_HEIGHT+BELOWBAR)/2-200,  "You Lose", "fonts/Montserrat-Regular.ttf", sf::Color::Red, sf::Color::Red, 50, 0, true);

}

LoseState::~LoseState(){
    delete this->mainMenuButt;
    delete this->exitButt;
    delete this->newGameButt;
    delete this->loseTitle;
}

void LoseState::reset(){
    this->goNextState = false;
    this->mainMenuButt->activated = false;
    this->newGameButt->activated = false;
}

void LoseState::updateState(){

    this->updateMousePos();
    this->mainMenuButt->updateButton(mousePos);
    this->newGameButt->updateButton(mousePos);
    this->exitButt->updateButton(mousePos);

    if(this->mainMenuButt->activated){
        this->goNextState = true;
        this->nextStateNum = 0;
    }

    if(this->newGameButt->activated){
        this->goNextState = true;
        this->nextStateNum = 1;
    }

    if(this->exitButt->activated){
        this->exit = true;
    }
}
    
void LoseState::drawState(sf::RenderTarget* target){

    //Piirtää spritet yms

    this->newGameButt->drawButton(target);
    this->mainMenuButt->drawButton(target);
    this->exitButt->drawButton(target);
    this->loseTitle->drawText(target);

    //std::cout << "Lose" << std::endl ;
}