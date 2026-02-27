#pragma once
#include <SFML/Graphics.hpp>

// Lista dostępnych scen
enum class SceneENUM {
    NONE, // stay in current scene / do nothing
    MENU,
    APP
};

class Scene {
public:
    //destruktor
    virtual ~Scene() = default;

    virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) = 0;

    virtual SceneENUM update() = 0;

    virtual void draw(sf::RenderWindow& window) = 0;

};