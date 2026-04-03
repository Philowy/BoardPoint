#include "LeftPanel.hpp"
#include <algorithm>
#include <cmath>

LeftPanel::LeftPanel() : toDoList("TO DO LIST", windowSizeClass::getY() / 17.f, true) {
    // Initial size and position of the floating panel
    // Set default offsets from screen edges
    float initialWidth = windowSizeClass::getX() * 0.3f;
    float initialHeight = windowSizeClass::getY() - 150.f; // Mocno odsuniete od góry (150px zostawione dla górnego paska, na dole 30px)

    panelRect.position.x = 20.f; // 20px od lewej
    panelRect.position.y = 120.f; // 120px od góry (na przyszły poziomy pasek)
    panelRect.size.x = initialWidth;
    panelRect.size.y = initialHeight;

    background.setFillColor(sf::Color(50, 50, 50));

    // Top bar handle for dragging
    topBar.setFillColor(sf::Color(40, 40, 40));

    // Resize handlers colors
    rightResizeHandle.setFillColor(sf::Color(80, 80, 80));
    bottomResizeHandle.setFillColor(sf::Color(80, 80, 80));
    cornerResizeHandle.setFillColor(sf::Color(100, 100, 100));

    // temporary data
    items.push_back({"Powitanie", sizeOfTextInList, {0.f, 0.f}});
    items.push_back({"Podpunkt", sizeOfTextInList, {0.f, 0.f}});
    items[1].toggleSubpoint();
    items.push_back({"Przedstawienie planu", sizeOfTextInList, {0.f, 0.f}});
    items.push_back({"Punkt A", sizeOfTextInList, {0.f, 0.f}});
    items.push_back({"Podpunkt 1", sizeOfTextInList, {0.f, 0.f}});
    items[4].toggleSubpoint();
    items.push_back({"Podpunkt 2", sizeOfTextInList, {0.f, 0.f}});
    items[5].toggleSubpoint();
    items.push_back({"Podpunkt 3", sizeOfTextInList, {0.f, 0.f}});
    items[6].toggleSubpoint();
    items.push_back({"Punkt B", sizeOfTextInList, {0.f, 0.f}});
    items.push_back({"Podpunkt 1", sizeOfTextInList, {0.f, 0.f}});
    items[8].toggleSubpoint();
    items.push_back({"Podpunkt 2", sizeOfTextInList, {0.f, 0.f}});
    items[9].toggleSubpoint();

    updateNumbers();
    updateLayout();
}

void LeftPanel::updateNumbers() {
    int num = 1;
    for (auto& item : items) {
        if (!item.isSubpoint) {
            item.setListNumber(num++);
        } else {
            item.setListNumber(0);
        }
    }
}

void LeftPanel::updateScroll() {
    float totalHeight = 0.f;
    for (const auto& item : items) {
        totalHeight += item.getGlobalBounds().size.y + 5.f;
    }
    float availHeight = panelRect.size.y - (panelRect.size.y / 8.f);
    maxScrollY = std::max(0.f, totalHeight - availHeight);
    scrollY = std::clamp(scrollY, 0.f, maxScrollY);
}

void LeftPanel::updateLayout() {
    float topBarHeight = panelRect.size.y / 8.f;

    // View for the items
    // Normalizing viewport (0 to 1 range relative to OS window)
    float winX = windowSizeClass::getX();
    float winY = windowSizeClass::getY();

    float normLeft = panelRect.position.x / winX;
    float normTop = panelRect.position.y / winY;
    float normWidth = panelRect.size.x / winX;
    float normHeight = panelRect.size.y / winY;

    view.setViewport(sf::FloatRect({normLeft, normTop}, {normWidth, normHeight}));

    // Internal size of the view (1:1 with pixels)
    view.setSize({panelRect.size.x, panelRect.size.y});
    // Centered at its own local middle
    view.setCenter({panelRect.size.x / 2.f, panelRect.size.y / 2.f});

    // Update global shapes
    background.setPosition({panelRect.position.x, panelRect.position.y});
    background.setSize({panelRect.size.x, panelRect.size.y});

    topBar.setPosition({panelRect.position.x, panelRect.position.y});
    topBar.setSize({panelRect.size.x, topBarHeight});

    toDoList.setPosition({panelRect.position.x + panelRect.size.x / 2.f, panelRect.position.y + topBarHeight / 2.f});

    // Handle positions
    float handleSize = 6.f;
    float cornerSize = 12.f;

    rightResizeHandle.setPosition({panelRect.position.x + panelRect.size.x - handleSize, panelRect.position.y});
    rightResizeHandle.setSize({handleSize, panelRect.size.y - cornerSize});

    bottomResizeHandle.setPosition({panelRect.position.x, panelRect.position.y + panelRect.size.y - handleSize});
    bottomResizeHandle.setSize({panelRect.size.x - cornerSize, handleSize});

    cornerResizeHandle.setPosition({
        panelRect.position.x + panelRect.size.x - cornerSize,
        panelRect.position.y + panelRect.size.y - cornerSize
    });
    cornerResizeHandle.setSize({cornerSize, cornerSize});

    // Resize list items to fit new width
    // Odleglosc elementow taka sama od lewej jak z prawej = 15.f marginesu z obu stron.
    float margin = 15.f;
    for (auto& item : items) {
        float itemWidth = panelRect.size.x - (margin * 2.f);

        if (item.isSubpoint) {
            // Further indented by 30px
            itemWidth -= 30.f;
        }

        float baseHeight = panelRect.size.y / 20.f; // Or fix a static height based on text size
        item.setSize({itemWidth, baseHeight});
    }

    updateScroll();
}

void LeftPanel::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    // mouse pressed
    if (const auto* press = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (press->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPosGlobal = window.mapPixelToCoords(press->position, window.getDefaultView());
            sf::Vector2f mPosLocal = window.mapPixelToCoords(press->position, view);

            // check corner resize
            if (cornerResizeHandle.getGlobalBounds().contains(mPosGlobal)) {
                isResizingCorner = true;
                cornerResizeHandle.setFillColor(sf::Color(150, 150, 150));
            }
            // check right resize
            else if (rightResizeHandle.getGlobalBounds().contains(mPosGlobal)) {
                isResizingRight = true;
                rightResizeHandle.setFillColor(sf::Color(150, 150, 150));
            }
            // check bottom resize
            else if (bottomResizeHandle.getGlobalBounds().contains(mPosGlobal)) {
                isResizingBottom = true;
                bottomResizeHandle.setFillColor(sf::Color(150, 150, 150));
            }
            // check top bar drag
            else if (topBar.getGlobalBounds().contains(mPosGlobal)) {
                isDraggingPanel = true;
                panelDragOffset = mPosGlobal - sf::Vector2f(panelRect.position.x, panelRect.position.y);
            }
            // check items
            else if (panelRect.contains(mPosGlobal)) {
                for (size_t i = 0; i < items.size(); ++i) {
                    if (items[i].getGlobalBounds().contains(mPosLocal)) {
                        activeIndex = i;
                        mouseDownIndex = i;
                        dragStartPos = mPosLocal;
                        break;
                    }
                }
            }
        }
    }
    // mouse released
    else if (const auto* release = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (release->button == sf::Mouse::Button::Left) {
            isResizingRight = false;
            isResizingBottom = false;
            isResizingCorner = false;
            isDraggingPanel = false;

            rightResizeHandle.setFillColor(sf::Color(80, 80, 80));
            bottomResizeHandle.setFillColor(sf::Color(80, 80, 80));
            cornerResizeHandle.setFillColor(sf::Color(100, 100, 100));

            if (isDraggingItem) {
                items.insert(items.begin() + dropIndex, draggedItems.begin(), draggedItems.end());
                draggedItems.clear();
                isDraggingItem = false;
                activeIndex = dropIndex;
                mouseDownIndex = -1;

                updateNumbers();
                updateLayout();
            } else {
                mouseDownIndex = -1;
            }
        }
    }
    // mouse moved
    else if (const auto* move = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mPosLocal = window.mapPixelToCoords(move->position, view);
        sf::Vector2f mPosGlobal = window.mapPixelToCoords(move->position, window.getDefaultView());

        // handle resizing
        if (isResizingCorner) {
            float newWidth = mPosGlobal.x - panelRect.position.x;
            float newHeight = mPosGlobal.y - panelRect.position.y;
            panelRect.size.x = std::max(200.f, newWidth);
            panelRect.size.y = std::max(200.f, newHeight);
            updateLayout();
        }
        else if (isResizingRight) {
            float newWidth = mPosGlobal.x - panelRect.position.x;
            panelRect.size.x = std::max(200.f, newWidth);
            updateLayout();
        }
        else if (isResizingBottom) {
            float newHeight = mPosGlobal.y - panelRect.position.y;
            panelRect.size.y = std::max(200.f, newHeight);
            updateLayout();
        }
        // handle dragging panel
        else if (isDraggingPanel) {
            panelRect.position.x = mPosGlobal.x - panelDragOffset.x;
            panelRect.position.y = mPosGlobal.y - panelDragOffset.y;
            updateLayout();
        }
        // drag and drop items
        else if (mouseDownIndex != -1 && !isDraggingItem) {
            float dist = std::sqrt(std::pow(mPosLocal.x - dragStartPos.x, 2) + std::pow(mPosLocal.y - dragStartPos.y, 2));
            if (dist > 5.f) {
                isDraggingItem = true;

                int count = 1;
                if (!items[mouseDownIndex].isSubpoint) {
                    for (size_t i = mouseDownIndex + 1; i < items.size(); ++i) {
                        if (items[i].isSubpoint) count++;
                        else break;
                    }
                }

                for (int i = 0; i < count; ++i) {
                    draggedItems.push_back(items[mouseDownIndex]);
                    items.erase(items.begin() + mouseDownIndex);
                }

                activeIndex = -1;
            }
        }
        else if (isDraggingItem) {
            dragMousePos = mPosLocal;

            dropIndex = 0;
            for (size_t i = 0; i < items.size(); ++i) {
                float itemMidY = items[i].getGlobalBounds().position.y + items[i].getGlobalBounds().size.y / 2.f;
                if (mPosLocal.y > itemMidY) {
                    dropIndex = i + 1;
                }
            }

            if (!draggedItems.empty()) {
                if (draggedItems[0].isSubpoint) {
                    int parentIdx = -1;
                    for (int i = std::min((int)items.size() - 1, mouseDownIndex - 1); i >= 0; --i) {
                        if (!items[i].isSubpoint) {
                            parentIdx = i;
                            break;
                        }
                    }
                    int nextMainIdx = items.size();
                    for (int i = parentIdx + 1; i < items.size(); ++i) {
                        if (!items[i].isSubpoint) {
                            nextMainIdx = i;
                            break;
                        }
                    }
                    dropIndex = std::clamp(dropIndex, parentIdx + 1, nextMainIdx);
                } else {
                    if (dropIndex > 0 && dropIndex < items.size()) {
                        if (items[dropIndex].isSubpoint) {
                            while (dropIndex < items.size() && items[dropIndex].isSubpoint) {
                                dropIndex++;
                            }
                        }
                    }
                }
            }
        }
        // hovering
        else {
            if (cornerResizeHandle.getGlobalBounds().contains(mPosGlobal)) {
                cornerResizeHandle.setFillColor(sf::Color(120, 120, 120));
            } else {
                cornerResizeHandle.setFillColor(sf::Color(100, 100, 100));
            }

            if (rightResizeHandle.getGlobalBounds().contains(mPosGlobal)) {
                rightResizeHandle.setFillColor(sf::Color(110, 110, 110));
            } else {
                rightResizeHandle.setFillColor(sf::Color(80, 80, 80));
            }

            if (bottomResizeHandle.getGlobalBounds().contains(mPosGlobal)) {
                bottomResizeHandle.setFillColor(sf::Color(110, 110, 110));
            } else {
                bottomResizeHandle.setFillColor(sf::Color(80, 80, 80));
            }

            pointedIndex = -1;
            for (size_t i = 0; i < items.size(); ++i) {
                if (items[i].getGlobalBounds().contains(mPosLocal)) {
                    pointedIndex = i;
                    break;
                }
            }
        }
    }
    // scrolling
    else if (const auto* scroll = event.getIf<sf::Event::MouseWheelScrolled>()) {
        sf::Vector2f mPosGlobal = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());
        if (panelRect.contains(mPosGlobal)) {
            scrollY -= scroll->delta * 30.f;
            scrollY = std::clamp(scrollY, 0.f, maxScrollY);
        }
    }
    // keyboard navigation
    else if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (activeIndex != -1) {
            if (key->code == sf::Keyboard::Key::Up) {
                if (activeIndex > 0) activeIndex--;
            }
            else if (key->code == sf::Keyboard::Key::Down) {
                if (activeIndex < (int)items.size() - 1) activeIndex++;
            }
            else if (key->code == sf::Keyboard::Key::Enter) {
                items.insert(items.begin() + activeIndex + 1, {"", sizeOfTextInList, {0.f, 0.f}});
                activeIndex++;
                updateNumbers();
                updateLayout();
            }
            else if (key->code == sf::Keyboard::Key::Tab) {
                items[activeIndex].toggleSubpoint();
                updateNumbers();
                updateLayout();
            }
            else if (key->code == sf::Keyboard::Key::Backspace) {
                if (items[activeIndex].getString().isEmpty() && items.size() > 1) {
                    items.erase(items.begin() + activeIndex);
                    if (activeIndex > 0) activeIndex--;
                    updateNumbers();
                    updateLayout();
                }
            }
        }
    }
    // text editing
    else if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
        if (activeIndex != -1) {
            if (textEvent->unicode > 31 && textEvent->unicode < 127) {
                items[activeIndex].addCharacter(static_cast<char>(textEvent->unicode));
                updateScroll();
            }
            else if (textEvent->unicode == 8) {
                if (!items[activeIndex].getString().isEmpty()) {
                    items[activeIndex].removeLastCharacter();
                    updateScroll();
                }
            }
        }
    }
}

void LeftPanel::draw(sf::RenderWindow& window) {
    // 1. Draw solid background in global space
    window.setView(window.getDefaultView());

    // Rysowanie zaokraglonego tla zamiast background z RectangleShape
    // Zeby osiagnac zaokraglone rogi bez uzywania zewnetrznych bibliotek,
    // zazwyczaj uzywa sie ConvexShape lub wielu nakladajacych sie na siebie elementow,
    // jednak najprostszym i najbardziej optymalnym rozwiazaniem z wykorzystaniem
    // samego bazowego SFML-a jest rysowanie 4 kół w rogach i 2 prostokątów w środku
    float radius = 15.f;
    sf::RectangleShape centerRect({panelRect.size.x, panelRect.size.y - 2.f * radius});
    centerRect.setPosition({panelRect.position.x, panelRect.position.y + radius});
    centerRect.setFillColor(background.getFillColor());

    sf::RectangleShape verticalRect({panelRect.size.x - 2.f * radius, panelRect.size.y});
    verticalRect.setPosition({panelRect.position.x + radius, panelRect.position.y});
    verticalRect.setFillColor(background.getFillColor());

    sf::CircleShape cornerTL(radius);
    cornerTL.setPosition({panelRect.position.x, panelRect.position.y});
    cornerTL.setFillColor(background.getFillColor());

    sf::CircleShape cornerTR(radius);
    cornerTR.setPosition({panelRect.position.x + panelRect.size.x - 2.f * radius, panelRect.position.y});
    cornerTR.setFillColor(background.getFillColor());

    sf::CircleShape cornerBL(radius);
    cornerBL.setPosition({panelRect.position.x, panelRect.position.y + panelRect.size.y - 2.f * radius});
    cornerBL.setFillColor(background.getFillColor());

    sf::CircleShape cornerBR(radius);
    cornerBR.setPosition({panelRect.position.x + panelRect.size.x - 2.f * radius, panelRect.position.y + panelRect.size.y - 2.f * radius});
    cornerBR.setFillColor(background.getFillColor());

    window.draw(centerRect);
    window.draw(verticalRect);
    window.draw(cornerTL);
    window.draw(cornerTR);
    window.draw(cornerBL);
    window.draw(cornerBR);


    // 2. Draw items in local view (with scrolling)
    window.setView(view);

    // Margins logic
    // Same distance from left as right: 15px margin
    float leftMargin = 15.f;
    float subpointMargin = 45.f; // further right for subpoints

    // Top bar takes up height = panelRect.size.y / 8
    float topBarHeight = panelRect.size.y / 8.f;
    float currentY = topBarHeight - scrollY;

    // placeholder for drag and drop
    sf::RectangleShape placeholder;
    placeholder.setFillColor(sf::Color(100, 100, 100, 100));

    // Draw placeholder where an element is being held
    for (size_t i = 0; i <= items.size(); ++i) {
        if (isDraggingItem && i == dropIndex) {
            float phHeight = 0.f;
            for (const auto& di : draggedItems) phHeight += di.getGlobalBounds().size.y + 5.f;

            placeholder.setSize({panelRect.size.x - (leftMargin * 2.f), phHeight - 5.f});
            placeholder.setPosition({leftMargin, currentY});
            window.draw(placeholder);

            currentY += phHeight;
        }

        if (i < items.size()) {
            float posX = items[i].isSubpoint ? subpointMargin : leftMargin;
            items[i].setPosition({posX, currentY});

            if (i == activeIndex && !isDraggingItem) {
                items[i].setState(ListItem::State::Selected);
            } else if (i == pointedIndex && !isDraggingItem) {
                items[i].setState(ListItem::State::Hovered);
            } else {
                items[i].setState(ListItem::State::Default);
            }

            items[i].draw(window);
            currentY += items[i].getGlobalBounds().size.y + 5.f;
        }
    }

    if (isDraggingItem && !draggedItems.empty()) {
        float dY = dragMousePos.y - 10.f;
        for (auto& di : draggedItems) {
            float posX = di.isSubpoint ? subpointMargin : leftMargin;
            di.setPosition({posX, dY});
            di.setState(ListItem::State::Selected);
            di.draw(window);
            dY += di.getGlobalBounds().size.y + 5.f;
        }
    }

    // 3. Draw Top Bar in global view to cover scrolled items
    window.setView(window.getDefaultView());

    // Top Bar rowniez zaokraglony u gory
    sf::RectangleShape topBarRect({panelRect.size.x, topBarHeight - radius});
    topBarRect.setPosition({panelRect.position.x, panelRect.position.y + radius});
    topBarRect.setFillColor(topBar.getFillColor());

    sf::RectangleShape topBarVert({panelRect.size.x - 2.f * radius, topBarHeight});
    topBarVert.setPosition({panelRect.position.x + radius, panelRect.position.y});
    topBarVert.setFillColor(topBar.getFillColor());

    sf::CircleShape topBarCornerTL(radius);
    topBarCornerTL.setPosition({panelRect.position.x, panelRect.position.y});
    topBarCornerTL.setFillColor(topBar.getFillColor());

    sf::CircleShape topBarCornerTR(radius);
    topBarCornerTR.setPosition({panelRect.position.x + panelRect.size.x - 2.f * radius, panelRect.position.y});
    topBarCornerTR.setFillColor(topBar.getFillColor());

    window.draw(topBarRect);
    window.draw(topBarVert);
    window.draw(topBarCornerTL);
    window.draw(topBarCornerTR);

    window.draw(toDoList);

    // scrollbar
    if (maxScrollY > 0) {
        float totalH = maxScrollY + (panelRect.size.y - topBarHeight);
        float scrollRatio = scrollY / maxScrollY;
        float sbHeight = std::max(20.f, panelRect.size.y * ((panelRect.size.y - topBarHeight) / totalH));

        float availSbSpace = panelRect.size.y - topBarHeight - sbHeight - radius; // Odebrany margines dolny zeby suwak nie najezdzal na zaokraglenie

        scrollbar.setSize({5.f, sbHeight});
        scrollbar.setFillColor(sf::Color(100, 100, 100));
        scrollbar.setPosition({
            panelRect.position.x + panelRect.size.x - 10.f,
            panelRect.position.y + topBarHeight + availSbSpace * scrollRatio
        });
        window.draw(scrollbar);
    }

    // Draw resize handles
    window.draw(rightResizeHandle);
    window.draw(bottomResizeHandle);
    window.draw(cornerResizeHandle);
}

void LeftPanel::updateViewSize() {
    updateLayout();
}