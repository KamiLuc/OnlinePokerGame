#pragma once
#include <SFML/Graphics.hpp>
#include "../OtherComponents/TextOptions.h"
#include "../Network/PacketCreator.h"
#include "../Games/ServerPokerPlayer.h"

class ServerPoker;
class PacketCreator;

class GameInfo : public sf::Drawable
{
private:
	friend class PacketCreator;

	std::vector<sf::Text> playersInfoTexts;
	std::vector<std::string> playersInfoStrings;
	std::vector<int> passedPlayersIndexes;
	std::vector<int> eliminatedPlayersIndexes;
	int endOfQueuePlayerIndex = 0;
	int playingPlayerIndex = 0;

public:
	GameInfo(TextOptions& textOptions, int textsNumber = 9);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
	void createInfoTextesWithPacket(sf::Packet packet);
	void updateGameInfo(ServerPoker& poker);
};