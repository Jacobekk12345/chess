#ifndef SETUP_H
#define SETUP_H

#include <SFML/Graphics.hpp>

#include "Board.h"

class Setup {
private:
	sf::Color darkSquare;
	sf::Color lightSquare;
	sf::Color selectedSquare;
	sf::RectangleShape square;

public:
	sf::RenderWindow window;

	Setup();
	void windowLoop();
};

#endif
