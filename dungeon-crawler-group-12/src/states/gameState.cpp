#include "gameState.hpp"

GameState::GameState(sf::RenderWindow* win) : State(1, win){

    //Tässä kutsutaan GameWorld construktor jonka Ville luo gameworld fileen 
    //GUI tarvii GameWorld vektorin (eli vektorin jossa on kaikki maailman huoneet, itse huoneissa on sitten itemit monsterit yms) 
    //world = GameWorld();
    this->pauseButt = new Button (32*20+300-20, 0, 20, 20, " ", 1, sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);
    if(!pauseButtTexture.loadFromFile("images/pauseButton.png")){
        std::cout << "ERROR FAILED BUTTON IMAGE LOAD" << "/n";
    }
    this->pauseButtSprite.setTexture(this->pauseButtTexture);
    this->pauseButtSprite.setPosition(sf::Vector2f(((TILESIZE*ROOM_WIDTH+SIDEBAR)-20), 0));
    this->pauseButtSprite.setScale(0.04, 0.04);

    this->border.setSize(sf::Vector2f(ROOM_WIDTH * TILESIZE +5, ROOM_HEIGHT * TILESIZE +5));
    this->border.setFillColor(sf::Color(36, 5, 1));

    if(!this->dialogueBackground.loadFromFile("images/dialoguebackground.png")){
        std::cout << "FAILED TEXTURE LOAD" << "\n";
    }
    this->dialogueBackgroundSprite.setTexture(dialogueBackground);
    this->dialogueBackgroundSprite.setPosition(320, ROOM_HEIGHT * TILESIZE -5);

    if(!HPandShieldsBackgroundTexture.loadFromFile("images/HP&Shieldsbackground320x167.png")){
        std::cout << "ERROR FAILED HP&Shields background IMAGE LOAD" << "/n";
    }
    this->HPandShieldsBackgroundSprite.setTexture(HPandShieldsBackgroundTexture);
    this->HPandShieldsBackgroundSprite.setPosition(0, (ROOM_HEIGHT + 4) * TILESIZE + 5);

    if(!statsBackgroundTexture.loadFromFile("images/statsbackground295x136.png")){
        std::cout << "ERROR FAILED stats background IMAGE LOAD" << "/n";
    }
    this->statsBackgroundSprite.setTexture(statsBackgroundTexture);
    this->statsBackgroundSprite.setPosition(645, (ROOM_HEIGHT) * TILESIZE - 132);

    vector<Room*> rooms;

    for(int i=0;i<27;i++){
        rooms.push_back(new Room);
        rooms[i]->SetLevel(i / 9 + 1);
    }

    rooms[0]->LoadRoom("rooms/f0/r00");
    rooms[1]->LoadRoom("rooms/f0/r01");
    rooms[2]->LoadRoom("rooms/f0/r02");
    rooms[3]->LoadRoom("rooms/f0/r10");
    rooms[4]->LoadRoom("rooms/f0/r11");
    rooms[5]->LoadRoom("rooms/f0/r12");
    rooms[6]->LoadRoom("rooms/f0/r20");
    rooms[7]->LoadRoom("rooms/f0/r21");
    rooms[8]->LoadRoom("rooms/f0/r22");
    rooms[9]->LoadRoom("rooms/f1/r00");
    rooms[10]->LoadRoom("rooms/f1/r01");
    rooms[11]->LoadRoom("rooms/f1/r02");
    rooms[12]->LoadRoom("rooms/f1/r10");
    rooms[13]->LoadRoom("rooms/f1/r11");
    rooms[14]->LoadRoom("rooms/f1/r12");
    rooms[15]->LoadRoom("rooms/f1/r20");
    rooms[16]->LoadRoom("rooms/f1/r21");
    rooms[17]->LoadRoom("rooms/f1/r22");
    rooms[18]->LoadRoom("rooms/f2/r00");
    rooms[19]->LoadRoom("rooms/f2/r01");
    rooms[20]->LoadRoom("rooms/f2/r02");
    rooms[21]->LoadRoom("rooms/f2/r10");
    rooms[22]->LoadRoom("rooms/f2/r11");
    rooms[23]->LoadRoom("rooms/f2/r12");
    rooms[24]->LoadRoom("rooms/f2/r20");
    rooms[25]->LoadRoom("rooms/f2/r21");
    rooms[26]->LoadRoom("rooms/f2/r22");
    std::cout << "ROOMS CREATED" << "\n";
    for(int i=0;i<27;i++){
        rooms[i]->CreateVertexArray(rooms[i]->GetInputs());
    }
 
    rooms_ = rooms;

    player_.SetRoom(rooms_[1]);
    roomIndex_=1;
}

GameState::~GameState(){
    delete this->pauseButt;
    for(auto i : rooms_){
        delete i;
    }
    delete this->null_room;
}

void GameState::reset(){
    this->goNextState = false;
}

void GameState::newDT(){
    //Päivitää DeltaTimeen ajan joka menee yhden vuoron pyörittämisessä
    this-> deltaTime = clock.restart().asSeconds();
}

//Päivittää aikaa jolla kutsuu monsterUpdattia
void GameState::updateTimeSinceLastMonsterUpdate(float dt){
    this->timeSinceLastMonsterUpdate += dt;
}

//Pelin update sykli, tässä kutsutaan kaikki updattattava
void GameState::updateState(){

    //Ottaa aikaa kuinka kauan updatissa meni
    this->newDT();

    this->player_.GetRoom()->updateTime(deltaTime);

    //Tässä kutsutaan sitten kaikki inputista tulevat komennot, eli updatataan pelaajan, itemien, monstereiden yms. objekteissa tapahtuvat muutokset riippuen inputista
    this->updateMousePos();
    this->pauseButt->updateButton(mousePos);
    //TESTI HÄVIÄÄ PELIN
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::L)){
        this->goNextState = true;
        this->nextStateNum = 2;
    }
    //TESTI VOITTAA PELIN
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::V)){
        this->goNextState = true;
        this->nextStateNum = 3;
    }

    //Tarkistaa onko pauseButton activoitu, jos on niin siirrytään seuraavaan stateen
    if(this->pauseButt->activated){
        this->nextStateNum = 4;
        this->goNextState = true;
    }

    //Updattaa player inventoria jatkuvasti (koska hiiri käyttöinen)
    this->player_.updateInvetory(mousePos, deltaTime);

    //Playerin moovia ja attackia rajoittava aika
    this->player_.updateTimes(deltaTime, mousePos);

    //GameState kutsuu monsterupdattia vain 0.1s välein
    this->updateTimeSinceLastMonsterUpdate(deltaTime);
    
    // PLAYER INPUT KOMENNOT, KÄYTETÄÄN PLAYER INPUTTIIN VAIN KEYBOARDIA

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
        this->player_.PickUpItem();                     //Pelkästään yrittää pickuppaa itemiä, käytö player_.addItem(Item*) jos haluat lisätä itemin
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        player_.Attack();
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::T)){
        player_.GetRoom()->Talk('T', player_.X(), player_.Y());
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Y)){
        player_.GetRoom()->Talk('Y', player_.X(), player_.Y());
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::N)){
        player_.GetRoom()->Talk('N', player_.X(), player_.Y());
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){
        player_.Buy(player_.GetRoom()->BuyItem(1,player_.GetGold()));
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){
        player_.Buy(player_.GetRoom()->BuyItem(2,player_.GetGold()));
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)){
        player_.Buy(player_.GetRoom()->BuyItem(3,player_.GetGold()));
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        if(player_.MvUp()==2){
            if(roomIndex_>FLOOR_WIDTH-1){
                roomIndex_-=FLOOR_WIDTH;
                player_.SetRoom(rooms_[roomIndex_]);
                player_.SetXY(player_.X(),player_.Y()+ROOM_HEIGHT-1);
            }
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        if(player_.MvLf()==2){
            if(roomIndex_%FLOOR_WIDTH!=0){
                roomIndex_--;
                player_.SetRoom(rooms_[roomIndex_]);
                player_.SetXY(player_.X()+ROOM_WIDTH-1,player_.Y());
            }
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        if(player_.MvDn()==2){
            if(roomIndex_%9<FLOOR_HEIGHT*(FLOOR_WIDTH-1)){
                roomIndex_+=FLOOR_WIDTH;
                player_.SetRoom(rooms_[roomIndex_]);
                player_.SetXY(player_.X(),player_.Y()-ROOM_HEIGHT+1);
            }
        }
    }
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        if(player_.MvRg()==2){
            if(roomIndex_%FLOOR_WIDTH!=2){
                roomIndex_++;
                player_.SetRoom(rooms_[roomIndex_]);
                player_.SetXY(player_.X()-ROOM_WIDTH+1,player_.Y());
            }
        }
    }

    if(this->timeSinceLastMonsterUpdate > this->monsterUpdateSpeed){
        timeSinceLastMonsterUpdate=0;
        int player_damage_taken = this->player_.GetRoom()->updateRoom(player_.X(), player_.Y());
        //TARKISTAA TILET
        switch (player_.GetRoom()->Get(player_.X(),player_.Y())){
        case 4:
            roomIndex_+=9;
            if(roomIndex_>=27){
                this->goNextState = true;
                this->nextStateNum = 3;}
            else{player_.SetRoom(rooms_[roomIndex_]);}
            break;
        case 6:
            player_damage_taken +=2;
            break;
        }
        if(player_.TakeDamage(player_damage_taken)){
            this->goNextState = true;
            this->nextStateNum = 2;           
        }
    }
}

void GameState::drawState(sf::RenderTarget* target){

    //Tässä piirrtään KAIKKI, ekat piiretyt jää uusien alle

    //KÄYTÄ TÄTÄ TESTAAMAAN AINA ETTÄ deltaTime EI KASVA LIIKAA,eli fps ei putoo liikää, jos fps putoo alle about 500, jotain hidastaa paljon -> KORJAA, hidas fps tekee inputeista unresposive
    //std::cout << deltaTime << " " << mousePos.x << " " << mousePos.y << std::endl ;
    
    //PIIRTÄÄ tausta kuvat
    target->draw(this->statsBackgroundSprite);
    target->draw(this->HPandShieldsBackgroundSprite);
    target->draw(dialogueBackgroundSprite);
    target->draw(border);

    //PIIRTÄÄ HUONEEN
    target->draw(*(player_.GetRoom()));

    //PIIRTÄÄ PELAAJAN JA INVENTORIN
    player_.Draw(target);

    //Piirtää vihut ja allyt
    player_.GetRoom()->DrawEnemies(target);
    player_.GetRoom()->DrawAllies(target);

    //Piirtää dialogin, jos ei dialogia, ei piirrä mitään
    player_.GetRoom()->drawDialogue(target);

    //Piirtää pause napin
    target->draw(this->pauseButtSprite);

}