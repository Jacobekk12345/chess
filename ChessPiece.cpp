#include "ChessPiece.h"

ChessPiece::ChessPiece(PieceColor color, PieceType type, sf::Vector2i position) : color(color), type(type), boardPosition(position) {
    // Create the texture as a member variable
    loadTexture(makeFilePath(this->color, this->type));
}

void ChessPiece::loadTexture(std::string imagePath) {
    // Load directly into the member texture
    this->texture.loadFromFile(imagePath);
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
