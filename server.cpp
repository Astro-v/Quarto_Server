/*---- LIBRARY ----*/
#include <iostream>

/*---- LIBRARY SFML ----*/
#include "SFML/Network.hpp"

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
	_socket.setBlocking(false);
}

/*
Return :
-1 : Not the good sender
0 : Nothing have been send
1 : P1 have send
2 : P2 have send
*/
int Server::receiveData(ToReceive &data)
{
	_packetR.clear();
	if (_socket.receive(_packetR,_addressClient,_portClient) == sf::Socket::Done)
	{
		_packetR >> data;
		_packetR.clear();
		if (_addressClient == _addressP1 && _portClient == _portP1)
		{
			return 1;
		}
		else if (_addressClient == _addressP2 && _portClient == _portP2)
		{
			return 2;
		}
		else
		{
			// Not the good sender
			return -1;
		}
	}
	else
	{
		return 0; // No data received
	}
}

void Server::sendData(const ToSend &data){
	_packetS.clear();
	_packetS << data;
	_socket.send(_packetS, _addressP1, _portP1);
	_socket.send(_packetS, _addressP2, _portP2);
	_packetS.clear();
}

sf::Packet& operator <<(sf::Packet& packet, const ToSend& data){    
	packet << data.status;
    for (int index=0;index<NUMBER_PIECES;++index){
    	packet << data.posX[index];
    }
    for (int index=0;index<NUMBER_PIECES;++index){
    	packet << data.posY[index];
    }
    return packet << data.pick;
}

sf::Packet& operator >>(sf::Packet& packet, ToReceive& data){
    return packet >> data.posX >> data.posY;
}