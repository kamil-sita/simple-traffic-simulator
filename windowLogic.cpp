
#include <SFML/System/Clock.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include "windowLogic.h"
#include "settings.h"
#include "city.h"


void createAndRunWindow(SimulationSettings settings) {
    sf::RenderWindow window(sf::VideoMode(settings.getWindowWidth(), settings.getWindowHeight()), "Symulator ruchu");
    sf::Clock clock;
    window.setFramerateLimit(60);

    City city(settings.getWindowHeight(), settings.getWindowWidth(), settings.getHorizontalBuildingCount(), settings.getVerticalBuildingCount(), settings);

    window.requestFocus();

    while (window.isOpen()) {
        sf::Event event {};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                std::cout << "Ending: closing window" << std::endl;
                window.close();
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 'R' || event.text.unicode == 'r') {
                    city.removeFirstCar();
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 'A' || event.text.unicode == 'a') {
                    city.addCar();
                }
            }
        }
        city.update();
        city.draw(window);
        window.display();
    }
}
