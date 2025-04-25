#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <iostream>

#include "ChessPiece.h"

class Board {
private:
	std::vector<ChessPiece> piecesInGame;
	void createPieces(PieceColor color);
	bool move = true; // white = true, black = false
	sf::Vector2i selectedPiecePos;
public:
	Board();
	void renderPieces(sf::RenderWindow& window) const;
	ChessPiece* clicked(sf::Vector2i pos);
	void select(ChessPiece& piece);
	void deselectPieces();
	sf::Vector2i getSelectedPiecePos() const;
};

#endif