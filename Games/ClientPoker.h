#pragma once
#include <SFML/Graphics.hpp>
#include "ClientPokerPlays.h"
#include "../Network/PacketCreator.h"
#include "../Games/GameInfo.h"
#include "../Settings/CardTexturesAndFontsContainer.h"
#include "../Games/RoundEndInfo.h"
#include "../Network/Client.h"
#include "../Cards/ClientCard.h"


class ClientPoker : public sf::Drawable
{
private:
	PacketCreator packetCreator;
	GameInfo gameInfo;
	RoundEndInfo roundEndInfo;
	CardTexturesAndFontsContainer* texturesContainer;
	Client* client;
	bool serverWantsPlay = false;
	bool gameEnd = false;
	int cardsOnBoard = 0;
	std::string winner = "";

	//options for cards delivered in future
	//0 xPos, 1 yPos, 2 Xfree, 3 Yfree
	float handOptions[4];
	std::vector<ClientCard> hand;
	std::vector<CardWithSprite*> board;	

public:
	ClientPoker(Client& client, CardTexturesAndFontsContainer* texturesContainer, ClientPokerPlays& pokerPlays,
		float boardXPos, float boardYPos, float boardXFreeSpace, float boardYFreeSpace,
		float handXPos, float handYPos, float handXFreeSpace, float handYFreeSpace, TextOptions& gameInfoTextOptions,
		TextOptions& endRoundInfoTextOptions,
		int handSize = 2, int boardSize = 5);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
	~ClientPoker();
	bool shouldDisplayEndOfRoundInfo(void) const { return roundEndInfo.getIsRoundEndInfoReady(); }
	bool isGameEnd(void) const { return gameEnd; }
	std::string getWinner(void) const { return winner; }
	bool updatePoker(void);
	bool sendPacketWithPlay(std::string play, int tokens);
	void EndOfRoundInfoDisplayingIsOver(void);

private:
	void putCardToHand(int cardNumber, char cardColor);
	void putCardOnBoard(int cardNumber, char cardColor);
	void resetForNextRound(void);
};