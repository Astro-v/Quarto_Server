/*---- LIBRARY ----*/
#include <iostream>

/*---- LIBRARY SFML ----*/

/*---- LOCAL FILE ----*/
#include "quarto.hpp"
#include "constant.hpp"

Quarto::Quarto():_status(PLAYER_1_PICK),_play(true){
	for (int index=0;index<NUMBER_PIECES;++index){
		_posX[index]=index/NUMBER_ROW;
		_posY[index]=index%NUMBER_COLUMN;
	}
	_pick = NONE;
}

void Quarto::play(){
    std::cout << "Initializing server ..." << std::endl;
    _server.initialize();
	std::cout << "Starting ..." << std::endl;
    while (_play){

		// Receive data
		int player(_server.receiveData(_receive));
		if (player != 0){
			if (player == 1 && _status == PLAYER_1_PICK && isPickable(_receive.posX,_receive.posY)){
				// Update data
				_status = PLAYER_2_PLACE;
				_pick = picked(_receive.posX,_receive.posY);
				_send.status = _status;
				_send.pick = _pick;
				
				// Send updated data
				_server.sendData(_send);
			}else if (player == 2 && _status == PLAYER_2_PLACE && isPlacable(_receive.posX,_receive.posY)){
				// Update data
				_status = PLAYER_2_PLACE;
				_posX[_pick] = _receive.posX;
				_posY[_pick] = _receive.posY;
				_send.status = _status;
				for (int index=0;index<NUMBER_PIECES;++index){
					_send.posX[index] = _posX[index];
					_send.posY[index] = _posY[index];
				}
				_send.pick = _pick;
				
				// Send updated data
				_server.sendData(_send);
			}else if (player == 2 && _status == PLAYER_2_PICK && isPickable(_receive.posX,_receive.posY)){
				// Update data
				_status = PLAYER_1_PLACE;
				_pick = picked(_receive.posX,_receive.posY);
				_send.status = _status;
				_send.pick = _pick;
				
				// Send updated data
				_server.sendData(_send);
			}else if (player == 1 && _status == PLAYER_1_PLACE && isPlacable(_receive.posX,_receive.posY)){
				// Update data
				_status = PLAYER_2_PLACE;
				_posX[_pick] = _receive.posX;
				_posY[_pick] = _receive.posY;
				_send.status = _status;
				for (int index=0;index<NUMBER_PIECES;++index){
					_send.posX[index] = _posX[index];
					_send.posY[index] = _posY[index];
				}
				_send.pick = _pick;
				
				// Send updated data
				_server.sendData(_send);
			}else{
				// Nothing to do
			}
		}else{
			// erreur
		}
    }
}

bool Quarto::isPickable(int posX, int posY) const{
	for (int index=0;index<NUMBER_PIECES;++index){
		if (_posX[index] == posX && _posY[index] == posY){
			return true;
		} 
	}
	return false;
}

int Quarto::picked(int posX, int posY) const{
	for (int index=0;index<NUMBER_PIECES;++index){
		if (_posX[index] == posX && _posY[index] == posY){
			return index;
		} 
	}
	return -1;
}

bool Quarto::isPlacable(int posX, int posY) const{
	for (int index=0;index<NUMBER_PIECES;++index){
		if (_posX[index] == posX && _posY[index] == posY){
			return false;
		} 
	}
	return true;
}