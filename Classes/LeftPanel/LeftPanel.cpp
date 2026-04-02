#include "LeftPanel.hpp"
#include <algorithm>
#include <cmath>

LeftPanel::LeftPanel() : toDoList("TO DO LIST", windowSizeClass::getY() / 17.f, true) {
    background.setFillColor(sf::Color(50, 50, 50));

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
    updateViewSize();
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
    float availHeight = windowSizeClass::getY() - windowSizeClass::getY() / 8.f;
    maxScrollY = std::max(0.f, totalHeight - availHeight);
    scrollY = std::clamp(scrollY, 0.f, maxScrollY);
}

void LeftPanel::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    // mouse pressed
    if (const auto* press = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (press->button == sf::Mouse::Button::Left) {
            // check splitter
            sf::Vector2f mPosGlobal = window.mapPixelToCoords(press->position, window.getDefaultView());
            if (splitter.getGlobalBounds().contains(mPosGlobal)) {
                isDraggingSplitter = true;
                splitter.setFillColor(sf::Color(150, 150, 150));
            }
            // check items
            else {
                sf::Vector2f mPosLocal = window.mapPixelToCoords(press->position, view);
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
            isDraggingSplitter = false;
            splitter.setFillColor(sf::Color(80, 80, 80));

            if (isDraggingItem) {
                items.insert(items.begin() + dropIndex, draggedItems.begin(), draggedItems.end());
                draggedItems.clear();
                isDraggingItem = false;
                activeIndex = dropIndex;
                mouseDownIndex = -1;

                updateNumbers();
                updateViewSize();
            } else {
                mouseDownIndex = -1;
            }
        }
    }
    // mouse moved
    else if (const auto* move = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mPosLocal = window.mapPixelToCoords(move->position, view);
        sf::Vector2f mPosGlobal = window.mapPixelToCoords(move->position, window.getDefaultView());

        // dragging splitter
        if (isDraggingSplitter) {
            float newRatio = static_cast<float>(move->position.x) / windowSizeClass::getX();
            newRatio = std::clamp(newRatio, 0.1f, 0.5f);

            if (splitRatio != newRatio) {
                splitRatio = newRatio;
                updateViewSize();
            }
        }

        // drag and drop items
        if (mouseDownIndex != -1 && !isDraggingItem && !isDraggingSplitter) {
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

        if (isDraggingItem) {
            dragMousePos = mPosLocal;

            // calculate drop index based on Y position
            dropIndex = 0;
            for (size_t i = 0; i < items.size(); ++i) {
                float itemMidY = items[i].getGlobalBounds().position.y + items[i].getGlobalBounds().size.y / 2.f;
                if (mPosLocal.y > itemMidY) {
                    dropIndex = i + 1;
                }
            }

            // constrain dropIndex
            if (!draggedItems.empty()) {
                if (draggedItems[0].isSubpoint) {
                    // Podpunkty można przeciągać tylko w obrębie swojego punktu głównego
                    int parentIdx = -1;
                    // Znajdźmy punkt główny w ORYGINALNEJ liście przed usunięciem draggedItems.
                    // Odtworzenie tego jest proste: punkt główny jest po prostu nad dawnym miejscem podpunktu.
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
                    // Punkt główny wraz ze swoimi podpunktami można przeciągnąć wszędzie,
                    // byle nie Pomiędzy podpunkty należące do INNEGO punktu głównego.
                    if (dropIndex > 0 && dropIndex < items.size()) {
                        if (items[dropIndex].isSubpoint) {
                            // Jeśli próbujemy zrzucić między podpunkty, musimy przesunąć dropIndex do końca tego bloku
                            while (dropIndex < items.size() && items[dropIndex].isSubpoint) {
                                dropIndex++;
                            }
                        }
                    }
                }
            }
        }

        // hovering
        if (!isDraggingItem && !isDraggingSplitter) {
            if (splitter.getGlobalBounds().contains(mPosGlobal)) {
                splitter.setFillColor(sf::Color(110, 110, 110));
            } else {
                splitter.setFillColor(sf::Color(80, 80, 80));
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
        if (mPosGlobal.x < windowSizeClass::getX() * splitRatio) {
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
                updateViewSize();
            }
            else if (key->code == sf::Keyboard::Key::Tab) {
                items[activeIndex].toggleSubpoint();
                updateNumbers();
                updateViewSize();
            }
            else if (key->code == sf::Keyboard::Key::Backspace) {
                if (items[activeIndex].getString().isEmpty() && items.size() > 1) {
                    items.erase(items.begin() + activeIndex);
                    if (activeIndex > 0) activeIndex--;
                    updateNumbers();
                    updateViewSize();
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
    int itemHorizontalMargins = windowSizeClass::getX() / 1920 * 15.f;
    int itemHorizontalMarginsButSubpoint = windowSizeClass::getX() / 1920 * 45.f;

    window.setView(view);

    // static background
    window.draw(background);

    float currentY = windowSizeClass::getY() / 8.f - scrollY;

    // placeholder for drag and drop
    sf::RectangleShape placeholder;
    placeholder.setFillColor(sf::Color(100, 100, 100, 100));

    for (size_t i = 0; i <= items.size(); ++i) {
        // Draw placeholder
        if (isDraggingItem && i == dropIndex) {
            float phHeight = 0.f;
            for (const auto& di : draggedItems) phHeight += di.getGlobalBounds().size.y + 5.f;

            placeholder.setSize({windowSizeClass::getX() * splitRatio - 30.f, phHeight - 5.f});
            placeholder.setPosition({(float)itemHorizontalMargins, currentY});
            window.draw(placeholder);

            currentY += phHeight;
        }

        if (i < items.size()) {
            float posX = items[i].isSubpoint ? itemHorizontalMarginsButSubpoint : itemHorizontalMargins;
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

    // draw dragged items floating at mouse pos
    if (isDraggingItem && !draggedItems.empty()) {
        float dY = dragMousePos.y - 10.f;
        for (auto& di : draggedItems) {
            float posX = di.isSubpoint ? itemHorizontalMarginsButSubpoint : itemHorizontalMargins;
            di.setPosition({posX, dY});
            di.setState(ListItem::State::Selected);
            di.draw(window);
            dY += di.getGlobalBounds().size.y + 5.f;
        }
    }

    // Top masking panel
    sf::RectangleShape topBg;
    topBg.setSize({windowSizeClass::getX() * splitRatio, windowSizeClass::getY() / 8.f});
    topBg.setFillColor(sf::Color(50, 50, 50));
    window.draw(topBg);

    window.draw(toDoList);

    // scrollbar
    if (maxScrollY > 0) {
        float totalH = maxScrollY + (windowSizeClass::getY() - windowSizeClass::getY() / 8.f);
        float scrollRatio = scrollY / maxScrollY;
        float sbHeight = std::max(20.f, windowSizeClass::getY() * (windowSizeClass::getY() / totalH));

        // ensure scrollbar doesn't overlap the top background
        float topOffset = windowSizeClass::getY() / 8.f;
        float availSbSpace = windowSizeClass::getY() - topOffset - sbHeight;

        scrollbar.setSize({5.f, sbHeight});
        scrollbar.setFillColor(sf::Color(100, 100, 100));
        scrollbar.setPosition({windowSizeClass::getX() * splitRatio - 10.f, topOffset + availSbSpace * scrollRatio});
        window.draw(scrollbar);
    }

    // splitter
    window.setView(window.getDefaultView());
    window.draw(splitter);
}

void LeftPanel::updateViewSize() {
    float winX = windowSizeClass::getX();
    float winY = windowSizeClass::getY();

    // update camera
    view.setSize({winX * splitRatio, winY});
    view.setCenter({(winX * splitRatio) / 2.f, winY / 2.f});

    // update viewport
    view.setViewport(sf::FloatRect({0.f, 0.f}, {splitRatio, 1.f}));

    // update background and title
    background.setSize({winX * splitRatio, winY});
    toDoList.setPosition({(winX * splitRatio) / 2.f, winY / 16.f});

    // update splitter
    splitter.setSize({6.f, winY});
    splitter.setPosition({winX * splitRatio - 2.f, 0.f});

    // update items width
    float panelWidth = winX * splitRatio;
    float baseHeight = winY / 20.f;

    for (auto& item : items) {
        float itemWidth = panelWidth - 30.f;
        if (item.isSubpoint) {
            itemWidth -= 30.f;
        }
        item.setSize({itemWidth, baseHeight});
    }

    updateScroll();
}