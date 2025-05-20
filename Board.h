#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <iostream>

#include "ChessPiece.h"

class Board {
private:
	std::vector<ChessPiece> piecesInGame;
	void createPieces(PieceColor color);
	PieceColor move = PieceColor::WHITE;
	sf::Vector2i selectedPiecePos;
public:
	Board();
	void renderPieces(sf::RenderWindow& window);
	ChessPiece* getPiece(sf::Vector2i pos) const;
	void select(ChessPiece& piece);
	void deselectPieces();
	sf::Vector2i getSelectedPiecePos() const;
	ChessPiece* getSelectedPiece() const;
	PieceColor getMove() const;
	void movePiece(ChessPiece* piece, sf::Vector2i moveTo);
	void calculateMoves(PieceColor move);
	bool checkForCheckmate(PieceColor move) const;
	void endGame();
	void capture(ChessPiece piece);
};

#endif