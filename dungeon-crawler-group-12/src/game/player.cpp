#include "player.hpp"
#include <constants>


Player::Player(int x, int y, Room* room):x_(x),y_(y),room_(room){

    //LOAD ALL NECESSARY PLAYER TEXTURES WHICH IS ALOT
    //EN JAKSANU LOOPPAA EI VOI MITÄÄ LOL
    if(!playerIdleTextures[0].loadFromFile("images/playerSprites/WarriorUpIdle.png")){
        std::cout << "ERROR LOADING PLAYER" << "\n";
    }
    if(!playerIdleTextures[1].loadFromFile("images/playerSprites/WarriorRightIdle.png")){
        std::cout << "ERROR LOADING PLAYER" << "\n";
    }
    if(!playerIdleTextures[2].loadFromFile("images/playerSprites/WarriorDownIdle.png")){
        std::cout << "ERROR LOADING PLAYER" << "\n";
    }
    if(!playerIdleTextures[3].loadFromFile("images/playerSprites/WarriorLeftIdle.png")){
        std::cout << "ERROR LOADING PLAYER" << "\n";
    }
    if(!playerAttackTextures[0].loadFromFile("images/playerSprites/WarriorUpAttack01.png")){
        std::cout << "ERROR LOADING PLAYER" << "\n";
    }
    if(!playerAttackTextures[1].loadFromFile("images/playerSprites/WarriorRightAttack01.png")){
        std::cout << "ERROR LOADING PLAYER" << "\n";
    }
    if(!playerAttackTextures[2].loadFromFile("images/playerSprites/WarriorDownAttack01.png")){
        std::cout << "ERROR LOADING PLAYER" << "\n";
    }
    if(!playerAttackTextures[3].loadFromFile("images/playerSprites/WarriorLeftAttack01.png")){
        std::cout << "ERROR LOADING PLAYER" << "\n";
    }
    if(!playerTakeDMGTextures[0].loadFromFile("images/playerSprites/WarriorUpHurt.png")){
        std::cout << "ERROR LOADING PLAYER" << "\n";
    }
    if(!playerTakeDMGTextures[1].loadFromFile("images/playerSprites/WarriorRightHurt.png")){
        std::cout << "ERROR LOADING PLAYER" << "\n";
    }
    if(!playerTakeDMGTextures[2].loadFromFile("images/playerSprites/WarriorDownHurt.png")){
        std::cout << "ERROR LOADING PLAYER" << "\n";
    }
    if(!playerTakeDMGTextures[3].loadFromFile("images/playerSprites/WarriorLeftHurt.png")){
        std::cout << "ERROR LOADING PLAYER" << "\n";
    }

    direction = array<pair<int, int>, 4> {pair<int, int>(0,-1), pair<int, int>(1,0), pair<int, int>(0,1), pair<int, int>(-1,0)};

    playerSprite.setTexture(playerIdleTextures[2]);
    playerSprite.setTextureRect(sf::IntRect(0,0,48,48));
    playerSprite.setPosition(x_, y_);

    for(int i = 0; i<4; ++i){
        for(int j = 0; j<3; ++j){
            if(!perkTextures[i][j].loadFromFile("images/perks/perk_" + perkNames[i*3+j] + ".png")){
                std::cout << "FAILED PERK TEXTURE LOAD" << "\n";
            }
        }
    }
    for(int i = 0; i < 3; ++i){
        perkButtons[i] = new Button(445 + 130 * i, 690, 110, 110, " ", 1, sf::Color::Red, sf::Color::Yellow, sf::Color::Green);
    }

    for(int i = 0; i<2; ++i){
        swapPerkTree[i] = new Button(345 + 505 * i, 695, 70, 100, " ", 1, sf::Color::Blue, sf::Color::Yellow, sf::Color::Green);
    }

    perkExamine = new Text(0, 0, " ", "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 15, 1, true);
    perkAqquired = new Text(0, 0, " ", "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::Green, 20, 1, true);

    inventory_ = new Inventory();

    perkTreeBackground.setSize(sf::Vector2f(620, 130));
    perkTreeBackground.setPosition(320, 680);
    
    if(!arrow.loadFromFile("images/perks/backicon.png")){
        std::cout << "FAILED ARROW LOAD" << "\n";
    }

    //Order is important! (I think):
    InitiateHeartSprites(stats_.MAX_HP/100);
    InitiateShieldSprites(stats_.MAX_Shields/200);
    InitiateSourceTextures();
    SetShieldandHeartspritepos(0, 20);
    InitiateStatsNumbers();
    SetShieldandHeartTextures();
}

Player::~Player()
{
    for(auto i : stats_.heart_sprites)
    {
        delete i;
    }
    for(auto i : stats_.shield_sprites)
    {
        delete i;
    }
    for(auto i : perkButtons){
        delete i;
    }
    for(auto i : swapPerkTree){
        delete i;
    }
    delete inventory_;
    delete perkExamine;
    delete perkAqquired;

    //EN ole varma onko free näiden kohalla oikea ratkaisu: KRISTIAN, MIELUMMIN KAIT DELETE MUT FREE PITÄIS TOIMIA
    delete(stats_.levelnumbertext);
    delete(stats_.XPnumbertext); 
    delete(stats_.DMGnumbertext);
    delete(stats_.Critnumbertext);
    delete(stats_.Goldnumbertext);
    delete(stats_.DMGreductionnumbertext);
    delete(stats_.Movementspeednumbertext);
    delete(stats_.Attackspeednumbertext);
    delete(stats_.Weightnumbertext);
    delete stats_.LastHeartnumber;
    delete stats_.LastShieldnumber;
    delete stats_.TotalHPtext;
    delete stats_.TotalShieldstext;
    delete stats_.Skillpointstext;
}

int Player::X(){return x_;}
int Player::Y(){return y_;}
char Player::G(){return g_;}
Room * Player::GetRoom(){return room_;}
Inventory* Player::GetInvetory(){return inventory_;}
int Player::GetGold(){
    if(Perks_["negotiator"].second){
        return 2*gold_;
    }
    return gold_;
    }

float Player::GetMoveSpeed(){
    if(Perks_["berserk"].second){
        return (moveSpeed + 0.01*inventory_->GetMsBonus() + 0.001*inventory_->GetTotalWeight()) * 0.75;
    }
    return moveSpeed + 0.01*inventory_->GetMsBonus() + 0.001*inventory_->GetTotalWeight(); } // + inventory_->GetMsBonus()

int Player::GetAttackDamage(){ return stats_.Damage + inventory_->GetDmgBonus(); }

float Player::GetAttackSpeed(){
    float baseAS = attackSpeed + 0.01*inventory_->GetAsBonus();
    if(frenzyActive){
        return baseAS*0.8;
    }
    return baseAS; }

float Player::GetDamageMultiplier(){ return 1/(1+0.01*inventory_->GetArmor()); }

void Player::SetRoom(Room * room){room_ = room;}

void Player::SetXY(int x, int y){
        x_=x;
        y_=y;
    }

void Player::GainGold(int amount){
        if(Perks_["plutocrat"].second){
            GainXP(amount);
        }
        gold_ += amount; 
    }


bool Player::MvTo(int x, int y){
        if(room_->Free(x,y)){ // MUUTA KUN TULEE TILEJÄ
            x_ = x;
            y_ = y;
            return true;
        }
        return false;
    }

bool Player::CanMove(){
        if(timeLastMove>GetMoveSpeed()){
            timeLastMove=0;
            return true;
        }
        return false;
    }

int Player::MvUp(){
        if(!playerInputCommandDone && !attacking){
            playerInputCommandDone = true;
            currentMoveDirection = 0;
            room_->StopTalking();
            if(room_->OutOfBounds(x_,y_-1) && CanMove()){
                return 2;            
            }
            if(room_->Free(x_,y_-1) && CanMove()){
                --y_;
                hasPlayerSwappedTiles = true;
                return 1;
            }
        }
        return 0;
    }

int Player::MvDn(){
        if(!playerInputCommandDone && !attacking){
            playerInputCommandDone = true;
            currentMoveDirection = 2;
            room_->StopTalking();
            if(room_->OutOfBounds(x_,y_+1) && CanMove()){
                return 2;
            }
            if(room_->Free(x_,y_+1) && CanMove()){
                ++y_;
                hasPlayerSwappedTiles = true;
                return 1;
            }
        }
        return 0;
    }

int Player::MvLf(){
        if(!playerInputCommandDone && !attacking){
            playerInputCommandDone = true;
            currentMoveDirection = 3;
            room_->StopTalking();
            if(room_->OutOfBounds(x_-1,y_) && CanMove()){
                return 2;
            }
            if(room_->Free(x_-1,y_) && CanMove()){
                --x_;
                hasPlayerSwappedTiles = true;
                return 1;
            }
        }
        return 0;
    }

int Player::MvRg(){
        if(!playerInputCommandDone && !attacking){
            playerInputCommandDone = true;
            currentMoveDirection = 1;
            room_->StopTalking();
            if(room_->OutOfBounds(x_+1,y_) && CanMove()){
                return 2;
            }
            if(room_->Free(x_+1,y_) && CanMove()){
                ++x_;
                hasPlayerSwappedTiles = true;
                return 1;
            }
        }
        return 0;
    }


bool Player::AddItem(Item* item){
        return inventory_->pickUpItem(item);
    }

void Player::updateTimes(float deltaTime, sf::Vector2i newMousePos){
        playerInputCommandDone = false;
        hasPlayerSwappedTiles = false;
        timeLastMove += deltaTime;
        timeLastPickUp += deltaTime;
        timeLastAttack += deltaTime;
        timeLastSpriteUpdate += deltaTime;
        timeSincePerkTreeSwap += deltaTime;
        mousePos = newMousePos;
        if(frenzyActive){
            frenzyTimer += deltaTime;
            if(frenzyTimer>5){
                frenzyActive = false;
                frenzyTimer = 0.0;
            }
        }
        dTime = deltaTime;
    }

void Player::updatePerkButtons(){
    
    for(int i = 0; i<perkButtons.size(); ++i){
        perkButtons[i]->updateButton(mousePos);
    }
    for(int i = 0; i<swapPerkTree.size(); ++i){
        swapPerkTree[i]->updateButton(mousePos);
    }

    for(int i = 0; i < perkButtons.size(); ++i){
        if(perkButtons[i]->hover == true){
            drawExamineText = true;
            perkExamine->setText(perkExamineTexts[currentPerkTree*3+i]);
            perkExamine->setCords(500 + 130 * i - perkExamine->returnText()->getGlobalBounds().width/2, 660 - perkExamine->returnText()->getGlobalBounds().height);
            perkExamine->setBackGround();
        }
    }

    if(skillpoints > 0){
        for(int i = 0; i<perkButtons.size(); ++i){
            if(perkButtons[i]->activated == true){
                if(i == 0 && !Perks_[perkNames[currentPerkTree*3 + i]].second){
                    Perks_[perkNames[currentPerkTree*3 + i]].second = true;
                    skillpoints -= 1;
                }else if(i == 1 && Perks_[perkNames[currentPerkTree*3+0]].second && !Perks_[perkNames[currentPerkTree*3 + i]].second){
                    Perks_[perkNames[currentPerkTree*3 + i]].second = true;
                    skillpoints -= 1;
                }else if(i == 2 && Perks_[perkNames[currentPerkTree*3+1]].second && !Perks_[perkNames[currentPerkTree*3 + i]].second){
                    Perks_[perkNames[currentPerkTree*3 + i]].second = true;
                    skillpoints -= 1;
                }
            }
        }
    }

    if(swapPerkTree[0]->activated == true && 0.25 < timeSincePerkTreeSwap){
        if(currentPerkTree == 0){
            currentPerkTree = 3;
        }else{currentPerkTree -= 1;}
        timeSincePerkTreeSwap = 0;
    }

    if(swapPerkTree[1]->activated == true && 0.25 < timeSincePerkTreeSwap){
        if(currentPerkTree == 3){
            currentPerkTree = 0;
        }else{currentPerkTree += 1;}
        timeSincePerkTreeSwap = 0;
    }
    stats_.Skillpointstext->setText("Skillpoints: " + to_string(skillpoints));

}

void Player::drawPerkAqquired(sf::RenderTarget* target){
    for(int i = 0; i < 3; ++i){
        if(Perks_[perkNames[currentPerkTree * 3 + i]].second){
            perkAqquired->setText("TAKEN");
            perkAqquired->setCords(500 + 130 * i - perkAqquired->returnText()->getGlobalBounds().width/2, 745 - perkAqquired->returnText()->getGlobalBounds().height/2);
            perkAqquired->drawText(target);
        }
    }
}

void Player::DropItem(Item* item){    
        item->setXY(x_, y_);                        //ei muuta kun asetetaan itemin cordinaatit ja asetetaan se room_:iin
        room_->AddItem(item);
    }

void Player::PickUpItem(){
        if(timeLastPickUp > pickUpSpeed){
            bool hasPickedUp = false;                            
            std::vector<Item*> placeHolder = room_->GetItems();
            auto index = placeHolder.begin(); 
            for(auto i : room_->GetItems()){                                    //Loopataan items vektorin yli ja tarkistetaan onko itemiä jolla sama x,y kord
                if(i->X() == x_ && i->Y() == y_ && !hasPickedUp){   //ja tarkistetaan myös onko kyseisellä pick up komennolla jo otettu itemi (ei nosta kaikkia itemiä samasta tilestä)                   
                    if(inventory_->pickUpItem(i)){                                 //jos on yritetään addaa itemi inventoriin, jos onnistuu niin pyyhitään items vektorista kyseinen itemi
                        placeHolder.erase(index);                               //Käytetään placeholder vektorii koska loopattavaa vektoria ei voi muokata loopin aikana
                        hasPickedUp = true;
                        timeLastPickUp = 0;
                        room_->setItems(placeHolder);
                   }
                }
                index += 1;
            }
        }
}

void Player::updateInvetory(sf::Vector2i mousePos, float dTime){              //Updattaa inventoria jatkuvasti, jos inventtorista yritetään droppaa, tai equippaa
        pair<Item*, pair<int,int>> item = inventory_->updateButtons(mousePos, dTime);
        if(item.second.first==-5){
            if(this->room_->OpenTile(x_ + direction[currentMoveDirection].first, y_ + direction[currentMoveDirection].second)){
                inventory_->removeInventoryItem(item.second.second);
            }
        }
        else if(item.first->GetName() != "null_texture"){

            DropItem(item.first);
        } else if(item.second.first != 0 || item.second.second != 0){
            if(item.second.first > 0){
                stats_.current_HP = min(item.second.first + stats_.current_HP, stats_.MAX_HP);
            }
            else if(item.second.first < 0){
                stats_.current_shields = min(- item.second.first + stats_.current_shields, stats_.MAX_Shields);
            }
            skillpoints += item.second.second;
            stats_.Skillpointstext->setText("Skillpoints: " + to_string(skillpoints));
        }
    }

void Player::Buy(Item* item){
        if(item != nullptr){
            if(AddItem(item)){
                if(Perks_["negotiator"].second){
                    gold_ -= item->getPrice() / 2;
                } else {
                    gold_ -= item->getPrice();
                }
            } else {
                room_->GetNPC()->NoInventory();
            }
        }
    }

void Player::GainXP(int amount)
{
    //Function is called every time player gains XP
    // Target XP = ((current_LVL)^2 + current_LVL) * 500
    // Level 2 = 1000 XP
    // Level 3 = 3000 XP
    // Level 4 = 6 000 XP
    // Level 5 = 10 000 XP
    // Level 6 = 15 000 XP
    // etc...

    stats_.XP += amount;

    while(NextLevelXP() < stats_.XP && stats_.lvl < stats_.MAX_lvl){
        Player::LevelUP();
    }
}

int Player::NextLevelXP(){
    return (stats_.lvl*stats_.lvl+stats_.lvl)*500;
}

void Player::LevelUP()
{
    //Stats go up by every lvl gained
    if(stats_.lvl < stats_.MAX_lvl)
    {
        stats_.lvl += 1;
        stats_.Damage += 1;
        stats_.current_HP += 50;
        stats_.current_shields += 100;
        if(stats_.lvl == stats_.MAX_lvl)
        {
            stats_.MAX_Shields += 200;
            stats_.MAX_HP += 100;
        }
        else
        {
            stats_.MAX_Shields += 100;
            stats_.MAX_HP += 50;
        }
        skillpoints += 1;

        //Shield and Heart sprites are added only every other lvl:
        if(stats_.lvl%2 == 0)
        {
            //find the position of the last heart sprite in GUI and calculate new position for new heart sprite:
            int x = stats_.heart_sprites.back()->first.getPosition().x + TILESIZE;
            int y = stats_.heart_sprites.back()->first.getPosition().y;
            //Add new heart sprite to vector and set the position:
            stats_.heart_sprites.push_back(new std::pair<sf::Sprite, int>);
            stats_.heart_sprites.back()->first.setPosition(x,y);

            //find the position of the last shield sprite in GUI and calculate new position for new shield sprite:
            x = stats_.shield_sprites.back()->first.getPosition().x + TILESIZE;
            y = stats_.shield_sprites.back()->first.getPosition().y;
            //Add new shield sprite to vector and set the position:
            stats_.shield_sprites.push_back(new std::pair<sf::Sprite, int>);
            stats_.shield_sprites.back()->first.setPosition(x,y);
        }

        //Set correct textures to HP and Shield vector items:
        SetShieldandHeartTextures();

        //Set skillpoints text:
        stats_.Skillpointstext->setText("Skillpoints: " + to_string(skillpoints));
    }

}

//Helper function to draw stats /
string Player::getBonusString(float bonus){
    if(bonus < 0){
        return " - " + floatTostring(abs(bonus), 2);
    }
    return "+ " + floatTostring(bonus, 2);
}

// Helper function to draw stats
std::string Player::floatTostring(const float value, int precision)
{
    std::ostringstream out;
    out.precision(precision);
    out<<std::fixed<<value;
    return out.str();
}

//*************************************************************************//
//********Draw stat numbers**********//
void Player::InitiateStatsNumbers()
{
    // Set stat number texts:

    stats_.levelnumbertext = new Text(658, (ROOM_HEIGHT) * TILESIZE - 120,
    "Level: " + std::to_string(stats_.lvl), "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 13, 1, false);

    stats_.XPnumbertext = new Text(768, (ROOM_HEIGHT) * TILESIZE - 120,
    "XP: " + std::to_string(stats_.XP), "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 13, 1, false);

    stats_.DMGnumbertext = new Text(658, (ROOM_HEIGHT) * TILESIZE - 100,
    "DMG: " + std::to_string(stats_.Damage), "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 13, 1, false);

    stats_.Goldnumbertext = new Text(768, (ROOM_HEIGHT) * TILESIZE - 100,
    "Gold : " + std::to_string(gold_), "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 13, 1, false);

    stats_.DMGreductionnumbertext = new Text(658, (ROOM_HEIGHT) * TILESIZE - 80,
    "DMG -%: " + floatTostring(GetDamageMultiplier(), 2), "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 13, 1, false);

    stats_.Movementspeednumbertext = new Text(768, (ROOM_HEIGHT) * TILESIZE - 80,
    "MV Delay: " + floatTostring(GetMoveSpeed(), 2), "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 13, 1, false);

    stats_.Attackspeednumbertext = new Text(768, (ROOM_HEIGHT) * TILESIZE - 60,
    "ATK Delay: " + floatTostring(GetAttackSpeed(), 2), "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 13, 1, false);

    stats_.Critnumbertext = new Text(658, (ROOM_HEIGHT) * TILESIZE - 60,
    "Crit change: " + floatTostring(GetAttackSpeed(), 2), "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 13, 1, false);

    stats_.Weightnumbertext = new Text(658, (ROOM_HEIGHT) * TILESIZE - 40,
    "Weight: " +  std::to_string(gold_), "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 13, 1, false);

    stats_.LastHeartnumber = new Text(stats_.heart_sprites.back()->first.getPosition().x + 2,stats_.heart_sprites.back()->first.getPosition().y + 3,
    "0", "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 15, 1, false);

    stats_.LastShieldnumber = new Text(stats_.shield_sprites.back()->first.getPosition().x + 3,stats_.shield_sprites.back()->first.getPosition().y + 4,
    "0", "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 15, 1, false);

    stats_.TotalHPtext = new Text(8, 732,
    "Total HP: " + to_string(stats_.current_HP) + "/" + to_string(stats_.MAX_HP), "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 13, 1, false);

    stats_.TotalShieldstext = new Text(8, 755,
    "Total Shields: " + to_string(stats_.current_shields) + "/" + to_string(stats_.MAX_Shields), "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 13, 1, false);

    stats_.Skillpointstext = new Text(9, 778,
    "Skillpoints: " + to_string(skillpoints), "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 13, 1, false);
}

void Player::DrawStatsNumbers(sf::RenderTarget* target)
{
    //Draw stat number texts:

    stats_.levelnumbertext->setText("Level: " + std::to_string(stats_.lvl));
    stats_.XPnumbertext->setText("XP: " + std::to_string(stats_.XP) + "/" + std::to_string(NextLevelXP()));
    stats_.DMGnumbertext->setText("DMG: " + std::to_string(stats_.Damage) + " + " + std::to_string(inventory_->GetDmgBonus()));
    stats_.Goldnumbertext->setText("Gold : " + std::to_string(gold_));
    stats_.DMGreductionnumbertext->setText("Armor: " + std::to_string(inventory_->GetArmor()));
    stats_.Critnumbertext->setText("Crit chance: " + std::to_string(inventory_->GetCritChange()) + "%");
    if(Perks_["berserk"].second && stats_.current_shields == 0){
        stats_.Movementspeednumbertext->setText("MV Delay: " + floatTostring(moveSpeed * 0.75, 2) + getBonusString(0.0075*(inventory_->GetMsBonus()+0.1*inventory_->GetTotalWeight())));
    } else {
        stats_.Movementspeednumbertext->setText("MV Delay: " + floatTostring(moveSpeed, 2) + getBonusString(0.01*(inventory_->GetMsBonus()+0.1*inventory_->GetTotalWeight())));
    }
    if(frenzyActive){
        stats_.Attackspeednumbertext->setText("ATK Delay: " + floatTostring(0.8*attackSpeed, 2) + getBonusString(0.008*inventory_->GetAsBonus()));
    } else {
        stats_.Attackspeednumbertext->setText("ATK Delay: " + floatTostring(attackSpeed, 2) + getBonusString(0.01*inventory_->GetAsBonus()));
    }
    stats_.Weightnumbertext->setText("Weight: " + to_string(inventory_->GetTotalWeight()));

    stats_.levelnumbertext->drawText(target);
    stats_.XPnumbertext->drawText(target);
    stats_.Critnumbertext->drawText(target);
    stats_.DMGnumbertext->drawText(target);
    stats_.Goldnumbertext->drawText(target);
    stats_.DMGreductionnumbertext->drawText(target);
    stats_.Movementspeednumbertext->drawText(target);
    stats_.Attackspeednumbertext->drawText(target);
    stats_.Weightnumbertext->drawText(target);

    stats_.LastHeartnumber->drawText(target);
    stats_.LastShieldnumber->drawText(target);

    stats_.TotalHPtext->drawText(target);
    stats_.TotalShieldstext->drawText(target);
    stats_.Skillpointstext->drawText(target);

}
//*************************************************************************//



//*************************************************************************//
//********HP********SHIELDS**********//

void Player::SetShieldandHPLastnumbers()
{
    //Set lastheartnumber:
    if(stats_.heart_sprites[0]->second != 0)
    {
        //Find the last sprite with half or full sprite:
        for(auto i : stats_.heart_sprites)
        {
            if(i->second != 0)
            {
                stats_.LastHeartnumber->setCords(i->first.getPosition().x + 2,i->first.getPosition().y + 3);
            }
        }
    }
    else
    {
        stats_.LastHeartnumber->setCords(stats_.heart_sprites[0]->first.getPosition().x + 2, stats_.heart_sprites[0]->first.getPosition().y + 3);
    }

    int tmp = stats_.current_HP;
    //Calculate the hp for last heart:
    if(tmp > 100)
    {
        for(; tmp > 0;)
        {
            tmp -= 100;
        }
        tmp += 100;
    }
    stats_.LastHeartnumber->setText(to_string(tmp));

    //Set lastshieldnumber:
    if(stats_.shield_sprites[0]->second != 0)
    {
        //Find the last sprite with half or full sprite:
        for(auto i : stats_.shield_sprites)
        {
            if(i->second != 0)
            {
                stats_.LastShieldnumber->setCords(i->first.getPosition().x + 3,i->first.getPosition().y + 4);
            }
        }
    }
    else
    {
        stats_.LastShieldnumber->setCords(stats_.shield_sprites[0]->first.getPosition().x + 3, stats_.shield_sprites[0]->first.getPosition().y + 4);
    }

    tmp = stats_.current_shields;
    //Calculate the shield amount for last shield:
    if(tmp > 200)
    {
        for(; tmp > 0;)
        {
            tmp -= 200;
        }
        tmp += 200;
    }
    stats_.LastShieldnumber->setText(to_string(tmp));
    stats_.TotalShieldstext->setText("Total Shields: " + to_string(stats_.current_shields) + "/" + to_string(stats_.MAX_Shields));
    stats_.TotalHPtext->setText("Total HP: " + to_string(stats_.current_HP) + "/" + to_string(stats_.MAX_HP));
}

void Player::InitiateHeartSprites(unsigned int size)
{
    for(int i = 0; i < size; i++)
    {
        stats_.heart_sprites.push_back(new std::pair<sf::Sprite, int>);
    }
}
void Player::InitiateShieldSprites(unsigned int size)
{
    for(int i = 0; i < size; i++)
    {
        stats_.shield_sprites.push_back(new std::pair<sf::Sprite, int>);
    }
}

void Player::InitiateSourceTextures()
    {
        //This method Initiates the source textures that will be used to update HP & Shield sprites:
        stats_.heart_texture.loadFromFile("images/heart.png");
        stats_.half_heart_texture.loadFromFile("images/half_heart.png");
        stats_.empty_heart_texture.loadFromFile("images/empty_heart.png");
        stats_.shield_texture.loadFromFile("images/shield_white.png");
        stats_.half_shield_texture.loadFromFile("images/half_shield_white.png");
        stats_.empty_shield_texture.loadFromFile("images/empty_shield_white.png");
        
    }

void Player::SetShieldandHeartspritepos(int x, int y)
{
    //Set Shields and HP sprite positions horizontally starting from x,y:
    int tmp = x;
    for(auto i : stats_.heart_sprites)
    {
        i->first.setPosition(tmp*TILESIZE + 5, y*TILESIZE + 12);
        tmp++;
    }
    y++;
    for(auto i : stats_.shield_sprites)
    {
        i->first.setPosition(x*TILESIZE + 5, y*TILESIZE + 12);
        x++;
    }
}

void Player::SetShieldandHeartTextures()
{
    //this function is called when player takes damage to set correct texture values
    // Amount of HP/100 amounts to hearts shown (100hp -> show one full heart):
    //51-100hp -> full heart
    //1-50hp -> half_heart
    //0hp -> empty_heart
    unsigned int tmp = stats_.current_HP;

    for(auto i : stats_.heart_sprites)
    {
        if(tmp > 50)
        {
            i->second = 2;
            if(tmp >= 100)
            {
                tmp -= 100;
            }
            else
            {
                tmp = 0;
            }
        }
        else if(tmp > 0)
        {
            i->second = 1;
            tmp = 0;
        }
        else
        {
            i->second = 0;
        }
    }
    // (Amount of shields)/200 amounts to shields shown:
    //101-200shields -> full_shield
    //1-100shields -> half_shield
    //0shields -> empty_shield
    tmp = stats_.current_shields;

    for(auto i : stats_.shield_sprites)
    {
        if(tmp > 100)
        {
            i->second = 2;
            if(tmp >= 200)
            {
                tmp -= 200;
            }
            else
            {
                tmp = 0;
            }
        }
        else if(tmp > 0)
        {
            i->second = 1;
            tmp = 0;
        }
        else
            i->second = 0;
    }
    SetShieldandHPLastnumbers();
}


void Player::DrawShieldsandHearts(sf::RenderTarget* target)
{

    //Draw hearts with correct texture:
    //51-100hp -> full heart
    //1-50hp -> half_heart
    //0hp -> empty_heart
    for(auto i : stats_.heart_sprites)
    {
        if(i->second == 2)
        {
            i->first.setTexture(stats_.heart_texture);
        }
        else if(i->second == 1)
        {
            i->first.setTexture(stats_.half_heart_texture);
        }
        else
        {
            i->first.setTexture(stats_.empty_heart_texture);
        }
        target->draw(i->first);

    }

    //Draw shields with correct textures:
    //101-200shields -> full_shield
    //1-100shields -> half_shield
    //0shields -> empty_shield
    for(auto i : stats_.shield_sprites)
    {
        if(i->second == 2)
        {
            i->first.setTexture(stats_.shield_texture);
        }
        else if(i->second == 1)
        {
            i->first.setTexture(stats_.half_shield_texture);
        }
        else
        {
            i->first.setTexture(stats_.empty_shield_texture);
        }
        target->draw(i->first);

    }
}


void Player::AddHP(unsigned int amount)
{
    if(stats_.current_HP < stats_.MAX_HP)
    {
        stats_.current_HP += amount;
        if(stats_.current_HP > stats_.MAX_HP)
        {
            stats_.current_HP = stats_.MAX_HP;
        }
        SetShieldandHeartTextures();
    }
}

void Player::AddShields(unsigned int amount)
{
    if(stats_.current_shields < stats_.MAX_Shields)
    {
        stats_.current_shields += amount;
        if(stats_.current_shields > stats_.MAX_Shields)
        {
            stats_.current_shields = stats_.MAX_Shields;
        }
        SetShieldandHeartTextures();
    }
}

//*************************************************************************//


void Player::Attack(){
    if(timeLastAttack > GetAttackSpeed()){
        attacking = true;
        moving = false;
        takingDMG = false;
        timeLastAttack = 0;
        int dmg = GetAttackDamage();
        //Critical strike roll
        if(rand()%100 < inventory_->GetCritChange()){
            dmg += GetAttackDamage();
        }
        if(Perks_["berserk"].second && stats_.current_shields == 0){
            dmg = 3*dmg/2;
        }
        bool hasPrimaryTarget = false;

        // Target enemy in front of the player
        for(auto enemy : room_->GetEnemies()){
            int x = enemy->getX();
            int y = enemy->getY();
            bool dead = false;
            pair<int, int> dir = direction[currentMoveDirection];
            if(x_ + dir.first == x && y_ + dir.second == y){ 
                dead = AttackEnemy(enemy, dmg);
                hasPrimaryTarget = true;
            } 
            if(dead){
                EnemyKilled(enemy);
            }
            
        }

        // If there was a target and aoe tree is unlocked
        if(hasPrimaryTarget && Perks_["cleave"].second)
        {
            pair<int, int> dir = direction[currentMoveDirection];
            for(auto enemy : room_->GetEnemies()){
                bool dead = false;
                int x = enemy->getX();
                int y = enemy->getY();
                if(Perks_["smack"].second) { // either smack or slam is unlocked
                    if(abs(x_ + dir.first - x) < 2 && abs(y_ + dir.second-y) < 2 && abs(x_ + dir.first - x) + abs(y_ + dir.second-y) != 0 ){
                        if(Perks_["slam"].second){
                            dead = AttackEnemy(enemy, dmg/2);
                        } else {
                            dead = AttackEnemy(enemy, dmg/4);
                        }
                    }
                } else if(Perks_["cleave"].second){ // if cleave is unlocked
                    if(dir.first==0){
                        if(y_ + dir.second == y && abs(x_ - x) == 1){
                            dead = AttackEnemy(enemy, dmg/4);
                        }
                    } else {
                        if(x_ + dir.first == x && abs(y_ - y) == 1){
                            dead = AttackEnemy(enemy, dmg/4);
                        }
                    }
                }
                if(dead){
                    EnemyKilled(enemy);                 
                }
            }
        }
        //VILLEN TESTI RIKKOO SEINÄT //KRISTIAN MUUTIN VAIN ETTÄ VOI RIKKOO SUUNTAAN JONNE KATTOO JA LATOIN MONSTER ATTACK PERÄÄN, JOS RIKKOO LAATIKON EI INSTA HYÖKKÄÄ
        room_->BreakTile(x_ + direction[currentMoveDirection].first, y_ + direction[currentMoveDirection].second, GetAttackDamage());
        //VILLEN TESTI  LOPPUU
    }    
}

void Player::EnemyKilled(Monster* enemy){
    if(Perks_["shield regen"].second){
        unsigned int a = 10;
        int shieldGain = min((stats_.MAX_Shields - stats_.current_shields)/40, a);
        stats_.current_shields = min(stats_.current_shields + shieldGain, stats_.MAX_Shields) + 0.99;
    }
    if(Perks_["frenzy"].second){
        frenzyTimer = 0.0;
        frenzyActive = true;
    }
    GainXP(enemy->XPReward()*(100+inventory_->GetBonusXPMultiplier())/100);
    GainGold(enemy->goldReward()*(100+inventory_->GetBonusGoldMultiplier())/100);
    room_->RemoveMonster(enemy);                   
}

bool Player::AttackEnemy(Monster* target, int damage){
    if(Perks_["cleptomania"].second){
        GainGold(5);
    }
    return target->TakeDamage(damage);
}

bool Player::TakeDamage(unsigned int damage)
{  
    // One shield blocks 200 damage
    // One hp equals 100 damage

    if(damage != 0 && !attacking){
        moving = false;
        takingDMG = true;               //Animation for taking dmg
    }
    int damageTaken  = damage * GetDamageMultiplier() + 0.99;
    if(Perks_["tough skin"].second){
        float multiplier = 0.0;
        bool first = true;
        for(auto enemy : room_->GetEnemies()){
            if(abs(enemy->getX()- x_) < 2 && abs(enemy->getY()-y_) < 2){
                if(!first){
                    multiplier += 0.15;
                }
                first = false;
            }
        }
        damageTaken = damageTaken - damageTaken * min(multiplier, float(0.6)) + 0.99;
    }
    if(Perks_["survivor"].second && stats_.current_shields/4 < damageTaken && stats_.current_shields != 0){
        damageTaken=damageTaken/2 + 0.99;
    }

    unsigned int tmp = damageTaken;
    //Take shield count from damage, tmp goes to 0 if enough shields to block the damage:
    if(tmp > stats_.current_shields)
    {
        tmp -= stats_.current_shields;
    }
    else
        tmp = 0;
    if(stats_.current_shields > damageTaken)
    {
        stats_.current_shields -= damageTaken;
    }
    //Shields go to 0 if damage is larger than shield amount:
    else
    {
        stats_.current_shields = 0;
    }
    //Take the remaining damage to HP:
    if(stats_.current_HP > tmp)
    {
        stats_.current_HP -= tmp;
        SetShieldandHeartTextures();
        return false;
    }
    else
    {   
        if(Perks_["second chance"].second){
            stats_.current_HP = stats_.MAX_HP/2;
            Perks_["second chance"]=make_pair("", false);
            return false;
        }
        stats_.current_HP = 0;
        SetShieldandHeartTextures();
        return true;
    }
}

void Player::updatePlayerSprite(){

        //LUE-------LUE-------LUE-------LUE-------LUE-------LUE-------LUE-------LUE-------LUE-------LUE-------LUE-------LUE
        //Player animaatio
        //Ensin tarkistetaan onko vaihtanut ruutua, jos on niin updatataan animaatioita, jos ei ole niin tarkistetaan onko moving 
        //(moving on perus tila, huonosti nimetty, jos ei hyökkää ota dmg yms niin on moving), jos moving niin sit updattaa
        //jos ei moving, niin sit tarkistetaan onko hyökkäys animaatio kesken, hyökkäys animaatio aloitetaan kun attack funktio muuttaa
        //attacking = true, hyökkäys animaatio pyörii niin kauan että on käynyt kaikki läpi, 6 eri kuvaa sitten alustaa taas että pelaaja moving yms
        //HUOM, PELAAJA EI VOI INPUTTAA MV komentohja kun attacking = true, eli kun hyökkäys animaatio on aloitettu, pelaaja on lukittu siihen tileen ja suuntaan
        //Kun hyökkäys animaatio on läpi palaa taas player moving = true tilaan
        //Sitten jos attacking ja moving swappedTiles on false niin mennään takeDMG, siinä player ottaa dmg
        //HUOM dmg animaation voi cancelata attack komennolla, mutta movement komento ei kancelaa sitä, se vain muuttaa mihin suuntaan pelaaja katsoo, silti näkyy dmg animaatio

        if(hasPlayerSwappedTiles && !takingDMG){
            playerSprite.setTexture(playerIdleTextures[currentMoveDirection]);
            playerSprite.setTextureRect(sf::IntRect(idleIndex * 48, 0, 48, 48));
            if(idleIndex == 4){
                idleIndex = 0;
            }else{idleIndex += 1;}
        }else if(moving && !takingDMG && idleSpriteUpdateSpeed < timeLastSpriteUpdate){
            attackIndex = 0;
            takeDMGIndex = 0;
            playerSprite.setTexture(playerIdleTextures[currentMoveDirection]);
            playerSprite.setTextureRect(sf::IntRect(idleIndex * 48, 0, 48, 48));
            timeLastSpriteUpdate = 0;
            if(idleIndex == 4){
                idleIndex = 0;
            }else{idleIndex += 1;}
        }else if(attacking && attackSpriteUpdateSpeed < timeLastSpriteUpdate){
            idleIndex = 0;
            takeDMGIndex = 0;
            playerSprite.setTexture(playerAttackTextures[currentMoveDirection]);
            playerSprite.setTextureRect(sf::IntRect(attackIndex * 48, 0, 48, 48));
            timeLastSpriteUpdate = 0;
            if(attackIndex == 5){
                attackIndex = 0;
                attacking = false;
                moving = true;
            }else{attackIndex += 1;}
        }else if(takingDMG && takeDMGSpriteUpdateSpeed < timeLastSpriteUpdate){
            idleIndex = 0;
            attackIndex = 0;
            playerSprite.setTexture(playerTakeDMGTextures[currentMoveDirection]);
            playerSprite.setTextureRect(sf::IntRect(takeDMGIndex * 48, 0, 48, 48));
            timeLastSpriteUpdate = 0;
            if(takeDMGIndex == 3){
                takeDMGIndex = 0;
                takingDMG = false;
                moving = true;
            }else{takeDMGIndex += 1;}
        }
    
    }

void Player::Draw(sf::RenderTarget* target){


        sf::Sprite perk;
        sf::Sprite arrows;

        if(currentPerkTree == 0){
            perkTreeBackground.setFillColor(sf::Color(60, 0, 0));
        }else if(currentPerkTree == 1){
            perkTreeBackground.setFillColor(sf::Color(103, 51, 0));
        }else if(currentPerkTree == 2){
            perkTreeBackground.setFillColor(sf::Color(0, 51, 0));
        }else if(currentPerkTree == 3){
            perkTreeBackground.setFillColor(sf::Color(51, 0, 51));
        }
        target->draw(perkTreeBackground);

        updatePerkButtons();

        arrows.setTexture(arrow);
        arrows.setPosition(345, 695);
        target->draw(arrows);
        arrows.setRotation(180);
        arrows.setPosition(345 + 505 + 70, 695 + 100);
        target->draw(arrows);

        for(int i = 0; i<3; ++i){
            perk.setTexture(perkTextures[currentPerkTree][i]);
            perk.setPosition(130 * i + 445, 690);
            perk.setScale(110.f/70.f, 110.f/70.f);
            target->draw(perk);
        }

        drawPerkAqquired(target);

        inventory_->drawInventory(target);
        DrawShieldsandHearts(target);
        DrawStatsNumbers(target);
        room_->DrawItems(target);
        updatePlayerSprite();
        playerSprite.setPosition(x_*TILESIZE - 7,y_*TILESIZE - 8);
        playerSprite.setScale(0.98, 0.98);
        target->draw(this->playerSprite);

        if(drawExamineText){
            perkExamine->drawText(target);
        }
        drawExamineText = false;

    }

//---------------------------------------------------------------------------------------------------------//
