#include "UpperPanel.hpp"
#include "../../windowSizeClass.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

UpperPanel::UpperPanel() :
                           dateText("", 20, true),
                           timeText("", 30, true),
                           speakerLabelText("Czas mowcy", 20, true),
                           speakerTimeText("", 30, true) {
    // Background setup
    background.setFillColor(sf::Color(40, 40, 40));

    // Logo setup
    if (logoTexture.loadFromFile("icon.png")) {
        logoSprite.emplace(logoTexture);
    }

    // Initial time update
    updateTimeAndDate();
    updateViewSize();
}

void UpperPanel::updateTimeAndDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);

    // Format Date (DD.MM.YYYY)
    std::ostringstream dateStream;
    dateStream << std::setfill('0') << std::setw(2) << now_tm->tm_mday << "."
               << std::setfill('0') << std::setw(2) << (now_tm->tm_mon + 1) << "."
               << (now_tm->tm_year + 1900);
    dateText.setString(dateStream.str());
    dateText.centerOrigin();

    // Format Time (HH:MM:SS)
    std::ostringstream timeStream;
    timeStream << std::setfill('0') << std::setw(2) << now_tm->tm_hour << ":"
               << std::setfill('0') << std::setw(2) << now_tm->tm_min << ":"
               << std::setfill('0') << std::setw(2) << now_tm->tm_sec;
    timeText.setString(timeStream.str());
    timeText.centerOrigin();

    // Speaker Time (currently just current time)
    speakerTimeText.setString(timeStream.str());
    speakerTimeText.centerOrigin();
}

void UpperPanel::update() {
    // Update time every second
    if (clock.getElapsedTime().asSeconds() >= 1.0f) {
        updateTimeAndDate();
        clock.restart();
        // re-center origin since text changed
        updateViewSize();
    }
}

void UpperPanel::draw(sf::RenderWindow& window) {
    // Always draw using default view to stay fixed at top
    sf::View oldView = window.getView();
    window.setView(window.getDefaultView());

    window.draw(background);
    if (logoSprite.has_value()) {
        window.draw(logoSprite.value());
    }

    // Center cluster
    window.draw(dateText);
    window.draw(timeText);

    // Right cluster
    window.draw(speakerLabelText);
    window.draw(speakerTimeText);

    window.setView(oldView);
}

void UpperPanel::updateViewSize() {
    float winX = windowSizeClass::getX();
    float panelHeight = 100.f; // Fixed height for upper panel

    // Background
    background.setPosition({0.f, 0.f});
    background.setSize({winX, panelHeight});

    // Logo (Left side)
    // Scale logo to fit height with some margin
    if (logoSprite.has_value() && logoTexture.getSize().y > 0) {
        float logoMargin = 10.f;
        float availableHeight = panelHeight - (2.f * logoMargin);
        float scale = availableHeight / static_cast<float>(logoTexture.getSize().y);
        logoSprite->setScale({scale, scale});
        logoSprite->setPosition({logoMargin, logoMargin});
    }

    // Date & Time (Center)
    dateText.setPosition({winX / 2.f, panelHeight * 0.35f});
    timeText.setPosition({winX / 2.f, panelHeight * 0.7f});

    // Speaker Time (Right side)
    float rightMargin = 100.f;
    speakerLabelText.setPosition({winX - rightMargin, panelHeight * 0.35f});
    speakerTimeText.setPosition({winX - rightMargin, panelHeight * 0.7f});
}