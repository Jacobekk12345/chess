#include "Board.h"

Board::Board() {
	createPieces(PieceColor::BLACK);
	createPieces(PieceColor::WHITE);

	this->calculateMoves(this->move);

	this->selectedPiecePos = sf::Vector2i(-1, -1);
}

void Board::renderPieces(sf::RenderWindow& window) {
    for (auto& piece : this->piecesInGame)
		piece.place(window);
}

void Board::createPieces(PieceColor color) {
	std::array<PieceType, 8> pieceTypes = { PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN, PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK };

	for (int i = 0; i < 8; i++) {
		this->piecesInGame.push_back(ChessPiece(color, pieceTypes[i], sf::Vector2i(i, (color == PieceColor::BLACK ? 0 : 7)), this));
	}
	for (int i = 0; i < 8; i++) {
		this->piecesInGame.push_back(ChessPiece(color, PieceType::PAWN, sf::Vector2i(i, (color == PieceColor::BLACK ? 1 : 6)), this));
	}
}

ChessPiece* Board::getPiece(sf::Vector2i pos) const {
	for (auto& piece : this->piecesInGame) {
		if (piece.getPosition() == pos)
			return const_cast<ChessPiece*>(&piece);
	}
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

PieceColor Board::getMove() const {
	return this->move;
}

void Board::movePiece(ChessPiece* piece, sf::Vector2i moveTo) {
	this->deselectPieces();

	std::vector<sf::Vector2i> legalMoves = piece->getPossibleMoves();

	auto it = std::find(legalMoves.begin(), legalMoves.end(), moveTo);

	if (it != legalMoves.end()) {

		ChessPiece* capturedPiece = getPiece(moveTo);

		piece->setPosition(moveTo);
	
		if (capturedPiece != nullptr)
			capture(*capturedPiece);

		if (this->move == PieceColor::WHITE)
			this->move = PieceColor::BLACK;
		else
			this->move = PieceColor::WHITE;

		this->calculateMoves(this->move);
		if (this->checkForCheckmate(this->move))
			this->endGame();
	}
}

void Board::capture(ChessPiece piece) {
	this->piecesInGame.erase(std::remove(this->piecesInGame.begin(), this->piecesInGame.end(), piece), this->piecesInGame.end());
}

ChessPiece* Board::getSelectedPiece() const {
	return this->getPiece(this->getSelectedPiecePos());
}

void Board::calculateMoves(PieceColor move) {
	for (auto& piece : this->piecesInGame) {
		if (piece.getColor() == move)
			piece.clearPossibleMoves();
			piece.calculatePossibleMoves();
	}
}

bool Board::checkForCheckmate(PieceColor move) const {
	for (auto& piece : this->piecesInGame) {
		if (piece.getColor() == move)
			if (piece.getPossibleMoves().size() != 0)
				return false;
	}
	return true;
}

void Board::endGame() { // TODO
	return;
}
