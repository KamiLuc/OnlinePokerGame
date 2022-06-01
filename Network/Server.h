#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <semaphore>
#include <list>
#include <algorithm>
#include "PacketCodes.h"
#include "../OtherComponents/TextOptions.h"


class Server : public sf::Drawable
{
private:

	sf::TcpListener listener;
	//used in setup
	std::list<std::pair<std::string, sf::TcpSocket*>> clients;

	//used for easier communication
	std::map<std::string, sf::TcpSocket*> clientsMap;

	sf::SocketSelector socketSelector;

	int playersRequired = 0;

	std::string lastAction = "";

	sf::Text lobbyInfo;

	sf::Thread serverRunThread;
	mutable std::binary_semaphore binarySemaphore;

	//used to make uqnique players names
	int unique = 1;

	bool gameReady = false;
	std::atomic<bool> startPoker = false;

	void sendLobbyStatus(void) const;
	void preparePlayers(void);
	void updateLobbyInfo(void);
	void informClientsThatGameIsStarting(void);
	void runServer();
	void startGame(void);

public:
	Server(sf::IpAddress serverIp, int serverPort, int playersRequired, TextOptions textOptions);
	bool isGameReady(void) const { return gameReady; }
	bool shouldGameStart(void) const { return startPoker.load(); }
	void sendPacket(std::string playerName, sf::Packet& packet);
	void sendPacketToAll(sf::Packet& packet) const;
	sf::Packet getAnswerFromClient(std::string playerName);
	std::vector<std::string> getPlayersNames(void) const;
	void run(void);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
	~Server();
};

