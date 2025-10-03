#ifndef GAMESERVER_TICTACTOESERVER_H
#define GAMESERVER_TICTACTOESERVER_H

#include <iostream>
#include <boost/asio.hpp>
#include <vector>

#include "GameSession.h"

using namespace boost::asio::ip;

class TicTacToeServer {
public:
    explicit TicTacToeServer(boost::asio::io_context& io_context, short port);
    void Start();

private:
    void DoAccept();

    tcp::acceptor acceptor_;
    boost::asio::io_context& io_context_;
};


#endif //GAMESERVER_TICTACTOESERVER_H
