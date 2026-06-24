#include "Board.h"

Board::Board() : lastMove(std::nullopt) {
	createStartingPos();
	calculateLegalMoves();
}

void Board::renderPieces(sf::RenderWindow& window) const {
	for (Piece piece : pieces) {
		piece.draw(window);
	}
}

void Board::createStartingPos() {
	std::array<PieceType, 8> piecesRow = {PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN, PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK};

	for (float i = 0; i < piecesRow.size(); i++) {
		pieces.push_back(Piece(PieceColor::WHITE, piecesRow.at(i), { i, 7.f }));	// white pieces row
		pieces.push_back(Piece(PieceColor::BLACK, piecesRow.at(i), { i, 0.f }));	// black pieces row
	}

	for (float i = 0; i < 8; i++) {
		pieces.push_back(Piece(PieceColor::WHITE, PieceType::PAWN, {i, 6.f}));
		pieces.push_back(Piece(PieceColor::BLACK, PieceType::PAWN, {i, 1.f}));
	}
}

void Board::movePiece(Piece& piece, sf::Vector2f moveTo, PieceType promotionType) {
	auto it = std::find_if(pieces.begin(), pieces.end(), [&](const Piece& p) { return p.getPos() == moveTo && p != piece; });
	MoveType type = MoveType::Normal;

	if (piece.getType() == PieceType::PAWN) {
		if (piece.getPos().x != moveTo.x && !getPiece(moveTo))
			type = MoveType::EnPassant; // moving diagonally but no piece there = en passant

	}
	else if (piece.getType() == PieceType::KING) {
		if (std::abs(moveTo.x - piece.getPos().x) == 2)
			type = moveTo.x > piece.getPos().x ? MoveType::ShortCastle : MoveType::LongCastle;
	}

	lastMove = { piece.getPos(), moveTo, type };

 	piece.setPos(moveTo);
	piece.markAsMoved();

	switch (type) {
	case MoveType::EnPassant: {
		int captureDir = piece.getColor() == PieceColor::WHITE ? 1 : -1;	// ik that white moves pawn move in negative but its easier like this since its checking behind the pawn
		it = std::find_if(pieces.begin(), pieces.end(), [&](const Piece& p) { return p.getPos() == sf::Vector2f{moveTo.x, moveTo.y + captureDir }&& p != piece; });
		break;
	}
	case MoveType::ShortCastle: {
		auto kingSideRook = getPiece({ 7, piece.getPos().y });
		kingSideRook->setPos({ moveTo.x - 1, moveTo.y });
		break;
	}
	case MoveType::LongCastle: {
		auto kingSideRook = getPiece({ 0, piece.getPos().y });
		kingSideRook->setPos({ moveTo.x + 1, moveTo.y });
		break;
	}
	}

	if (piece.getType() != PieceType::PAWN)
		halfMoveClock++;
	else
		halfMoveClock = 0;

	if (it != pieces.end()) {
		pieces.erase(it);
		halfMoveClock = 0;
	}

}

void Board::promotePawn(PieceType type) {
	for (auto pawn : getPieces(PieceType::PAWN)) {
		int y = pawn->getPos().y;
		if (y == 0 || y == 7) {
			pawn->promote(type);
			break;
		}
	}
}

std::optional<Move> Board::getLastMove() const {
	return lastMove;
}

Piece* Board::getPiece(sf::Vector2f position) const {
	for (const Piece& piece : pieces) {
		if (piece.getPos() == position)
			return const_cast<Piece*>(&piece);
	}
	return nullptr;
}
std::vector<Piece*> Board::getPieces(PieceColor color, PieceType type) {
	std::vector<Piece*> matchingPieces;
	
	for (Piece& piece : pieces)
		if (piece.getColor() == color && piece.getType() == type)
			matchingPieces.push_back(&piece);

	return matchingPieces;
}
std::vector<Piece*> Board::getPieces(PieceColor color) {
	std::vector<Piece*> matchingPieces;

	for (Piece& piece : pieces)
		if (piece.getColor() == color)
			matchingPieces.push_back(&piece);

	return matchingPieces;
}
std::vector<Piece*> Board::getPieces(PieceType type) {
	std::vector<Piece*> matchingPieces;

	for (Piece& piece : pieces)
		if (piece.getType() == type)
			matchingPieces.push_back(&piece);

	return matchingPieces;
}

std::vector<Piece*> Board::getPieces() {
	std::vector<Piece*> piecesCopy;

	for (Piece& piece : pieces)
		piecesCopy.push_back(&piece);

	return piecesCopy;
}


void Board::calculateLegalMoves() {
	auto pinnedPieces = getPinnedPieces();
	for (auto piece : getPieces()) {
		std::vector<Move> moves;
		switch (piece->getType()) {
		case PieceType::PAWN: moves = getPawnMoves(*piece); break;
		case PieceType::KNIGHT: moves = getKnightMoves(*piece); break;
		case PieceType::BISHOP: moves = getBishopMoves(*piece); break;
		case PieceType::ROOK: moves = getRookMoves(*piece); break;
		case PieceType::QUEEN: moves = getQueenMoves(*piece); break;
		case PieceType::KING: moves = getKingMoves(*piece); break;
		}

		if (piece->getType() == PieceType::KING) {
			moves.erase(std::remove_if(moves.begin(), moves.end(), [&](Move move) {
				return wouldBeInCheck(*piece, move.to);
				}), moves.end());
		}
		else {
			if (isInCheck(piece->getColor())) {
				moves.erase(std::remove_if(moves.begin(), moves.end(), [&](Move move) {
					return wouldBeInCheck(*piece, move.to);
					}), moves.end());
			}
			if (pinnedPieces.count(piece)) {
				sf::Vector2f pinRay = pinnedPieces[piece];
				moves.erase(std::remove_if(moves.begin(), moves.end(), [&](Move move) {
					sf::Vector2f dir = move.to - move.from;
					return !(dir.x * pinRay.y == dir.y * pinRay.x);
					}), moves.end());
			}
		}
		
		piece->setLegalMoves(moves);
	}
}

std::vector<Move> Board::getPawnMoves(Piece& pawn) {
	std::vector<Move> moves;
	auto [x, y] = pawn.getPos();
	int dir = pawn.getColor() == PieceColor::WHITE ? -1 : 1;

	if (!getPiece({ x, y + dir })) {
		moves.push_back({ pawn.getPos(), { x, y + dir}});
		if (!pawn.hasMoved() && !getPiece({ x, y + dir * 2 }))
			moves.push_back({ pawn.getPos(), { x, y + dir * 2 } });
	}

	for (int dx : {-1, 1}) {
		auto target = getPiece({ x + dx, y + dir });
		if (target && target->getColor() != pawn.getColor())
			moves.push_back({ pawn.getPos(), { x + dx, y + dir } });

		if (lastMove) {	// en passant
			auto target = getPiece({ x + dx, y });
			if (target &&
				target->getColor() != pawn.getColor() &&
				(*lastMove).to == target->getPos() &&
				(*lastMove).from == sf::Vector2f(x + dx, y + dir * 2))
				moves.push_back({ pawn.getPos(), { x + dx, y + dir }, MoveType::EnPassant });
		}
		
	}

	return moves;
}
std::vector<Move> Board::getKnightMoves(Piece& knight) {
	std::vector<Move> moves;
	auto [x, y] = knight.getPos();

	for (int d1 : {-1, 1}) {
		for (int d2 : {-2, 2}) {
			for (sf::Vector2f pos : std::vector<sf::Vector2f>{{x + d1, y + d2}, {x + d2, y + d1}})
				if (pos.x >= 0 && pos.x <= 7 && pos.y >= 0 && pos.y <= 7)
					if (!getPiece(pos) || getPiece(pos)->getColor() != knight.getColor())
						moves.push_back({knight.getPos(), pos});
		}
	}

	return moves;
}
std::vector<Move> Board::getBishopMoves(Piece& bishop) {
	std::vector<Move> moves;

	std::vector<sf::Vector2f> dirs = {
		{-1, -1},
		{-1, 1},
		{1, -1},
		{1, 1}
	};

	for (auto dir : dirs) {
		auto pos = bishop.getPos();
		while (true) {
			pos += dir;
			if (pos.x < 0 || pos.x > 7 || pos.y < 0 || pos.y > 7)
				break;
			if (auto piece = this->getPiece(pos)) {
				if (piece->getColor() != bishop.getColor())
					moves.push_back({ bishop.getPos(), pos });
				break;
			}	
			moves.push_back({ bishop.getPos(), pos });
		}
	}
	
	return moves;
}
std::vector<Move> Board::getRookMoves(Piece& rook) {
	std::vector<Move> moves;

	std::vector<sf::Vector2f> dirs = {
		{-1, 0},
		{1, 0},
		{0, -1},
		{0, 1}
	};

	for (auto dir : dirs) {
		auto pos = rook.getPos();
		while (true) {
			pos += dir;
			if (pos.x < 0 || pos.x > 7 || pos.y < 0 || pos.y > 7)
				break;
			if (auto piece = this->getPiece(pos)) {
				if (piece->getColor() != rook.getColor())
					moves.push_back({ rook.getPos(), pos });
				break;
			}
			moves.push_back({ rook.getPos(), pos });
		}
	}

	return moves;
}
std::vector<Move> Board::getQueenMoves(Piece& queen) {
	std::vector<Move> moves, rookMoves = getRookMoves(queen), bishopMoves = getBishopMoves(queen);

	moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
	moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
	
	return moves;
}

std::vector<Move> Board::getKingMoves(Piece& king, bool checkCastling) {
	std::vector<Move> moves;

	auto[x, y] = king.getPos();

	if (checkCastling && !isInCheck(king.getColor()) && !king.hasMoved()) {
		// kingside
		if (!getPiece({ 6, y }) && !getPiece({ 5, y }))
			if (auto kingSideRook = getPiece({ 7, y }))
				if (!kingSideRook->hasMoved()) {
					sf::Vector2f passingSquare = { x + 1, y },
								 landingSquare = { x + 2, y };
					if (!wouldBeInCheck(king, passingSquare) && !wouldBeInCheck(king, landingSquare))
						moves.push_back({ king.getPos(), landingSquare, MoveType::ShortCastle });
			
				}
		// queenside
		if (!getPiece({ 3, y }) && !getPiece({ 2, y }) && !getPiece({ 1, y }))
			if (auto queenSideRook = getPiece({ 0, y }))
				if (!queenSideRook->hasMoved()) {
					sf::Vector2f passingSquare = { x - 1, y },
								 landingSquare = { x - 2, y };
					if (!wouldBeInCheck(king, passingSquare) && !wouldBeInCheck(king, landingSquare))
						moves.push_back({ king.getPos(), landingSquare, MoveType::LongCastle });
				}
			
		
	}

	for (int dx : {-1, 0, 1}) {
		for (int dy : {-1, 0, 1}) {
			if (dx == 0 && dy == 0)
				continue;
			sf::Vector2f pos = { x + dx, y + dy };
			if (pos.x < 0 || pos.x > 7 || pos.y < 0 || pos.y > 7)
				continue;
			if (auto piece = getPiece(pos))
				if (piece->getColor() == king.getColor())
					continue;
			moves.push_back({ {x, y}, pos});
		}
	}
	return moves;
}

bool Board::isInCheck(PieceColor color) {
    Piece* king = this->getKing(color);
    PieceColor opponent = (color == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;

    for (auto piece : this->getPieces(opponent)) {
        std::vector<Move> moves;
        switch (piece->getType()) {
        case PieceType::PAWN:   moves = getPawnMoves(*piece);   break;
        case PieceType::KNIGHT: moves = getKnightMoves(*piece); break;
        case PieceType::BISHOP: moves = getBishopMoves(*piece); break;
        case PieceType::ROOK:   moves = getRookMoves(*piece);   break;
        case PieceType::QUEEN:  moves = getQueenMoves(*piece);  break;
        case PieceType::KING:   moves = getKingMoves(*piece, false);   break;
        }

        for (auto& move : moves)
            if (move.to == king->getPos()) return true;
    }
    return false;
}

std::map<Piece*, sf::Vector2f> Board::getPinnedPieces() {
	std::map<Piece*, sf::Vector2f> pinnedPieces;

	for (PieceColor color : {PieceColor::WHITE, PieceColor::BLACK}) {
		Piece* king = getKing(color);

		for (int dx : {-1, 0, 1}) {
			for (int dy : {-1, 0, 1}) {
				sf::Vector2f pos = king->getPos();
				if (dx == 0 && dy == 0)
					continue;
				while (true) {
					pos += sf::Vector2f(dx, dy);
					if (pos.x < 0 || pos.x > 7 || pos.y < 0 || pos.y > 7)
						break;
					if (auto maybePinned = getPiece(pos)) {
						if (maybePinned->getColor() == king->getColor()) {
							pos += sf::Vector2f(dx, dy);
							while (true) {
								if (pos.x < 0 || pos.x > 7 || pos.y < 0 || pos.y > 7)
									break;
								if (auto attacker = getPiece(pos)) {
									if (attacker->getColor() != king->getColor()) {
										bool diagonal = dx != 0 && dy != 0;
										auto t = attacker->getType();
										if (t == PieceType::QUEEN || (diagonal ? t == PieceType::BISHOP : t == PieceType::ROOK))
											pinnedPieces[maybePinned] = sf::Vector2f(dx, dy);
									}
									break;
								}
								pos += sf::Vector2f(dx, dy);
							}
						}
						break;
					}
				}
			}
		}
	}
	return pinnedPieces;
}

bool Board::wouldBeInCheck(Piece piece, sf::Vector2f moveTo) {
	Board boardCopy = *this;
	Piece* pieceInCopy = boardCopy.getPiece(piece.getPos());

	boardCopy.movePiece(*pieceInCopy, moveTo);

	return boardCopy.isInCheck(piece.getColor());
}

bool Board::isCheckmated(PieceColor color) {
	for (auto piece : getPieces(color)) {
		if (!piece->getLegalMoves().empty())
			return false;
	}
	return true;
}

int Board::getHalfMoveClock() const {
	return halfMoveClock;
}

PieceColor Board::getSideToMove() const {
	return sideToMove;
}
void Board::switchSideToMove() {
	sideToMove = (sideToMove == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
}

Piece* Board::getKing(PieceColor color) {
	for (auto& piece : pieces) {
		if (piece.getType() == PieceType::KING && piece.getColor() == color)
			return &piece;
	}
	return nullptr;
}

std::string Board::stringify() {
	std::string position = "";

	for (auto piece : getPieces()) {
		auto [x, y] = piece->getPos();
		position += std::to_string(static_cast<int>(piece->getColor()));
		position += std::to_string(static_cast<int>(piece->getType()));
		position += std::to_string(static_cast<int>(x));
		position += std::to_string(static_cast<int>(y));
		position += ',';
	}
	position += "|";
	position += std::to_string(static_cast<int>(sideToMove));
	position += "|";

	if (auto king = getKing(PieceColor::WHITE)) {
		if (!king->hasMoved()) {
			if (auto kingSideRook = getPiece({7, 7}))
				if (!kingSideRook->hasMoved() && kingSideRook->getColor() == PieceColor::WHITE && kingSideRook->getType() == PieceType::ROOK)
					position += "K";
			if (auto queenSideRook = getPiece({ 0, 7 }))
				if (!queenSideRook->hasMoved() && queenSideRook->getColor() == PieceColor::WHITE && queenSideRook->getType() == PieceType::ROOK)
					position += "Q";
		}	
	}
	if (auto king = getKing(PieceColor::BLACK)) {
		if (!king->hasMoved()) {
			if (auto kingSideRook = getPiece({ 7, 0 }))
				if (!kingSideRook->hasMoved() && kingSideRook->getColor() == PieceColor::BLACK && kingSideRook->getType() == PieceType::ROOK)
					position += "k";
			if (auto queenSideRook = getPiece({ 0, 0 }))
				if (!queenSideRook->hasMoved() && queenSideRook->getColor() == PieceColor::BLACK && queenSideRook->getType() == PieceType::ROOK)
					position += "q";
		}
	}

	position += "|";

	if (lastMove) {
		auto piece = getPiece(lastMove->to);
		if (piece->getType() == PieceType::PAWN && abs(lastMove->to.y - lastMove->from.y) == 2)
			position += std::to_string(static_cast<int>(piece->getPos().x));
		else
			position += '-';
	}
	else
		position += '-';

	return position;
}

bool Board::insufficientMaterial() const {
	std::vector<Piece> white, black;

	for (const auto& piece : pieces) {
		if (piece.getColor() == PieceColor::WHITE) white.push_back(piece);
		else black.push_back(piece);
	}

	if (white.size() == 1 && black.size() == 1)
		return true;

	Piece *whiteMinor = kingAndMinor(white),
		  *blackMinor = kingAndMinor(black);

	if (white.size() == 1 && blackMinor)
		return true;
	if (black.size() == 1 && whiteMinor)
		return true;

	if (blackMinor && whiteMinor) {
		if (blackMinor->getType() == PieceType::BISHOP && whiteMinor->getType() == PieceType::BISHOP) {
			if (isLightSquare(blackMinor) && isLightSquare(whiteMinor))
				return true;
		}
	}
	return false;
}

Piece* Board::kingAndMinor(std::vector<Piece> side) const {
	// returns the minor piece that the king is left with
	if (side.size() != 2)
		return nullptr;

	for (auto& piece : side) {
		if (piece.getType() == PieceType::BISHOP || piece.getType() == PieceType::KNIGHT)
			return &piece;
	}
	return nullptr;
}

bool Board::isLightSquare(Piece* bishop) const {
	int x = static_cast<int>(bishop->getPos().x);
	int y = static_cast<int>(bishop->getPos().y);

	return ((x % 2 == 0) ^ (y % 2 == 0));
}
