#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include "TicTacToeGame.h"

using boost::asio::ip::tcp;

class GameSession : public std::enable_shared_from_this<GameSession> {
public:
    explicit GameSession(tcp::socket socket);
    void Start();

private:
    // Основной цикл игры
    void ReadPlayerInput();
    void ProcessPlayerMove(const std::string& input);
    void MakeServerMove();
    void SendGameState();
    void SendGameResult();
    void SendMessageToClient(const std::string& message);

    // Вспомогательные методы
    bool IsValidMove(int cell) const;
    void HandleGameOver();

    tcp::socket socket_;
    std::vector<char> buffer_;
    TicTacToeGame game_;
};

#endif