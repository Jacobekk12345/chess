#include "Piece.h"

Piece::Piece(PieceColor color, PieceType type, sf::Vector2f position) : color(color), type(type), position(position){
    texture.loadFromFile(makeFilePath());
    texture.setSmooth(true);
}

bool Piece::operator==(const Piece& other) const {
    return color == other.color &&
        type == other.type &&
        position == other.position;
}

bool Piece::operator!=(const Piece& other) const {
    return !(*this == other);
}

std::string Piece::makeFilePath() {
    std::string path;
    path += "assets/pieces/";
    path += std::to_string(static_cast<int>(color));
    path += std::to_string(static_cast<int>(type));
    path += ".png";
    return path;
}

sf::Vector2f Piece::getPos() const {
    return position;
}
void Piece::setPos(sf::Vector2f newPos) {
    position = newPos;
}

PieceColor Piece::getColor() const {
    return color;
}

PieceType Piece::getType() const {
    return type;
}


void Piece::draw(sf::RenderWindow& window) const {
    sf::Sprite sprite(texture);
    sprite.setTexture(texture);
    sprite.setPosition(position * 100.f);

    window.draw(sprite);
}

bool Piece::canMoveTo(sf::Vector2f pos) const {
    return std::find(legalMoves.begin(), legalMoves.end(), pos) != legalMoves.end();
}

std::vector<Move> Piece::getLegalMoves() const {
    return legalMoves;
}
void Piece::setLegalMoves(const std::vector<Move>& moves) {
    legalMoves = moves;
}

bool Piece::hasMoved() const {
    return moved;
}
void Piece::markAsMoved() {
    moved = true;
}

void Piece::promote(const PieceType& type) {
    this->type = type;
}
