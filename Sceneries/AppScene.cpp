#include "AppScene.hpp"
#include "../windowSizeClass.hpp"

AppScene::AppScene() {

    // this will be deleted
    teksty.emplace_back("App", 90, true);
    teksty[0].centerOrigin();
    teksty[0].setPosition({windowSizeClass::getX()/2, windowSizeClass::getY()/3.f});

    teksty.emplace_back("BACK", 72, true);
    teksty[1].centerOrigin();
    teksty[1].setPosition({windowSizeClass::getX()/2, windowSizeClass::getY()/2});

    // green button in center
    startButton.setSize({windowSizeClass::getX()/8.f, windowSizeClass::getY()/16.f});
    startButton.setFillColor(sf::Color::Red);
    startButton.setOrigin({startButton.getLocalBounds().size.x/2,startButton.getLocalBounds().size.y/2});
    startButton.setPosition({windowSizeClass::getX()/2, windowSizeClass::getY()/2});
}

void AppScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // pass event forward
    leftPanel.handleEvent(event, window);

    // if "green button in center" pressed leave (will be deleted)
    if (const auto* press = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (press->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPos = window.mapPixelToCoords(press->position);
            if (startButton.getGlobalBounds().contains(mPos)) {
                wantsToReturn = true;
            }
        }
    }
}

SceneENUM AppScene::update() {

    if (wantsToReturn) {
        return SceneENUM::MENU;
    }
    return SceneENUM::NONE;
}

void AppScene::draw(sf::RenderWindow& window) {
    leftPanel.draw(window);
    window.setView(window.getDefaultView());

    window.draw(startButton);
    for (const auto& tekst : teksty) {
        window.draw(tekst);
    }
}