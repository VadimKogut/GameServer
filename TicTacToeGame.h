#ifndef TICTACTOEGAME_H
#define TICTACTOEGAME_H

#include <array>
#include <string>

class TicTacToeGame {
public:
    enum class Player { NONE, X, O };
    enum class GameState { WAITING, PLAYING, FINISHED };

    TicTacToeGame();

    // Основные методы игры
    bool MakeMove(int cell, Player player);
    void ResetGame();

    // Проверки состояния
    bool CheckWinner() const;
    bool IsBoardFull() const;
    bool IsGameFinished() const;

    // Геттеры
    Player GetCurrentPlayer() const;
    Player GetWinner() const;
    GameState GetGameState() const;
    const std::array<Player, 9>& GetBoard() const;

    // Визуализация
    std::string GetBoardString() const;
    char PlayerToChar(Player player) const;

private:
    bool CheckLine(int a, int b, int c) const;

    std::array<Player, 9> board_;
    Player current_player_;
    GameState game_state_;
    Player winner_;
};

#endif