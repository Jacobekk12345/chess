#pragma once

#include <optional>
#include <iostream>	// only for debug
#include "Board.h"

class GameLoop {
private:
	sf::Color darkSquareColor, lightSquareColor, selectedSquareColor, checkedSquareColor;
	sf::Color moveIndicatorColor;

	Board* board;

	std::optional<sf::Vector2f> selectedSquare;

	Piece* pawnAtBackrank();
	void drawPromotionView();

	bool finish = false;

	bool promoting = false;
	std::array<PieceType, 4> availablePromotions = { PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK, PieceType::QUEEN };
	std::array<sf::Vector2f, 4> promotionsPositions;
	const float promotionSelectorsScale = 1.5;

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
