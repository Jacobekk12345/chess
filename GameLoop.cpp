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

    float promotionWidth = availablePromotions.size() * promotionSelectorsScale * 100;
    for (int i = 0; i < promotionsPositions.size(); i++) {
        promotionsPositions[i] = { 
            ((800.f - promotionWidth) / 2.f) + i * (100.f * promotionSelectorsScale),
            400.f - ((promotionSelectorsScale * 100.f) / 2)
        };
    }

}

void GameLoop::start() {
    while (window.isOpen()) {
        eventLoop();

        window.clear();
        drawBoard();

        if (promoting)
            drawPromotionView();

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

Piece* GameLoop::pawnAtBackrank() {
    for (auto pawn : board->getPieces(PieceType::PAWN)) {
        int y = pawn->getPos().y;
        if (y == 0 || y == 7) {
            return pawn;
        }
    }
    return nullptr;
}

void GameLoop::drawPromotionView() {
    sf::RectangleShape bg = sf::RectangleShape({ 800, 800 });
    bg.setFillColor(sf::Color(0, 0, 0, 150));

    sf::Texture texture;
    texture.setSmooth(true);

    window.draw(bg);

    for (int i = 0; i < availablePromotions.size(); i++) {
        texture.loadFromFile(makeFilePath(turn, availablePromotions[i]));
        sf::Sprite sprite(texture);
        sprite.setScale({promotionSelectorsScale, promotionSelectorsScale });
        sprite.setPosition(promotionsPositions[i]);
        window.draw(sprite);
    }
}

void GameLoop::handleMouse(const sf::Event::MouseButtonPressed* mouse) {
    if (mouse->button == sf::Mouse::Button::Left) {

        if (checkmate)
            return;

        sf::Vector2f mouseWorldPos = sf::Vector2f(mouse->position / 100);

        if (promoting) {
            for (int i = 0; i < promotionsPositions.size(); i++) {
                float mX = mouse->position.x,
                      mY = mouse->position.y,
                      pX = promotionsPositions[i].x,
                      pY = promotionsPositions[i].y;

                if ((mX > pX && mX < (pX + promotionSelectorsScale * 100)) && (mY > pY && mY < (pY + promotionSelectorsScale * 100))) {
                    promoting = false;
                    pawnAtBackrank()->promote(availablePromotions[i]);
                    turn = (turn == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
                    board->calculateLegalMoves();
                }
            }
            return;
        }

        Piece* clickedOnSquare = board->getPiece(mouseWorldPos);

        if (clickedOnSquare && clickedOnSquare->getColor() == turn)
            selectPiece(*clickedOnSquare);

        else if (selectedSquare) {
            Piece* selectedPiece = board->getPiece(*selectedSquare);

            if (selectedPiece->canMoveTo(mouseWorldPos)) {
                board->movePiece(*selectedPiece, mouseWorldPos);
                deselectPieces();

                if (pawnAtBackrank()) {
                    promoting = true;
                    return;
                }
                turn = (turn == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;

                board->calculateLegalMoves();

                if (board->isCheckmated(turn)) {
                    std::cout << "checkmate\n";
                    checkmate = true;
                }
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