#include "menuState.hpp"

MenuState::MenuState(sf::RenderWindow* win) : State(0, win){
    
    //Luo Tarvittavat textuurit spritet yms
    this->startGameButt = new Button((TILESIZE*ROOM_WIDTH+SIDEBAR)/2-100, (TILESIZE*ROOM_HEIGHT+BELOWBAR)/2-100, 200, 50, "Start New Game", 16, sf::Color::Red, sf::Color::Green, sf::Color::Green);
    this->exitButt = new Button((TILESIZE*ROOM_WIDTH+SIDEBAR)/2-100, (TILESIZE*ROOM_HEIGHT+BELOWBAR)/2 + 100, 200, 50, "Quit Game", 16, sf::Color::Red, sf::Color::Green, sf::Color::Green);
    this->controlButton = new Button((TILESIZE*ROOM_WIDTH+SIDEBAR)/2-100, (TILESIZE*ROOM_HEIGHT+BELOWBAR)/2, 200, 50, "Controls", 16, sf::Color::Red, sf::Color::Green, sf::Color::Green);
    this->title = new Text((TILESIZE*ROOM_WIDTH+SIDEBAR)/2, (TILESIZE*ROOM_HEIGHT+BELOWBAR)/2-200,  "Tower Of Doom", "fonts/Montserrat-Regular.ttf", sf::Color::Red, sf::Color::Green, 50, 5, true);
    
}

MenuState::~MenuState(){
    delete startGameButt;
    delete exitButt;
    delete controlButton;
    delete title;
}

void MenuState::reset(){
    this->goNextState = false;
    this->startGameButt->activated = false;
    this->controlButton->activated = false;
}

void MenuState::updateState(){

    this->updateMousePos();
    this->startGameButt->updateButton(mousePos);
    this->exitButt->updateButton(mousePos);
    this->controlButton->updateButton(mousePos);

    if(this->startGameButt->activated){
        this->goNextState = true;
        this->nextStateNum = 1;
    }

    if(this->controlButton->activated){
        this->goNextState = true;
        this->nextStateNum = 5;
    }

    if(this->exitButt->activated){
        this->exit = true;
    }

}
    
void MenuState::drawState(sf::RenderTarget* target){

    //Piirtää spritet yms
    this->startGameButt->drawButton(target);
    this->exitButt->drawButton(target);
    this->title->drawText(target);
    this->controlButton->drawButton(target);
    
    //std::cout << "Menu " << std::endl ;

}
