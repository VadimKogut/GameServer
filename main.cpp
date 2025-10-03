#include <iostream>
#include <boost/asio.hpp>

#include "TicTacToeServer.h"

int main(int argc, char* argv[]){
    try{
        if(argc!= 2){
            std::cerr << "Usage: " << argv[0] << " <port> " << argv[0] << std::endl;
            return 1;
        }

        short port = std::atoi(argv[1]);

        boost::asio::io_context io_context;
        TicTacToeServer server(io_context,port);
        server.Start();

        std::cout << "Tic-Tac-Toe Server started on port " << port << std::endl;
        std::cout << "Press Ctrl+C to stop" << std::endl;

        io_context.run();
    }
    catch(std::exception& e){
        std::cerr << "Exception:" << e.what() << std::endl;
    }


    return 0;
}