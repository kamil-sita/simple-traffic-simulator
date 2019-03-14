
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include "building.h"

Building::Building(int x, int y, int width, int height) {
    setPos(x, y);
    setSize(width, height);
}

void Building::setPos(int x, int y) {
    this->xPos = x;
    this->yPos = y;
}

void Building::setSize(int width, int height) {
    this->width = width;
    this->height = height;
}

void drawAt(sf::RenderWindow& window, int x, int y, int width, int height, sf::Color color) {
    sf::RectangleShape rect;
    rect.setPosition(x, y);
    rect.setFillColor(color);
    rect.setSize(sf::Vector2f(width, height));
    window.draw(rect);
}

void drawShadow(sf::RenderWindow& window, int x, int y, int width, int height, sf::Color color) {
    sf::ConvexShape convex;
    convex.setPointCount(6);
    convex.setPoint(0, sf::Vector2f(x, y));
    convex.setPoint(1, sf::Vector2f(x + width, y));
    convex.setPoint(2, sf::Vector2f(x + 1.3f * width, y + 0.3f * height));
    convex.setPoint(3, sf::Vector2f(x + 1.3f * width, y + 1.3f * height));
    convex.setPoint(4, sf::Vector2f(x + 0.3f * width, y + 1.3f * height));
    convex.setPoint(5, sf::Vector2f(x, y + height));
    convex.setFillColor(color);
    window.draw(convex);
}

void Building::draw(sf::RenderWindow& window) {
    drawAt(window, xPos, yPos, width, height, sf::Color(220, 180, 170)); //chodnik
    drawAt(window, xPos + 8, yPos + 8, width - 16, height - 16, sf::Color(70, 160, 70)); //trawnik
    drawShadow(window, xPos + 16, yPos + 16, width - 32, height - 32, sf::Color(0, 0, 0, 92)); //cień rzucany przez budynek
    drawAt(window, xPos + 16, yPos + 16, width - 32, height - 32, sf::Color(190, 190, 190)); //budynek
}
