#include "Server.h"

void Server::sendLobbyStatus(void) const
{
	sf::Packet lobbyPacket;

	int temp1 = clients.size();
	int temp2 = playersRequired;

	lobbyPacket << packetCode::lobbyInfo;
	lobbyPacket << temp1;
	lobbyPacket << temp2;

	for (auto& client : clients) {
		(client).second->send(lobbyPacket);
	}
}

void Server::preparePlayers(void)
{
	for (auto client : clients)
	{
		clientsMap[client.first] = client.second;
	}
}

void Server::updateLobbyInfo(void)
{
	binarySemaphore.acquire();
	std::string temp = lastAction;
	temp += "\n		Players: ";
	temp += std::to_string(clients.size());
	temp += " | ";
	temp += std::to_string(playersRequired);
	lobbyInfo.setString(temp);
	binarySemaphore.release();
}

void Server::informClientsThatGameIsStarting(void)
{

	//send message to all clients, that game started
	sf::Packet pokerStartedPacket;

	gameReady = true;

	pokerStartedPacket << packetCode::startPoker;
	for (auto& client : clients)
	{
		(*client.second).send(pokerStartedPacket);
	}
}

void Server::runServer()
{
	std::string playerName;

	updateLobbyInfo();

	//packet code
	int code = 0;

	int clientsConnected = 0;

	while (clientsConnected < playersRequired)
	{
		if (socketSelector.wait())
		{
			if (socketSelector.isReady(listener))
			{
				sf::TcpSocket* client = new sf::TcpSocket;
				if (listener.accept(*client) == sf::TcpListener::Done)
				{
					clients.push_back(std::pair<std::string, sf::TcpSocket*>("anonymous", client));
					socketSelector.add(*client);
				}
				else
				{
					delete client;
				}
			}
			if (clients.size() > 0)
			{
				for (auto it = clients.begin(); it != clients.end(); it++)
				{
					if (socketSelector.isReady(*((*it).second)))
					{
						auto client = (*it).second;
						sf::Packet packet;
						auto status = client->receive(packet);
						if (status == sf::TcpSocket::Done)
						{
							packet >> code;
							{
								if (code == packetCode::playerName)
								{
									packet >> playerName;

									//userName must be unique
									for (auto c : Server::clients)
									{
										if (c.first == playerName)
										{
											playerName += std::to_string(unique);
											unique++;
										}
									}
								}
								(*it).first = playerName;

								lastAction = "";
								lastAction += "Player connected! ";
								lastAction += playerName;

								clientsConnected++;

								updateLobbyInfo();
								sendLobbyStatus();
							}
						}
						if (status == sf::TcpSocket::Disconnected)
						{
							clientsConnected--;
							unique++;
							socketSelector.remove(*client);

							lastAction = "";
							lastAction += "Player disconnected! ";
							//client playerName;
							lastAction += (*it).first;



							delete client;
							clients.erase(it);

							updateLobbyInfo();
							sendLobbyStatus();

							break;
						}
					}
				}
			}
		}
	}

	startGame();
}

void Server::startGame(void)
{
	preparePlayers();

	sf::sleep(sf::Time(sf::seconds(1.0f)));

	informClientsThatGameIsStarting();
	startPoker.store(true);
}

Server::Server(sf::IpAddress serverIp, int serverPort, int playersRequired, TextOptions textOptions) :
	playersRequired(playersRequired), serverRunThread(&Server::runServer, &*this), binarySemaphore{ 1 }
{
	listener.listen(serverPort, serverIp);
	socketSelector.add(listener);
	textOptions.LoadTextOptions(lobbyInfo);
	lastAction = "Server started!\n	";
	lastAction += (sf::IpAddress::getLocalAddress()).toString();
}

void Server::sendPacket(std::string playerName, sf::Packet& packet)
{
	clientsMap[playerName]->send(packet);
}

void Server::sendPacketToAll(sf::Packet& packet) const
{
	for (auto& client : clientsMap)
	{
		client.second->send(packet);
	}
}

sf::Packet Server::getAnswerFromClient(std::string playerName)
{
	sf::Packet packet;

	auto socket = clientsMap[playerName];
	//unlimited time to answear
	while (socket->receive(packet) != sf::TcpSocket::Done)
	{
		continue;
	}

	return packet;
}

std::vector<std::string> Server::getPlayersNames(void) const
{
	std::vector<std::string> playersNames;
	for (auto client : clients)
	{
		playersNames.push_back(client.first);
	}
	return playersNames;
}

void Server::run(void)
{
	serverRunThread.launch();
}

void Server::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	binarySemaphore.acquire();
	target.draw(lobbyInfo);
	binarySemaphore.release();
}

Server::~Server()
{
	for (auto& client : clients)
	{
		delete client.second;
	}
	serverRunThread.terminate();
}