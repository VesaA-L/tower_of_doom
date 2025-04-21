#pragma once
#include "room.hpp"
#include <utility>
#include <vector>
#include "SFML/Window.hpp"

using namespace std;
//TODO: The class Monster should be purely virtual class.
class Item;
class Room;
class Monster{
public:
    Monster(int hitpoints, int damage, int xpreward, int goldReward, int initx, int inity, int movementspeed, int attackspeed, int attackrange = 1);

    ~Monster();

    int getX(){return x_;}

    int getY(){return y_;}

    //Call this function at every game tick
    int TakeTurn(int player_x, int player_y);

    //Monster moves in the map
    void Move(int player_x, int player_y);

    //If we want to randomize the damage, we can do it either here or in TakeDamage functions!
    int Attack(){return damage_;}

    //Returns whether the monster is alive. This can be probably be done somewhere else.
    bool TakeDamage(int damage);

    int XPReward(){return xp_given_;}

    void ItemReward();

    int goldReward(){return gold_reward_;}

    void SetRoom(Room* room){room_=room;}

    void updateTime(float deltaTime){dTime = deltaTime;}

    // It seems we have done positions differently so this might be useless
    std::pair<int, int> getPosition(){ return make_pair(x_, y_); }

    virtual void Draw(sf::RenderTarget* target){        //KRISTIAN MUUTIN VIRTUAALISEKS NIIN VOI TEHÄ ERIKSEEN ANIMOITUJA MONSTEREITA
        sprite.setPosition(x_*TILESIZE,y_*TILESIZE);
        target->draw(this->sprite);
    }

private:

    //Attributes common to all different monsters
    bool aggressive_=false;
    int vision_radius=4;
    int attackrange_;
    int max_hitpoints_;
    int current_hitpoints_;
    int damage_;
    int xp_given_;
    int gold_reward_;
    int movementspeed_;
    int attackspeed_;
    Room* room_;
    int attack_state = 0;
    int movement_state = 0;

protected:
    //KRISTIAN ANIMATION VARIABLES
    float idleSpriteUpdateSpeed = 0.25;
    float attackSpriteUpdateSpeed = 0.02;
    float takeDMGSpriteUpdateSpeed = 0.01;
    float timeLastSpriteUpdate;
    int idleIndex;
    int attackIndex = 0;
    int takeDMGIndex = 0;
    bool hasMoved = false;
    bool attacking = false;
    bool moving = true;
    bool takingDMG = false;
    int currentMoveDirection = 2;
    //KRISTIAN ANIMATION ENDS
    int x_;
    int y_;
    float dTime;
    int chanceForNoDrop = 0;
    sf::Texture texture;
    std::array<std::array<sf::Texture, 4>, 3> textureMatrix;  //only for animated monsters, otherwise use texture 
    sf::Sprite sprite;
    std::vector<string> items_;
};

class Room;

class Rat : public Monster {
public:

    Rat(int level, int x, int y) : Monster( level * 3 + 3, 2 + level , 20 + level * 10, 2, x, y, 10 + rand()%4 - 2, 5) {
        if(!texture.loadFromFile("images/rat.png")){
            cout<<"Could not load the file!"<<endl;
        }
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0,96,32,128));
        chanceForNoDrop = 84;
        items_ = {"apple"};
        if(rand() % 100 > 85){
            items_.push_back("cyan_old");
        }
    }

};

class Spider : public Monster {
public:

    Spider(int level, int x, int y) : Monster( level * 4 + 7, 4 + level , 30 + level * 10, 5, x, y, 10 + rand()%4 - 2, 5) {
        if(!texture.loadFromFile("images/BlackWidow.png")){
            cout<<"Could not load the file!"<<endl;
        }
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0,0,32,32));
        chanceForNoDrop = 110-15*level;
        items_ = {"apple"};
        if(rand() % 10 > 8){
            items_.push_back("cyan_old");
        }
    }

};

class Goblin : public Monster {            //Kristian animoitu monster

public:

    Goblin(int level, int x, int y) : Monster( level * 7 + 7, 6 + level*4 , 100 + level * 50, 30, x, y, 10 + rand()%4 - 2, 20 + rand()%4 - 2, 3) {
        
        std::array<std::string, 4> directions = {"Up", "Right", "Down", "Left"};
        std::array<std::string, 3> fileNames = {"Idle", "Attack01", "Hurt"};

        idleIndex = rand()%5;
        timeLastSpriteUpdate = 1;

        int a = 0;
        int b = 0;

        for(auto i : fileNames){
            for(auto j : directions){
                if(!textureMatrix[b][a].loadFromFile("images/goblinSprites/GoblinSlinger" + j + i + ".png")){
                    std::cout << "ERROR GOBLIN FILE LOAD" << "\n";
                }
                a += 1;
            }
            a = 0;
            b += 1;
        }
        // Drops for all goblins
        items_ = {"apple"};
        // Drops for different level goblins
        chanceForNoDrop = 60-level*30;
        vector<vector<string>> dropsByLevel = vector<vector<string>>{
            vector<string>{"w_longsword_copper", "apple"},
            vector<string>{"scithersword", "repair_kit"},
            vector<string>{"spiked_boots", "apple", "repair_kit"}
        };
        for(auto item : dropsByLevel[level-1]){
            items_.push_back(item);
        }
        sprite.setTexture(textureMatrix[0][currentMoveDirection]);
        sprite.setTextureRect(sf::IntRect(0,0,48,48));
    }

    void UpdateSprite(){

        timeLastSpriteUpdate = timeLastSpriteUpdate + dTime;
        
        if(hasMoved && !takingDMG){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[0][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(idleIndex * 48, 0, 48, 48));
            if(idleIndex == 5){
                idleIndex = 0;
            }else{idleIndex += 1;}
        }else if(moving && !takingDMG && timeLastSpriteUpdate > idleSpriteUpdateSpeed){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[0][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(idleIndex * 48, 0, 48, 48));
            if(idleIndex == 5){
                idleIndex = 0;
            }else{idleIndex += 1;}
        }else if(attacking && !takingDMG && timeLastSpriteUpdate > attackSpriteUpdateSpeed){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[1][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(attackIndex * 48, 0, 48, 48));
            if(attackIndex == 20){
                attackIndex = 0;
                attacking = false;
                moving = true;
            }else{attackIndex += 1;}
        }else if(takingDMG && timeLastSpriteUpdate > takeDMGSpriteUpdateSpeed){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[2][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(takeDMGIndex * 48, 0, 48, 48));
            if(takeDMGIndex == 3){
                takeDMGIndex = 0;
                takingDMG = false;
                moving = true;
            }else{takeDMGIndex += 1;}
        }

    }

    void Draw(sf::RenderTarget* target){      
        UpdateSprite();  
        sprite.setPosition(x_ * TILESIZE - 7, y_ * TILESIZE - 9);
        target->draw(this->sprite);
        hasMoved = false;
    }


};

class Possesed : public Monster {            //Kristian animoitu monster

public:

    Possesed(int level, int x, int y) : Monster( level * 30 + 10, 15 + level * 5 , 400 + level * 400, 100, x, y, 10 + rand()%2 - 1, 20 + rand()%4 - 2, 1) {
        
        std::array<std::string, 4> directions = {"Up", "Right", "Down", "Left"};
        std::array<std::string, 3> fileNames = {"Idle", "Attack01", "Hurt"};

        idleIndex = rand()%5;
        timeLastSpriteUpdate = 1;

        int a = 0;
        int b = 0;

        for(auto i : fileNames){
            for(auto j : directions){
                if(!textureMatrix[b][a].loadFromFile("images/possesedSprites/Possesed" + j + i + ".png")){
                    std::cout << "ERROR POSESSED FILE LOAD" << "\n";
                }
                a += 1;
            }
            a = 0;
            b += 1;
        }
        // Only posessed should drop keys, amd they should always drop one
        items_ = {"key"};
        chanceForNoDrop = -20 + 20 * level;
        vector<vector<string>> dropsByLevel = vector<vector<string>>{
            vector<string>{"nice_helmet", "nice_legs", "scithersword"},
            vector<string>{"gilded_shield", "nice_armour","cyan_old"},
            vector<string>{"w_longsword_dark","w_axe_war_mithril"}
        };
        for(auto item : dropsByLevel[level-1]){
            items_.push_back(item);
        }

        sprite.setTexture(textureMatrix[0][currentMoveDirection]);
        sprite.setTextureRect(sf::IntRect(0,0,48,48));
    }

    void UpdateSprite(){

        timeLastSpriteUpdate = timeLastSpriteUpdate + dTime;
        
        if(hasMoved && !takingDMG){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[0][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(idleIndex * 48, 0, 48, 48));
            if(idleIndex == 4){
                idleIndex = 0;
            }else{idleIndex += 1;}
        }else if(moving && !takingDMG && timeLastSpriteUpdate > idleSpriteUpdateSpeed){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[0][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(idleIndex * 48, 0, 48, 48));
            if(idleIndex == 4){
                idleIndex = 0;
            }else{idleIndex += 1;}
        }else if(attacking && !takingDMG && timeLastSpriteUpdate > attackSpriteUpdateSpeed * 2){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[1][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(attackIndex * 48, 0, 48, 48));
            if(attackIndex == 10){
                attackIndex = 0;
                attacking = false;
                moving = true;
            }else{attackIndex += 1;}
        }else if(takingDMG && timeLastSpriteUpdate > takeDMGSpriteUpdateSpeed){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[2][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(takeDMGIndex * 48, 0, 48, 48));
            if(takeDMGIndex == 3){
                takeDMGIndex = 0;
                takingDMG = false;
                moving = true;
            }else{takeDMGIndex += 1;}
        }

    }

    void Draw(sf::RenderTarget* target){      
        UpdateSprite();  
        sprite.setPosition(x_ * TILESIZE - 7, y_ * TILESIZE - 9);
        target->draw(this->sprite);
        hasMoved = false;
    }


};

class SkeletonAnime : public Monster {            //Kristian animoitu monster

public:

    SkeletonAnime(int level, int x, int y) : Monster( level * 15 + 20, 6 + 6 * level , 150 + level * 50, 20, x, y, 10 + rand()%2 - 1, 20 + rand()%4 - 2, 1) {
        
        std::array<std::string, 4> directions = {"Up", "Right", "Down", "Left"};
        std::array<std::string, 3> fileNames = {"Idle", "Attack01", "Hurt"};

        idleIndex = rand()%5;
        timeLastSpriteUpdate = 1;

        int a = 0;
        int b = 0;

        for(auto i : fileNames){
            for(auto j : directions){
                if(!textureMatrix[b][a].loadFromFile("images/skeletonSprites/SkeletonWithSword" + j + i + ".png")){
                    std::cout << "ERROR SKELETONANIME FILE LOAD" << "\n";
                }
                a += 1;
            }
            a = 0;
            b += 1;
        }
        // Drops for all goblins
        items_ = {"repair_kit", "apple"};
        // Drops for different level goblins
        chanceForNoDrop = 60-level*15;
        vector<vector<string>> dropsByLevel = vector<vector<string>>{
            vector<string>{"wood_shield", "steel_boots", "basic_gloves"},
            vector<string>{"metal_helmet", "heavy_armor","steel_shield","steel_boots"},
            vector<string>{"w_longsword_dark","repair_kit", "apple"}
        };
        for(auto item : dropsByLevel[level-1]){
            items_.push_back(item);
        }
        sprite.setTexture(textureMatrix[0][currentMoveDirection]);
        sprite.setTextureRect(sf::IntRect(0,0,48,48));
    }

    void UpdateSprite(){

        timeLastSpriteUpdate = timeLastSpriteUpdate + dTime;
        
        if(hasMoved && !takingDMG){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[0][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(idleIndex * 48, 0, 48, 48));
            if(idleIndex == 5){
                idleIndex = 0;
            }else{idleIndex += 1;}
        }else if(moving && !takingDMG && timeLastSpriteUpdate > idleSpriteUpdateSpeed){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[0][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(idleIndex * 48, 0, 48, 48));
            if(idleIndex == 5){
                idleIndex = 0;
            }else{idleIndex += 1;}
        }else if(attacking && !takingDMG && timeLastSpriteUpdate > attackSpriteUpdateSpeed * 2){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[1][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(attackIndex * 48, 0, 48, 48));
            if(attackIndex == 7){
                attackIndex = 0;
                attacking = false;
                moving = true;
            }else{attackIndex += 1;}
        }else if(takingDMG && timeLastSpriteUpdate > takeDMGSpriteUpdateSpeed){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[2][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(takeDMGIndex * 48, 0, 48, 48));
            if(takeDMGIndex == 3){
                takeDMGIndex = 0;
                takingDMG = false;
                moving = true;
            }else{takeDMGIndex += 1;}
        }

    }

    void Draw(sf::RenderTarget* target){      
        UpdateSprite();  
        sprite.setPosition(x_ * TILESIZE - 7, y_ * TILESIZE - 9);
        target->draw(this->sprite);
        hasMoved = false;
    }


};

class Boss : public Monster {            //Kristian animoitu monster

public:

    Boss(int level, int x, int y) : Monster( 600, 120 , 0, 0, x, y, 4, 12, 1) {
        
        std::array<std::string, 4> directions = {"Up", "Right", "Down", "Left"};
        std::array<std::string, 3> fileNames = {"Idle", "Attack01", "Hurt"};

        idleIndex = rand()%5;
        timeLastSpriteUpdate = 1;

        int a = 0;
        int b = 0;

        for(auto i : fileNames){
            for(auto j : directions){
                if(!textureMatrix[b][a].loadFromFile("images/possesedSprites/Possesed" + j + i + ".png")){
                    std::cout << "ERROR POSESSED FILE LOAD" << "\n";
                }
                a += 1;
            }
            a = 0;
            b += 1;
        }
        items_ = {"key"};

        sprite.setTexture(textureMatrix[0][currentMoveDirection]);
        sprite.setTextureRect(sf::IntRect(0,0,48,48));
        sprite.setScale(1.5, 1.5);
    }

    void UpdateSprite(){

        timeLastSpriteUpdate = timeLastSpriteUpdate + dTime;
        
        if(hasMoved && !takingDMG){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[0][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(idleIndex * 48, 0, 48, 48));
            if(idleIndex == 4){
                idleIndex = 0;
            }else{idleIndex += 1;}
        }else if(moving && !takingDMG && timeLastSpriteUpdate > idleSpriteUpdateSpeed){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[0][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(idleIndex * 48, 0, 48, 48));
            if(idleIndex == 4){
                idleIndex = 0;
            }else{idleIndex += 1;}
        }else if(attacking && !takingDMG && timeLastSpriteUpdate > attackSpriteUpdateSpeed * 2){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[1][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(attackIndex * 48, 0, 48, 48));
            if(attackIndex == 10){
                attackIndex = 0;
                attacking = false;
                moving = true;
            }else{attackIndex += 1;}
        }else if(takingDMG && timeLastSpriteUpdate > takeDMGSpriteUpdateSpeed){
            timeLastSpriteUpdate = 0;
            sprite.setTexture(textureMatrix[2][currentMoveDirection]);
            sprite.setTextureRect(sf::IntRect(takeDMGIndex * 48, 0, 48, 48));
            if(takeDMGIndex == 3){
                takeDMGIndex = 0;
                takingDMG = false;
                moving = true;
            }else{takeDMGIndex += 1;}
        }

    }

    void Draw(sf::RenderTarget* target){      
        UpdateSprite();  
        sprite.setPosition(x_ * TILESIZE - 7 - 12, y_ * TILESIZE - 9 - 16);
        target->draw(this->sprite);
        hasMoved = false;
    }


};


