#include "ChessPiece.h"
#include "Board.h"

ChessPiece::ChessPiece(PieceColor color, PieceType type, sf::Vector2i position, Board* board) : color(color), type(type), boardPosition(position), board(board) {
    loadTexture(makeFilePath(this->color, this->type));
}

bool ChessPiece::operator==(ChessPiece other) {
    if (this->color == other.color &&   // safety checks
        this->type == other.type &&     // safety checks
        this->boardPosition == other.boardPosition) // actually needed
        return true;
    else
        return false;
}

void ChessPiece::loadTexture(std::string imagePath) {
    this->texture.loadFromFile(imagePath); // TODO: if texture not found use a url or close the program and kill the user (send a nuke to his home)
    this->texture.setSmooth(true);
}

std::string makeFilePath(PieceColor color, PieceType type) {
    std::string path;
    path += "assets/pieces/";
    path += std::to_string(static_cast<int>(color));
    path += std::to_string(static_cast<int>(type));
    path += ".png";
    return path;
}

void ChessPiece::place(sf::RenderWindow& window) {
    sf::Sprite sprite(texture);
    sprite.setPosition(sf::Vector2f(boardPosition.x * 100, boardPosition.y * 100));
    window.draw(sprite);
}

sf::Vector2i ChessPiece::getPosition() const {
    return this->boardPosition;
}

void ChessPiece::setPosition(sf::Vector2i newPos) {
    this->boardPosition = newPos;
}

PieceType ChessPiece::getType() const {
    return this->type;
}

PieceColor ChessPiece::getColor() const {
    return this->color;
}

void ChessPiece::promote() {
    if (this->type == PieceType::PAWN)
        this->type = PieceType::QUEEN;
}

std::vector<sf::Vector2i> ChessPiece::getPossibleMoves() const {
    return this->possibleMoves;
}

void ChessPiece::calculatePossibleMoves() {
    switch (this->getType()) {
    case PieceType::PAWN: {
        this->defaultMovesPawn();
        break;
    }
    case PieceType::KNIGHT: {
        this->defaultMovesKnight();
        break;
    }
    case PieceType::BISHOP: {
        this->defaultMovesBishop();
        break;
    }
    case PieceType::ROOK: {
        this->defaultMovesRook();
        break;
    }
    case PieceType::QUEEN: {
        this->defaultMovesQueen();
        break;
    }
    case PieceType::KING: {
        this->defaultMovesKing();
        break;
    }
    default: {
        break;
    }
    }

}

void ChessPiece::defaultMovesPawn() {

    if (this->color == PieceColor::WHITE) {
        // can move 2 squares forward
        if (this->boardPosition.y == 6)
            if (board->getPiece(sf::Vector2i(this->boardPosition.x, 5)) == nullptr &&
                board->getPiece(sf::Vector2i(this->boardPosition.x, 4)) == nullptr)
                this->possibleMoves.push_back(sf::Vector2i(this->boardPosition.x, 4));
        // normal move forwards
        if (board->getPiece(sf::Vector2i(this->boardPosition.x, this->boardPosition.y - 1)) == nullptr)
            this->possibleMoves.push_back(sf::Vector2i(this->boardPosition.x, this->boardPosition.y - 1));
        // capturing left
        if (board->getPiece(sf::Vector2i(this->boardPosition.x - 1, this->boardPosition.y - 1)) != nullptr && this->boardPosition.x > 1)
            this->possibleMoves.push_back(sf::Vector2i(this->boardPosition.x - 1, this->boardPosition.y - 1));
        // capturing right
        if (board->getPiece(sf::Vector2i(this->boardPosition.x + 1, this->boardPosition.y - 1)) != nullptr && this->boardPosition.x < 7)
            this->possibleMoves.push_back(sf::Vector2i(this->boardPosition.x + 1, this->boardPosition.y - 1));
    }
    else {
        if (this->boardPosition.y == 1)
            if (board->getPiece(sf::Vector2i(this->boardPosition.x, 2)) == nullptr &&
                board->getPiece(sf::Vector2i(this->boardPosition.x, 3)) == nullptr)
                this->possibleMoves.push_back(sf::Vector2i(this->boardPosition.x, 3));

        if (board->getPiece(sf::Vector2i(this->boardPosition.x, this->boardPosition.y + 1)) == nullptr)
            this->possibleMoves.push_back(sf::Vector2i(this->boardPosition.x, this->boardPosition.y - 1));
        
        if (board->getPiece(sf::Vector2i(this->boardPosition.x - 1, this->boardPosition.y + 1)) != nullptr && this->boardPosition.x > 1)
            this->possibleMoves.push_back(sf::Vector2i(this->boardPosition.x - 1, this->boardPosition.y + 1));
        
        if (board->getPiece(sf::Vector2i(this->boardPosition.x + 1, this->boardPosition.y + 1)) != nullptr && this->boardPosition.x < 7)
            this->possibleMoves.push_back(sf::Vector2i(this->boardPosition.x + 1, this->boardPosition.y + 1));

    }

}

void ChessPiece::defaultMovesKnight() {

}

void ChessPiece::defaultMovesBishop() {

}

void ChessPiece::defaultMovesRook() {

}

void ChessPiece::defaultMovesQueen() {

}

void ChessPiece::defaultMovesKing() {

}