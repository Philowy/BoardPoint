#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>

#include "Classes/ClassText.hpp"
#include "windowSizeClass.hpp"

#include "Sceneries/Scene.hpp"
#include "Sceneries/MenuScene.hpp"
#include "Sceneries/AppScene.hpp"

auto main() -> int {
    ClassText::initialize();

    // get native resolution of your screen
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(
        desktopMode),
        "BoardPoint - std::unique_ptr",
        sf::Style::None
        );

    sf::Image icon;
    if (icon.loadFromFile("icon.png")) {
        window.setIcon({icon.getSize().x, icon.getSize().y}, icon.getPixelsPtr());
    }

    window.setFramerateLimit(60);

    // here I just take window size to windowSizeClass
    windowSizeClass::windowSize = window.getSize();

    //std::unique_ptr<Scene> currentScene = std::make_unique<MenuScene>();
    std::unique_ptr<Scene> currentScene = std::make_unique<AppScene>();


    while (window.isOpen()) {

        // ======= EVENTS =======

        while (const std::optional event = window.pollEvent()) {
            // for closing the app with "x"
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // passing event var to the active scene
            currentScene->handleEvent(*event, window);
        }

        // check if scene is changed
        SceneENUM request = currentScene->update();
        switch (request) {
            case SceneENUM::APP:
                currentScene = std::make_unique<AppScene>();
                break;

            case SceneENUM::MENU:
                currentScene = std::make_unique<MenuScene>();
                break;
        }

        window.clear();

        currentScene->draw(window);
        window.display();
    }
    return 0;
}