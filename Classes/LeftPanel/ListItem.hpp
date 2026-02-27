#pragma once
#include <SFML/Graphics.hpp>
#include "../ClassText.hpp"
#include "../../windowSizeClass.hpp"

class ListItem {
private:
    static inline int total;

    sf::RectangleShape box;
    bool selected;
    bool ifMainPoint;
    ClassText text;

public:
    bool isSubpoint = false;

    sf::String getString();
    void addCharacter(char c);
    void removeLastCharacter();
    void setBackgroundColor(sf::Color color);

    ListItem(std::string text, int lettersSize,  sf::Vector2f size);
    ~ListItem();

    void setPosition(sf::Vector2f pos);
    sf::FloatRect getGlobalBounds() const;
    void updateTextPosition();
    void draw(sf::RenderWindow& window) const;
};