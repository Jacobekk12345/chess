#include <vector>
#include "ChessPiece.h"

class Board {
private:
	std::vector<ChessPiece> piecesInGame;
	void createPieces(PieceColor color);
	bool move = true; // white = true, black = false
public:
	Board();
	void renderPieces(sf::RenderWindow& window) const;
	ChessPiece clicked(sf::Vector2i pos) const;
	void select(ChessPiece piece);
};