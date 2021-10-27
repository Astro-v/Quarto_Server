/*---- LIBRARY ----*/
#include <iostream>

/*---- LIBRARY SFML ----*/

/*---- LOCAL FILE ----*/
#include "constant.hpp"
#include "server.hpp"

Server::Server(){
	if (_socket.bind(SERVER_PORT,SERVER_ADRESS) != sf::Socket::Done){
    	// error...
	}
	_packetR.clear();
	_packetS.clear();
}

/*
Wait until the two player connect to the server
*/
void Server::initialize(){
	std::cout << "waiting for 2 players ..." << std::endl;
	_socket.setBlocking(true);
	int nbrPlayer(0);
	while (nbrPlayer!=2){
		_packetR.clear();
		if (_socket.receive(_packetR,_addressClient,_portClient) == sf::Socket::Done){
			++nbrPlayer;
			if (nbrPlayer == 1){
				_portP1 = _portClient;
				_addressP1 = _addressClient;
				std::cout << "Player 1 connected" << std::endl;
			}else if (nbrPlayer == 2){
				_portP2 = _portClient;
				_addressP2 = _addressClient;
				std::cout << "Player 2 connected" << std::endl;
			}
		}else{
			// error ...
		}
		_packetR.clear();
	}

	_packetS.clear();
	ToSend data = {PLAYER_1_PICK};
	_packetS << data;
	_socket.send(_packetS,_addressP1,_portP1);
	_socket.send(_packetS,_addressP2,_portP2);
	_packetS.clear();

	_socket.setBlocking(false);
}


void Server::sendData(const int &i, const ToSend &data){
	_packetS.clear();
	_packetS << data;
	switch (i){
		case 1: // send to the player 1
			_socket.send(_packetS, _addressP1, _portP1);
			break;
		case 2: // send to the player 2
			_socket.send(_packetS, _addressP2, _portP2);
			break;
		// default: // error
	}
	_packetS.clear();
}

sf::Packet& operator <<(sf::Packet& packet, const ToSend& data){
    return packet << data.posP1 << data.posP2 << data.posBallX << data.posBallY << data.sizeBarP1 << data.sizeBarP2 << data.scoreP1 << data.scoreP2 << data.winner << data.sound << data.bonus1PosX << data.bonus1PosY << data.bonus1Id << data.bonus2PosX << data.bonus2PosY << data.bonus2Id << data.bonus3PosX << data.bonus3PosY << data.bonus3Id<< data.bonus4PosX << data.bonus4PosY << data.bonus4Id << data.bonus5PosX << data.bonus5PosY << data.bonus5Id << data.bonus6PosX << data.bonus6PosY << data.bonus6Id;
}

sf::Packet& operator >>(sf::Packet& packet, ToReceive& data){
    return packet >> data.pos;
}