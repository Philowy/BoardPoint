#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../ClassText.hpp"
#include "../../windowSizeClass.hpp"
#include "ListItem.hpp"

class LeftPanel {
private:
    sf::View view;
    sf::RectangleShape background;
    int sizeOfTextInList = (int)windowSizeClass::getY()/27;

    // --- SPLITTER ---
    float splitRatio = 0.3f;
    bool isDraggingSplitter = false;
    sf::RectangleShape splitter;

    ClassText toDoList;

    std::vector<ListItem> items;

    size_t pointedIndex = 0;
    size_t activeIndex = 0;

    bool ifMouseOnPoints = false;
    sf::RectangleShape pointedlistItem;

public:
    LeftPanel();
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    void updateViewSize();
};