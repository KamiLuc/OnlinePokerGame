#include "PacketCreator.h"

sf::Packet PacketCreator::createPacketWithCardToBoard(int cardNumber, int cardColor)
{
	sf::Packet packet;
	packet << packetCode::cardToBoard;
	packet << cardNumber;
	packet << cardColor;
	return packet;
}

sf::Packet PacketCreator::createPacketWithCardToHand(int cardNumber, int cardColor)
{
	sf::Packet packet;
	packet << packetCode::cardToHand;
	packet << cardNumber;
	packet << cardColor;
	return packet;
}

sf::Packet PacketCreator::createPacketWithPlay(std::string play, int tokens)
{
	sf::Packet packet;
	packet << packetCode::clientPlay;
	packet << play;
	packet << tokens;
	return packet;
}

sf::Packet PacketCreator::createPacketWithResetForNextRound(void)
{
	sf::Packet packet;
	packet << packetCode::resetForNextRound;
	return packet;
}

sf::Packet PacketCreator::createPacketWithPlayRequest(void)
{
	sf::Packet packet;
	packet << packetCode::playRequired;
	return packet;
}

sf::Packet PacketCreator::createPacketWithGameInfo(GameInfo& gameInfo)
{
	sf::Packet packet;

	packet << packetCode::gameInfo;
	packet << (int)gameInfo.playersInfoStrings.size();
	for (auto el : gameInfo.playersInfoStrings)
	{
		packet << el;
	}

	packet << (int)gameInfo.passedPlayersIndexes.size();
	for (auto el : gameInfo.passedPlayersIndexes)
	{
		packet << (int)el;
	}

	packet << (int)gameInfo.eliminatedPlayersIndexes.size();
	for (auto el : gameInfo.eliminatedPlayersIndexes)
	{
		packet << (int)el;
	}

	packet << (int)gameInfo.playingPlayerIndex;
	packet << (int)gameInfo.endOfQueuePlayerIndex;

	return packet;
}

sf::Packet PacketCreator::createPacketWithGameEndInfo(const std::string winner)
{
	sf::Packet packet;

	packet << packetCode::gameEnd;
	packet << winner;

	return packet;
}

sf::Packet PacketCreator::createPacketWithRoundEndInfo(const std::string roundWinner, const std::vector<Card> winnerCards)
{
	sf::Packet packet;

	packet << packetCode::roundEndInfo;
	packet << roundWinner;

	packet << (int)winnerCards.size();
	for (auto& card : winnerCards)
	{
		packet << card.getNumber();
		std::string temp = "";
		temp += card.getColor();
		packet << temp;
	}

	return packet;
}
