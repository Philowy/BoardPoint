#include "ListItem.hpp"
#include <algorithm>

ListItem::ListItem(std::string textStr, int lettersSize, sf::Vector2f size) : text(textStr, lettersSize) {
    total += 1;
    rawText = textStr;
    baseTextSize = lettersSize;
    box.setFillColor(sf::Color(0, 0, 0, 0));
    setSize(size);
}

ListItem::~ListItem() {
    total -= 1;
}

sf::String ListItem::getString() {
    return rawText;
}

void ListItem::addCharacter(char c) {
    rawText += c;
    wrapText();
}

void ListItem::removeLastCharacter() {
    if (!rawText.empty()) {
        rawText.pop_back();
        wrapText();
    }
}

void ListItem::setBackgroundColor(sf::Color color) {
    box.setFillColor(color);
}

void ListItem::setState(State state) {
    switch (state) {
        case State::Default:
            box.setFillColor(sf::Color(0, 0, 0, 0));
            break;
        case State::Hovered:
            box.setFillColor(sf::Color(0, 0, 40, 50));
            break;
        case State::Selected:
            box.setFillColor(sf::Color(80, 80, 80, 200));
            break;
    }
}

void ListItem::setSize(sf::Vector2f size) {
    currentMaxWidth = size.x;
    currentMinHeight = size.y;

    wrapText();
}

void ListItem::wrapText() {
    if (currentMaxWidth <= 20.f) return;

    std::string wrapped = "";
    std::string currentLine = "";
    std::string currentWord = "";

    for (size_t i = 0; i < rawText.length(); ++i) {
        char c = rawText[i];

        // user pressed enter manually
        if (c == '\n') {
            wrapped += currentLine + currentWord + "\n";
            currentLine = "";
            currentWord = "";
            continue;
        }

        // check if a single word without spaces is longer than the panel
        this->text.setString(currentWord + c);
        if (this->text.getLocalBounds().size.x > currentMaxWidth - 10.f) {
            if (!currentLine.empty()) {
                wrapped += currentLine + "\n";
                currentLine = "";
            }
            wrapped += currentWord + "\n";
            currentWord = std::string(1, c);
            continue;
        }

        currentWord += c;

        // check line width when space is hit or text ends
        if (c == ' ' || i == rawText.length() - 1) {
            this->text.setString(currentLine + currentWord);

            // move whole word to new line if it exceeds max width
            if (this->text.getLocalBounds().size.x > currentMaxWidth - 10.f) {
                wrapped += currentLine + "\n";
                currentLine = currentWord;
                currentWord = "";
            } else {
                currentLine += currentWord;
                currentWord = "";
            }
        }
    }

    wrapped += currentLine + currentWord;
    this->text.setString(wrapped);

    // adjust box height based on wrapped text
    float textHeight = this->text.getLocalBounds().size.y;
    box.setSize({currentMaxWidth, std::max(currentMinHeight, textHeight + 20.f)});
}

void ListItem::setPosition(sf::Vector2f pos) {
    box.setPosition(pos);
    text.setPosition(pos);
}

sf::FloatRect ListItem::getGlobalBounds() const {
    return box.getGlobalBounds();
}

void ListItem::draw(sf::RenderWindow& window) const {
    window.draw(box);
    window.draw(text);
}

void ListItem::toggleSubpoint() {
    isSubpoint = !isSubpoint;
    int newSize = isSubpoint ? (baseTextSize - 10) : baseTextSize;
    text.setCharacterSize(newSize);

    wrapText();
}