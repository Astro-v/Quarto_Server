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

    private:
    Status _status;
    
    // SERVER
    Server _server;
};

#endif // __QUARTO_HPP__