#pragma once
#include "Scene.hpp"
#include "../Classes/ClassText.hpp"
#include "../Classes/LeftPanel/LeftPanel.hpp"
#include "../windowSizeClass.hpp"

class AppScene : public Scene {
private:
    LeftPanel leftPanel;

    sf::RectangleShape startButton;
    bool wantsToReturn = false; // Flag, if button pressed
    std::vector<ClassText> teksty;

public:
    AppScene();
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    SceneENUM update() override;
    void draw(sf::RenderWindow& window) override;
};
