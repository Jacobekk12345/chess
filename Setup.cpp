#include "Setup.h"

Setup::Setup() {
	window = sf::RenderWindow(sf::VideoMode({ 1000, 800 }), "Chess");

    darkSquare = sf::Color(118, 150, 86);
    lightSquare = sf::Color(238, 238, 210);
    selectedSquare = sf::Color(186, 202, 68);

    square = sf::RectangleShape(sf::Vector2f(100.f, 100.f));

	windowLoop();
}

void Setup::windowLoop() {

    Board board;

    while (window.isOpen()) {
        eventLoop(board);
        
        window.clear();
        drawBoard(board);
    }
}

void Setup::eventLoop(Board& board) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window) / 100;

    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            ChessPiece* selectedPiece = board.clicked(mousePos);
            if (selectedPiece)
                board.select(*selectedPiece);
        }

    }
}

void Setup::drawBoard(Board& board) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            square.setPosition(sf::Vector2f(i * 100.f, j * 100.f));

            if (sf::Vector2i(i, j) == board.getSelectedPiecePos())
                square.setFillColor(selectedSquare);
            else if ((i % 2 == 0) ^ (j % 2 == 0))
                square.setFillColor(lightSquare);
            else
                square.setFillColor(darkSquare);

            window.draw(square);
        }
    }

    board.renderPieces(window);

    window.display();
}
