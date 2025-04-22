#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>

#include <array>
#include <iostream>

#include "Board.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({ 1000, 800 }), "Fimos Chess");

    sf::Color darkSquare(118, 150, 86);
    sf::Color lightSquare(238, 238, 210);
    sf::Color selectedSquare(186, 202, 68);

    sf::RectangleShape square(sf::Vector2f(100.f, 100.f));

    Board board;

    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (event->is<sf::Event::MouseButtonPressed>()) {
                ChessPiece selectedPiece = board.clicked(mousePos);
                if (!selectedPiece.isBlank())
                    board.select(selectedPiece);
            }
                
        }
        window.clear();

        // draw the board
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                square.setPosition(sf::Vector2f(i * 100.f, j * 100.f));

//                if (sf::Vector2i(i, j) == newPiece.getPosition() && newPiece.isSelected)
  //                  square.setFillColor(selectedSquare);
                if ((i % 2 == 0) ^ (j % 2 == 0))
                    square.setFillColor(lightSquare);
                else
                    square.setFillColor(darkSquare);

                window.draw(square);
            }
        }

        board.renderPieces(window);

        window.display();
    }
}