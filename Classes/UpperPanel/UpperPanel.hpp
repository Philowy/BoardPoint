#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "../ClassText.hpp"

class UpperPanel {
private:
    sf::RectangleShape background;

    sf::Texture logoTexture;
    std::optional<sf::Sprite> logoSprite;

    ClassText dateText;
    ClassText timeText;

    // Zmienne na tymczasowy zegar mówcy
    ClassText speakerTimeText;
    ClassText speakerLabelText;

    // Time update timer
    sf::Clock clock;

    void updateTimeAndDate();

public:
    UpperPanel();

    void update();
    void draw(sf::RenderWindow& window);
    void updateViewSize();
};