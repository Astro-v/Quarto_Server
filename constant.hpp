#ifndef __CONSTANT_HPP__
#define __CONSTANT_HPP__

/*---- LIBRARY ----*/
#include <iostream>

/*---- LIBRARY SFML ----*/

/*---- LOCAL FILE ----*/

/*---- GAME ----*/
#define NUMBER_PIECES 16

/*---- SERVER ----*/
#define SERVER_PORT 5500	
#define SERVER_ADRESS "90.39.145.229" // IP adress of the server

/*---- STATUS ----*/
enum Status{
    PLAYER_1_PICK, PLAYER_2_PLACE, PLAYER_2_PICK, PLAYER_1_PLACE, PAUSE
};

/*---- TO SEND ----*/
struct ToSend{
    Status status;
    int posX[NUMBER_PIECES];
    int posY[NUMBER_PIECES];
};

/*---- TO RECEIVE ----*/
struct ToReceive{
}

#endif // __CONSTANT_HPP__