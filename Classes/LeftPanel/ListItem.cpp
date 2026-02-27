#include "ListItem.hpp"

ListItem::ListItem(std::string text, int lettersSize, sf::Vector2f size) : text(text, lettersSize) {
    total += 1;
    box.setFillColor(sf::Color(0, 0, 0, 0));
    box.setSize(size);
}

ListItem::~ListItem() {
    total -= 1;
}

sf::String ListItem::getString() {
    return text.getString();
}

void ListItem::addCharacter(char c) {
    std::string current = text.getString();
    text.setString(current + c);
}

void ListItem::removeLastCharacter() {
    std::string current = text.getString();
    if (!current.empty()) {
        current.pop_back();
        text.setString(current);
    }
}

void ListItem::setBackgroundColor(sf::Color color) {
    box.setFillColor(color);
}

void ListItem::setPosition(sf::Vector2f pos) {
    box.setPosition(pos);
    text.setPosition(pos);
}

sf::FloatRect ListItem::getGlobalBounds() const {
    return box.getGlobalBounds();
}

void ListItem::updateTextPosition() {
    isSubpoint ? this->text.setPosition(box.getPosition()) : this->text.setPosition({box.getPosition().x + windowSizeClass::getX()/10.f,box.getPosition().y});
}

void ListItem::draw(sf::RenderWindow& window) const {
    window.draw(box);
    window.draw(text);
}