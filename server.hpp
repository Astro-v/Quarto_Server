#ifndef __SERVER_HPP__
#define __SERVER_HPP__

/*---- LIBRARY ----*/
#include <iostream>

/*---- LIBRARY SFML ----*/
#include "SFML/Network.hpp"

/*---- LOCAL FILE ----*/
#include "constant.hpp"

class Server {
    public:
    Server();
    void initialize();
    int receiveData(ToReceive &data);
	void sendData(const int &i,const ToSend &data);

    private:
    sf::UdpSocket _socket;
	sf::Packet _packetR; // packet to receive
	sf::Packet _packetS; // packet to send
    unsigned short _portClient;
	sf::IpAddress _addressClient; // For the last sender
	unsigned short _portP1;
	sf::IpAddress _addressP1; // For the  first player
	unsigned short _portP2;
	sf::IpAddress _addressP2; // For the second player
};

sf::Packet& operator <<(sf::Packet& packet, const ToSend& data); // Overload of the packet in order to allow Packet << Message
sf::Packet& operator >>(sf::Packet& packet, ToReceive& data); // Overload of the packet in order to allow Packet >> Message

#endif // __SERVER_HPP__