#ifndef PIECE_H
#define PIECE_H

#include <SFML/Graphics.hpp>

class Board;

enum class PieceColor {
	WHITE = 0, BLACK = 1
};

enum class PieceType {
	PAWN = 0, KNIGHT, BISHOP, ROOK, QUEEN, KING
};

class ChessPiece {
private:

	sf::Texture texture;
	sf::Vector2i boardPosition;
	PieceColor color;
	PieceType type;
	std::vector<sf::Vector2i> possibleMoves;

	std::vector<sf::Vector2i> pinDirections;
	std::vector<sf::Vector2i> pinningPiecesPositions;

	Board* board;

	void loadTexture(std::string imagePath);
public:

	ChessPiece(PieceColor color, PieceType type, sf::Vector2i position, Board* board);

	bool operator==(ChessPiece other);

	void place(sf::RenderWindow& window);

	sf::Vector2i getPosition() const;
	void setPosition(sf::Vector2i newPos);
	PieceType getType() const;
	PieceColor getColor() const;
	void promote();
	std::vector<sf::Vector2i> getPossibleMoves() const;
	void clearPossibleMoves();
	void clearPins();
	void calculatePossibleMoves();
	void calculateMovesPawn();
	void calculateMovesKnight();
	void calculateMovesBishop();
	void calculateMovesRook();
	void calculateMovesQueen();
	void calculateMovesKing();
};

std::string makeFilePath(PieceColor color, PieceType type);

#endif