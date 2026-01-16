#include <SFML/Graphics.hpp>

#include "Classes/Test.hpp"

#include "Enums.hpp"

#include "Sceneries/Menu.hpp"
/*#include "Sceneries/PreGame.hpp"
#include "Sceneries/Game.hpp"
#include "Sceneries/Settings.hpp"
#include "Sceneries/Author.hpp"*/

auto main() -> int {
    auto window = sf::RenderWindow(
        sf::VideoMode({800,600}),
        "BoardPoint",
        sf::Style::Default,
        sf::State::Windowed,
        sf::ContextSettings{.antiAliasingLevel = 8}
    );

    auto windowState = MENU;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            switch (windowState) {
                case 0: menuEvent(event, windowState, window); break;
                //case 1: settingsEvent(event, windowState, window); break;
            }

            // gives different lighting when the mouse hovers
            //Button::backlightArea(sf::Mouse::getPosition(window));
        }

        switch (windowState) {
            case 0: menuMain(window); break;
            //case 1: settingsMain(window); break;
        }

        window.display();
    }

    return 0;
}
