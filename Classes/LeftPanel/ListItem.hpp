#pragma once
#include <SFML/Graphics.hpp>
#include "../ClassText.hpp"

class ListItem {
public:
    enum class State { Default, Hovered, Selected };

private:
    static inline int total;

    sf::RectangleShape box;
    bool selected;
    bool ifMainPoint;
    ClassText text;

    std::string rawText;
    float currentMaxWidth = 0.f;
    float currentMinHeight = 0.f;
    int baseTextSize;

    void wrapText();

public:
    bool isSubpoint = false;
    int listNumber = 0;

    sf::String getString();
    void addCharacter(char c);
    void removeLastCharacter();
    void setBackgroundColor(sf::Color color);
    void setState(State state);
    void setListNumber(int num);

    ListItem(std::string text, int lettersSize,  sf::Vector2f size);
    ~ListItem();

    void setPosition(sf::Vector2f pos);
    sf::FloatRect getGlobalBounds() const;
    void draw(sf::RenderWindow& window) const;
    void setSize(sf::Vector2f size);
    void toggleSubpoint();
};