#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <vector>
#include "constants"

class Tile{
public:
    Tile(float x, float y, int type){
        type_ =type;
        hp_ = 16;
        
    }
    Tile(){
        type_ =0;
    }
    void SetType(int type){
        type_ =type;
    }
    int GetType(){
        return type_;
    }
    int GetHP(){
        return type_;
    }
    bool Damage(int dmg){
        if(dmg>0){hp_-=dmg;}
        if(hp_>0){return false;}
        else{hp_=16;return true;}
    }


private:
    int type_;
    int hp_;
};
