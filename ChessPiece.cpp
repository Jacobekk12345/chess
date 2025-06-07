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

// draws the piece on the board in its position
void ChessPiece::place(sf::RenderWindow& window) {
    sf::Sprite sprite(texture);
    sprite.setPosition(sf::Vector2f(this->boardPosition.x * 100, this->boardPosition.y * 100));
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
    if (this == nullptr)
        return {};
    return this->possibleMoves;
}

void ChessPiece::clearPins() {
    this->pinDirections.clear();
}

void ChessPiece::clearPossibleMoves() {
    this->possibleMoves.clear();
}

void ChessPiece::calculatePossibleMoves() {
    switch (this->getType()) {
    case PieceType::PAWN: {
        this->calculateMovesPawn();
        break;
    }
    case PieceType::KNIGHT: {
        this->calculateMovesKnight();
        break;
    }
    case PieceType::BISHOP: {
        this->calculateMovesBishop();
        break;
    }
    case PieceType::ROOK: {
        this->calculateMovesRook();
        break;
    }
    case PieceType::QUEEN: {
        this->calculateMovesQueen();
        break;
    }
    case PieceType::KING: {
        this->calculateMovesKing();
        break;
    }
    default: {
        break;
    }
    }

}

void ChessPiece::calculateMovesPawn() {

    //TODO: if the pinning piece can be taken by the pawn, the pawn can take it, for now it just cant move
    if (!this->pinDirections.empty())
        return;

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
        if (board->getPiece(sf::Vector2i(this->boardPosition.x - 1, this->boardPosition.y - 1)) != nullptr && this->boardPosition.x > 0)
            this->possibleMoves.push_back(sf::Vector2i(this->boardPosition.x - 1, this->boardPosition.y - 1));
        // capturing right
        if (board->getPiece(sf::Vector2i(this->boardPosition.x + 1, this->boardPosition.y - 1)) != nullptr && this->boardPosition.x < 7)
            this->possibleMoves.push_back(sf::Vector2i(this->boardPosition.x + 1, this->boardPosition.y - 1));
        // TODO: en passant logic
    
    }
    else {
        if (this->boardPosition.y == 1)
            if (board->getPiece(sf::Vector2i(this->boardPosition.x, 2)) == nullptr &&
                board->getPiece(sf::Vector2i(this->boardPosition.x, 3)) == nullptr)
                this->possibleMoves.push_back(sf::Vector2i(this->boardPosition.x, 3));

        if (board->getPiece(sf::Vector2i(this->boardPosition.x, this->boardPosition.y + 1)) == nullptr)
            this->possibleMoves.push_back(sf::Vector2i(this->boardPosition.x, this->boardPosition.y + 1));
        
        if (board->getPiece(sf::Vector2i(this->boardPosition.x - 1, this->boardPosition.y + 1)) != nullptr && this->boardPosition.x > 0)
            this->possibleMoves.push_back(sf::Vector2i(this->boardPosition.x - 1, this->boardPosition.y + 1));
        
        if (board->getPiece(sf::Vector2i(this->boardPosition.x + 1, this->boardPosition.y + 1)) != nullptr && this->boardPosition.x < 7)
            this->possibleMoves.push_back(sf::Vector2i(this->boardPosition.x + 1, this->boardPosition.y + 1));

    }

}

void ChessPiece::calculateMovesKnight() {

    if (!this->pinDirections.empty())
        return;

    std::vector<sf::Vector2i> directions = {
        sf::Vector2i(2, 1),
        sf::Vector2i(2, -1),
        sf::Vector2i(-2, -1),
        sf::Vector2i(-2, 1),
        sf::Vector2i(1, 2),
        sf::Vector2i(1, -2),
        sf::Vector2i(-1, -2),
        sf::Vector2i(-1, 2)
    };

    for (sf::Vector2i point : directions) {

        if ((this->boardPosition + point).x >= 0 && (this->boardPosition + point).x <= 7 &&
            (this->boardPosition + point).y >= 0 && (this->boardPosition + point).y <= 7)
            if (board->getPiece(boardPosition + point) == nullptr)
                this->possibleMoves.push_back(this->boardPosition + point);
            else
                if (board->getPiece(boardPosition + point)->getColor() != this->getColor())
                    this->possibleMoves.push_back(this->boardPosition + point);
    }

}

void ChessPiece::calculateMovesBishop() {
    std::vector<sf::Vector2i> directions = {
        sf::Vector2i(1, 1),
        sf::Vector2i(1, -1),
        sf::Vector2i(-1, 1),
        sf::Vector2i(-1, -1)
    };


    for (sf::Vector2i direction : directions) {

        if (!(std::find(this->pinDirections.begin(), this->pinDirections.end(), direction) != this->pinDirections.end() ||
            std::find(this->pinDirections.begin(), this->pinDirections.end(), direction * -1) != this->pinDirections.end()) && !this->pinDirections.empty())
            continue;

        int i = 1;
        bool stopPushingMoves = false;
        sf::Vector2i lastEnemyPiecePos;

        while (true) {
            sf::Vector2i pos = this->boardPosition + direction * i;
            i++;

            if (!(pos.x >= 0 && pos.x <= 7 && pos.y >= 0 && pos.y <= 7))
                break;
            if (board->getPiece(pos) != nullptr) {

                if (stopPushingMoves) {

                    if (board->getPiece(pos) != nullptr)
                        if (board->getPiece(pos)->getType() == PieceType::KING && board->getPiece(pos)->getColor() != this->getColor()) {
                            board->getPiece(lastEnemyPiecePos)->pinDirections.push_back(direction);
                            board->getPiece(lastEnemyPiecePos)->pinningPiecesPositions.push_back(this->getPosition());
                        }

                    break;
                }

                if (board->getPiece(pos)->getColor() != this->getColor()) {
                    this->possibleMoves.push_back(pos);
                    stopPushingMoves = true;
                    lastEnemyPiecePos = pos;
                }
                else
                    break;
            }
            if (!stopPushingMoves)
                this->possibleMoves.push_back(pos);
        }
    }

}

void ChessPiece::calculateMovesRook() {
    std::vector<sf::Vector2i> directions = {
        sf::Vector2i(0, 1),
        sf::Vector2i(0, -1),
        sf::Vector2i(-1, 0),
        sf::Vector2i(1, 0)
    };

    for (sf::Vector2i direction : directions) {

        if (!(std::find(this->pinDirections.begin(), this->pinDirections.end(), direction) != this->pinDirections.end() ||
            std::find(this->pinDirections.begin(), this->pinDirections.end(), direction * -1) != this->pinDirections.end()) && !this->pinDirections.empty())
            continue;

        int i = 1;
        bool stopPushingMoves = false;
        sf::Vector2i lastEnemyPiecePos;

        while (true) {
            sf::Vector2i pos = this->boardPosition + direction * i;
            i++;

            if (!(pos.x >= 0 && pos.x <= 7 && pos.y >= 0 && pos.y <= 7))
                break;
            if (board->getPiece(pos) != nullptr) {

                if (stopPushingMoves) {

                    if (board->getPiece(pos) != nullptr)
                        if (board->getPiece(pos)->getType() == PieceType::KING && board->getPiece(pos)->getColor() != this->getColor()) {
                            board->getPiece(lastEnemyPiecePos)->pinDirections.push_back(direction);
                            board->getPiece(lastEnemyPiecePos)->pinningPiecesPositions.push_back(this->getPosition());
                        }

                    break;
                }

                if (board->getPiece(pos)->getColor() != this->getColor()) {
                    this->possibleMoves.push_back(pos);
                    stopPushingMoves = true;
                    lastEnemyPiecePos = pos;
                }
                else
                    break;
            }
            if (!stopPushingMoves)
                this->possibleMoves.push_back(pos);
        }
    }
}

void ChessPiece::calculateMovesQueen() {
    this->calculateMovesBishop();
    this->calculateMovesRook();
}

void ChessPiece::calculateMovesKing() {
    std::vector<sf::Vector2i> directions = {
        sf::Vector2i(0, 1),
        sf::Vector2i(0, -1),
        sf::Vector2i(-1, 0),
        sf::Vector2i(1, 0),
        sf::Vector2i(1, 1),
        sf::Vector2i(1, -1),
        sf::Vector2i(-1, 1),
        sf::Vector2i(-1, -1)
    };

    for (sf::Vector2i point : directions) {
        if ((this->boardPosition + point).x >= 0 && (this->boardPosition + point).x <= 7 &&
            (this->boardPosition + point).y >= 0 && (this->boardPosition + point).y <= 7)
            if (board->getPiece(boardPosition + point) == nullptr)
                this->possibleMoves.push_back(this->boardPosition + point);
            else
                if (board->getPiece(boardPosition + point)->getColor() != this->getColor())
                    this->possibleMoves.push_back(this->boardPosition + point);
    }

}