#pragma once
#include "room.hpp"
#include <map>
#include <bits/stdc++.h>
using namespace std;

class Ally {
public:
    Ally(int x, int y, int level);
    ~Ally(){}

    Item* Buy(int index, int player_gold);

    string GetShop();

    int getX(){return x_;}

    int getY(){return y_;}

    void resetDialogue() {lastDialogue = 'a', dialogue = "";}

    void Talk(char option);

    string GetDialogue();

    void Draw(sf::RenderTarget* target){
        sprite.setPosition(x_*TILESIZE,y_*TILESIZE);
        target->draw(this->sprite);
    }

    void removeItem(Item *item) { }

    void NoInventory();


private:
    int x_;
    int y_;

    string dialogue;
    vector<string> items_;
    char lastDialogue = 'a';
    map<char, vector<char>> available_options;
    map<char, string> dialogue_options;
    map<char, bool> buystate;
    map<string, pair<int, int>> item_details; // name -> (price, stock)
    sf::Texture texture;
    sf::Sprite sprite;

    vector<map<char, vector<char>>> available_options_by_level = vector<map<char, vector<char>>>{
        map<char, vector<char>>{
            {'a', vector<char>{'T'}},
            {'T', vector<char>{'N','Y','T'}},
            {'N', vector<char>{'T'}},
            {'Y', vector<char>{'T', 'Y', 'N'}}
        },
        map<char, vector<char>>{
            {'a', vector<char>{'T'}},
            {'T', vector<char>{'N','Y','T'}},
            {'N', vector<char>{'T', 'Y', 'N'}},
            {'Y', vector<char>{'T'}}
        },
        map<char, vector<char>>{
            {'a', vector<char>{'T'}},
            {'T', vector<char>{'N','Y','T'}},
            {'N', vector<char>{'T'}},
            {'Y', vector<char>{'T', 'Y', 'N'}}
        },
        map<char, vector<char>>{
            {'a', vector<char>{'T'}},
            {'T', vector<char>{'N','Y','T'}},
            {'N', vector<char>{'T', 'Y', 'N'}},
            {'Y', vector<char>{'T'}}
        },
        map<char, vector<char>>{
            {'a', vector<char>{'T'}},
            {'T', vector<char>{'N','Y','T'}},
            {'N', vector<char>{'T'}},
            {'Y', vector<char>{'T', 'Y', 'N'}}
        },
        map<char, vector<char>>{
            {'a', vector<char>{'T'}},
            {'T', vector<char>{'N','Y','T'}},
            {'N', vector<char>{'T'}},
            {'Y', vector<char>{'T', 'Y', 'N'}}
        },
        map<char, vector<char>>{
            {'a', vector<char>{'T'}},
            {'T', vector<char>{'T'}},
        }
    };

    vector<map<char, string >> dialogue_options_by_level = vector<map<char, string >>{
        map<char, string>{
            {'T', "\"Oh no! The evil creatures have taken over the tower and hold our \nprincess captive! You must save us!\nI can give you some supplies if you want.\"\nChoose y for yes and n for no."},
            {'N', "\"Good luck!\""},
            {'Y', ""}
        },
        map<char, string>{
            {'T', "\"Good day, adventurer! Are you ready to fight your way to the top?\"\nChoose y for yes and n for no."},
            {'N', ""},
            {'Y', "\"Well good luck then!\""}
        },        map<char, string>{
            {'T', "\"Hello there! My name is Hermanni! Do you want to check my stores?\"\nChoose y for yes and n for no."},
            {'N', "\"You know I have kids to feed!\""},
            {'Y', "Player should never be able to see this!"}
        },
        map<char, string>{
            {'T', "\"You look like a real warrior. \"\nChoose y for yes and n for no."},
            {'N', ""},
            {'Y', "\"Come back if you change your mind!\""}
        },
        map<char, string>{
            {'T', "\"You are almost there! Do you need anything? I have some supplies to sell.\""},
            {'N', "\"You know I have kids to feed!\""},
            {'Y', ""}
        },
        map<char, string>{
            {'T', "\"I am Gimli and I am the most skilled armorer in around here.\nAre you interested to buy some high quality stuff?\""},
            {'N', "\"Not everyone has a good taste !\""},
            {'Y', ""}
        },
        map<char, string>{
            {'T', "\"Yippii, you saved me! Now do you want to get out of here?\"\nPress 1."},
        }
    };
    vector<map<char, bool>> buystate_by_level = vector<map<char, bool>>{
        map<char, bool>{
            {'a', false},
            {'T', false},
            {'N', false},
            {'Y', true}
        },
        map<char, bool>{
            {'a', false},
            {'T', false},
            {'N', true},
            {'Y', false}
        },
        map<char, bool>{
            {'a', false},
            {'T', false},
            {'N', false},
            {'Y', true}
        },
        map<char, bool>{
            {'a', false},
            {'T', false},
            {'N', true},
            {'Y', false}
        }, 
        map<char, bool>{
            {'a', false},
            {'T', false},
            {'N', false},
            {'Y', true}
        },
        map<char, bool>{
            {'a', false},
            {'T', false},
            {'N', false},
            {'Y', true}
        },
        map<char, bool>{
            {'T', true},
            {'a', false},
        }
    };
    vector<map<string, pair<int, int>>> items_prices_stock_by_level = vector<map<string, pair<int, int>>>{
        map<string, pair<int, int>>{
            {"axe", make_pair(0, 1)},
            {"apple", make_pair(0, 5)}
        },
        map<string, pair<int, int>>{
            {"boots_of_speed", make_pair(250, 1)},
            {"repair_kit", make_pair(50, 5)},

        },
        map<string, pair<int, int>>{
            {"steel_legs", make_pair(500, 2)},
            {"apple", make_pair(50, 5)},
            {"repair_kit", make_pair(100, 3)}
        },
        map<string, pair<int, int>>{
            {"steel_legs", make_pair(350, 1)},
            {"steel_shield", make_pair(500, 1)},
            {"metal_helmet", make_pair(250, 1)}            
        }, 
        map<string, pair<int, int>>{
            {"steel_gloves", make_pair(500, 1)},
            {"cyan_old", make_pair(2000, 1)},
            {"apple", make_pair(150, 10)}
        },
        map<string, pair<int, int>>{
            {"nice_legs", make_pair(1200, 1)},
            {"nice_armour", make_pair(1500, 1)},
            {"nice_helmet", make_pair(900, 1)}
        },
        map<string, pair<int, int>>{
            {"key", make_pair(0, 2)},
        },      
    };
};