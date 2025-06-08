#include "Setup.h"

Setup::Setup() {

    windowWidth = 1000;
    windowHeight = 800;

	window = sf::RenderWindow(sf::VideoMode(sf::Vector2u( windowWidth, windowHeight )), "Chess");

    darkSquare = sf::Color(118, 150, 86);
    lightSquare = sf::Color(238, 238, 210);
    selectedSquare = sf::Color(186, 202, 68);
    movesIndicatorCircleColor = sf::Color(100, 100, 100, 200);

    square = sf::RectangleShape(sf::Vector2f(100.f, 100.f));
    availableMovesIndicator = sf::CircleShape(20.f);
    availableMovesIndicator.setFillColor(movesIndicatorCircleColor);

    isResizing = false;

	windowLoop();
}

void Setup::windowLoop() {

    Board board;

    while (window.isOpen()) {
        eventLoop(board);
        
        window.clear();
        drawBoard(board);
    }
}

void Setup::eventLoop(Board& board) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2i worldPos = sf::Vector2i(window.mapPixelToCoords(mousePos)) / 100;

    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            ChessPiece* clickedOnPiece = board.getPiece(worldPos);   // piece that the player clicked on, nullptr if clicked on empty field

            if (clickedOnPiece && clickedOnPiece->getColor() == board.getMove()) // if the player clicked on a piece and the piece's color matches color of the player that has the move
                board.select(*clickedOnPiece);

            else if (board.getSelectedPiecePos() != sf::Vector2i(-1, -1)) // if there is a selected piece
                board.movePiece(board.getSelectedPiece(), worldPos);

            else if (!clickedOnPiece)
                board.deselectPieces();

        }
        else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            if (isResizing) return; // prevent recursion

            int widthChange = abs(static_cast<int>(resized->size.x) - windowWidth);
            int heightChange = abs(static_cast<int>(resized->size.y) - windowHeight);

            if (widthChange > heightChange) {
                isResizing = true;

                float ratio = float(resized->size.x) / float(windowWidth);
                window.setSize(sf::Vector2u(resized->size.x, windowHeight * ratio));

                isResizing = false;
            }
            else {
                isResizing = true;

                float ratio = float(resized->size.y) / float(windowHeight);
                window.setSize(sf::Vector2u(windowWidth * ratio, resized->size.y));

                isResizing = false;
            }

            windowWidth = window.getSize().x;
            windowHeight = window.getSize().y;
        }

    }
}

void Setup::drawBoard(Board& board) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            square.setPosition(sf::Vector2f(i * 100.f, j * 100.f));

            if (sf::Vector2i(i, j) == board.getSelectedPiecePos())
                square.setFillColor(selectedSquare);
            else if ((i % 2 == 0) ^ (j % 2 == 0))
                square.setFillColor(lightSquare);
            else
                square.setFillColor(darkSquare);

            window.draw(square);
        }
    }

    board.renderPieces(window);

    drawAvailableMovesIndicators(board);

    window.display();
}

void Setup::drawAvailableMovesIndicators(Board& board) {
    for (sf::Vector2i pos : board.getSelectedPiece()->getPossibleMoves()) {
        availableMovesIndicator.setPosition(sf::Vector2f((pos.x * 100) + (50 - availableMovesIndicator.getRadius()), (pos.y * 100) + (50 - availableMovesIndicator.getRadius())));
        window.draw(availableMovesIndicator);
    }
}
