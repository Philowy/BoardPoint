#pragma once
#include "Scene.hpp"
#include "../Classes/ClassText.hpp"

class MenuScene : public Scene {
private:
    sf::RectangleShape startButton;
    bool wantsToStart = false;

    std::vector<ClassText> teksty;

public:
    MenuScene();
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    SceneENUM update() override;
    void draw(sf::RenderWindow& window) override;
};
