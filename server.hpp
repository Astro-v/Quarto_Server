#ifndef __SERVER_HPP__
#define __SERVER_HPP__

/*---- LIBRARY ----*/
#include <iostream>
#include <vector>
#include <string>

/*---- LIBRARY SFML ----*/
#include "SFML/Network.hpp"

/*---- LOCAL FILE ----*/
#include "constant.hpp"

class Server {
public:
    Server();
	~Server();
    void initialize(const ToSend &data);
    int receiveData(ToReceive &dataR, ToSend &dataS);
	void sendData(const ToSend &data, const TypeSend &typeSend);

protected:
	bool unregistred(sf::IpAddress address, unsigned short port);

private:
	sf::TcpListener _listener;
    sf::TcpSocket _socket[2];
	sf::Packet _packetR;                       // packet to receive
	sf::Packet _packetS;                       // packet to send
	TypeReceive _typeReceive;
    unsigned short _lastPort;                  // For the last sender
	sf::IpAddress _lastAddress;                // For the last sender
	std::string _lastName;                     // For the last sender
	std::vector<sf::IpAddress> _addressPlayer;
	std::vector<unsigned short> _portPlayer;
	std::vector<std::string> _namePlayer;
	int _nbrPlayer;
};

sf::Packet& operator <<(sf::Packet& packet, const ToSend& data);    // Overload of the packet in order to allow Packet << Message
sf::Packet& operator <<(sf::Packet& packet, const TypeSend& data);
sf::Packet& operator >>(sf::Packet& packet, ToReceive& data);       // Overload of the packet in order to allow Packet >> Message
sf::Packet& operator >>(sf::Packet& packet, TypeReceive& data);
#endif // __SERVER_HPP__