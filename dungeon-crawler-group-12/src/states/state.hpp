#pragma once

#include <vector>
#include <iostream>
#include <ctime>
#include <map>

#include "constants"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "button.cpp"
#include "text.cpp"



//Has 4 subclasses, menuenuState, GameState, loseState, winState

class State{

protected:

    sf::Vector2i mousePos;
    sf::RenderWindow* window;
    sf::Font font;

public:

    bool goNextState = false; //Muuttuja jota tarkastellaan game.cpp:ssä, jos true state ilmoittaa että valmis siirtymään seuraavaan. 
    int nextStateNum; //Muuttuja jolla game.cpp tietää mihinkä stateen siirrytään seuraavaksi
    int type; //0 = Menu, 1 = Game, 2 = Lose, 3 = Win 4 = Pause, 5 = Control
    bool exit = false; // tarkistaa vaan halutaanko ohjelma sulkea staten funktion takia

    //Konstructor ottaa sisään identifikaatio int arvon, ja sf::RenderWindow pointerin, joka on pelin oma window jonne staten drawState funktio piirtää
    State(int, sf::RenderWindow*);
    virtual ~State();

    virtual void reset() = 0; //Ei tee muuta kuin sen jälkeen kun ollaan tarkistettu game.cpp::ssä että siirrytään seuraavaan stateen, niin resetataan goNextState ja buttonit

    virtual void drawState(sf::RenderTarget*) = 0;
    virtual void updateState() = 0;

    //Updattaa hiiri position suhteessa gameWindowiin
    void updateMousePos();

};
