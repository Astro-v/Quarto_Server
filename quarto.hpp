#ifndef __QUARTO_HPP__
#define __QUARTO_HPP__

/*---- LIBRARY ----*/
#include <iostream>

/*---- LIBRARY SFML ----*/

/*---- LOCAL FILE ----*/
#include "constant.hpp"
#include "server.hpp"

class Quarto {
    public:
    Quarto();
    void play();
    void receive();

    private:
    // GAME
    Status _status;
    bool _play;
    int _posX[NUMBER_PIECES];
    int _posY[NUMBER_PIECES];
    
    // SERVER
    Server _server;
    ToReceive _receive;
	ToSend _send;
};

#endif // __QUARTO_HPP__