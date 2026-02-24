#pragma once
#include <SFML/Graphics.hpp>

class ClassText : public sf::Text{
    private:
        inline static sf::Font font;
    public:
        void centerOrigin();

        ClassText(std::string text, int size);

        static void initialize() {
            if (!font.openFromFile("Fonts/OpenSans-VariableFont_wdth,wght.ttf")) {
                exit(1);
            }
            font = sf::Font("Fonts/OpenSans-VariableFont_wdth,wght.ttf");
        }
};