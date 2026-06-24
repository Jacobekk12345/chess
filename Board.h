#pragma once

#include <vector>
#include <array>
#include <map>
#include "Piece.h"

class Board {
private:
	std::vector<Piece> pieces;
	std::optional<Move> lastMove;
	PieceColor sideToMove = PieceColor::WHITE;

	int halfMoveClock = 0;

	bool wouldBeInCheck(Piece piece, sf::Vector2f moveTo);

	Piece* kingAndMinor(std::vector<Piece> side) const;
	bool isLightSquare(Piece* bishop) const;

	std::vector<Move> getPawnMoves(Piece& pawn);
	std::vector<Move> getKnightMoves(Piece& knight);
	std::vector<Move> getBishopMoves(Piece& bishop);
	std::vector<Move> getRookMoves(Piece& rook);
	std::vector<Move> getQueenMoves(Piece& queen);
	std::vector<Move> getKingMoves(Piece& king, bool checkCastling = true);

public:
	std::unordered_map<std::string, int> positionHistory;

	Board();

	void renderPieces(sf::RenderWindow& window) const;
	void createStartingPos();

	Piece* getPiece(sf::Vector2f position) const;
	std::vector<Piece*> getPieces(PieceColor color, PieceType type);
	std::vector<Piece*> getPieces(PieceColor color);
	std::vector<Piece*> getPieces(PieceType type);
	std::vector<Piece*> getPieces();
	Piece* getKing(PieceColor color);

	PieceColor getSideToMove() const;
	void switchSideToMove();

	std::optional<Move> getLastMove() const;

	void movePiece(Piece& piece, sf::Vector2f moveTo, PieceType promotionType = PieceType::QUEEN);
	void calculateLegalMoves();
	bool isInCheck(PieceColor color);
	void promotePawn(PieceType type);

	std::map<Piece*, sf::Vector2f> getPinnedPieces();

	bool isCheckmated(PieceColor color);
	int getHalfMoveClock() const;
	std::string stringify();
	bool insufficientMaterial() const;
};