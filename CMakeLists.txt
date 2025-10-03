cmake_minimum_required(VERSION 3.16)
project(GameServer)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
        main.cpp
        GameSession.cpp
        TicTacToeGame.cpp
        TicTacToeServer.cpp
)

# Просто указываем путь к Boost
target_include_directories(${PROJECT_NAME} PRIVATE "C:/boost")

# Минимальный набор библиотек
target_link_libraries(${PROJECT_NAME} PRIVATE
        ws2_32
        mswsock
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
            WIN32_LEAN_AND_MEAN
            BOOST_ASIO_DISABLE_IOCP=1
    )
endif()