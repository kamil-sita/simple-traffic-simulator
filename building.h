#ifndef CISKITEST_BUILDING_H
#define CISKITEST_BUILDING_H
#include <SFML/Graphics.hpp>
#include "drawable.h"

///Reprezentuje budynek, ma podstawową logikę dotyczącą rysowania. Ponadto nie ma żadnych ciekawych funkcji
class Building : public Drawable {
private:
    int xPos;
    int yPos;
    int width;
    int height;
    void setPos(int x, int y);
    void setSize(int width, int height);
public:
    Building(int x, int y, int width, int height);
    void draw(sf::RenderWindow& window) override;
};



#endif