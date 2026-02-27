#include "LeftPanel.hpp"

LeftPanel::LeftPanel() : toDoList("TO DO LIST", windowSizeClass::getY()/17.f, true) {
    // viewport setup
    view.setSize({windowSizeClass::getX()/3, windowSizeClass::getY()});
    view.setCenter({windowSizeClass::getX()/6, windowSizeClass::getY()/2});
    view.setViewport(sf::FloatRect({0.f, 0.f}, {0.33f, 1.f}));

    background.setSize(view.getSize());
    background.setFillColor(sf::Color(50, 50, 50));

    toDoList.setPosition({view.getSize().x / 2.f, view.getSize().y / 16.f});

    pointedlistItem.setSize({view.getSize().x / 1.05f, view.getSize().y / 20.f});
    pointedlistItem.setFillColor(sf::Color(0, 0, 40, 50));

    // temporary data
    items.push_back({"Powitanie", sizeOfTextInList, pointedlistItem.getSize()});
    items.push_back({"Przedstawienie planu", sizeOfTextInList, pointedlistItem.getSize()});
    items.push_back({"Punkt A", sizeOfTextInList, pointedlistItem.getSize()});
    items.push_back({"Punkt B", sizeOfTextInList, pointedlistItem.getSize()});
}

void LeftPanel::handleEvent(const sf::Event& event, sf::RenderWindow& window) {

    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        // nawigation thrugh arrow keys
        if (key->code == sf::Keyboard::Key::Up) {
            if (activeIndex > 0) activeIndex--;
        }
        else if (key->code == sf::Keyboard::Key::Down) {
            if (activeIndex < items.size() - 1) activeIndex++;
        }
        // adding new point below active
        if (key->code == sf::Keyboard::Key::Enter) {
            items.insert(items.begin() + activeIndex + 1, {"", sizeOfTextInList, pointedlistItem.getSize()});
            activeIndex++;
        }
        // switching active point to subpoint, or reverse
        if (key->code == sf::Keyboard::Key::Tab) {
            items[activeIndex].isSubpoint ? items[activeIndex].isSubpoint = false : items[activeIndex].isSubpoint = true;
        }
        // deletion of a active point
        if (key->code == sf::Keyboard::Key::Backspace) {
            if (items[activeIndex].getString().isEmpty()) { items.erase(items.begin() + activeIndex); }
        }
    }

    // set pointedIndex value from mouse position
    ifMouseOnPoints = false;
    for (size_t i = 0; i < items.size(); i++) {
        sf::Vector2i mPos = sf::Mouse::getPosition(window);
        if (items[i].getGlobalBounds().contains(sf::Vector2<float>(mPos))) {
            pointedIndex = i;
            ifMouseOnPoints = true;

            if (const auto* press = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (press->button == sf::Mouse::Button::Left) {
                    activeIndex = i;
                }
            }
        }
    }
    if (!ifMouseOnPoints) {
        pointedIndex = -1;
    }

    // text edition
    if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
        // bez znaków specjalnych (backspace enter itp)
        if (textEvent->unicode > 31 && textEvent->unicode < 127) {
            // Dodajemy wciśniętą literkę do aktywnego punktu
            items[activeIndex].addCharacter(static_cast<char>(textEvent->unicode));
        }
        // delete
        else if (textEvent->unicode == 8) {
            if (!items[activeIndex].getString().isEmpty()) {
                items[activeIndex].removeLastCharacter();
            }
        }
    }
}

void LeftPanel::draw(sf::RenderWindow& window) {

    window.setView(view);

    window.draw(background);
    window.draw(toDoList);

    // y value from which I start points list
    float startY = windowSizeClass::getY()/8.f;

    for (size_t i = 0; i < items.size(); ++i) {
        items[i].updateTextPosition();

        float posX = 10.f;
        float posY = startY + (i * windowSizeClass::getY()/17.f);

        items[i].setPosition({posX, posY});

        // highlight active point
        if (i == activeIndex) {
            items[i].setBackgroundColor(sf::Color(80, 80, 80, 200));
        }
        else
            items[i].setBackgroundColor(sf::Color(0, 0, 0, 0));

        items[i].draw(window);

        // highlight on pointing point :)
        if (i == pointedIndex) {
            pointedlistItem.setPosition({10.f, posY});
            window.draw(pointedlistItem);
        }
        //else pointedIndex = -1;
    }
}