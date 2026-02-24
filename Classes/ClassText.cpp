#include "ClassText.hpp"
#include <iostream>

ClassText::ClassText(std::string text, int size) : Text(font, text, size) {
    this->setFillColor(sf::Color::White);


    centerOrigin();
}

void ClassText::centerOrigin() {
    sf::FloatRect textBounds = this->getLocalBounds();

    this->setOrigin({
        textBounds.position.x + textBounds.size.x / 2.0f,
        textBounds.position.y + textBounds.size.y / 2.0f
    });
}