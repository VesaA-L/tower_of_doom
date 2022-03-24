#include "window.hpp"
#include "constants"
//Initializer functions 

void Game::initializeWindow(){
    float windowWidth = TILESIZE*ROOM_WIDTH+300;
    float windowHeight = TILESIZE*ROOM_HEIGHT+300;
    this->gameWindow = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), "Cave Crawler");
    this->gameWindow->setFramerateLimit(FRAMERATE);
}

void Game::initializeStates(){

    menuState = new MenuState(gameWindow);  
    loseState = new LoseState(gameWindow);
    winState = new WinState(gameWindow);
    pauseState = new PauseState(gameWindow);
    controlState = new ControlState(gameWindow);

    currentState = menuState;
}

//Construct Destruct

Game::Game(){

    this->initializeWindow();
    this->initializeStates();

}

Game::~Game(){
    delete this->gameWindow;
    delete this->gameState;
    delete this->menuState;
    delete this->winState;
    delete this->loseState;
    delete this->pauseState;
    delete this->controlState;
    delete this->gameState;
}

//vaihtaa vaan currentState:n seuraavaan
void Game::newCurrentState(){

    this->currentState->reset();
    
    if(this->currentState->nextStateNum == 0){
        this->currentState = menuState;
        }
    else if(this->currentState->nextStateNum == 1){
        if(this->currentState != pauseState){
            delete this->gameState;
            gameState = new GameState(gameWindow);
        }
        this->currentState = gameState;
        }
    else if(this->currentState->nextStateNum == 2){
        this->currentState = loseState;
        }
    else if(this->currentState->nextStateNum == 3){
        this->currentState = winState;
        }
    else if(this->currentState->nextStateNum == 4){
        this->currentState = pauseState;
    }
    else if(this->currentState->nextStateNum == 5){
        this->currentState = controlState;
    }
 
}
//Functions used to get game "turn" forward

void Game::update(){

    if(this->currentState->exit){
        this->gameWindow->close();
    }

    //lähinnä vaan varmistamaan että homma sulkeutuu ruksia painamalla
    this->updateSFML();

    //updattaa current staten, muissa kuin gamestatessa tämä ei tee käytännössä mitään
    this->currentState->updateState();

    //Tarkastaa onko currentState valmis siirtymään seuraavaan
    if(this->currentState->goNextState){
        this->newCurrentState();
    }

}

void Game::updateSFML(){

        while(this->gameWindow->pollEvent(eventSFML)){ 
            
            if(this->eventSFML.type == sf::Event::Closed){
                this->gameWindow->close();
            }
        }

}

void Game::draw(){

    //alustetaan window että ei jää muuta krääsää alle
    this->gameWindow->clear();

    //piirretään kyseinen state, menu state on yksinkertainen menu kun taas gamestate piirtää itse pelin
    this->currentState->drawState(this->gameWindow);
    
    //displayataa kaikki piiretty ruudulle
    this->gameWindow->display();

}

void Game::run(){
    while(this->gameWindow->isOpen()){ 
        //Päivittää peliä kieroksella
        this->update();
        //Piirtää päivitetyn staten
        this->draw();
    }
}