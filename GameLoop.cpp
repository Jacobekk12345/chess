#include "GameLoop.h"

GameLoop::GameLoop() : darkSquareColor(sf::Color(118, 150, 86)),
                       lightSquareColor(sf::Color(238, 238, 210)),
                       selectedSquareColor(sf::Color(186, 202, 68)),
                       moveIndicatorColor(sf::Color(100, 100, 100, 200)),
                       checkedSquareColor(sf::Color(200, 0, 0)),
                       turn(PieceColor::WHITE),
                       selectedSquare(std::nullopt) {
	window = sf::RenderWindow(sf::VideoMode({800, 800}), "Chess", sf::Style::Close);

    board = new Board();
}

void GameLoop::start() {
    while (window.isOpen()) {
        eventLoop();

        window.clear();
        drawBoard();
        window.display();
    }
}

void GameLoop::drawBoard() {
    sf::RectangleShape square = sf::RectangleShape({ 100, 100 });
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            square.setPosition({ i * 100.f, j * 100.f });

            if (selectedSquare == sf::Vector2f(i, j) || (board->getLastMove() && *board->getLastMove() == sf::Vector2f(i, j)))
                square.setFillColor(selectedSquareColor);
            else if ((i % 2 == 0) ^ (j % 2 == 0))
                square.setFillColor(darkSquareColor);
            else
                square.setFillColor(lightSquareColor);

            window.draw(square);
        }
    }

    for (auto color : { PieceColor::WHITE, PieceColor::BLACK }) {
        if (board->isInCheck(color)) {
            auto king = board->getPieces(color, PieceType::KING).at(0);
            square.setPosition(king->getPos() * 100.f);
            square.setFillColor(checkedSquareColor);
            window.draw(square);
        }
            
    }

    board->renderPieces(window);
    drawMoveIndicators();
}

void GameLoop::drawMoveIndicators() {
    if (selectedSquare) {
        sf::CircleShape moveIndicator(25.f);
        moveIndicator.setFillColor(moveIndicatorColor);
        moveIndicator.setOrigin({ -25.f, -25.f });

        sf::CircleShape captureMoveIndicator(45.f);
        captureMoveIndicator.setFillColor(sf::Color::Transparent);
        captureMoveIndicator.setOutlineColor(sf::Color::Red);
        captureMoveIndicator.setOutlineThickness(5.f);
        captureMoveIndicator.setOrigin({ -5.f, -5.f });

        Piece* selectedPiece = board->getPiece(*selectedSquare);

        for (auto move : selectedPiece->getLegalMoves()) {
            if (board->getPiece(move.to) || move.type == MoveType::EnPassant) {
                captureMoveIndicator.setPosition(move.to * 100.f);
                window.draw(captureMoveIndicator);
            }
            else {
                moveIndicator.setPosition(move.to * 100.f);
                window.draw(moveIndicator);
            }
        }
    }
}

void GameLoop::eventLoop() {
    while (const std::optional event = window.pollEvent()) {
        if (const sf::Event::MouseButtonPressed* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            handleMouse(mousePressed);
        }
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void GameLoop::handleMouse(const sf::Event::MouseButtonPressed* mouse) {
    if (mouse->button == sf::Mouse::Button::Left) {
        sf::Vector2f mouseWorldPos = sf::Vector2f(mouse->position / 100);

        Piece* clickedOnSquare = board->getPiece(mouseWorldPos);

        if (clickedOnSquare && clickedOnSquare->getColor() == turn)
            selectPiece(*clickedOnSquare);

        else if (selectedSquare) {
            Piece* selectedPiece = board->getPiece(*selectedSquare);

            if (selectedPiece->canMoveTo(mouseWorldPos)) {
                board->movePiece(*selectedPiece, mouseWorldPos);
                deselectPieces();
                turn = (turn == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
                board->calculateLegalMoves();
            }
        }

        else
            deselectPieces();
    }
}

void GameLoop::selectPiece(Piece& piece) {
    selectedSquare = piece.getPos();
}

void GameLoop::deselectPieces() {
    selectedSquare = std::nullopt;
}