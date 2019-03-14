#ifndef SIMULATOR_DRAWABLE_H
#define SIMULATOR_DRAWABLE_H

#include <SFML/Graphics/RenderWindow.hpp>

/**
 * Prosty interfejs, wskazujący na możliwość rysowania obiektu.
 */
class Drawable {
public:
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual ~Drawable() {};
};


#endif
