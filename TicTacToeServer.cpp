#include "TicTacToeServer.h"

TicTacToeServer::TicTacToeServer(boost::asio::io_context& io_context, short port)
: acceptor_(io_context,tcp::endpoint (tcp::v4(),port)),
io_context_(io_context){
    std::cout << "TicTacToe server initialized on port" << port << std::endl;
}

void TicTacToeServer::Start() {
    DoAccept();
    std::cout << "Server started - waiting for players..." << std::endl;
}

void TicTacToeServer::DoAccept() {
    acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket){
                if(!ec){
                    std::string client_ip = socket.remote_endpoint().address().to_string();
                    int client_port_ = socket.remote_endpoint().port();
                    std::cout << "New player connected: " << client_ip << ":" << client_port_;

                    auto session = std::make_shared<GameSession>(std::move(socket));
                    session->Start();

                    std::cout << "Game session created for " << client_ip << ":" << client_port_ << std::endl;
                }
                else{
                    std::cerr << "Accept error: " << ec.message() << std::endl;
                }
                DoAccept();
            }
            );
}