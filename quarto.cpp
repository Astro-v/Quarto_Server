/*---- LIBRARY ----*/
#include <iostream>

/*---- LIBRARY SFML ----*/

/*---- LOCAL FILE ----*/
#include "quarto.hpp"
#include "constant.hpp"

Quarto::Quarto():_status(PLAYER_1_PICK),_play(true){
}

void Quarto::play(){
    std::cout << "Starting ..." << std::endl;
    _server.initialize();
    while (_play){

		// Receive data
		receive();

		// Update data

		// Send updated data
		_server.sendData(1,_send);
		_server.sendData(2,_send);
    }
}

void Quarto::receive(){
	// Receive information
	switch (_server.receiveData(_receive))
	{
		case 1: // player 1 move
			m_mutex.lock();

			m_mutex.unlock();
			break;
		case 2: // player 2 move
			m_mutex.lock();
			
			m_mutex.unlock();
			break;
		// default: // no data received
	}
}