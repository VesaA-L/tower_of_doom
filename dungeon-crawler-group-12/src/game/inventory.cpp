#include "inventory.hpp"

Inventory::Inventory(){
    //create buttons
    for(int i = 0; i<2; ++i){
        for(int j = 0; j<5; ++j){
            this->inventoryButtons[i * 5 +j] = new Button (64 * j, ROOM_HEIGHT * TILESIZE + i * 64 + 5, 64, 64, "", 1, sf::Color::Red, sf::Color::Yellow, sf::Color::Green);
        }
    }
    for(int i = 0; i<11; ++i){
        equipmentButtons[i] = new Button(cordinateVector[i].x, cordinateVector[i].y, 64, 64, "", 1, sf::Color::Red, sf::Color::Yellow, sf::Color::Green);
    }

    //Luodaan Text muuttujat joita käytetään numeroiden määrässä
    inventoryNumbers[0] = new Text(100, 100, " ", "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 15, 1, true);
    for(int i = 1; i<9; ++i){
        inventoryNumbers[i] = new Text(100, 100, std::to_string(i+1), "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 15, 1, true);
    }
    inventoryNumbers[9] = new Text(100, 100, "9+", "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 15, 1, true);

    //inventory examine text muuttuja
    examineText = new Text(0, 0, " ", "fonts/Montserrat-Regular.ttf", sf::Color::Black, sf::Color::White, 15, 1, true);

    //Hakee kaikki itemit mitä pelissä on, katso item.hpp
    std::vector<std::string> inventoryItemNames = nullItem->GetItemNames();

    //Luo mapin itemin nimestä käytettävään textureen
    for(auto i : inventoryItemNames){
        sf::Texture placeHolder;
        if(!placeHolder.loadFromFile("images/items/" + i + ".png")){
            std::cout << "FAILED INV ITEM LAOD" << "\n";
        }
        textures.insert(std::pair<std::string, sf::Texture>(i, placeHolder));
    }

    //Luo taustat ja asettaa kohilleen
    if(!inventoryBackground.loadFromFile("images/inventorybackground.png")){
        std::cout << "ERROR LOADING IMAGE" << "\n";
    }
    inventoryBackgroundSprite.setTexture(inventoryBackground);
    inventoryBackgroundSprite.setPosition(0, ROOM_HEIGHT * TILESIZE + 5);

    if(!equipmentBackGround.loadFromFile("images/equipeditems.png")){
        std::cout << "ERROR LOADING TEXTURE" << "\n";
    }
    equipmentBackGroundSprite.setTexture(equipmentBackGround);
    equipmentBackGroundSprite.setPosition(ROOM_WIDTH * TILESIZE + 5, 0);

    //ALOITUS ITEMIT
    inventoryAddItem(new Item("leather_armour"));
    inventoryAddItem(new Item("w_shortsword_holy"));
    inventoryAddItem(new Item("repair_kit"));
    inventoryAddItem(new Item("apple"));
};

Inventory::~Inventory(){
    for(auto i : inventoryNumbers){
        delete i;
    }
    for(auto i : inventoryButtons){
        delete i;
    }
    for(auto i : equipmentButtons){
        delete i;
    }
    for(auto i : allItems){
        delete i;
    }
    delete nullItem;
}

Item* Inventory::removeInventoryItem(int index){

    if((inventoryItems[index]->GetName() == "null_texture" ) && (inventoryItemsCount[index] == 0)){
        return nullItem;
    }else{
        if(inventoryItemsCount[index] > 1){
            inventoryItemsCount[index] = inventoryItemsCount[index] - 1;
            timeSinceLastAction = 0;
            Item* removedItem = findItem(allItems, inventoryItems[index]->GetName());
            allItems.erase(std::remove(allItems.begin(), allItems.end(), removedItem), allItems.end());
            return removedItem;
        }else{
            Item* removedItem = findItem(allItems, inventoryItems[index]->GetName());
            allItems.erase(std::remove(allItems.begin(), allItems.end(), removedItem), allItems.end());
            Item* returnItem = inventoryItems[index];
            inventoryItems[index] = nullItem;
            inventoryItemsCount[index] = inventoryItemsCount[index] - 1;
            timeSinceLastAction = 0;
            return removedItem;
        }
    }
    return nullItem;
}

Item* Inventory::removeEquipmentItem(int index){

    if(equipedItems[index]->GetName() == "null_texture"){
        return nullItem;
    }else{
        Item* removedItem = equipedItems[index];
        equipedItems[index] = nullItem;
        timeSinceLastAction = 0;
        return removedItem;
    }
    return nullItem;
}

bool Inventory::inventoryAddItem(Item* item){

    bool contains = false;
    for(auto i : inventoryItems){
        if(i->GetName() == item->GetName()){
            contains = true;
        }
    }
    int index = 0;
    for(auto i : inventoryItems){
        if(contains){
            if(item->GetName() == i->GetName()){
                inventoryItemsCount[index] = inventoryItemsCount[index] + 1;
                allItems.push_back(item);
                return true;
            }
        }else{
            if(i->GetName() == "null_texture"){
                inventoryItems[index] = item;
                inventoryItemsCount[index] = inventoryItemsCount[index] + 1;
                allItems.push_back(item);
                return true;
            }
        }
        index += 1;
    }
    
    return false;
}

bool Inventory::equipmentAddItem(Item* item){

    int itemType = item->getEquipmentSlot();
    
    if(equipedItems[itemType]->GetName() == "null_texture"){
        equipedItems[itemType] = item;
        timeSinceLastAction = 0;
        return true;
    }
    return false;
}

bool Inventory::pickUpItem(Item* item){
    return inventoryAddItem(item);
}

Item* Inventory::dropItem(int index){ 
    return removeInventoryItem(index);
}

pair<int, int> Inventory::equipItem(int index){

    Item* item = inventoryItems[index];

    if(item->GetName() != "null_texture" && item->getEquipmentSlot() > -1){
        int slot = item->getEquipmentSlot();
        if(equipedItems[slot]->GetName() == "null_texture"){
            removeInventoryItem(index);
            equipmentAddItem(item);
        }else if(inventoryAddItem(equipedItems[slot])){
            removeEquipmentItem(slot);
            equipmentAddItem(item);
            removeInventoryItem(index);
        }
    } else if(item->GetName() != "null_texture" && item->getEquipmentSlot() == -1){
        pair<int, int> statsGiven = make_pair(item->GetPrimaryStat(), item->GetSecondaryStat());
        removeInventoryItem(index);
        return statsGiven;
    }
    else if(item->GetName() != "null_texture" && item->getEquipmentSlot() == -2){
        return make_pair(-5,index);
    }
    return make_pair(0, 0);
}

void Inventory::unequipItem(int index){

    Item* item = equipedItems[index];

    if(item->GetName() != "null_texture"){
        if(inventoryAddItem(item)){
            removeEquipmentItem(index);
        }
    }
}

void Inventory::createExamineText(int index){

    if(inventoryItems[index]->GetName() != "null_texture"){
        examineText->setText(inventoryItems[index]->getExamine());
        examineText->setBackGround();
        if(index<5){
            examineText->setCords(65 + 64 * index, ROOM_HEIGHT * TILESIZE + 5 + 20 - examineText->returnText()->getGlobalBounds().height/2);
        }else{
            examineText->setCords(65 + 64 * (index-5), ROOM_HEIGHT * TILESIZE + 64 + 5 + 20);
        }
        drawExamineText = true;
    }
}

//Vesa: player class needs to know about consumed items stats, so this function needs to unfortunately to return those values
pair<Item*, pair<int,int>> Inventory::updateButtons(sf::Vector2i mousePos, float dTime){

    timeSinceLastAction += dTime;

    for(int i = 0; i<10; ++i){
        inventoryButtons[i]->updateButton(mousePos);
    }

    for(int i = 0; i<10; ++i){
        if(inventoryButtons[i]->hover){
            createExamineText(i);
        }
    }

    for(int i = 0; i<10; ++i){
        if(inventoryButtons[i]->rightActivated && timeSinceLastAction > updateSpeed){
            return make_pair(dropItem(i), make_pair(0, 0));
        }
    }
    
    for(int i = 0; i<10; ++i){
        if(inventoryButtons[i]->activated && timeSinceLastAction > updateSpeed){
            return make_pair(nullItem, equipItem(i));
        }
    }

    for(int i = 0; i<11; ++i){
        equipmentButtons[i]->updateButton(mousePos);
    }

    for(int i = 0; i<11; ++i){
        if(equipmentButtons[i]->activated && timeSinceLastAction > updateSpeed){
            unequipItem(i);
        }
    }
    return make_pair(nullItem, make_pair(0, 0));

}

int find(std::array<Item*, 10> array, int len, std::string seek)
{
    for (int i = 0; i < len; ++i)
    {
        if (array[i]->GetName() == seek){
            return i;
        }
    }
    return -1;
}

Item* Inventory::findItem(std::vector<Item*> items, std::string seek){
    for(auto i : items){
        if(i->GetName() == seek){
            return i;
        }
    }
    return nullItem;
}

void Inventory::drawInventory(sf::RenderTarget* target){

    target->draw(inventoryBackgroundSprite);
    target->draw(equipmentBackGroundSprite);

    int index_x = 0;
    int index_y = 0;
    for(auto i : inventoryItems){
        if(i->GetName() != "null_texture"){    
            sf::Sprite sprite;
            sprite.setTexture(textures[i->GetName()]);
            sprite.setScale(1.5, 1.5);
            sprite.setPosition(10 + 0 + index_x * TILESIZE * 2, ROOM_HEIGHT * TILESIZE + TILESIZE * 2 * index_y + 15);
            target->draw(sprite);
            int a;
            if(inventoryItemsCount[find(inventoryItems, 10, i->GetName())] > 9){
                a = 9;
            }else{a = inventoryItemsCount[find(inventoryItems, 10, i->GetName())] - 1;}

            inventoryNumbers[a]->setCords(64 - 10 + 64 * index_x - 4, TILESIZE*ROOM_HEIGHT + 64 - 10 + TILESIZE * 2 * index_y - 6);
            inventoryNumbers[a]->drawText(target);
        }
        else{}
        index_x += 1;
        if(index_x == 5){
            index_y += 1;
            index_x = 0;
        }
    }

    if(drawExamineText){
        examineText->drawText(target);
    }
    drawExamineText = false;

    int index = 0;

    for(auto i : equipedItems){
        if(i->GetName() != "null_texture"){
            sf::Sprite sprite;
            float scale = 1.5;
            sprite.setTexture(textures[i->GetName()]);
            sprite.setScale(scale, scale);
            sprite.setPosition(cordinateVector[index].x + 10, cordinateVector[index].y + 5);
            target->draw(sprite);
        }
        index += 1;
    }

}

// Primary/secondary stats are : 0 Headslot armor/critChance, 1 cape critchange/weight-reduction, 2 necklace crit change/bonus xp, 3 ammo damage/range, 4 weapon damage/attackspeed,
// 5 torso armor, 6 shield armor/damage, 7 legs armor/movementspeed, 8 glove armor/attackspeed, 9 boots movementspeed/armor, 10 ring bonus xp/ bonus gold multiplier

int Inventory::GetTotalWeight(){
    int weight = 0;
    for(auto item : allItems){
        weight += item->GetWeight();
    }
    for(auto item : equipedItems){
        weight += item->GetWeight();
    }
    return weight * ( 100 - equipedItems[1]->GetSecondaryStat()) / 100;
}

int Inventory::GetMsBonus(){
    return - equipedItems[9]->GetPrimaryStat() - equipedItems[7]->GetSecondaryStat() - equipedItems[2]->GetSecondaryStat();
}
    
int Inventory::GetArmor(){
    return equipedItems[7]->GetSecondaryStat() + equipedItems[9]->GetSecondaryStat() + equipedItems[0]->GetPrimaryStat() + equipedItems[5]->GetPrimaryStat()+ equipedItems[6]->GetPrimaryStat() + equipedItems[8]->GetPrimaryStat();
}

int Inventory::GetCritChange(){
    return equipedItems[1]->GetPrimaryStat() + equipedItems[2]->GetPrimaryStat() + equipedItems[0]->GetSecondaryStat();
}

int Inventory::GetBonusXPMultiplier(){ return equipedItems[10]->GetPrimaryStat() + equipedItems[2]->GetSecondaryStat();}

int Inventory::GetBonusGoldMultiplier(){return equipedItems[10]->GetSecondaryStat(); }

int Inventory::GetAsBonus(){ return - equipedItems[8]->GetSecondaryStat() - equipedItems[4]->GetSecondaryStat();}

int Inventory::GetDmgBonus(){return equipedItems[4]->GetPrimaryStat() + equipedItems[3]->GetPrimaryStat();}