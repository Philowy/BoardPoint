#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>

#include "Classes/ClassText.hpp"

#include "Sceneries/Scene.hpp"
#include "Sceneries/MenuScene.hpp"
#include "Sceneries/AppScene.hpp"

auto main() -> int {
    ClassText::initialize();
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "BoardPoint - std::unique_ptr");

    std::unique_ptr<Scene> currentScene = std::make_unique<MenuScene>();


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            currentScene->handleEvent(*event, window);
        }

        //
        // odpowiedź ze sceny
        SceneRequest request = currentScene->update();

        switch (request) {
            case SceneRequest::APP:
                currentScene = std::make_unique<AppScene>();
                break;

            case SceneRequest::MENU:
                currentScene = std::make_unique<MenuScene>();
                break;
        }

        window.clear();

        currentScene->draw(window);
        window.display();
    }
    return 0;
}