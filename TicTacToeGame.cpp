#include "TicTacToeGame.h"
#include <sstream>

TicTacToeGame::TicTacToeGame() {
    ResetGame();
}

void TicTacToeGame::ResetGame() {
    board_.fill(Player::NONE);
    current_player_ = Player::X;  // X всегда ходит первым
    game_state_ = GameState::PLAYING;
    winner_ = Player::NONE;
}

bool TicTacToeGame::MakeMove(int cell, Player player) {
    // Проверяем валидность хода
    if (cell < 0 || cell > 8) return false;
    if (player != current_player_) return false;
    if (game_state_ != GameState::PLAYING) return false;
    if (board_[cell] != Player::NONE) return false;

    // Делаем ход
    board_[cell] = player;

    // Проверяем победу
    if (CheckWinner()) {
        winner_ = player;
        game_state_ = GameState::FINISHED;
    } else if (IsBoardFull()) {
        // Ничья
        game_state_ = GameState::FINISHED;
    } else {
        // Передаём ход следующему игроку
        current_player_ = (player == Player::X) ? Player::O : Player::X;
    }

    return true;
}

bool TicTacToeGame::CheckWinner() const {
    // Проверяем все выигрышные комбинации
    return CheckLine(0, 1, 2) || CheckLine(3, 4, 5) || CheckLine(6, 7, 8) ||  // Горизонтали
           CheckLine(0, 3, 6) || CheckLine(1, 4, 7) || CheckLine(2, 5, 8) ||  // Вертикали
           CheckLine(0, 4, 8) || CheckLine(2, 4, 6);                          // Диагонали
}

bool TicTacToeGame::CheckLine(int a, int b, int c) const {
    return board_[a] != Player::NONE &&
           board_[a] == board_[b] &&
           board_[b] == board_[c];
}

bool TicTacToeGame::IsBoardFull() const {
    for (const auto& cell : board_) {
        if (cell == Player::NONE) return false;
    }
    return true;
}

bool TicTacToeGame::IsGameFinished() const {
    return game_state_ == GameState::FINISHED;
}

TicTacToeGame::Player TicTacToeGame::GetCurrentPlayer() const {
    return current_player_;
}

TicTacToeGame::Player TicTacToeGame::GetWinner() const {
    return winner_;
}

TicTacToeGame::GameState TicTacToeGame::GetGameState() const {
    return game_state_;
}

const std::array<TicTacToeGame::Player, 9>& TicTacToeGame::GetBoard() const {
    return board_;
}

std::string TicTacToeGame::GetBoardString() const {
    std::stringstream ss;
    ss << "\r\n " << PlayerToChar(board_[0]) << " | " << PlayerToChar(board_[1]) << " | " << PlayerToChar(board_[2]);
    ss << "\r\n---+---+---";
    ss << "\r\n " << PlayerToChar(board_[3]) << " | " << PlayerToChar(board_[4]) << " | " << PlayerToChar(board_[5]);
    ss << "\r\n---+---+---";
    ss << "\r\n " << PlayerToChar(board_[6]) << " | " << PlayerToChar(board_[7]) << " | " << PlayerToChar(board_[8]);
    return ss.str();
}

char TicTacToeGame::PlayerToChar(Player player) const {
    switch (player) {
        case Player::X: return 'X';
        case Player::O: return 'O';
        case Player::NONE: return ' ';
        default: return '?';
    }
}