#ifndef __QUARTO_HPP__
#define __QUARTO_HPP__

/*---- LIBRARY ----*/
#include <iostream>

/*---- LIBRARY SFML ----*/

/*---- LOCAL FILE ----*/
#include "constant.hpp"
#include "server.hpp"

class Quarto{
    public:
    Quarto();
    void play();
    

    private:
    // METHOD
    bool isPlacable(int posX, int posY) const;   // Return true if the given position car receive a piece
    bool isPickable(int posX, int posY) const;   // Return true if the picked piece is pickable
    int picked(int posX, int posY) const;        // Return the picked piece
    bool victory(int i, int j);                  // Retrun true if there is a victory
    bool rapidTestRow(int j, int k);
    bool rapidTestColumn(int i, int k);
    bool rapidTestAntiDiag(int k);
    bool rapidTestDiag(int k);

    // GAME
    Status _status;                               // Status of the game
    bool _play;                                   // Booleen that worth true as long as no one win
    int _posX[NUMBER_PIECES];                     // Abscissa of the pieces
    int _posY[NUMBER_PIECES];                     // Ordinate of the pieces
    bool _used[NUMBER_PIECES];                    // True if a piece is used
    int _pieces[NUMBER_ROW][NUMBER_COLUMN];       // Stock the placed pieces
    int _pick;                                    // Correspond to the picked piece -1:NONE
    
    // SERVER
    Server _server;                               // Server that allow to communicate with client
    ToReceive _receive;              
	ToSend _send;
};

#endif // __QUARTO_HPP__