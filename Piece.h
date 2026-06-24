#pragma once

#include <iostream>	// onyl for debug
#include <SFML/Graphics.hpp>

enum class MoveType {
	Normal, EnPassant, ShortCastle, LongCastle
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
	std::vector<Move> legalMoves;
	bool moved = false;

	sf::Texture texture;

public:
	Piece(PieceColor color, PieceType type, sf::Vector2f position);

	bool operator==(const Piece& other) const;
	bool operator!=(const Piece& other) const;

	sf::Vector2f getPos() const;
	void setPos(sf::Vector2f newPos);

	PieceColor getColor() const;
	PieceType getType() const;
	
	bool hasMoved() const;
	void markAsMoved();

	void promote(const PieceType& type);

	std::vector<Move> getLegalMoves() const;
	void setLegalMoves(const std::vector<Move>& moves);
	bool canMoveTo(sf::Vector2f move) const;

	void draw(sf::RenderWindow& window) const;

};