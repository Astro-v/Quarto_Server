#ifndef __CONSTANT_HPP__
#define __CONSTANT_HPP__

/*---- LIBRARY ----*/
#include <iostream>

/*---- LIBRARY SFML ----*/

/*---- LOCAL FILE ----*/

/*---- GAME ----*/
#define NUMBER_ROW 4
#define NUMBER_COLUMN 4
#define NUMBER_PIECES (NUMBER_ROW*NUMBER_COLUMN)


/*---- SERVER ----*/
#define SERVER_PORT 5500	
#define SERVER_ADRESS sf::IpAddress::getLocalAddress() // IP adress of the server

/*---- STATUS ----*/
enum Status{
    PLAYER_1_PICK, PLAYER_2_PLACE, PLAYER_2_PICK, PLAYER_1_PLACE, PAUSE
};
#define NONE -1

/*---- TO SEND ----*/
enum TypeSend{
    CONNECT_SERVER, GAME_SERVER, WIN_P1, WIN_P2, NOTHING
};

struct ToSend{
    Status status;
    int posX[NUMBER_PIECES];
    int posY[NUMBER_PIECES];
    bool used[NUMBER_PIECES];
    int pick;
};

/*---- TO RECEIVE ----*/
enum TypeReceive{
    CONNECT_CLIENT, GAME_CLIENT, LEAVE
};

struct ToReceive{
    int posX;
    int posY;
};

#endif // __CONSTANT_HPP__