#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

enum class MoveType {
	Normal, EnPassant, ShortCastle, LongCastle, PawnPromotion
};

struct Move {
	sf::Vector2f from, to;
	MoveType type = MoveType::Normal;

	bool operator==(sf::Vector2f to) const {
		return this->to == to;
	}
};

enum class PieceColor {
	WHITE,
	BLACK
};

enum class PieceType {
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
};

std::string makeFilePath(PieceColor color, PieceType type);

class Piece {
private:
	PieceColor color;
	PieceType type;
	sf::Vector2f position;
	sf::Texture texture;
	std::vector<Move> legalMoves;
	bool moved = false;

public:
	Piece(PieceColor color, PieceType type, sf::Vector2f position);

	bool operator==(const Piece& other) const;
	bool operator!=(const Piece& other) const;

	sf::Vector2f getPos() const;
	void setPos(sf::Vector2f newPos);

	PieceColor getColor() const;
	PieceType getType() const;
	
	void draw(sf::RenderWindow& window) const;

	std::vector<Move> getLegalMoves() const;
	void setLegalMoves(const std::vector<Move>& moves);
	bool canMoveTo(sf::Vector2f move) const;

	bool hasMoved() const;
	void markAsMoved();

	void promote(const PieceType& type);
};