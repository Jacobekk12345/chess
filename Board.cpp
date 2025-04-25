#include "Board.h"

Board::Board() {
	createPieces(PieceColor::BLACK);
	createPieces(PieceColor::WHITE);

	this->selectedPiecePos = sf::Vector2i(-1, -1);
}

void Board::renderPieces(sf::RenderWindow& window) const {
	for (ChessPiece piece : this->piecesInGame)
		piece.place(window);
}

void Board::createPieces(PieceColor color) {
	std::array<PieceType, 8> pieceTypes = { PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN, PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK };

	for (int i = 0; i < 8; i++) {
		this->piecesInGame.push_back(ChessPiece(color, pieceTypes[i], sf::Vector2i(i, (color == PieceColor::BLACK ? 0 : 7))));
	}
	for (int i = 0; i < 8; i++) {
		this->piecesInGame.push_back(ChessPiece(color, PieceType::PAWN, sf::Vector2i(i, (color == PieceColor::BLACK ? 1 : 6))));
	}
}

ChessPiece* Board::clicked(sf::Vector2i pos) {
	for (ChessPiece piece : this->piecesInGame) {
		if (piece.getPosition() == pos)
			return &piece;
	}
	this->deselectPieces();
	return nullptr;
}

void Board::deselectPieces() {
	this->selectedPiecePos = sf::Vector2i(-1, -1);
}

void Board::select(ChessPiece& piece) {
	this->selectedPiecePos = piece.getPosition();
}

sf::Vector2i Board::getSelectedPiecePos() const {
	return this->selectedPiecePos;
}
