#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../ClassText.hpp"
#include "../../windowSizeClass.hpp"
#include "ListItem.hpp"

class LeftPanel {
private:
    sf::View view;
    sf::RectangleShape background;
    int sizeOfTextInList = (int)windowSizeClass::getY()/27;

    ClassText toDoList;

    std::vector<ListItem> items;

    size_t activeIndex = 0;
    size_t pointedIndex = 0;

    bool ifMouseOnPoints = false;
    sf::RectangleShape pointedlistItem;
    sf::RectangleShape btnUp;
    sf::RectangleShape btnDown;

public:
    LeftPanel();
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
};