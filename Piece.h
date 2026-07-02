#pragma once

#include <iostream>	// only for debug
#include "Utils.h"
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
inline PieceColor operator!(PieceColor color) {
	return (color == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE);
}
inline std::ostream& operator<<(std::ostream& os, const PieceColor& color)
{
	os << (color == PieceColor::WHITE ? "white" : "black");
	return os;
}

enum class PieceType {
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
	EMPTY
};
inline std::ostream& operator<<(std::ostream& os, const PieceType& type)
{
	switch (type) {
	case PieceType::PAWN: os << "pawn"; break;
	case PieceType::KNIGHT: os << "knight"; break;
	case PieceType::BISHOP: os << "bishop"; break;
	case PieceType::ROOK: os << "rook"; break;
	case PieceType::QUEEN: os << "queen"; break;
	case PieceType::KING: os << "king"; break;
	case PieceType::EMPTY: os << "empty"; break;
	}
	return os;
}

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

inline std::ostream& operator<<(std::ostream& os, const Piece& p)
{
	os << "Color: " << p.getColor() << ", Type: " << p.getType() << ", Position: (" << p.getPos().x << ", " << p.getPos().y << ")\n";
	return os;
}