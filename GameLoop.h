#pragma once

#include <optional>
#include <iostream>	// only for debug
#include "Board.h"

class GameLoop {
private:
	sf::Color darkSquareColor, lightSquareColor, selectedSquareColor, checkedSquareColor;
	sf::Color moveIndicatorColor;

	Board* board;

	PieceColor turn;

	std::optional<sf::Vector2f> selectedSquare;

public:

	sf::RenderWindow window;

	GameLoop();
	void start();
	void drawBoard();

	void drawMoveIndicators();

	void eventLoop();
	void handleMouse(const sf::Event::MouseButtonPressed* mouse);

	void selectPiece(Piece& piece);
	void deselectPieces();
};
