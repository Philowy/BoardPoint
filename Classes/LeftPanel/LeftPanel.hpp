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

    // --- PANEL DRAG AND RESIZE ---
    sf::FloatRect panelRect;       // Represents the whole panel area in window coordinates
    bool isDraggingPanel = false;
    bool isResizingPanel = false;
    sf::Vector2f panelDragOffset;  // Where user clicked relative to panel top-left

    // --- EDGES FOR RESIZING ---
    sf::RectangleShape rightResizeHandle;
    sf::RectangleShape bottomResizeHandle;
    sf::RectangleShape cornerResizeHandle;

    // Resize states
    bool isResizingRight = false;
    bool isResizingBottom = false;
    bool isResizingCorner = false;

    // --- TOP BAR ---
    sf::RectangleShape topBar;     // For dragging the panel
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
    void updateLayout();

public:
    LeftPanel();
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    void updateViewSize();
};