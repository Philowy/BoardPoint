#pragma once
#include "../Enums.hpp"
#include <SFML/Graphics.hpp>

void menuEvent(const std::optional<sf::Event>& event, Stage& gameState, sf::RenderWindow& window);

void menuMain(sf::RenderWindow& window);
