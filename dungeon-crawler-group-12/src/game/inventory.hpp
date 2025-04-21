#pragma once
#include "player.hpp"
#include "button.hpp"
#include "constants"
#include "text.hpp"
#include "item.hpp"
#include <map>
#include <vector>
#include <array>
#include <string>
#include <map>
#include <list>
#include <algorithm>

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

class Inventory {

public:
    Inventory();                                     //Alustaa tyhjän inventorin jossa ei mitään itemejä, paitsi nullItem*
    ~Inventory();

    bool pickUpItem(Item*);                             //nostaa/saa itemin, tätä kutsutaan ulkoa päin kun halutaan että pelaaja saa itemin

    pair<Item*, pair<int,int>> updateButtons(sf::Vector2i, float);           //Updattaa buttonit, tätä kutsutaan playeristä, jatkuvasti ja Vector2i on hiiren asema ja float on dTime, määrittää kaikki inventorin sisäiset liikkeet

    //Tähän kaikki tarvittavat funktioit jotta player saa infon equipatusta tavarasta
    int GetTotalWeight();
    int GetMsBonus();

    int GetArmor();

    int GetAsBonus();

    int GetDmgBonus();

    int GetCritChange();

    int GetBonusXPMultiplier();

    int GetBonusGoldMultiplier();

    void drawInventory(sf::RenderTarget*);              //Piirtää inventorin kaikkineen, HUOM! piirtämisessä ei käytetä allItems vektoria, vaan inventoryItems, equipedItems, inventoryItemsCount, inventoryNumbers ja cordinateVector muuttujia, PAIKALLA ON VÄLIÄ
    Item* removeInventoryItem(int index);               //-||- paitsi inventorista, sylkee ulos mitä itemiä on poistamassa

private:
    Item* dropItem(int);                                //droppaa itemin, tätä ei kutsuta ulkoa, saa komennon inventorista dropata 
    pair<int, int> equipItem(int);                      //lisää itemin inventorista equipmenttiin, jos on jo equipattu niin voi vaihtaa jos inventoryAddItem(equipattu item) antaa true
    void unequipItem(int);                              //siirtää itemin equipmentistä inventoriin
    bool inventoryAddItem(Item*);                       //lisää inventoriin itemin
    bool equipmentAddItem(Item* item);                  //lisää equipmenttiin itemin
    Item* removeEquipmentItem(int index);               //poistaa equipmentistä itemin, ottaa sisään button indexin, eli mitä nappia painetaan ja tarkistaa onko equippedItems arrayssa samassa kohtaa nullITEM vai oikea item
    Item* findItem(std::vector<Item*>, std::string);    //apu funktio jolla tarkistetaan onko AllItems vectorissa kyseisen nimistä itemiä
    void createExamineText(int);
    bool drawExamineText = false;

    float updateSpeed = 0.25;                           //Kuinka nopeasti inventorissa voi tehdä muutoksia, max 1 kpl per 0.25
    float timeSinceLastAction = 0;                      //aika viimeisestä muutoksesta

    Item* nullItem = new Item(0, 0, "null_texture");    //nullItem, eli placeHolder itemi inventoryItems ja equipedItems arraylle jossa ei ole "oikeata" itemiä

    std::map<std::string, sf::Texture> textures;                //Kaikki tarvittavat item textuurit piirtämistä varten
    std::vector<Item*> allItems;                                //Kaikki itemit mitä inventory sisältää, pudottaessa palauttaa tästä itemin ja poistaa sen

    std::array<Item*, 10> inventoryItems = {nullItem, nullItem, nullItem, nullItem, nullItem, nullItem, nullItem, nullItem, nullItem, nullItem};    //alustettu inventory, tyhjä 
    std::array<unsigned int, 10> inventoryItemsCount = {0,0,0,0,0,0,0,0,0,0};                       //itemien määrä jokaisessa inventori kohdassa, aluksi tietysti nolla
    std::array<Button*, 10> inventoryButtons;                                                       //inventory napit jotka rekistöröivät hiien painalluksia
    std::array<Text*, 10> inventoryNumbers;                     //etukäteen ladatut Text objectit, itemien määrille ala indexissä
    Text* examineText;

    std::array<Item*, 11> equipedItems = {nullItem, nullItem, nullItem, nullItem, nullItem, nullItem, nullItem, nullItem, nullItem, nullItem, nullItem};    //alustettu equipedItems, thyja
    std::array<sf::Vector2i, 11> cordinateVector = {sf::Vector2i(760, 10), sf::Vector2i(684, 80), sf::Vector2i(760, 80), sf::Vector2i(837, 80), sf::Vector2i(655, 153), sf::Vector2i(760, 153),
                                                    sf::Vector2i(864, 153), sf::Vector2i(760, 229), sf::Vector2i(655, 304), sf::Vector2i(760, 304), sf::Vector2i(864, 304)};    //kordinaatti vektori missä mikäkin equipped item on
    std::array<Button*, 11> equipmentButtons;       //equipped items napit 

    sf::Texture equipmentBackGround;                //tausta kuvat
    sf::Sprite equipmentBackGroundSprite;
    sf::Texture inventoryBackground;
    sf::Sprite inventoryBackgroundSprite;
    
};