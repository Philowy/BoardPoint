#include "LeftPanel.hpp"
#include <algorithm> // std::clamp

LeftPanel::LeftPanel() : toDoList("TO DO LIST", windowSizeClass::getY() / 17.f, true) {
    updateViewSize();
    background.setFillColor(sf::Color(50, 50, 50));
    pointedlistItem.setFillColor(sf::Color(0, 0, 40, 50));

    // temporary data
    items.push_back({"Powitanie", sizeOfTextInList, pointedlistItem.getSize()});
    items.push_back({"Przedstawienie planu", sizeOfTextInList, pointedlistItem.getSize()});
    items.push_back({"Punkt A", sizeOfTextInList, pointedlistItem.getSize()});
    items.push_back({"Punkt B", sizeOfTextInList, pointedlistItem.getSize()});
}

void LeftPanel::handleEvent(const sf::Event& event, sf::RenderWindow& window) {

    // ==========================================
    // MOUSE PRESSED
    // ==========================================
    if (const auto* press = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (press->button == sf::Mouse::Button::Left) {

            // check splitter (global coordinates)
            sf::Vector2f mPosGlobal = window.mapPixelToCoords(press->position, window.getDefaultView());
            if (splitter.getGlobalBounds().contains(mPosGlobal)) {
                isDraggingSplitter = true;
                splitter.setFillColor(sf::Color(150, 150, 150)); // Brighten when dragging
            }
            // check items (LPanel coordinates)
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

    // ==========================================
    // MOUSE RELEASED
    // ==========================================
    else if (const auto* release = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (release->button == sf::Mouse::Button::Left) {
            isDraggingSplitter = false;
            splitter.setFillColor(sf::Color(80, 80, 80)); // back to normal color
        }
    }

    // ==========================================
    // MOUSE MOVED (Dragging & Hovering)
    // ==========================================
    else if (const auto* move = event.getIf<sf::Event::MouseMoved>()) {

        // dragging the splitter
        if (isDraggingSplitter) {
            float newRatio = static_cast<float>(move->position.x) / windowSizeClass::getX();
            newRatio = std::clamp(newRatio, 0.1f, 0.5f); // limit between 10% and 50%

            if (splitRatio != newRatio) {
                splitRatio = newRatio;
                updateViewSize();
            }
        }
        // hovering (Splitter and Items)
        else {
            // splitter hover
            sf::Vector2f mPosGlobal = window.mapPixelToCoords(move->position, window.getDefaultView());
            if (splitter.getGlobalBounds().contains(mPosGlobal)) {
                splitter.setFillColor(sf::Color(110, 110, 110));
            } else {
                splitter.setFillColor(sf::Color(80, 80, 80));
            }

            // items hover (Pointed Index)
            sf::Vector2f mPosLocal = window.mapPixelToCoords(move->position, view);
            pointedIndex = -1; // reset pointed index

            for (size_t i = 0; i < items.size(); ++i) {
                if (items[i].getGlobalBounds().contains(mPosLocal)) {
                    pointedIndex = i; // set newly hovered item
                    break;
                }
            }
        }
    }

    // ==========================================
    // KEYBOARD NAVIGATION
    // ==========================================
    else if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Up) {
            if (activeIndex > 0) activeIndex--;
        }
        else if (key->code == sf::Keyboard::Key::Down) {
            if (activeIndex < items.size() - 1) activeIndex++;
        }
        else if (key->code == sf::Keyboard::Key::Enter) {
            // add new empty point below active
            items.insert(items.begin() + activeIndex + 1, {"", sizeOfTextInList, pointedlistItem.getSize()});
            activeIndex++;
        }
        else if (key->code == sf::Keyboard::Key::Tab) {
            // toggle subpoint status
            items[activeIndex].isSubpoint = !items[activeIndex].isSubpoint;
        }
        else if (key->code == sf::Keyboard::Key::Backspace) {
            // delete active point if it's empty
            if (items[activeIndex].getString().isEmpty() && items.size() > 1) {
                items.erase(items.begin() + activeIndex);
                if (activeIndex > 0) activeIndex--;
            }
        }
    }

    // ==========================================
    // TEXT EDITING
    // ==========================================
    else if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
        if (textEvent->unicode > 31 && textEvent->unicode < 127) {
            items[activeIndex].addCharacter(static_cast<char>(textEvent->unicode));
        }
        else if (textEvent->unicode == 8) { // Backspace
            if (!items[activeIndex].getString().isEmpty()) {
                items[activeIndex].removeLastCharacter();
            }
        }
    }
}

void LeftPanel::draw(sf::RenderWindow& window) {
    window.setView(view);

    // static
    window.draw(background);
    window.draw(toDoList);

    // list items
    float startY = windowSizeClass::getY() / 8.f;

    for (size_t i = 0; i < items.size(); ++i) {
        items[i].updateTextPosition();

        float posX = items[i].isSubpoint ? 40.f : 10.f; // Indent if subpoint
        float posY = startY + (i * windowSizeClass::getY() / 17.f);

        items[i].setPosition({posX, posY});

        // set active highlight color
        if (i == activeIndex) {
            items[i].setBackgroundColor(sf::Color(80, 80, 80, 200));
        } else {
            items[i].setBackgroundColor(sf::Color(0, 0, 0, 0));
        }

        items[i].draw(window);

        // draw hover highlight
        if (i == pointedIndex && i != activeIndex) {
            pointedlistItem.setPosition({10.f, posY});
            window.draw(pointedlistItem);
        }
    }

    // splitter
    window.setView(window.getDefaultView());
    window.draw(splitter);
}

void LeftPanel::updateViewSize() {
    float winX = windowSizeClass::getX();
    float winY = windowSizeClass::getY();

    // update camera (pixels)
    view.setSize({winX * splitRatio, winY});
    view.setCenter({(winX * splitRatio) / 2.f, winY / 2.f});

    // update viewport (percentage)
    view.setViewport(sf::FloatRect({0.f, 0.f}, {splitRatio, 1.f}));

    // update panel background & title
    background.setSize({winX * splitRatio, winY});
    toDoList.setPosition({(winX * splitRatio) / 2.f, winY / 16.f});

    // update hover box width to match new panel width
    pointedlistItem.setSize({(winX * splitRatio) / 1.05f, winY / 20.f});

    // update Splitter position
    splitter.setSize({6.f, winY});
    splitter.setPosition({winX * splitRatio - 2.f, 0.f});
}