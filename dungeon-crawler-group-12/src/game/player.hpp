#pragma once

#include "room.cpp"
#include "inventory.cpp"
#include "button.hpp"

#include "SFML/Window.hpp"

#include <vector>
#include <string>
#include <map>
#include <list>
#include <sstream>

using namespace std;

struct Stats
{
    unsigned int lvl = 1;
    unsigned int MAX_lvl = 10;
    unsigned int XP = 0;
    unsigned int Damage = 5;
    unsigned int MAX_HP = 100;
    //200 equals to 1 shields shown
    unsigned int MAX_Shields = 200;
    //Only MAX amount can be carried
    unsigned int current_HP = 100;
    unsigned int current_shields = 200;
    //One Shield blocks total of 200 damage
    //If no shields, HP takes the damage
    //-----REGAIN SHIELDS BY REPAIRING ARMOUR FROM NPC, ARMOUR STILL GRANTSDMG REDUCTION-----

    //Source textures for setting correct HP and Shield textures:
    sf::Texture heart_texture;
    sf::Texture half_heart_texture;
    sf::Texture empty_heart_texture;
    std::vector<std::pair<sf::Sprite, int>*> heart_sprites;
    //0 = empty, 1 = half, 2 = full;

    sf::Texture shield_texture;
    sf::Texture half_shield_texture;
    sf::Texture empty_shield_texture;
    std::vector<std::pair<sf::Sprite, int>*> shield_sprites;
    //0 = empty, 1 = half, 2 = full

    Text *levelnumbertext;
    Text *XPnumbertext;
    Text *DMGnumbertext;
    Text *Goldnumbertext;
    Text *DMGreductionnumbertext;
    Text *Movementspeednumbertext;
    Text *Attackspeednumbertext;
    Text *Critnumbertext;
    Text *Weightnumbertext;

    Text *LastHeartnumber;
    Text *LastShieldnumber;
    Text *TotalHPtext;
    Text *TotalShieldstext;
    Text *Skillpointstext;

};

class Player{
public:
    Player(int, int, Room*);
   ~Player();                           //ÄLÄ UNOHDA LAITTAA TÄNNE KAIKKI LUODUT POINTERIT

    void SetRoom(Room * room);          //Settaa roomin nykyiseen huoneeseen + vaihtaa sit huonetta tarpeen mukaan

    void SetXY(int x, int y);           //ERO MvTo ja setXY välillä, että MvTo tarkastaa, voiko liikkua.

    bool MvTo(int x, int y);            // TRUE IF MOVE SUCCESSFUL

    bool CanMove();                     //Tarkistaa vaan movespeedind kanssa saako liikkua

    int MvUp();                     //NÄMÄ KAIKKI SIIRTÄÄ JOHONKIN SUUNTAAN
    int MvDn();                     // PALAUTTAA 0, jos seinä, 1 jos liike onnistui, 2 jos liikkuminen johtaisi ulos kentältä.
    int MvLf();                     // 2 avulla siirrytään huoneesta toiseen gameStatessa
    int MvRg();
    bool AddItem(Item* item);               //Yrittää addaa itemii inventoriin true jos onnistuu
    void DropItem(Item* item);              //Lisää itemin roomin items vektoriin
    void PickUpItem();                      //Pick up item funktio, yrittää pick uppaa itemiä samasta cordinaateista missä player on
    void Buy(Item* item);               //Ostaa itemin

    void updateTimes(float deltaTime, sf::Vector2i newMousePos);      //Päivittää gameStatesta saatua aikaa ja resettaa swappedTiles ja inputCommand boolit = false

    void updateInvetory(sf::Vector2i mousePos, float dTime);        //Päivittää inventoria, tämän kautta equipataa, dropataa itemit yms. 

    void updatePlayerSprite();      //PELKKÄÄ ANIMAATIOTA VARTEN, muuttujia attacking moving yms. voi käyttää muualla, mutta ÄLKÄÄ MUUTTAKO NIITÄ vaan lukekaa vaan

    void updatePerkTree();

    void updatePerkButtons();

    int X();        // PALAUTTAA KOORDINAATIN
    int Y();
    char G();
    Room* GetRoom();
    Inventory* GetInvetory();
    int GetGold();
    int NextLevelXP();

    void InitiateStatsNumbers();
    void DrawStatsNumbers(sf::RenderTarget*);

    void GainXP(int);           //Function is called every time player gains XP, Every certain amount of XP levels up the player
    void GainGold(int amount); 
    void LevelUP();

    //********HP****&****SHIELDS**********//
    void SetShieldandHPLastnumbers();
    void InitiateHeartSprites(unsigned int);            //Reserving memory for (int) sprites vector objects:
    void InitiateShieldSprites(unsigned int);           //give amount of hearts/shields you want drawn:
    void InitiateSourceTextures();      //Set the source textures (empty, half, full shields/hearts):
    void SetShieldandHeartspritepos(int start_x, int y);        //Set Shields and HP sprite positions horizontally to hight y starting from x (will be multiplied by TILESIZE):
    void SetShieldandHeartTextures();           //Set correct HP& Shield textures based on current stats:
    void DrawShieldsandHearts(sf::RenderTarget*);       //Draw HP&Shields on the screen:
    void AddHP(unsigned int);       //Add int amount of HP (100 = 1 heart) to player:
    void AddShields(unsigned int);  //Add int amount of shields (200 = 1 shield) to player:
    //*************************************************************************//
    float GetMoveSpeed();
    int GetAttackDamage();
    float GetAttackSpeed();
    float GetDamageMultiplier();

    void Attack();
    bool TakeDamage(unsigned int);  // Returns true, if player dies
    bool AttackEnemy(Monster* target, int damage);
    void EnemyKilled(Monster* enemy);

    void UnlockPerk(string perk);
    void drawPerkAqquired(sf::RenderTarget* target);

    void Draw(sf::RenderTarget* target);

private:

    float dTime = 0;

    int x_;
    int y_;
    char g_ = 'P';
    Room* room_;
    Inventory* inventory_;
    int gold_ = 100;
    int skillpoints = 1;

    // Dont change these values in functions
    float timeLastMove = 0;        //Used to check if player is allowed to move
    float moveSpeed = 0.25;
    float timeLastAttack = 0;
    float attackSpeed = 1.0;
    float timeLastPickUp = 0;
    float pickUpSpeed = 0.25;
    
    //Player animaatioita ja movementtia ja rajoittavia tekijöitä
    bool playerInputCommandDone = false;
    bool hasPlayerSwappedTiles = false;
    sf::Sprite playerSprite;
    std::array<sf::Texture, 4> playerIdleTextures;
    std::array<sf::Texture, 4> playerAttackTextures;
    std::array<sf::Texture, 4> playerTakeDMGTextures;
    float idleSpriteUpdateSpeed = 0.25;
    float attackSpriteUpdateSpeed = 0.04;
    float takeDMGSpriteUpdateSpeed = 0.005;
    float timeLastSpriteUpdate = 0;
    int idleIndex = 0;
    int attackIndex = 0;
    int takeDMGIndex = 0;
    bool attacking = false;
    bool moving = true;
    bool takingDMG = false;
    int currentMoveDirection = 2;
    array<pair<int, int>, 4> direction;  

    Stats stats_;
    //Vesa: Ehdotan, että tungetaan tähän 5 eri kolmen 'perkin' puuta, missä puun sisällä perkit pitää ottaa järjestyksessä, mutta saa valita mistä puusta ottaa.
    //Tällä hetkellä laiton, että skillpointin saa levelupista, mutta niitä voisi esim saada myös consumableista.
    std::map<std::string, std::pair<string, bool>> Perks_ = {
        //aoe tree
        {"cleave", make_pair("", false) }, //Deals damage to all enemies in 3 enemies in front of player. secondary targets take 50% of damage
        {"smack", make_pair("cleave", false) }, //Smacks enemies around the target instead dealing 50% damage to all 
        {"slam", make_pair("smack", false) }, //Enemies around the target take full damage instead
        //utility tree
        {"cleptomania", make_pair("", false) }, //Steals 5 gold from enemy when attacking
        {"negotiator", make_pair("cleptomania", false) }, //Item shops will give player 25% discount
        {"plutocrat", make_pair("negotiator", false) }, //Every time player gains gold they will also gain the same amount of xp
        //survivalibity tree
        {"shield regen", make_pair("", false)}, //When an enemy dies, restore 10% of max shields
        {"tough skin", make_pair("shield regen", false)}, //Gain 15% for every enemy next to you besides the first (cornering tiles count as well),
            // counted separately from armor bonus. Cant exceed 60%
        {"survivor", make_pair("tough skin", false)}, //If an enemy attack would reduce your shields over 25% of their current value, take only half the damage
        //dps tree
        {"frenzy", make_pair("", false)}, //When an enemy dies, gain 25% attackspeed for 5 seconds
        {"berserk", make_pair("frenzy", false)}, //Gain 50% more damage and 25% movementspeed when shields are down
        {"second chance", make_pair("berserk", false)}, // If player would drop below 0 health, he will respawn once with 50% of max hp and zero shields
    };

    //Tarvii tän jotta mappiin pääsee indexillä käsiksi
    std::array<std::string, 12> perkNames = {"cleave", "smack", "slam",
                                            "cleptomania", "negotiator", "plutocrat", 
                                            "shield regen", "tough skin", "survivor",
                                            "frenzy", "berserk", "second chance"};

    //Tän voi yhistää Perks_mappii, mutta en jaksanu muuttaa kaikkee, huomioi sit updateButtons funktio täytyy muuttaa samalla, muualla ei muutoksia
    std::array<std::string, 12> perkExamineTexts = {
        "Cleave\nDeals damage to all enemies in front of player.\nSecondary targets take 25% of damage.",
        "Smack\nSmacks all enemies around the target dealing 25% damage.",
        "Slam\nEnemies around the target take 50% damage.",
        "Cleptomania\nSteals 5 gold from enemy when attacking.",
        "Negotiator\nItem shops will give player 50% discount.",
        "Plutocrat\nEvery time player gains gold,\nthey will also gain the same amount of xp.", 
        "Shield regen\nWhen an enemy dies, restore 2,5% of missing shields, up to 10",
        "Tough skin\nGain 15% DMG reduction for every enemy next to the player\nbesides the first, including cornering enemies.\nCounted separately from armor bonus. Cant exceed 60%.",
        "Survivor\nIf an attack would reduce shields over 25% of\n their current value, take only half the damage.",
        "Frenzy\nWhen an enemy dies, gain 25% attackspeed for 5 seconds.",
        "Berserk\nGain 50% more damage and 25% movementspeed when shields are down.",
        "Second chance\nIf player would drop below 0 health,\nhe will respawn once with 50% of max hp."
        };

    float frenzyTimer = 0.0;
    bool frenzyActive = false;

    sf::RectangleShape perkTreeBackground;
    std::array<std::array<sf::Texture, 3>, 4> perkTextures;
    sf::Texture arrow;
    int currentPerkTree = 0;
    float timeSincePerkTreeSwap = 0;

    Text* perkExamine;
    Text* perkAqquired;
    bool drawExamineText = false;

    std::array<Button*, 3> perkButtons;
    std::array<Button*, 2> swapPerkTree;

    sf::Vector2i mousePos;

    //APU FUNKTIOITA
    std::string floatTostring(const float, int);    //transfer float number to string with precision int:
    string getBonusString(float bonus);
};