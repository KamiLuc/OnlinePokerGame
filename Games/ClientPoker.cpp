#include "ClientPoker.h"

ClientPoker::ClientPoker(Client& client, CardTexturesAndFontsContainer* texturesContainer, ClientPokerPlays& pokerPlays,
	float boardXPos, float boardYPos, float boardXFreeSpace, float boardYFreeSpace, float handXPos,
	float handYPos, float handXFreeSpace, float handYFreeSpace, TextOptions& gameInfoTextOptions,
	TextOptions& endRoundInfoTextOptions, int handSize, int boardSize)
	:
	texturesContainer(texturesContainer),
	board(boardSize),
	client(&client), serverWantsPlay(false), gameInfo(gameInfoTextOptions), roundEndInfo(endRoundInfoTextOptions)
{
	handOptions[0] = handXPos;
	handOptions[1] = handYPos;
	handOptions[2] = handXFreeSpace;
	handOptions[3] = handYFreeSpace;

	for (int i = 0; i < boardSize; i++)
	{
		board[i] = new CardWithSprite(Cards::reverse, Colors::nothing, texturesContainer);
		(*board[i]).setPosition((i * boardXPos) + boardXFreeSpace, (i * boardYPos) + boardYFreeSpace);
	}
}

void ClientPoker::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (roundEndInfo.getIsRoundEndInfoReady())
	{
		target.draw(roundEndInfo);
	}
	else
	{
		for (auto card : board)
		{
			target.draw(*card, states);
		}

		for (auto card : hand)
		{
			target.draw(card, states);
		}
		target.draw(gameInfo);
	}
}

ClientPoker::~ClientPoker()
{
	for (auto card : board)
	{
		delete card;
	}
}

void ClientPoker::putCardToHand(int cardNumber, char cardColor)
{
	ClientCard card(cardNumber, cardColor, texturesContainer);
	card.setPosition(float(handOptions[0] + handOptions[2] * hand.size()),
		float(handOptions[1] + handOptions[3] * hand.size()));
	hand.push_back(std::move(card));
}

void ClientPoker::putCardOnBoard(int cardNumber, char cardColor)
{
	auto position = ((*board[cardsOnBoard]).getPosition());
	board[cardsOnBoard] = new ClientCard(cardNumber, cardColor, texturesContainer);
	(*board[cardsOnBoard]).setPosition(position.x, position.y);
	cardsOnBoard++;
}

void ClientPoker::EndOfRoundInfoDisplayingIsOver(void)
{
	roundEndInfo.cleanInfo();
	roundEndInfo.setIsRoundEndInfoReady(false);
}

void ClientPoker::resetForNextRound(void)
{
	int boardSize = board.size();
	std::vector<sf::Vector2f> oldCardPostitions;

	for (auto card : board)
	{
		oldCardPostitions.push_back(card->getPosition());
	}

	hand.clear();
	hand.shrink_to_fit();
	cardsOnBoard = 0;

	for (int i = 0; i < boardSize; i++)
	{
		board[i] = new CardWithSprite(Cards::reverse, Colors::nothing, texturesContainer);
		board[i]->setPosition(oldCardPostitions[i].x, oldCardPostitions[i].y);
	}
}

bool ClientPoker::sendPacketWithPlay(std::string play, int tokens)
{
	if (serverWantsPlay)
	{
		sf::Packet packet(packetCreator.createPacketWithPlay(play, tokens));
		client->sendPacket(packet);
		serverWantsPlay = false;
		return true;
	}
	else
		return false;
}

bool ClientPoker::updatePoker(void)
{
	sf::Packet packet;
	if ((*client).pollPacket(packet))
	{
		int code = 0;
		packet >> code;

		if (code == packetCode::playRequired)
		{
			serverWantsPlay = true;
		}

		else if (code == packetCode::cardToBoard)
		{
			int number = 0;
			int color = 0;
			packet >> number;
			packet >> color;
			putCardOnBoard(number, char(color));
		}

		else if (code == packetCode::cardToHand)
		{
			int number;
			int color;
			packet >> number;
			packet >> color;
			putCardToHand(number, char(color));
		}

		else if (code == packetCode::resetForNextRound)
		{
			resetForNextRound();
		}

		else if (code == packetCode::gameInfo)
		{
			gameInfo.createInfoTextesWithPacket(packet);
		}

		else if (code == packetCode::gameEnd)
		{
			gameEnd = true;
			packet >> winner;
		}


		else if (code == packetCode::roundEndInfo)
		{
			resetForNextRound();

			roundEndInfo.cleanInfo();
			std::string roundWinner = "";
			packet >> roundWinner;
			int numberOfCards = 0;
			packet >> numberOfCards;
			int cardNumber = 0;
			std::string cardColor = "";
			std::vector<ClientCard> winnerCards;
			for (int i = 0; i < numberOfCards; i++)
			{
				packet >> cardNumber;
				packet >> cardColor;
				ClientCard newCard(cardNumber, cardColor[0], texturesContainer);
				newCard.setPosition(150.f + 120 * i, 255.f);
				winnerCards.push_back(newCard);
			}
			roundEndInfo.setTextAndCards(winnerCards, roundWinner);
			roundEndInfo.setIsRoundEndInfoReady(true);
		}

		return true;
	}
	else
		return false;
}
