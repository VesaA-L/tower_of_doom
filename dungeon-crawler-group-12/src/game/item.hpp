#pragma once

#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "constants"
#include <map>
#include <vector>

using namespace std;

class Item{
public:

    // USE THE CONSTRUCTOR BELOW, IF POSSIBLE
    Item(int x, int y, std::string name, int equipmentSlot=0, int primaryStatBoost=0, int secondaryStatBoost=0)
            :x_(x),y_(y), name_(name), equipmentSlot_(equipmentSlot), primaryStatBoost_(primaryStatBoost), secondaryStatBoost_(secondaryStatBoost){
        if(!texture.loadFromFile("images/items/" + name + ".png")){
            std::cout << "FAILED LOAD " << name << "\n";
        }
        //cout<<name<<endl;
        sprite.setTexture(texture);
        sprite.setScale(0.7, 0.7);
        //Muutin vaan itemin hieman pienemmäksi ja asetan sen keskelle tileä
        sprite.setPosition(x_ * TILESIZE + TILESIZE/2 - sprite.getGlobalBounds().width/2, y_ * TILESIZE + TILESIZE/2 - sprite.getGlobalBounds().height/2);
    }

    vector<string> GetItemNames() {
        vector<string> keys;
        for(std::map<string, pair<vector<int>, string>>::iterator it = itemsByName.begin(); it != itemsByName.end(); ++it) {
            keys.push_back(it->first);
        }
        return keys;
    }


    Item(string name, int x=0, int y=0){
        vector<string> v = GetItemNames();
        if(std::find(v.begin(), v.end(), name) != v.end()) {
            // Generic stuff
            vector<int> stats = itemsByName[name].first;
            x_=x; y_=y; name_=name;
            // Stat stuff
            equipmentSlot_ = stats[0];
            primaryStatBoost_ = stats[1];
            secondaryStatBoost_ = stats[2];
            weight = stats[3];
            

            //Gui stuff¨
            examineText_ = itemsByName[name].second;
            if(!texture.loadFromFile("images/items/" + name + ".png")){
                std::cout << "FAILED LOAD " << name << "\n";
            }
            sprite.setTexture(texture);
            sprite.setScale(0.7, 0.7);
            sprite.setPosition(x_ * TILESIZE + TILESIZE/2 - sprite.getGlobalBounds().width/2, y_ * TILESIZE + TILESIZE/2 - sprite.getGlobalBounds().height/2);
        }
        else {
            //cout<<"name in" << name<<endl;
            Item(x, y, name);
        }
    }

    // Needs to be called when dropping an item to a room
    void setXY(int x, int y){
        x_ = x;
        y_ = y;
        //Taas asetetaan vaan keskelle tileä
        sprite.setPosition(x_ * TILESIZE + TILESIZE/2 - sprite.getGlobalBounds().width/2, y_ * TILESIZE + TILESIZE/2 - sprite.getGlobalBounds().height/2);
    }

    void Draw(sf::RenderTarget* target){
        target->draw(this->sprite);
    }

    std::string GetName(){
        return name_;
    }

    int X(){return x_;}

    int Y(){return y_;}

    int getPrice(){
        return price_;
    }
    int setPrice(int i){
        return price_ = i;
    }

    int getEquipmentSlot(){ return equipmentSlot_; }

    int GetPrimaryStat(){ return primaryStatBoost_; }

    int GetSecondaryStat(){ return secondaryStatBoost_; }

    std::string getExamine(){ return examineText_; }

    int GetWeight(){ return weight; }
//---------------------------------------------------------------------------------------------------------//

private:
    int x_;
    int y_;
    //EquipmentSlot -1=consumable, 0=Headslot, 1=cape, 2=necklace, 3=ammo, 4=weapon, 5=torso, 6=shield, 7=legs, 8=glove, 9=boots, 10 =ring, -1 for all else
    int equipmentSlot_;
    int weight = 0;
    int price_ = 0;

    //What these will do are defined in the inventory slot. Most of the time secondaryStatBoost = 0;
    int primaryStatBoost_; // Primary/secondary stats are : 0 Headslot armor/critChance, 1 cape critchange/weight-reduction, 2 necklace crit change/bonus xp, 3 ammo damage/range, 4 weapon damage/attackspeed,
    int secondaryStatBoost_;// 5 torso armor, 6 shield armor/damage, 7 legs armor/movementspeed, 8 glove armor/attackspeed, 9 boots movementspeed/armor, 10 ring bonus xp/ bonus gold multiplier

    std::string name_;
    std::string examineText_ = ""; //Change Examine text accordinly to each item, dynaaminen koko, background muuttuu textin mukaan

    sf::Texture texture;
    sf::Sprite sprite;


    map<string, pair<vector<int>, string>> itemsByName = map<string, pair<vector<int>, string>>{
        //Format for the int vector:
        // itemType, primaryStatBoost, secondaryStatBoost, weight

        //Item types are: -1=consumable, 0=Headslot, 1=cape, 2=necklace, 3=ammo, 4=weapon, 5=torso, 6=shield, 7=legs, 8=glove, 9=boots, 10 =ring
        // Item count

        //Helmets
        {"metal_helmet",  make_pair(vector<int>{0, 15, 0, 8}, "Having your head protected makes it easier to not get hurt")},
        {"nice_helmet",  make_pair(vector<int>{0, 20, 5, 5}, "An ornate helmet with excellent craftsmanship")},

        //Necklaces
        {"blue_amulet",  make_pair(vector<int>{2, 3, 3, 1}, "A nice piece of jewellery")},
        {"luck_amulet",  make_pair(vector<int>{2, 10, 0, 1}, "Maybe this necklace will bring me luck")},
        {"feather_amulet",  make_pair(vector<int>{2, 0, 10, 1}, "A amulet made from the feather of a speedy bird")},

        //Weapons
        {"w_shortsword_holy",  make_pair(vector<int>{4, 1, 0, 5}, "A cute little dagger")},
        {"axe", make_pair(vector<int>{4, 5, 0, 10}, "Sturdy tool that can be used as a weapon")},
        {"staff_1",  make_pair(vector<int>{4, 5, 0, 15}, "An old staff, maybe I can hit someone with it")},
        {"w_longsword_copper",  make_pair(vector<int>{4, 7, -10, 15}, "Looks old and rusty, but its better than nothing")},
        {"scithersword",  make_pair(vector<int>{4, 4, 22, 5}, "Fast attackspeed, low damage")},
        {"w_longsword_dark",  make_pair(vector<int>{4, 15, -10, 15}, "A longword that radiates magical energy")},
        {"w_axe_war_mithril",  make_pair(vector<int>{4, 20, 0, 10}, "An anciet axe, this looks deadly and is suprisingly light")},

        //Torso armour
        {"leather_armour",  make_pair(vector<int>{5, 30, 0, 20}, "A basic leather torso")},
        {"heavy_armor",  make_pair(vector<int>{5, 50, 0, 40}, "A heavy steel chestpiece")},
        {"nice_armour",  make_pair(vector<int>{5, 75, 0, 30}, "A fabulous piece of equipment")},

        //Shields
        {"steel_shield",  make_pair(vector<int>{6, 30, 3, 20}, "A heavy steel shield")},
        {"wood_shield",  make_pair(vector<int>{6, 20, 0, 7}, "A wooden shield, is this a childs toy?")},
        {"gilded_shield",  make_pair(vector<int>{6, 50, 1, 40}, "A shield fit for a king")},

        //Leg armour
        {"steel_legs",  make_pair(vector<int>{7, 20, -3, 20}, "Heavy steel leg protection")},
        {"nice_legs",  make_pair(vector<int>{7, 30, 3, 20}, "Beatiful light weight leg armour")},
        
        //Gloves
        {"basic_gloves",  make_pair(vector<int>{8, 2, 5, 2}, "Handy leather gloves")},
        {"steel_gloves",  make_pair(vector<int>{8, 10, 0, 5}, "WOW! These look sturdy")},

        //Boots
        {"boots_of_speed",  make_pair(vector<int>{9, 3, 0, 1}, "Lightweight and nimble shoes")},
        {"steel_boots",  make_pair(vector<int>{9, 0, 5, 8}, "A pair of heavy steel boots")},
        {"spiked_boots",  make_pair(vector<int>{9, -2, 15, 5}, "I sure hope noone kicks me with these...")},

        //Rings
        {"silver_ring",  make_pair(vector<int>{10, 1, 1, 1}, "A nice piece of silver jewellery")},
        {"snake_ring",  make_pair(vector<int>{10, 0, 10, 1}, "A ring with a snake coiling around it")},
        {"dragon_ring",  make_pair(vector<int>{10, 10, 0, 1}, "You feel it emit some sort of greate power you cannot begin to understand")},

        //Consumables
        {"apple", make_pair(vector<int>{-1, 15, 0, 1}, "Looks tasty!")},
        {"cyan_old", make_pair(vector<int>{-1, 0, 1, 1}, "Magical liquid")},
        {"repair_kit", make_pair(vector<int>{-1, -30, 0, 1}, "Maybe I can repair my armour with this")},

        //Keys // KÄYTÄ VILLE TOTA -2 IDENTIFIOIMAAN AVAINTA JOSTAIN MUUSTA ITEMISTÄ, avaimet voi sit keskenään erotella noilla nollilla jos tarvii, mutta noi stackaa inventorissa jos nimi on sama
        {"key",  make_pair(vector<int>{-2, 0, 0, 0}, "This will probably open something...")},
        
    };


};