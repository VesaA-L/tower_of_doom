#include "monster.hpp"
#include <cstdlib>
#include <algorithm>

Monster::Monster(int hitpoints, int damage, int xpreward, int goldReward, int initx, int inity, int movementspeed, int attackspeed, int attackrange)
    : max_hitpoints_(hitpoints) , damage_(damage), xp_given_(xpreward), gold_reward_(goldReward), x_(initx),y_(inity), movementspeed_(movementspeed),
        attackspeed_(attackspeed), attackrange_(attackrange){
        current_hitpoints_ = max_hitpoints_;
    };

Monster::~Monster(){}

//A helper function to check whether the monster is next to a player
bool nextToPlayer(int x, int y) {
    //return abs(player.x - x) < 2 && abs(player.y - y) <2;
    return false;
}

int Monster::TakeTurn(int player_x, int player_y){

    //First check, if monster can see the player
    if(aggressive_ && 7 < max(abs(player_x-x_), abs(player_y-y_))) {
        aggressive_=false;
    } else if(vision_radius > max(abs(player_x-x_), abs(player_y-y_))) {
        aggressive_=true;
    }
    //Then check, if player is within the attack range of the monster
    if(abs(player_x - x_) <= attackrange_ && abs( player_y - y_) <= attackrange_) {
        if(attack_state % attackspeed_ == 0){
            attack_state=1;
            //ANIMATION
            attacking = true;
            if(player_x < x_){
                currentMoveDirection = 3;
            }else if(player_x > x_){
                currentMoveDirection = 1;
            }else if(player_y > y_){
                currentMoveDirection = 2;
            }else if(player_y < y_){
                currentMoveDirection = 0;
            }
            //ANIMATION ENDS
            return Attack();
        }
        attack_state++;
    }
    else {
        if(movement_state % movementspeed_ == 0){
            Move(player_x, player_y);
        }
        movement_state += 1;
    }
    return 0;
}

void Monster::Move(int player_x, int player_y){
    //ANIMATION
    moving = true;
    //ANIMATION ENDS
    if (aggressive_){
        int xdisp = 0;
        int ydisp = 0;
        if (player_x > x_) {
            currentMoveDirection = 1;   //ANIMATION
            xdisp = 1;
        } else if (player_x < x_){
            currentMoveDirection = 3;   //ANIMATION
           xdisp = -1;
        }
        if (player_y > y_) {
            currentMoveDirection = 2;   //ANIMATION
            ydisp = 1;
        } else if (player_y < y_){
            currentMoveDirection = 0;   //ANIMATION
           ydisp = -1;
        }
        if(room_->Free(x_ + xdisp, y_ + ydisp)){
            hasMoved = true;            //ANIMATION
            x_ += xdisp;
            y_ += ydisp;
        }
        
    }
    //There is 1/3 change for monster to move towards random direction, if it is not aggressive
    // Monsters can be "stucked" in a way that they are too stupid to go around objects
    else if(rand() % 3 == 0) {
        int xdisp = rand()%3-1;
        int ydisp = rand()%3-1;
        if(room_->Free(x_ + xdisp, y_ + ydisp)){
            x_ += xdisp;
            y_ += ydisp;
            //ANIMATION
            hasMoved = true;
            if(xdisp == 1){
                currentMoveDirection == 1;
            }else if(xdisp == -1){
                currentMoveDirection == 3;
            }else if(ydisp == -1){
                currentMoveDirection == 0;
            }else if(xdisp == 1){
                currentMoveDirection == 2;
            }
            //ANIMATION ENDS
        }
    }
}

bool Monster::TakeDamage(int damage){
    //ANIMATION
    if(damage > 0){
        takingDMG = true;
    }
    //ANIMATION ENDS
    current_hitpoints_ -= damage;
    if (current_hitpoints_ <= 0) {
        ItemReward();
        return true;
    }
    return false;
}

void Monster::ItemReward(){
    if(items_[0] == "key"){
        Item* key_drop = new Item("key");
        key_drop->setXY(x_, y_);
        room_->AddItem(key_drop);
        items_.erase(items_.begin());
    }
    if(items_.size() != 0 && rand() % 100 >= chanceForNoDrop){
        std::string name = items_.at(rand() % items_.size());
        Item* drop = new Item(name);
        drop->setXY(x_, y_);
        room_->AddItem(drop);
    }
}

