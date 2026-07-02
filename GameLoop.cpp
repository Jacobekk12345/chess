#include "GameLoop.h"

GameLoop::GameLoop() : darkSquareColor(sf::Color(118, 150, 86)),
                       lightSquareColor(sf::Color(238, 238, 210)),
                       selectedSquareColor(sf::Color(186, 202, 68)),
                       moveIndicatorColor(sf::Color(100, 100, 100, 200)),
                       checkedSquareColor(sf::Color(200, 0, 0)),
                       selectedSquare(std::nullopt) {
	window = sf::RenderWindow(sf::VideoMode({800, 800}), "Chess", sf::Style::Close);

    board = new Board();

    board->positionHistory[board->stringify()]++;

    // this piece of code allows for chganging the scale of the pieces that show up when the user wants to promote without needing to rewrite its positions, spacing etc. and no hardcoding
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
            auto king = board->getKing(color);
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

        for (const auto& move : selectedPiece->getLegalMoves()) {
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
        texture.loadFromFile(makeFilePath(board->getSideToMove(), availablePromotions[i]));
        sf::Sprite sprite(texture);
        sprite.setScale({promotionSelectorsScale, promotionSelectorsScale });
        sprite.setPosition(promotionsPositions[i]);
        window.draw(sprite);
    }
}

void GameLoop::handlePromotionClick(const sf::Event::MouseButtonPressed* mouse) {
    for (int i = 0; i < promotionsPositions.size(); i++) {
        float mX = mouse->position.x,
            mY = mouse->position.y,
            pX = promotionsPositions[i].x,
            pY = promotionsPositions[i].y;

        if ((mX > pX && mX < (pX + promotionSelectorsScale * 100)) && (mY > pY && mY < (pY + promotionSelectorsScale * 100))) {
            promoting = false;

            pawnAtBackrank()->promote(availablePromotions[i]);
            finishTurn(availablePromotions[i]);
        }
    }
}

void GameLoop::finishTurn(PieceType promotionType) {

    deselectPieces();

    if (pawnAtBackrank()) {
        promoting = true;
        return;
    }

    if (board->getSideToMove() == PieceColor::BLACK)
        board->increaseMoveCount();

    board->switchSideToMove();
    board->calculateLegalMoves();

    board->finishPGNMove(board->getSideToMove(), promotionType);

    checkGameEnd();
}

void GameLoop::checkGameEnd() {
    if (board->isCheckmated(board->getSideToMove())) {
        if (board->isInCheck(board->getSideToMove()))
            std::cout << '{' << board->getSideToMove() << " won by checkmate}\n" << board->getPGN(board->getSideToMove() == PieceColor::WHITE ? "0-1" : "1-0");
        else
            std::cout << "{draw by stalemate}\n" << board->getPGN("1/2-1/2");
        finish = true;
    }
    else if (board->getHalfMoveClock() >= 100) {
        std::cout << "{draw by the fifty move rule}\n" << board->getPGN("1/2-1/2");
        finish = true;
    }
    else if (++board->positionHistory[board->stringify()] >= 3) {
        std::cout << "{draw by threefold repetition}\n" << board->getPGN("1/2-1/2");
        finish = true;
    }
    else if (board->insufficientMaterial()) {
        std::cout << "{draw by insufficient material}\n" << board->getPGN("1/2-1/2");
        finish = true;
    }
}

void GameLoop::handleBoardClick(const sf::Event::MouseButtonPressed* mouse) {
    sf::Vector2f mouseWorldPos = sf::Vector2f(mouse->position / 100);

    Piece* clickedOnSquare = board->getPiece(mouseWorldPos);

    if (clickedOnSquare && clickedOnSquare->getColor() == board->getSideToMove())
        selectPiece(*clickedOnSquare);

    else if (selectedSquare) {
        Piece* selectedPiece = board->getPiece(*selectedSquare);

        if (selectedPiece->canMoveTo(mouseWorldPos)) {

            board->movePiece(*selectedPiece, mouseWorldPos);
            finishTurn();

        }
    }

    else
        deselectPieces();
}

void GameLoop::handleMouse(const sf::Event::MouseButtonPressed* mouse) {

    if (finish)
        return;

    if (mouse->button != sf::Mouse::Button::Left)
        return;

    if (promoting) {
        handlePromotionClick(mouse);
        return;
    }

    handleBoardClick(mouse);
}

void GameLoop::selectPiece(Piece& piece) {
    selectedSquare = piece.getPos();
}

void GameLoop::deselectPieces() {
    selectedSquare = std::nullopt;
}
