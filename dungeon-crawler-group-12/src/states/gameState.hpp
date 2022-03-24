#pragma once

#include "SFML/System.hpp"

#include "state.hpp"
#include "game/room.cpp"
#include "game/player.cpp"

class GameState : public State {

public:

    GameState(sf::RenderWindow*);
    ~GameState();

    void reset();       //Resettaa vaan napit ja goNextState

    void newDT();       //Aika joka menee yhen syklin pyöräyttämiseen

    void updateTimeSinceLastMonsterUpdate(float); //Pitää kirjaa siitä millon viimeksi updattasi monsterit

    void updateState();                 //Päivittää GameStaten (pelaaja location, huone nro yms.), kaikki backend tapahtuu tässä
    void drawState(sf::RenderTarget*);  //Piirtää kaiken 

private:

    Button* pauseButt;
    sf::Texture pauseButtTexture;               //Textuureja
    sf::Sprite pauseButtSprite;

    sf::RectangleShape border;
    sf::Texture dialogueBackground;
    sf::Sprite dialogueBackgroundSprite;

    sf::Texture HPandShieldsBackgroundTexture;
    sf::Sprite HPandShieldsBackgroundSprite;

    sf::Texture statsBackgroundTexture;
    sf::Sprite statsBackgroundSprite;

    //pelin sisäinen kello, pitää kirjaa kuinka kauan jokaisessa update cylklissä menee
    sf::Clock clock;
    float deltaTime;

    //Kello jolla gamestatekutsuu monsterupdattii
    float timeSinceLastMonsterUpdate = 0;
    float monsterUpdateSpeed = 0.1;

    vector< Room* > rooms_;
    int roomIndex_=0;
    Room* null_room = new Room();
    Player player_= Player(8,8,null_room);
    //player muuttuja sisältää pelaajan
    //Player player;

};


