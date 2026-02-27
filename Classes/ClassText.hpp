#pragma once
#include <SFML/Graphics.hpp>

class ClassText : public sf::Text{
    private:
        inline static sf::Font font;
    public:
        void centerOrigin();

        ClassText(std::string text, int size);
        ClassText(std::string text, int size, bool centered);

        static void initialize() {
            if (!font.openFromFile("Fonts/OpenSans-VariableFont_wdth,wght.ttf")) {
                exit(69);
            }
            font = sf::Font("Fonts/OpenSans-VariableFont_wdth,wght.ttf");
        }
};