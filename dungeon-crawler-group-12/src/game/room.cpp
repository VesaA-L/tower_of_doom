
#pragma once

#include "room.hpp"
#include "monster.cpp"
#include "ally.cpp"

void Room::SetLevel(int level){
    level_=level;
}

void Room::LoadRoom(string filename){
        fstream newfile;
        string tp;
        newfile.open(filename,ios::in);
        if(newfile.is_open()){
            int col = 0;
            int row = 0;
            int z = 0;
            while(row < szy_){
                getline(newfile,tp);
                tp.erase(remove(tp.begin(),tp.end(),' '),tp.end()); //Poistaa spacet
                // Roomin level pitäisi tietää ennen tätä riviä, että npc:t luodaan oikein.
                // Se siis kannattaa tallettaa roomin ensimmäiselle riville.
                while(col < szx_){
                    switch(tp[col]){
                        case '#':   //PERUS SEINÄ
                            room_[row][col].SetType(0);                          
                            break;
                        case '.': //PERUS VAPAA
                            room_[row][col].SetType(1);
                            break;
                        case 'B':
                            room_[row][col].SetType(2);
                            break;
                        case 'S':
                            room_[row][col].SetType(3);
                            break;
                        case 'T': //TIKKAAT (SEURAAVA KERROS)
                            room_[row][col].SetType(4);
                            break;
                        case 'R': //ROCK rikottava kivi
                            room_[row][col].SetType(5);
                            break;
                        case 'P': //PIIKKI satuttava lattia
                            room_[row][col].SetType(6);
                            break;
                        case 'L': //LOCK avaimella avattava kivi
                            room_[row][col].SetType(7);
                            break;
                       // case 'W': //WATER hidastava vesi
                         //   room_[row][col].SetType(8);
                           // break;
                        case 's':
                            enemies_.push_back(new Goblin(level_, col, row));
                            room_[row][col].SetType(1);
                            break;
                        case 'r':
                            enemies_.push_back(new Rat(level_, col, row));
                            room_[row][col].SetType(1);
                            break;
                        case 'b':
                            enemies_.push_back(new Spider(level_, col, row));
                            room_[row][col].SetType(1);
                            break;
                        case 'p':
                            enemies_.push_back(new Possesed(level_, col, row));
                            room_[row][col].SetType(1);
                            break;
                        case 'c':
                            enemies_.push_back(new SkeletonAnime(level_, col, row));
                            room_[row][col].SetType(1);
                            break;
                        case 'h':
                            allies_.push_back(new Ally(col, row, 2 * level_-1));
                            room_[row][col].SetType(1);
                            break;
                        case 'x':
                            allies_.push_back(new Ally(col, row, 2 * level_));
                            room_[row][col].SetType(1);
                            break;
                        case 'a':
                            items_.push_back(new Item("key", col, row));
                            room_[row][col].SetType(1);
                            break;
                        case 'Z':
                            enemies_.push_back(new Boss(level_, col, row));
                            room_[row][col].SetType(1);
                            break;
                        case 'A':
                            items_.push_back(new Item("nice_armour", col, row));
                            room_[row][col].SetType(1);
                            break;
                        case 'd':
                            allies_.push_back(new Ally(col, row, 7));
                            room_[row][col].SetType(1);
                            break;
                        default:
                            room_[row][col].SetType(tp[col]-48);
                    }
                    col++;
                }
                row++;
                col = 0;
            }
                
        }
        setRooms();
    }

void Room::CreateVertexArray(array<int, 320> levelInput){

    if (!this->mapTileTexture.loadFromFile("images/mapTileSet2.png")){
        cout << "ERROR LOAD MAPTILE TEXTURE" << "\n";
    }

    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(ROOM_WIDTH * ROOM_HEIGHT * 4);
    
    for(int i = 0; i<ROOM_WIDTH; ++i){
        for(int j = 0; j<ROOM_HEIGHT; ++j){
            int a = i + j * ROOM_WIDTH;
            levelInput[a]=room_[j][i].GetType();
            /*if(room_[j][i].GetType() == 0){
                levelInput[a]= 0;
            }else if(room_[j][i].GetType() == 1){
                levelInput[a]= 1;
            }else if(room_[j][i].GetType() == 2){
                levelInput[a]= 2;
            }else if(room_[j][i].GetType() == 3){
                levelInput[a]= 3;
            }
            else if(room_[j][i].GetType() == 4){
                levelInput[a]= 4;
            }
            else if(room_[j][i].GetType() == 5){
                levelInput[a]= 5;
            }*/
        }
    }

    for (unsigned int i = 0; i < ROOM_WIDTH; ++i){
        for (unsigned int j = 0; j < ROOM_HEIGHT; ++j){
            
            int tileNumber = levelInput[i + j * ROOM_WIDTH];
                
            int tu = tileNumber % (this->mapTileTexture.getSize().x / TILESIZE);
            int tv = tileNumber / (this->mapTileTexture.getSize().x / TILESIZE);
        
            sf::Vertex* quad = &vertices[(i + j * ROOM_WIDTH) * 4];
         
            quad[0].position = sf::Vector2f(i * TILESIZE, j * TILESIZE);
            quad[1].position = sf::Vector2f((i + 1) * TILESIZE, j * TILESIZE);
            quad[2].position = sf::Vector2f((i + 1) * TILESIZE, (j + 1) * TILESIZE);
            quad[3].position = sf::Vector2f(i * TILESIZE, (j + 1) * TILESIZE);

                
            quad[0].texCoords = sf::Vector2f(tu * TILESIZE, tv * TILESIZE);
            quad[1].texCoords = sf::Vector2f((tu + 1) * TILESIZE, tv * TILESIZE);
            quad[2].texCoords = sf::Vector2f((tu + 1) * TILESIZE, (tv + 1) * TILESIZE);
            quad[3].texCoords = sf::Vector2f(tu * TILESIZE, (tv + 1) * TILESIZE);

        }
    }
}

array<int, 320> Room::GetInputs(){
    return levelInput;
}

Item* Room::BuyItem(int index, int player_gold){
    if(!talkingToNPC){
        return nullptr;
    }
    return currentTalkNPC->Buy(index, player_gold);
}
    

void Room::RemoveMonster(Monster * monster){
    enemies_.erase(std::remove(enemies_.begin(), enemies_.end(), monster), enemies_.end());
    delete monster;
}

void Room::RandGen(int x, int y){
        srand(time(NULL));
        int col = 0;
        int row = 0;
        while(row < szy_){
            while(col < szx_){
                room_[row][col].SetType(0);
                if(rand()%4 == 0){room_[row][col].SetType(1);}
                col++;
            }
            row++;
            col = 0;
        }
        col = 0; row = 0;
        while(row < szy_){
            while(col < szx_){
                if(Free(col-1,row) && rand()%4==0){room_[row][col].SetType(1);}
                if(Free(col+1,row) && rand()%4==0){room_[row][col].SetType(1);}
                if(Free(col,row+1) && rand()%4==0){room_[row][col].SetType(1);}
                if(Free(col,row-1) && rand()%4==0){room_[row][col].SetType(1);}
                col++;
            }
            row++;
            col = 0;
        }
        tilechange_ = true;
    }
void Room::setRooms(){
    for(auto enemy : enemies_){
        enemy->SetRoom(this);
    }
}

int Room::Get(int x, int y){
        if(x >= szx_ || y >= szy_ || x < 0 || y < 0){
            return 0; //Jos yrittää ulkopuolelle, palauttaa seinän
        }
        return room_[y][x].GetType();
    }
bool Room::OpenTile(int x, int y){
    if(x >= szx_ || y >= szy_ || x < 0 || y < 0);
    else{
        if(room_[y][x].GetType()==7){
                SetTile(x,y,1);
                CreateVertexArray(GetInputs());
                return true;
            }
        }
    return false;
    }
void Room::BreakTile(int x, int y,int dmg){
    if(x >= szx_ || y >= szy_ || x < 0 || y < 0);
    else{
        if(room_[y][x].GetType()==5){
            if(room_[y][x].Damage(dmg-2)){
                SetTile(x,y,1);
                CreateVertexArray(GetInputs()); 
                char rng = rand()%16;
                if(rng==0){
                    enemies_.push_back(new SkeletonAnime(level_, x, y));
                    enemies_.back()->SetRoom(this);
                }
            }
        }
        if(room_[y][x].GetType()==2){
            if(room_[y][x].Damage(16)){
                SetTile(x,y,1);
                CreateVertexArray(this->GetInputs());
                char rng = rand()%80;
                if(rng<2){
                    Item* drop = new Item(vrare_drop.at(rand() % vrare_drop.size()));
                    drop->setXY(x, y);
                    AddItem(drop);
                }
                else if(rng<10){
                    Item* drop = new Item(rare_drop.at(rand() % rare_drop.size()));
                    drop->setXY(x, y);
                    AddItem(drop);
                }
                else if(rng<50){
                    Item* drop = new Item(common_drop.at(rand() % common_drop.size()));
                    drop->setXY(x, y);
                    AddItem(drop);
                }
                else if(rng<60){
                    enemies_.push_back(new Rat(level_, x, y));
                    enemies_.back()->SetRoom(this);
                }
            }
        }
        }
    }

    // Item* drop = new Item(items_.at(rand() % items_.size()));
    //     drop->setXY(x_, y_);
    //     room_->AddItem(drop);

int Room::updateRoom(int player_x, int player_y){
    int player_damage_taken = 0;
    for(auto monster : enemies_){
        monster->updateTime(dTime);
        player_damage_taken += monster->TakeTurn(player_x, player_y);
    }
    return player_damage_taken;
}

void Room::DrawEnemies(sf::RenderTarget* target){
    for(auto monster : enemies_){
        monster->Draw(target);
    }
}

void Room::Talk(char option, int x, int y){
    for(auto ally : allies_){
        if(abs(ally->getX()-x) < 2 && abs(ally->getY()-y) < 2){
            talkingToNPC = true;
            currentTalkNPC = ally;
            currentTalkNPC->Talk(option);
        }
    }
}

void Room::DrawAllies(sf::RenderTarget* target){
    for(auto ally : allies_){
        ally->Draw(target);
    }
}

void Room::drawDialogue(sf::RenderTarget* target){
    if(talkingToNPC){
        std::string output = currentTalkNPC->GetDialogue();
        dialogueText->setText(output);
        int x_cord = 330;
        if(x_cord < (600 - dialogueText->returnText()->getGlobalBounds().width/2)){
            x_cord = 600 - dialogueText->returnText()->getGlobalBounds().width/2;
        }
        dialogueText->setCords(x_cord, 600 - dialogueText->returnText()->getGlobalBounds().height/2);
        dialogueText->drawText(target);           
    } else {
        if(currentTalkNPC!=nullptr){
            currentTalkNPC->resetDialogue();
        }
    }
}

void Room::SetTile(int x, int y, char tile)     // 0 seinä, 1 vapaa
    {
        room_[y][x].SetType(tile);
        tilechange_ = true;
    }
    
bool Room::Free(int x, int y){
        if(x >= szx_ || y >= szy_ || x < 0 || y < 0){
            return false; //Jos yrittää ulkopuolelle antaa 2 eli ulos, ei vapaa
        }
        for(auto enemy : enemies_){
            if(enemy->getX()==x && enemy->getY()==y){
                return false;
            }
        }
        for(auto ally : allies_){
            if(ally->getX()==x && ally->getY()==y){
                return false;
            }
        }
        int type = room_[y][x].GetType();
        return type==1 || type==4 || type==6 || type==8;
    }

int Room::OutOfBounds(int x, int y){
        if(x >= szx_ || y >= szy_ || x < 0 || y < 0){
            return true;
        }
        return false;
    }

