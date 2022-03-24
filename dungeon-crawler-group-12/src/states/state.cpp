#include "state.hpp"

//State abstrakti ei luo mitään constructorissa, vain type/indikaattori, eli uniikki numero jokaiselle luodulle statelle
State::State(int x, sf::RenderWindow* win){
    this->type = x;
    this->window = win;

    //initialize a font for each state, can add more later if need, jos paljon lisää nii lisätään constructorii lisä input, fontfile
    if(!font.loadFromFile("fonts/Montserrat-Regular.ttf")){
        std::cout << "ERROR FONT FILE NOT FOUND STATE" << std::endl;
    }

}

State::~State(){

}

void State::updateMousePos(){
    this->mousePos = sf::Mouse::getPosition(*this->window);
}


