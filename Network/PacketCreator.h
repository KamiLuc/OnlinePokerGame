#pragma once
#include <string>
#include "../Network/PacketCodes.h"
#include "../Games/GameInfo.h"
#include "../Cards/Card.h"
#include "SFML/Network.hpp"

class GameInfo;

class PacketCreator
{
private:
public:
	sf::Packet createPacketWithCardToBoard(int cardNumber, int cardColor);
	sf::Packet createPacketWithCardToHand(int cardNumber, int cardColor);
	sf::Packet createPacketWithPlay(std::string play, int tokens);
	sf::Packet createPacketWithResetForNextRound(void);
	sf::Packet createPacketWithPlayRequest(void);
	sf::Packet createPacketWithGameInfo(GameInfo& gameInfo);
	sf::Packet createPacketWithGameEndInfo(const std::string winner);
	sf::Packet createPacketWithRoundEndInfo(const std::string roundWinner, const std::vector<Card> winnerCards);

};