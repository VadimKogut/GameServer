#include "GameSession.h"
#include <iostream>
#include <sstream>

GameSession::GameSession(tcp::socket socket)
        : socket_(std::move(socket)) {
    buffer_.resize(1024);
}

void GameSession::Start() {
    std::cout << "New game session started" << std::endl;

    // Сбрасываем игру и начинаем
    game_.ResetGame();

    SendMessageToClient(" Welcome to Tic-Tac-Toe Server!");
    SendMessageToClient("You are playing as X against the server (O)");
    SendMessageToClient("");
    SendMessageToClient("Board positions:");
    SendMessageToClient(" 0 | 1 | 2 ");
    SendMessageToClient(" 3 | 4 | 5 ");
    SendMessageToClient(" 6 | 7 | 8 ");
    SendMessageToClient("");
    SendMessageToClient("Enter a number 0-8 to make your move:");
    SendMessageToClient("");

    SendGameState();
    ReadPlayerInput();
}

void GameSession::ReadPlayerInput() {
    auto self = shared_from_this();

    socket_.async_read_some(
            boost::asio::buffer(buffer_),
            [this, self](boost::system::error_code ec, size_t length) {
                if (!ec) {
                    std::string input(buffer_.data(), length);
                    // Убираем пробелы и переводы строк
                    input.erase(input.find_last_not_of(" \n\r\t") + 1);

                    if (!input.empty()) {
                        ProcessPlayerMove(input);
                    } else {
                        SendMessageToClient("Please enter a move (0-8):");
                        ReadPlayerInput();
                    }
                } else {
                    std::cout << "Player disconnected" << std::endl;
                }
            });
}

void GameSession::ProcessPlayerMove(const std::string& input) {
    std::cout << "Player input: '" << input << "'" << std::endl;

    // Проверяем что сейчас ход игрока
    if (game_.GetCurrentPlayer() != TicTacToeGame::Player::X) {
        SendMessageToClient("  Please wait for server's move");
        ReadPlayerInput();
        return;
    }

    // Парсим ввод
    try {
        int cell = std::stoi(input);

        if (!IsValidMove(cell)) {
            SendMessageToClient(" Invalid move! Please enter a number 0-8 for an empty cell");
            ReadPlayerInput();
            return;
        }

        // Делаем ход игрока
        if (game_.MakeMove(cell, TicTacToeGame::Player::X)) {
            SendMessageToClient(" Your move: cell " + std::to_string(cell));
            SendGameState();

            // Проверяем конец игры
            if (game_.IsGameFinished()) {
                HandleGameOver();
                return;
            }

            // Ход сервера
            MakeServerMove();

        } else {
            SendMessageToClient(" Move failed! Try another cell");
            ReadPlayerInput();
        }

    } catch (const std::exception& e) {
        SendMessageToClient(" Invalid input! Please enter a number 0-8");
        ReadPlayerInput();
    }
}

void GameSession::MakeServerMove() {
    SendMessageToClient(" Server is thinking...");

    // Простой AI: ищем первую свободную ячейку
    const auto& board = game_.GetBoard();
    int serverMove = -1;

    for (int i = 0; i < 9; ++i) {
        if (board[i] == TicTacToeGame::Player::NONE) {
            serverMove = i;
            break;
        }
    }

    if (serverMove != -1) {
        // Делаем ход сервера
        if (game_.MakeMove(serverMove, TicTacToeGame::Player::O)) {
            SendMessageToClient("Server moved to cell " + std::to_string(serverMove));
            SendGameState();

            // Проверяем конец игры
            if (game_.IsGameFinished()) {
                HandleGameOver();
                return;
            }

            // Передаём ход игроку
            SendMessageToClient("");
            SendMessageToClient("Your turn! Enter move (0-8):");
            ReadPlayerInput();

        } else {
            SendMessageToClient(" Server move failed!");
            ReadPlayerInput();
        }
    } else {
        SendMessageToClient(" No moves available!");
        HandleGameOver();
    }
}

bool GameSession::IsValidMove(int cell) const {
    if (cell < 0 || cell > 8) return false;

    const auto& board = game_.GetBoard();
    return board[cell] == TicTacToeGame::Player::NONE;
}

void GameSession::SendGameState() {
    SendMessageToClient("");
    SendMessageToClient(" Current board:" + game_.GetBoardString());
    SendMessageToClient("");

    if (!game_.IsGameFinished()) {
        if (game_.GetCurrentPlayer() == TicTacToeGame::Player::X) {
            SendMessageToClient("Your turn (X)");
        } else {
            SendMessageToClient("Server's turn (O)");
        }
    }
}

void GameSession::HandleGameOver() {
    SendGameResult();

    // Можно добавить опцию рестарта, но пока просто завершаем сессию
    SendMessageToClient("");
    SendMessageToClient(" Game finished. Disconnect and reconnect to play again!");
}

void GameSession::SendGameResult() {
    SendMessageToClient("");
    SendMessageToClient(" GAME OVER!");

    auto winner = game_.GetWinner();
    if (winner == TicTacToeGame::Player::X) {
        SendMessageToClient(" CONGRATULATIONS! YOU WON!");
    } else if (winner == TicTacToeGame::Player::O) {
        SendMessageToClient(" Server won! Better luck next time!");
    } else {
        SendMessageToClient(" It's a draw! Good game!");
    }
}

void GameSession::SendMessageToClient(const std::string& message) {
    auto self = shared_from_this();
    auto message_ptr = std::make_shared<std::string>(message + "\r\n");

    boost::asio::async_write(socket_, boost::asio::buffer(*message_ptr),
                             [this, self, message_ptr](boost::system::error_code ec, size_t) {
                                 if (ec) {
                                     std::cout << "Error sending message: " << ec.message() << std::endl;
                                 }
                             });
}