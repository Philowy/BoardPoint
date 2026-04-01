#include "MenuScene.hpp"

MenuScene::MenuScene() {

    teksty.emplace_back("Menu", 81, true);
    teksty[0].centerOrigin();
    teksty[0].setPosition({windowSizeClass::windowSize.x/2.f, windowSizeClass::windowSize.y/3.f});

    teksty.emplace_back("Start", 72, true);
    teksty[1].centerOrigin();
    teksty[1].setPosition({windowSizeClass::windowSize.x/2.f, windowSizeClass::windowSize.y/2.f});

    // green button in the center
    startButton.setSize({200.f, 80.f});
    startButton.setFillColor(sf::Color::Green);
    startButton.setOrigin({startButton.getSize().x/2.f, startButton.getSize().y/2.f});
    startButton.setPosition({windowSizeClass::windowSize.x/2.f, windowSizeClass::windowSize.y/2.f});
}

void MenuScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (const auto* press = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (press->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (startButton.getGlobalBounds().contains(mPos)) {
                wantsToStart = true;
            }
        }
    }
}

SceneENUM MenuScene::update() {
    if (wantsToStart) {
        return SceneENUM::APP;
    }
    return SceneENUM::NONE;
}

void MenuScene::draw(sf::RenderWindow& window) {
    window.draw(startButton);
    for (const auto& tekst : teksty) {
        window.draw(tekst);
    }
}