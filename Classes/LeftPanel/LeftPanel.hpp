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

    int pointedIndex = -1;
    int activeIndex = 0;

    // --- DRAG AND DROP ---
    bool isDraggingItem = false;
    int mouseDownIndex = -1;
    sf::Vector2f dragStartPos;
    std::vector<ListItem> draggedItems;
    int dropIndex = -1;
    sf::Vector2f dragMousePos;

    // --- SCROLLING ---
    float scrollY = 0.f;
    float maxScrollY = 0.f;
    sf::RectangleShape scrollbar;

    void updateNumbers();
    void updateScroll();

public:
    LeftPanel();
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    void updateViewSize();
};