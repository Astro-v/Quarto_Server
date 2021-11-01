/*---- LIBRARY ----*/
#include <iostream>
#include <vector>
#include <string>

/*---- LIBRARY SFML ----*/
#include "SFML/Network.hpp"

/*---- LOCAL FILE ----*/
#include "constant.hpp"
#include "server.hpp"

Server::Server():_nbrPlayer(0){
	_listener.listen(SERVER_PORT);
	_addressPlayer.push_back(sf::IpAddress("0.0.0.0"));
	_addressPlayer.push_back(sf::IpAddress("0.0.0.0"));
	_portPlayer.push_back(0);
	_portPlayer.push_back(0);
	_namePlayer.push_back("unknown");
	_namePlayer.push_back("unknown");
	_packetR.clear();
	_packetS.clear();
}

Server::~Server(){
	_listener.close();
	for (int index=0;index<2 && (_addressPlayer[index] != sf::IpAddress("0.0.0.0") || _portPlayer[index] != 0);++index){
		_socket[index].disconnect();
	}
}

/*
Wait until the two player connect to the server
*/
void Server::initialize(const ToSend &dataS){
	std::cout << "Waiting for player" << std::endl;
	_socket[0].setBlocking(true);
	_socket[1].setBlocking(true);
	while (_nbrPlayer < 2){
		switch (_nbrPlayer){
			case 0:
				if (_listener.accept(_socket[0]) == sf::Socket::Done)
				{
					// A new client just connected!
					++_nbrPlayer;
					_addressPlayer[0] = _socket[0].getRemoteAddress();
					_portPlayer[0] = _socket[0].getRemotePort();
					_packetR.clear();
					_socket[0].receive(_packetR);
					_packetR >> _typeReceive;
					if (_typeReceive == CONNECT_CLIENT){
						_packetR >> _namePlayer[0];
					}
					_packetR.clear();
					std::cout << _nbrPlayer << " connected ---- New connected : (address)" << _addressPlayer[0] << " (port)" << _portPlayer[0] << " (name)" << _namePlayer[0] << std::endl;				
				}
			break;
			case 1:
				int index = 0;
				if (_addressPlayer[index] != sf::IpAddress("0.0.0.0") || _portPlayer[index] != 0){
					++index;
				}
				if (_listener.accept(_socket[index]) == sf::Socket::Done)
				{
					// A new client just connected!
					++_nbrPlayer;
					_addressPlayer[index] = _socket[index].getRemoteAddress();
					_portPlayer[index] = _socket[index].getRemotePort();
					
					_packetR.clear();
					_socket[index].receive(_packetR);
					_packetR >> _typeReceive;
					if (_typeReceive == CONNECT_CLIENT){
						_packetR >> _namePlayer[index];
					}
					_packetR.clear();
					std::cout << _nbrPlayer << " connected ---- New connected : (address)" << _addressPlayer[index] << " (port)" << _portPlayer[index] << " (name)" << _namePlayer[index] << std::endl;
				}
			break;
		}
	}
	/*while (_nbrPlayer < 2){
		_packetR.clear();
		if (_socket.receive(_packetR,_lastAddress,_lastPort) == sf::Socket::Done){
			_packetR >> _typeReceive;
			if (_typeReceive == CONNECT_CLIENT && unregistred(_lastAddress,_lastPort)){
				++_nbrPlayer;
				int index = 0;
				if (_addressPlayer[index] != sf::IpAddress("0.0.0.0") || _portPlayer[index] != 0){
					++index;
				}
				_portPlayer[index] = _lastPort;
				_addressPlayer[index] = _lastAddress;
				_packetR >> _lastName;
				_namePlayer[index] = _lastName;
				std::cout << _nbrPlayer << " players connected" << std::endl << "Last connected : " << _lastName << std::endl;
				std::cout << "Ip address : " << _lastAddress << std::endl << "Port : " << _lastPort << std::endl << std::endl;
			}else if (_typeReceive == LEAVE && !unregistred(_lastAddress,_lastPort)){
				--_nbrPlayer;
				int index = 0;
				while (_addressPlayer[index] != _lastAddress || _portPlayer[index] != _lastPort){
					++index;
				}
				std::cout << _namePlayer[index] << " disconected" << std::endl << "Only " << _nbrPlayer << " player left " << std::endl << std::endl;
				_addressPlayer[index] = sf::IpAddress("0.0.0.0");
				_portPlayer[index] = 0;
				_namePlayer[index] = "unknown";
			}
		}else{
			// error ...
		}
		_packetR.clear();
	}*/

	sendData(dataS,CONNECT_SERVER);
	sendData(dataS,GAME_SERVER);
	_socket[0].setBlocking(false);
	_socket[1].setBlocking(false);
	std::cout << "Start playing" << std::endl;
}

/*
Return :
-1 : Not the good sender
0 : Nothing have been send
1 : P1 have send
2 : P2 have send
*/
int Server::receiveData(ToReceive &dataR, ToSend &dataS)
{
	_packetR.clear();
	for (int index=0;index<2;++index){
		if (_socket[index].receive(_packetR) == sf::Socket::Done)
		{
			_packetR >> _typeReceive;
			if (_typeReceive == GAME_CLIENT){
				_packetR >> dataR;
				_packetR.clear();
				return index+1;
			}else if (_typeReceive == LEAVE){
				--_nbrPlayer;
				std::cout << _namePlayer[index] << " disconected" << std::endl << "Only " << _nbrPlayer << " player left " << std::endl << std::endl;
				_addressPlayer[index] = sf::IpAddress("0.0.0.0");
				_portPlayer[index] = 0;
				_namePlayer[index] = "unknown";
				_socket[index].disconnect();
				initialize(dataS);
			}
		}
	}
	return 0; // No data received
}

void Server::sendData(const ToSend &data, const TypeSend &typeSend){
	if (typeSend == GAME_SERVER){
		_packetS.clear();
		_packetS << typeSend << data.status << data;
		_socket[0].send(_packetS);
		_packetS.clear();
		_packetS << typeSend;
		if (data.status == PLAYER_1_PICK){
			_packetS << PLAYER_2_PICK;
		}else if (data.status == PLAYER_2_PICK){
			_packetS << PLAYER_1_PICK;
		}else if (data.status == PLAYER_1_PLACE){
			_packetS << PLAYER_2_PLACE;
		}else if (data.status == PLAYER_2_PLACE){
			_packetS << PLAYER_1_PLACE;
		}else{
			_packetS << data.status;
		}
		_packetS << data;
		_socket[1].send(_packetS);
		_packetS.clear();
	}else if (typeSend == WIN_P1){
		_packetS.clear();
		_packetS << WIN_P1;
		_socket[0].send(_packetS);
		_packetS.clear();
		_packetS << WIN_P2;
		_socket[1].send(_packetS);
		_packetS.clear();
	}else if (typeSend == WIN_P2){
		_packetS.clear();
		_packetS << WIN_P2;
		_socket[0].send(_packetS);
		_packetS.clear();
		_packetS << WIN_P1;
		_socket[1].send(_packetS);
		_packetS.clear();
	}else if (typeSend == CONNECT_SERVER){
		_packetS.clear();
		_packetS << typeSend << _namePlayer[1];
		_socket[0].send(_packetS);
		_packetS.clear();
		_packetS << typeSend << _namePlayer[0];
		_socket[1].send(_packetS);
		_packetS.clear();
	}
}

bool Server::unregistred(sf::IpAddress address, unsigned short port){
	for (int index=0;index<_nbrPlayer;++index){
		if (_addressPlayer[index] == address && _portPlayer[index] == port){
			return false;
		}
	}
	return true;
}


/*---- FLUX OPERATOR ----*/

sf::Packet& operator <<(sf::Packet& packet, const ToSend& data){   
    for (int index=0;index<NUMBER_PIECES;++index){
    	packet << data.posX[index];
    }
    for (int index=0;index<NUMBER_PIECES;++index){
    	packet << data.posY[index];
    }
	for (int index=0;index<NUMBER_PIECES;++index){
    	packet << data.used[index];
    }
    return packet << data.pick;
}

sf::Packet& operator <<(sf::Packet& packet, const TypeSend& data){   
	int send;
	send = (int)data;
    return packet << send;
}

sf::Packet& operator >>(sf::Packet& packet, ToReceive& data){
    return packet >> data.posX >> data.posY;
}

sf::Packet& operator >>(sf::Packet& packet, TypeReceive& data){
	int receive;
	packet >> receive;
	data = (TypeReceive)receive;
    return packet;
}