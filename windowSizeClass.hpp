#pragma once
#include <SFML/Graphics.hpp>

class windowSizeClass {
public:
    static inline sf::Vector2u windowSize;

    static float getX() {
        return windowSize.x;
    }

    static float getY() {
        return windowSize.y;
    }
};
