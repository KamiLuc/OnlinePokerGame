#pragma once
#include <vector>
#include <algorithm>
#include <map>
#include <ranges>
#include <set>
#include "../Games/ServerPokerPlayer.h"
#include "../Cards/Card.h"

//it is much more simple than in real poker
// club = c = 99
// diamond = d = 100
// heart = h = 104
// spade = s = 115


class ScoreCalculator
{
private:

	std::vector<Card> sortedCardsByNumber;
	std::vector<Card> sortedClubCardsByNumber;
	std::vector<Card> sortedDiamondCardsByNumber;
	std::vector<Card> sortedHeartCardsByNumber;
	std::vector<Card> sortedSpadeCardsByNumber;
	mutable const std::vector<Card>* colorPointer = nullptr;

public:
	ScoreCalculator() {}

	void calculatePlayersPoints(std::vector<ServerPokerPlayer>& players, const std::vector<Card>& board);
	const ServerPokerPlayer* findPokerWinner(const std::vector<ServerPokerPlayer>& players) const;
	
private:
	std::pair<PokerHand, std::vector<Card>> findPokerHand() const;
	std::pair<bool, int> checkIfCardsHaveSameNumber(const std::vector<Card>& cardVec, int numberOfCards, std::vector<Card>& bestCards) const;
	bool CheckIfColor(std::vector<Card>& bestCards) const;
	bool checkIfRoyalPoker(std::vector<Card>& bestCards) const;
	bool checkIfPoker(std::vector<Card>& bestCards) const;
	bool checkIfStrit(std::vector<Card>& bestCards) const;
	bool checkIfFull(std::vector<Card>& bestCards, int x = 3, int y = 2) const;
};