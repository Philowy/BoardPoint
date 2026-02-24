#include "MenuScene.hpp"

MenuScene::MenuScene() {

    teksty.emplace_back("Menu", 81);
    teksty[0].setPosition({600, 200});

    teksty.emplace_back("Start", 72);
    teksty[1].setPosition({600, 400});

    // zielony guzik na środku
    startButton.setSize({200.f, 80.f});
    startButton.setFillColor(sf::Color::Green);
    startButton.setPosition({500.f, 360.f});
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

SceneRequest MenuScene::update() {
    if (wantsToStart) {
        return SceneRequest::APP;
    }
    return SceneRequest::NONE;
}

void MenuScene::draw(sf::RenderWindow& window) {
    window.draw(startButton);
    for (const auto& tekst : teksty) {
        window.draw(tekst);
    }
}