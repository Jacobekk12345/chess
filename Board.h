#pragma once

#include <vector>
#include <array>
#include <map>
#include "Piece.h"

struct MoveInfo {
	Move move;
	Piece piece;
	std::optional<Piece> capturedPiece;
	PieceType promotionTo;
	bool isCheck, isMate;

	MoveInfo(Move move, Piece piece): move(move), piece(piece) {
		isCheck = false; isMate = false;
	}
};

class Board {
private:
	std::vector<Piece> pieces;
	std::optional<Move> lastMove;
	PieceColor sideToMove = PieceColor::WHITE;

	int halfMoveClock = 0;
	int moveCount = 1;

	bool wouldBeInCheck(Piece piece, sf::Vector2f moveTo);

	Piece* kingAndMinor(std::vector<Piece> side) const;
	bool isLightSquare(Piece* bishop) const;

	std::vector<Move> getPawnMoves(Piece& pawn);
	std::vector<Move> getKnightMoves(Piece& knight);
	std::vector<Move> getBishopMoves(Piece& bishop);
	std::vector<Move> getRookMoves(Piece& rook);
	std::vector<Move> getQueenMoves(Piece& queen);
	std::vector<Move> getKingMoves(Piece& king, bool checkCastling = true);

	std::string disambiguity;
	std::string getDisambiguity() const;
	void checkDisambiguity(Piece movedPiece, Move move);

	std::optional<MoveInfo> lastMoveInfo;
	
	std::string pgn, moveHistory;

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

	void movePiece(Piece& piece, sf::Vector2f moveTo, bool actuallyMoving = true);
	void calculateLegalMoves();
	bool isInCheck(PieceColor color);
	void promotePawn(PieceType type);

	std::map<Piece*, sf::Vector2f> getPinnedPieces();

	bool isCheckmated(PieceColor color);
	int getHalfMoveClock() const;
	std::string stringify();
	bool insufficientMaterial() const;

	void moveToPGN(MoveInfo moveInfo);

	void increaseMoveCount();

	std::string getMoveHistory() const;

	void finishPGNMove(PieceColor enemy, PieceType promotionType = PieceType::EMPTY);
	std::string getPGN(std::string result);
};