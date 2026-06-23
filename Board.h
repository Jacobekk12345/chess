#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <map>
#include "Piece.h"

class Board {
private:
	std::vector<Piece> pieces;
	std::optional<Move> lastMove;
	PieceColor sideToMove = PieceColor::WHITE;

	std::vector<Move> getPawnMoves(Piece& pawn);
	std::vector<Move> getKnightMoves(Piece& knight);
	std::vector<Move> getBishopMoves(Piece& bishop);
	std::vector<Move> getRookMoves(Piece& rook);
	std::vector<Move> getQueenMoves(Piece& queen);
	std::vector<Move> getKingMoves(Piece& king, bool checkCastling = true);

	bool wouldBeInCheck(Piece piece, sf::Vector2f moveTo);

	int halfMoveClock = 0;
public:
	Board();

	PieceColor getSideToMove() const;
	void switchSideToMove();

	void renderPieces(sf::RenderWindow& window) const;
	void createStartingPos();
	Piece* getPiece(sf::Vector2f position) const;

	void movePiece(Piece& piece, sf::Vector2f moveTo, PieceType promotionType = PieceType::QUEEN);

	std::optional<Move> getLastMove() const;

	std::vector<Piece*> getPieces(PieceColor color, PieceType type);
	std::vector<Piece*> getPieces(PieceColor color);
	std::vector<Piece*> getPieces(PieceType type);
	std::vector<Piece*> getPieces();
	Piece* getKing(PieceColor color);

	std::map<Piece*, sf::Vector2f> getPinnedPieces();

	void calculateLegalMoves();

	bool isInCheck(PieceColor color);

	void promotePawn(PieceType type);

	bool isCheckmated(PieceColor color);

	int getHalfMoveClock() const;

	std::string stringify();
	std::unordered_map<std::string, int> positionHistory;
};