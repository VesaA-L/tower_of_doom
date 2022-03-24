#include "text.hpp"

Text::Text(int x, int y, std::string inputText, std::string fontFile, sf::Color outlineColour, sf::Color fillColour, int size, int outlineThickness, bool center){

    if(!font.loadFromFile(fontFile)){
        std::cout << "ERROR FONT FILE NOT FOUND TEXT" << std::endl;
    }

    this->text.setFont(this->font);
    this->text.setString(inputText);
    this->text.setCharacterSize(size);
    this->text.setFillColor(fillColour);
    this->text.setOutlineColor(outlineColour);
    this->text.setOutlineThickness(outlineThickness);
    
    if(!center){
        this->text.setPosition(x, y);
    }else if(center){
        this->text.setPosition(
            x-(this->text.getGlobalBounds().width/2.f),
            y-(this->text.getGlobalBounds().height/2.f)
        );
    }

    this->backGround.setFillColor(sf::Color(0, 51, 0));
}

Text::~Text(){}

void Text::setBackGround(){
    drawBackGround = true;
}

sf::Text* Text::returnText(){
    return &text;
}

void Text::setCords(int x, int y){
    this->text.setPosition(x, y);
    this->backGround.setPosition(x-3, y+1);
}

void Text::setText(std::string textOutput){
    this->text.setString(textOutput);
    this->backGround.setSize(sf::Vector2f(this->text.getGlobalBounds().width+6, this->text.getGlobalBounds().height+6));
}

void Text::drawText(sf::RenderTarget* target){
    if(drawBackGround){
        target->draw(backGround);
    }
    drawBackGround = false;
    target->draw(this->text);
}
