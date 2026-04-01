#include "LeftPanel.hpp"
#include <algorithm>

LeftPanel::LeftPanel() : toDoList("TO DO LIST", windowSizeClass::getY() / 17.f, true) {
    background.setFillColor(sf::Color(50, 50, 50));

    // temporary data (passing dummy size, will be overwritten in updateViewSize)
    items.push_back({"Powitanie", sizeOfTextInList, {0.f, 0.f}});
    items.push_back({"Przedstawienie planu", sizeOfTextInList, {0.f, 0.f}});
    items.push_back({"Punkt A", sizeOfTextInList, {0.f, 0.f}});
    items.push_back({"Punkt B", sizeOfTextInList, {0.f, 0.f}});

    updateViewSize();
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
        }
    }
    // mouse moved
    else if (const auto* move = event.getIf<sf::Event::MouseMoved>()) {
        // dragging splitter
        if (isDraggingSplitter) {
            float newRatio = static_cast<float>(move->position.x) / windowSizeClass::getX();
            newRatio = std::clamp(newRatio, 0.1f, 0.5f);

            if (splitRatio != newRatio) {
                splitRatio = newRatio;
                updateViewSize();
            }
        }
        // hovering
        else {
            // splitter hover
            sf::Vector2f mPosGlobal = window.mapPixelToCoords(move->position, window.getDefaultView());
            if (splitter.getGlobalBounds().contains(mPosGlobal)) {
                splitter.setFillColor(sf::Color(110, 110, 110));
            } else {
                splitter.setFillColor(sf::Color(80, 80, 80));
            }

            // items hover
            sf::Vector2f mPosLocal = window.mapPixelToCoords(move->position, view);
            pointedIndex = -1;

            for (size_t i = 0; i < items.size(); ++i) {
                if (items[i].getGlobalBounds().contains(mPosLocal)) {
                    pointedIndex = i;
                    break;
                }
            }
        }
    }
    // keyboard navigation
    else if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Up) {
            if (activeIndex > 0) activeIndex--;
        }
        else if (key->code == sf::Keyboard::Key::Down) {
            if (activeIndex < items.size() - 1) activeIndex++;
        }
        else if (key->code == sf::Keyboard::Key::Enter) {
            // add new point
            items.insert(items.begin() + activeIndex + 1, {"", sizeOfTextInList, {0.f, 0.f}});
            activeIndex++;
            updateViewSize();
        }
        else if (key->code == sf::Keyboard::Key::Tab) {
            // toggle subpoint
            items[activeIndex].toggleSubpoint();
            updateViewSize();
        }
        else if (key->code == sf::Keyboard::Key::Backspace) {
            // delete empty point
            if (items[activeIndex].getString().isEmpty() && items.size() > 1) {
                items.erase(items.begin() + activeIndex);
                if (activeIndex > 0) activeIndex--;
            }
        }
    }
    // text editing
    else if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
        if (textEvent->unicode > 31 && textEvent->unicode < 127) {
            items[activeIndex].addCharacter(static_cast<char>(textEvent->unicode));
        }
        else if (textEvent->unicode == 8) {
            if (!items[activeIndex].getString().isEmpty()) {
                items[activeIndex].removeLastCharacter();
            }
        }
    }
}

void LeftPanel::draw(sf::RenderWindow& window) {
    int itemHorizontalMargins = windowSizeClass::getX() / 1920 * 15.f;
    int itemHorizontalMarginsButSubpoint = windowSizeClass::getX() / 1920 * 45.f;

    window.setView(view);

    // static elements
    window.draw(background);
    window.draw(toDoList);

    // for which height items start
    float currentY = windowSizeClass::getY() / 8.f;

    for (size_t i = 0; i < items.size(); ++i) {
        float posX = items[i].isSubpoint ? itemHorizontalMarginsButSubpoint : itemHorizontalMargins;

        items[i].setPosition({posX, currentY});

        // Set state
        if (i == activeIndex) {
            items[i].setState(ListItem::State::Selected);
        } else if (i == pointedIndex) {
            items[i].setState(ListItem::State::Hovered);
        } else {
            items[i].setState(ListItem::State::Default);
        }

        items[i].draw(window);

        // move cursor down
        currentY += items[i].getGlobalBounds().size.y + 5.f;
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
}