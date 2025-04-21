#include "ally.hpp"

 Ally::Ally(int x, int y, int level): x_(x), y_(y){
    // If multiple textures are used, do textures vector and use textures[level-1] as parameter for 'loadFromFile' function
    if(!texture.loadFromFile("images/LesanGuard.png")){ 
        cout<<"Couldn't download file images/LesanGuard.png"<<endl;
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0,0,32,32));
    // One npc per floor, level defines the npc
    dialogue_options = dialogue_options_by_level[level-1];
    available_options = available_options_by_level[level-1];
    buystate = buystate_by_level[level-1];
    item_details = items_prices_stock_by_level[level-1];
    // Set up the shop
    vector<string> keys;
    for(std::map<string, pair<int, int>>::iterator it = item_details.begin(); it != item_details.end(); ++it) {
        keys.push_back(it->first);
    }
    items_ = keys;

 }

string Ally::GetShop(){
    string start = "\"Do you want to buy any of these?\"\n";
    string shop = "";
    for(int i = 0; i < items_.size(); i++){
        string temp = to_string(1+i) + ". " + items_[i] + ": ";
        if(item_details[items_[i]].first == 0){
            temp += "FREE";
        } else {
            temp += to_string(item_details[items_[i]].first)+ "coins.";
        }
        temp += + " Stock: " + to_string(item_details[items_[i]].second) + "\n";
        shop += temp;
    }
    if(buystate['Y']){
        shop += "Press item number of the item you wish to buy or 'n' if none.";
    } else {
        shop += "Press 'y' if you think you are good to go.";
    }
    return start + shop;
}


void Ally::Talk(char option){
    if(buystate[option] && option != 'T'){
        dialogue = GetShop();
        lastDialogue = option;
    } else if(find(available_options[lastDialogue].begin(), available_options[lastDialogue].end(), option) != available_options[lastDialogue].end()){
        dialogue = dialogue_options[option];
        lastDialogue = option;
    }
}

Item* Ally::Buy(int index, int player_gold){
    if( !buystate[lastDialogue] ){
        return nullptr;
    } else {
        if(buystate[lastDialogue]){
            lastDialogue = 'a'; // This sets the dialogue to the start
        } else {
            lastDialogue = 'T'; // This sets the dialogue to the start          
        }

    }
    if(index>items_.size()){
        dialogue="\"No such item!\"";
        return nullptr;
    }
    if(index>items_.size() || item_details[items_[index-1]].second == 0){
        dialogue="\"Out of stock!\"";
        return nullptr;
    }
    if( item_details[items_[index-1]].first <= player_gold){
        if(buystate['Y']){
            dialogue = "\"Use it well, friend!\nDo you need something else?\"";
        } else if (buystate['T']){
            dialogue = "\"Now let's get out of here!\"";
        } else {
            dialogue = "\"That should do it!\nAre you finished?\"";
        }
        item_details[items_[index-1]] = make_pair(item_details[items_[index-1]].first, item_details[items_[index-1]].second - 1);
        Item * soldItem = new Item(items_[index-1]);
        soldItem->setPrice(item_details[items_[index-1]].first);
        return soldItem;
    }
    if(buystate['Y']){
        dialogue = "\"You dont have enough money!\nDo you wish to continue shopping?\"";
    } else {
        dialogue = "\"You dont have enough money!\nAre you done shopping?\"";
    }
    return nullptr;
}

void Ally::NoInventory(){
    dialogue = "\"You have no room for that item!\"";
}

string Ally::GetDialogue(){
    return dialogue;
}