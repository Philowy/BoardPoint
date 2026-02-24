#pragma once
#include "Scene.hpp"
#include "../Classes/ClassText.hpp"

class AppScene : public Scene {
private:
    sf::RectangleShape startButton;
    bool wantsToReturn = false; // Flaga, czy kliknięto przycisk
    std::vector<ClassText> teksty;

public:
    AppScene();
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    SceneRequest update() override;
    void draw(sf::RenderWindow& window) override;
};
