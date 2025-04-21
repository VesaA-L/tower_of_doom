#pragma once

#include <vector>
#include <time.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include "item.hpp"
#include "tile.hpp"
#include "fstream"
#include "monster.hpp"
#include "ally.hpp"
#include <stdlib.h> //RANDOM

using namespace std;

class Room : public sf::Drawable, public sf::Transformable{
public:
    Room(int szx, int szy):szx_(szx), szy_(szy){
        dialogueText = new Text(0, 0, "", "fonts/Montserrat-Regular.ttf", sf::Color::Red, sf::Color::Yellow, 15, 1, true);
        room_.resize(szy_,vector<Tile>(szx_)); //CHARIN TILALLE TILE OBJEKTI
       int col = 0;
        int row = 0;
        while(row < szy_){
            while(col < szx_){
                room_[row][col]=Tile((float)col,(float)row,0);
                col++;
            }
            row++;
            col = 0;
        }
        tilechange_ = true;
    };
    //KANNATTAA KÄYTTÄÄ TÄTÄ
    Room():szx_(ROOM_WIDTH), szy_(ROOM_HEIGHT){
        dialogueText = new Text(0, 0, "", "fonts/Montserrat-Regular.ttf", sf::Color::Red, sf::Color::Yellow, 15, 1, true);
        room_.resize(szy_,vector<Tile>(szx_)); //CHARIN TILALLE TILE OBJEKTI
        int col = 0;
        int row = 0;
        while(row < szy_){
            while(col < szx_){
                room_[row][col]=Tile((float)col,(float)row,0);
                col++;
            }
            row++;
            col = 0;
        }
        tilechange_ = true;
    };

    ~Room(){
        for(auto i : items_){
            delete i;
        }
        for(auto i : enemies_){
            delete i;
        }
        for(auto i : allies_){
            delete i;
        }
        delete dialogueText;
    }
    bool OpenTile(int x, int y);

    void SetLevel(int);

    void LoadRoom(string filename);     //LATAA HUONEEN FILUSTA

    void RandGen(int x, int y);     // TÄMÄ SATUNNAISGENEROI MAPPEJA; ÄLÄ KÄYTÄ

    void SetTile(int x, int y, char tile);     // 0 seinä, 1 vapaa
    void BreakTile(int x, int y, int dmg);     // LAittaa damagea viereisiin tileihin

    
    int Get(int x, int y);          // Palauttaa 0, jos ulkopuolella, myöhemmin palauttaa seinän?

    bool Free(int x, int y);     // Ei kerro tiletyyppiä, vaan voiko liikkua siihe, 
   
    int updateRoom(int player_x, int player_y);

    int OutOfBounds(int x, int y);  //Onko koordinaatti ulkona, käytetään huoneen vaihtoon
    
    //Piirtää koko huoneen kaikki tilet

    void RemoveMonster(Monster * monster);

    Ally * GetNPC(){return currentTalkNPC;}

    Item* BuyItem(int index, int player_gold); //returns the cost of item, if purchase was successfull, otherwise return -1

    void DrawEnemies(sf::RenderTarget* target);

    vector<Monster*> GetEnemies(){return enemies_;}

    void DrawAllies(sf::RenderTarget* target);
    //void AddEnemy(Enemy enemy){enemies_.push_back(enemy);}
    void Talk(char option, int x, int y);

    vector<Item*> GetItems(){return items_;}

    void setItems(std::vector<Item*> items){
        items_ = items;
    }

    void AddItem(Item* item){
        items_.push_back(item);
    }

    void DrawItems(sf::RenderTarget* target){
        for(auto i : items_){
            i->Draw(target);
        }
    }
    void CreateVertexArray(array<int, 320>);
    void setRooms();
    array<int, 320> GetInputs();

    //Vesa lisäätarvittavat funktiot allyyn että on jotain piirettävää
    void drawDialogue(sf::RenderTarget* target);

    void StopTalking(){ talkingToNPC = false; }

    void updateTime(float deltaTime){
        dTime = deltaTime;
        }

private:
    int szx_;
    int szy_;
    float dTime;
    bool tilechange_ = true; //FLAGI sille onko tilet muuttuneet, EI KÄYTETÄ MISSÄÄN
    vector<vector<Tile>> room_; //2D vektori
    vector<Monster*> enemies_;
    vector<Ally*> allies_;
    int level_=1;

    Text* dialogueText;

    std::vector<string> common_drop={"apple","repair_kit"};
    std::vector<string> rare_drop={"blue_amulet","feather_amulet","silver_ring","snake_ring"};
    std::vector<string> vrare_drop={"luck_amulet","dragon_ring","cyan_old"};
    //Vesa updattaa tarpeen mukaan
    bool talkingToNPC = false;
    Ally* currentTalkNPC = nullptr;

    sf::Texture mapTileTexture;
    sf::VertexArray vertices;   
    array<int, 320> levelInput;

    vector<Item*> items_;

    //Overloadattu draw funktio huoneelle
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{
        states.transform *= getTransform();
        states.texture = &mapTileTexture;
        target.draw(vertices, states);
    }
    // Kaikenlaisia huoneenlaajuisia muuttujia tähän.
};

vector<string> GetItemNames();