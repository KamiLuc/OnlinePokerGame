#pragma once
#include <string>
#include <SFML/Network.hpp>
#include "../Cards/Card.h"
#include "../OtherComponents/PokerHand.h"

class ServerPokerPlayer
{
private:
	PokerHand pokerHand;
	std::vector<Card> hand;
	std::vector<Card> bestCards;
	std::string playerName;
	std::string lastPlay = "no play";
	mutable int tokens;
	int tokensOnPile = 0;
	int score = 0;
	int lastPlayTokens = 0;
	bool isEliminated = false;
	bool passedThisRound = false;

public:
	ServerPokerPlayer(std::string playerName = "", int tokens = 0);
	PokerHand getPokerHand(void) const { return pokerHand; }
	void setPokerHand(PokerHand pH) { pokerHand = pH; }
	bool getIsEliminated(void) const { return isEliminated; }
	void setIsEliminated(bool value) { isEliminated = value; }
	std::vector<Card> getHand(void) const { return hand; }
	std::vector<Card> getBestCards(void) const { return bestCards; }
	void setBestCards(std::vector<Card> cards) { bestCards = cards; }
	void placeCardInHand(Card card) { hand.push_back(card); }
	Card getBestCardFromHand(void) const;
	Card getBestCardFromBestCards(void) const;
	void giveTokens(int value) const { tokens += value; }
	std::string getPlayerName(void) const { return playerName; }
	void setPassedThisRound(bool value) { passedThisRound = value; }
	bool getPassedThisRound(void) const { return passedThisRound; }
	int getLastPlayTokens(void) const { return lastPlayTokens; }
	std::string getLastPlay(void) const { return lastPlay; }
	void setLastPlayAndTokens(std::string play, int tokensConnectedWithPlay);
	int getTokens(void) const { return tokens; }
	int getTokensOnPile(void) const { return tokensOnPile; }
	void putTokensOnPile(int tokensToPile);
	void resetForNextPhase(void);
	bool readyToStartPhase(void);
	void resetForNextRound(void);
	void eliminatePlayer(void);
	void resetForNextGame(int newTokens);
};