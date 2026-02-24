#pragma once
#include <SFML/Graphics.hpp>

// Lista dostępnych scen
enum class SceneRequest {
    NONE, // zostań w tej scenie
    MENU,
    APP
};

class Scene {
public:
    virtual ~Scene() = default;

    virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) = 0;

    virtual SceneRequest update() = 0;

    virtual void draw(sf::RenderWindow& window) = 0;

};