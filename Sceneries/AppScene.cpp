#include "AppScene.hpp"


AppScene::AppScene() {
    teksty.emplace_back("App", 81);
    teksty[0].setPosition({600, 200});

    teksty.emplace_back("BACK", 72);
    teksty[1].setPosition({600, 400});

    // zielony guzik na środku
    startButton.setSize({200.f, 80.f});
    startButton.setFillColor(sf::Color::Red);
    startButton.setPosition({500.f, 360.f});
}

void AppScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (const auto* press = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (press->button == sf::Mouse::Button::Left) {

            sf::Vector2f mPos = window.mapPixelToCoords(press->position);

            if (startButton.getGlobalBounds().contains(mPos)) {
                wantsToReturn = true;
            }
        }
    }
}

SceneRequest AppScene::update() {

    if (wantsToReturn) {
        return SceneRequest::MENU;
    }
    return SceneRequest::NONE;
}

void AppScene::draw(sf::RenderWindow& window) {
    window.draw(startButton);
    for (const auto& tekst : teksty) {
        window.draw(tekst);
    }
}