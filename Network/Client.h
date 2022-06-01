#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <list>
#include <semaphore>
#include "PacketCodes.h"
#include "../OtherComponents/TextOptions.h"



class Client : public sf::Drawable
{
private:
	sf::TcpSocket socket;
	sf::IpAddress serverIp;

	std::string playerName;
	
	int serverPort;

	std::atomic<bool> stopPacketReceiving;
	std::atomic<bool> startLobby;
	std::atomic<bool> startPoker;
	std::atomic<bool> errorOccured;
	std::atomic<bool> connectionLost;

	//connecting to server, etc
	sf::Text message;

	//players connected, players required
	sf::Text lobbyInfo;

	//Thread will be trying to receive packet from server
	sf::Thread thGetPacket;
	sf::Thread runClientThread;
	std::list<sf::Packet> gamePackets;

	mutable std::binary_semaphore binarySemaphore;

	void getPacket(void);
	void startPacketReceiving(void);
	void runClient(void);

public:
	Client(sf::IpAddress serverIp, int serverPort, std::string playerName,
		const TextOptions &messageTextOptions, const TextOptions &lobbyTextOptions);
	void run(void);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
	void sendPacket(sf::Packet& packet);
	bool shouldLobbyStart(void) const {return startLobby.load(); }
	bool shouldPokerStart(void) const { return startPoker.load(); }
	bool isError(void) const { return errorOccured.load(); }
	bool isConnectionLost(void) const { return connectionLost.load(); }
	bool pollPacket(sf::Packet& packet);
	void terminateThread(void) { thGetPacket.terminate(); runClientThread.terminate(); }
	~Client();
};