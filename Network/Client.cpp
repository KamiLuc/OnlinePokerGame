#include "Client.h"
#include "../Network/PacketCodes.h"

void Client::getPacket(void)
{
	sf::Packet packet;
	int code = 0;

	while (!stopPacketReceiving.load())
	{
		packet.clear();
		auto status = socket.receive(packet);
		if (status == sf::TcpSocket::Done)
		{

			//get access to code value
			auto voidPtr = packet.getData();
			char* charPtr = (char*)voidPtr;
			charPtr = charPtr + 3;
			code = *charPtr;

			if (code == packetCode::lobbyInfo)
			{
				int playersConnected = 0;
				int playersRequired = 0;
				static std::string tempString = "Players in lobby: ";

				charPtr = charPtr + 4;
				tempString += std::to_string(*charPtr);
				tempString += " | ";

				charPtr = charPtr + 4;
				tempString += std::to_string(*charPtr);

				startLobby.store(true);

				binarySemaphore.acquire();
				
				lobbyInfo.setString(tempString);

				binarySemaphore.release();

				packet.clear();
			}

			else if (code == packetCode::startPoker)
			{
				startPoker.store(true);
			}
			else
			{
				gamePackets.push_back(packet);
			}
		}
		else if (status == sf::TcpSocket::Disconnected)
		{
			startLobby.store(false);
			startPoker.store(false);
			connectionLost.store(true);
		}
	}
}

void Client::startPacketReceiving(void)
{
	//Start thread which will be trying to receive packet
	thGetPacket.launch();
}

Client::Client(sf::IpAddress serverIp, int serverPort, std::string playerName,
	const TextOptions &messageTextOptions, const TextOptions &lobbyTextOptions) :
	thGetPacket(&Client::getPacket, &*this), binarySemaphore{ 1 },
	serverPort(serverPort),
	serverIp(serverIp),
	playerName(playerName),
	runClientThread(&Client::runClient, &*this)
{

	stopPacketReceiving.store(false);
	errorOccured.store(false);
	connectionLost.store(false);

	//setup message
	messageTextOptions.LoadTextOptions(message);
	message.setString("Connecting to server...");

	//setup lobby info
	lobbyTextOptions.LoadTextOptions(lobbyInfo);
}

void Client::runClient(void)
{
	sf::Socket::Status status = socket.connect(serverIp, serverPort);


	//if can't connect to server
	if (status != sf::TcpSocket::Done)
	{
		binarySemaphore.acquire();
		message.setString("Can't connect to server...\nPress enter to exit.");
		binarySemaphore.release();
		errorOccured.store(true);
	}
	else
	{
		//connection successful
		startLobby.store(true);

		//Send packet with playerName
		sf::Packet packet;
		packet << packetCode::playerName;
		packet << playerName;
		socket.send(packet);

		startPacketReceiving();
	}
}

void Client::run(void)
{
	runClientThread.launch();	
}

void Client::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!startLobby.load())
	{
		binarySemaphore.acquire();
		target.draw(message, states);
		binarySemaphore.release();
	}
	else
	{
		target.draw(lobbyInfo, states);
	}
}

void Client::sendPacket(sf::Packet& packet)
{
	socket.send(packet);
}

bool Client::pollPacket(sf::Packet& packet)
{
	packet.clear();
	binarySemaphore.acquire();
	if (!gamePackets.empty())
	{
		packet = gamePackets.front();
		gamePackets.pop_front();
		binarySemaphore.release();
		return true;
	}
	else
	{
		int code = packetCode::empty;
		packet << code;
		binarySemaphore.release();
		return false;
	}
}

Client::~Client()
{
	stopPacketReceiving.store(true);

	//this will also work
	//thGetPacket.terminate();
}
