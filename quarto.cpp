/*---- LIBRARY ----*/
#include <iostream>

/*---- LIBRARY SFML ----*/

/*---- LOCAL FILE ----*/
#include "quarto.hpp"
#include "constant.hpp"

Quarto::Quarto():_status(PLAYER_1_PICK),_play(true),_pick(NONE){
	_send.status = _status;
	_send.pick = _pick;
	for (int index=0;index<NUMBER_PIECES;++index){
		_posX[index] = index/NUMBER_ROW;
		_posY[index] = index%NUMBER_COLUMN;
		_used[index] = false;
		_send.posX[index] = _posX[index];
		_send.posY[index] = _posY[index];
		_send.used[index] = _used[index];
	}
	for (int i=0;i<NUMBER_ROW;++i){
		for (int j=0;j<NUMBER_COLUMN;++j){
			_pieces[i][j] = NONE;
		}
	}
}

void Quarto::play(){
    std::cout << "Initializing server ..." << std::endl;
    _server.initialize(_send);
	std::cout << "Starting ..." << std::endl;
    while (_play){

		// Receive data
		int player(_server.receiveData(_receive, _send));
		if (player == 1 || player == 2){
			if (player == 1 && _status == PLAYER_1_PICK && isPickable(_receive.posX,_receive.posY)){
				// Update data
				_status = PLAYER_2_PLACE;
				_pick = picked(_receive.posX,_receive.posY);
				_send.status = _status;
				_send.pick = _pick;
				
				// Send updated data
				_server.sendData(_send,GAME_SERVER);
			}else if (player == 2 && _status == PLAYER_2_PLACE && isPlacable(_receive.posX,_receive.posY)){
				// Update data
				_status = PLAYER_2_PICK;
				_posX[_pick] = _receive.posX;
				_posY[_pick] = _receive.posY;
				_used[_pick] = true;
				_pieces[_posX[_pick]][_posY[_pick]] = _pick;
				_send.status = _status;
				for (int index=0;index<NUMBER_PIECES;++index){
					_send.posX[index] = _posX[index];
					_send.posY[index] = _posY[index];
					_send.used[index] = _used[index];
				}
				_send.pick = NONE;
				
				// Send updated data
				_server.sendData(_send,GAME_SERVER);

				if (victory(_posX[_pick],_posX[_pick])){
					_play = false;
					_server.sendData(_send,WIN_P2);
				} 
				
				_pick = NONE;
			}else if (player == 2 && _status == PLAYER_2_PICK && isPickable(_receive.posX,_receive.posY)){
				// Update data
				_status = PLAYER_1_PLACE;
				_pick = picked(_receive.posX,_receive.posY);
				_send.status = _status;
				_send.pick = _pick;
				
				// Send updated data
				_server.sendData(_send,GAME_SERVER);
			}else if (player == 1 && _status == PLAYER_1_PLACE && isPlacable(_receive.posX,_receive.posY)){
				// Update data
				_status = PLAYER_1_PICK;
				_posX[_pick] = _receive.posX;
				_posY[_pick] = _receive.posY;
				_used[_pick] = true;
				_pieces[_posX[_pick]][_posY[_pick]] = _pick;
				_send.status = _status;
				for (int index=0;index<NUMBER_PIECES;++index){
					_send.posX[index] = _posX[index];
					_send.posY[index] = _posY[index];
					_send.used[index] = _used[index];
				}

				_send.pick = NONE;
				
				// Send updated data
				_server.sendData(_send,GAME_SERVER);

				if (victory(_posX[_pick],_posX[_pick])){
					_play = false;
					_server.sendData(_send,WIN_P1);
				} 
				
				_pick = NONE;
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
		if (_posX[index] == posX && _posY[index] == posY && !_used[index]){
			return true;
		} 
	}
	return false;
}

int Quarto::picked(int posX, int posY) const{
	for (int index=0;index<NUMBER_PIECES;++index){
		if (_posX[index] == posX && _posY[index] == posY && !_used[index]){
			return index;
		} 
	}
	return -1;
}

bool Quarto::isPlacable(int posX, int posY) const{
	for (int index=0;index<NUMBER_PIECES;++index){
		if (_posX[index] == posX && _posY[index] == posY && _used[index]){
			return false;
		} 
	}
	return true;
}

bool Quarto::victory(int i, int j){
	// Victory over the row
	if (_pieces[(i+1)%NUMBER_ROW][j] != NONE && _pieces[(i+2)%NUMBER_ROW][j] != NONE && _pieces[(i+3)%NUMBER_ROW][j] != NONE){
		if (rapidTestRow(j,1) || rapidTestRow(j,2) || rapidTestRow(j,4) || rapidTestRow(j,8)){
			return true; // Victory
		}
	}
	// Victory over the column
	if (_pieces[i][(j+1)%NUMBER_COLUMN] != NONE && _pieces[i][(j+2)%NUMBER_COLUMN] != NONE && _pieces[i][(j+3)%NUMBER_COLUMN] != NONE){
		if (rapidTestColumn(i,1) || rapidTestColumn(i,2) || rapidTestColumn(i,4) || rapidTestColumn(i,8)){
			return true; // Victory
		}
	}
	// Victory over the anti diagonal
	if (i+j==3 && _pieces[(i+1)%NUMBER_ROW][(j-1)%NUMBER_COLUMN] != NONE && _pieces[(i+2)%NUMBER_ROW][(j-2)%NUMBER_COLUMN] != NONE && _pieces[(i+3)%NUMBER_ROW][(j-3)%NUMBER_COLUMN] != NONE){
		if (rapidTestAntiDiag(1) || rapidTestAntiDiag(2) || rapidTestAntiDiag(4) || rapidTestAntiDiag(8)){
			return true; // Victory
		}
	}
	// Victory over the anti diagonal
	if (i==j && _pieces[(i+1)%NUMBER_ROW][(j+1)%NUMBER_COLUMN] != NONE && _pieces[(i+2)%NUMBER_ROW][(j+2)%NUMBER_COLUMN] != NONE && _pieces[(i+3)%NUMBER_ROW][(j+3)%NUMBER_COLUMN] != NONE){
		if (rapidTestDiag(1) || rapidTestDiag(2) || rapidTestDiag(4) || rapidTestDiag(8)){
			return true; // Victory
		}
	}
	return false;
}

bool Quarto::rapidTestRow(int j, int k){
	return (_pieces[0][j]/k)%2 == (_pieces[1][j]/k)%2 && (_pieces[0][j]/k)%2 == (_pieces[2][j]/k)%2 && (_pieces[0][j]/k)%2 == (_pieces[3][j]/k)%2;
}

bool Quarto::rapidTestColumn(int i, int k){
	return (_pieces[i][0]/k)%2 == (_pieces[i][1]/k)%2 && (_pieces[i][0]/k)%2 == (_pieces[i][2]/k)%2 && (_pieces[i][0]/k)%2 == (_pieces[i][3]/k)%2;
}

bool Quarto::rapidTestAntiDiag(int k){
	return (_pieces[3][0]/k)%2 == (_pieces[2][1]/k)%2 && (_pieces[3][0]/k)%2 == (_pieces[1][2]/k)%2 && (_pieces[3][0]/k)%2 == (_pieces[0][3]/k)%2;
}

bool Quarto::rapidTestDiag(int k){
	return (_pieces[0][0]/k)%2 == (_pieces[1][1]/k)%2 && (_pieces[0][0]/k)%2 == (_pieces[2][2]/k)%2 && (_pieces[0][0]/k)%2 == (_pieces[3][3]/k)%2;
}