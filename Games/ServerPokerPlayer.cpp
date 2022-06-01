#include "ServerPokerPlayer.h"

ServerPokerPlayer::ServerPokerPlayer(std::string playerName, int tokens) :
	playerName(playerName), tokens(tokens), isEliminated(false), tokensOnPile(0)
{

}

Card ServerPokerPlayer::getBestCardFromHand(void) const
{
	if (hand.size() == 0)
	{
		throw std::exception("Hand empty");
	}
	const Card* best = &hand[0];

	for (auto& card : hand)
	{
		if (best->getNumber() < card.getNumber())
		{
			best = &card;
		}
	}
	return *best;
}

Card ServerPokerPlayer::getBestCardFromBestCards(void) const
{
	if (bestCards.size() == 0)
	{
		throw std::exception("Best cards empty");
	}
	const Card* best = &bestCards[0];

	for (auto& card : bestCards)
	{
		if (best->getNumber() < card.getNumber())
		{
			best = &card;
		}
	}
	return *best;
}

void ServerPokerPlayer::setLastPlayAndTokens(std::string play, int tokensConnectedWithPlay)
{
	lastPlay = play;
	tokensOnPile += tokensConnectedWithPlay;
	lastPlayTokens = tokensConnectedWithPlay;
	tokens -= tokensConnectedWithPlay;
}

void ServerPokerPlayer::putTokensOnPile(int tokensToPile)
{
	if (tokensToPile > tokens)
	{
		throw std::exception("Player dont have enough tokens to put it on pile");
	}
	else
	{
		tokens -= tokensToPile;
		tokensOnPile += tokensToPile;
	}
}

void ServerPokerPlayer::resetForNextPhase(void)
{
	lastPlayTokens = 0;
	lastPlay = "no play";
}

bool ServerPokerPlayer::readyToStartPhase(void)
{
	if (lastPlayTokens == 0 and lastPlay == "no play")
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ServerPokerPlayer::resetForNextRound(void)
{
	hand.clear();
	bestCards.clear();
	pokerHand = highCard;
	resetForNextPhase();
	tokensOnPile = 0;
	if (!isEliminated)
	{
		passedThisRound = false;
	}
}

void ServerPokerPlayer::eliminatePlayer(void)
{
	isEliminated = true;
	passedThisRound = true;
	tokens = 0;
	tokensOnPile = 0;
}

void ServerPokerPlayer::resetForNextGame(int newTokens)
{
	tokens = newTokens;
	isEliminated = false;
	resetForNextRound();
	hand.clear();
}
