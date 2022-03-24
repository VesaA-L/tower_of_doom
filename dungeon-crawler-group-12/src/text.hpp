#pragma once

#include <iostream>
#include <ctime>
#include <stack>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

class Text{

public:

    Text(int, int, std::string, std::string, sf::Color, sf::Color, int, int, bool);
    ~Text();

    void setCords(int, int);
    void setText(std::string);
    void setBackGround();

    void drawText(sf::RenderTarget*);

    sf::Text* returnText();


private:

    bool drawBackGround = false;

    sf::Text text;
    sf::Font font;
    sf::RectangleShape backGround = sf::RectangleShape(sf::Vector2f(0,0));

    sf::Color outlineColour;
    sf::Color fillColour;

};