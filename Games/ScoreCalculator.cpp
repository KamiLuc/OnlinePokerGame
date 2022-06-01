#include "ScoreCalculator.h"

void ScoreCalculator::calculatePlayersPoints(std::vector<ServerPokerPlayer>& players, const std::vector<Card>& board)
{

	for (auto& player : players)
	{
		if (player.getIsEliminated() == true)
		{
			continue;
		}
		sortedCardsByNumber.clear();
		sortedClubCardsByNumber.clear();
		sortedDiamondCardsByNumber.clear();
		sortedHeartCardsByNumber.clear();
		sortedSpadeCardsByNumber.clear();
		colorPointer = nullptr;

		std::vector<Card> temp;

		for (auto card : player.getHand())
		{
			temp.push_back(card);
		}

		for (auto card : board)
		{
			temp.push_back(card);
		}

		std::sort(temp.begin(), temp.end(), [](Card& A, Card& B) { return A.getNumber() > B.getNumber(); });

		if (temp.size() != 7)
		{
			throw std::exception("no cards");
		}

		sortedCardsByNumber = temp;

		for (auto& card : temp)
		{
			char color = card.getColor();

			switch (color)
			{
			case 'c':
				sortedClubCardsByNumber.push_back(std::move(card));
				break;

			case 'd':
				sortedDiamondCardsByNumber.push_back(std::move(card));
				break;

			case 'h':
				sortedHeartCardsByNumber.push_back(std::move(card));
				break;

			case 's':
				sortedSpadeCardsByNumber.push_back(std::move(card));
				break;

			default:
				break;
			}
		}

		auto result = findPokerHand();

		if (result.second.size() == 0)
		{
			result.second.push_back(player.getBestCardFromHand());
		}



		player.setPokerHand(result.first);
		player.setBestCards(result.second);
	}
}

const ServerPokerPlayer* ScoreCalculator::findPokerWinner(const std::vector<ServerPokerPlayer>& players) const
{
	if (players.size() == 0)
	{
		throw std::exception("Players empty");
	}

	std::vector<const ServerPokerPlayer*> potentialWinners;
	PokerHand bestPoker = highCard;

	for (auto& player : players)
	{
		if (player.getPokerHand() > bestPoker and !player.getIsEliminated() and !player.getPassedThisRound())
		{
			bestPoker = player.getPokerHand();
		}
	}

	for (auto& player : players)
	{
		if (player.getPokerHand() == bestPoker and !player.getIsEliminated() and !player.getPassedThisRound())
		{
			potentialWinners.push_back(&player);
		}
	}


	if (bestPoker == poker or bestPoker == carriage or bestPoker == full or bestPoker == triple or bestPoker == pair or bestPoker == twoPairs)
	{
		std::sort(potentialWinners.begin(), potentialWinners.end(), [](const ServerPokerPlayer*& A, const ServerPokerPlayer*& B) {
			return A->getBestCardFromBestCards().getNumber() > B->getBestCardFromBestCards().getNumber(); });
	}
	else
	{
		std::sort(potentialWinners.begin(), potentialWinners.end(), [](const ServerPokerPlayer*& A, const ServerPokerPlayer*& B) {
			return A->getBestCardFromHand().getNumber() > B->getBestCardFromHand().getNumber(); });
	}
	return potentialWinners[0];
}

std::pair<PokerHand, std::vector<Card>> ScoreCalculator::findPokerHand() const
{
	std::vector<Card> bestCards;

	if (CheckIfColor(bestCards))
	{
		if (checkIfRoyalPoker(bestCards))
		{
			return { PokerHand::royalPoker, bestCards };
		}

		else if (checkIfPoker(bestCards))
		{
			return { PokerHand::poker, bestCards };
		}
	}

	if (checkIfCardsHaveSameNumber(sortedCardsByNumber, 4, bestCards).first)
	{
		return { PokerHand::carriage, bestCards };
	}

	else if (checkIfFull(bestCards))
	{
		return { PokerHand::full, bestCards };
	}

	else if (CheckIfColor(bestCards))
	{
		return { PokerHand::color, bestCards };
	}

	else if (checkIfStrit(bestCards))
	{
		return { PokerHand::strit, bestCards };
	}

	else if (checkIfCardsHaveSameNumber(sortedCardsByNumber, 3, bestCards).first)
	{
		return { PokerHand::triple, bestCards };
	}

	else if (checkIfFull(bestCards, 2, 2))
	{
		return { PokerHand::twoPairs, bestCards };
	}

	else if (checkIfCardsHaveSameNumber(sortedCardsByNumber, 2, bestCards).first)
	{
		return { PokerHand::pair, bestCards };
	}

	else
	{
		bestCards.clear();
		return { PokerHand::highCard, bestCards };
	}
}

std::pair<bool, int> ScoreCalculator::checkIfCardsHaveSameNumber(const std::vector<Card>& cardVec, int numberOfCards, std::vector<Card>& bestCards) const
{
	std::vector<Card> newBestCards;
	std::vector<int> cardValues;

	for (auto card : cardVec)
	{
		cardValues.push_back(card.getNumber());
	}

	std::sort(cardValues.begin(), cardValues.end());

	cardValues.erase(std::unique(cardValues.begin(), cardValues.end()), cardValues.end());

	for (auto value : cardValues)
	{
		newBestCards.clear();
		int counter = 0;
		for (auto card : cardVec)
		{
			if (value == card.getNumber())
			{
				newBestCards.push_back(card);
				counter++;

				if (counter == numberOfCards)
				{
					bestCards = std::move(newBestCards);
					return { true,value };
				}
			}
		}

	}

	return { false,0 };
}

bool ScoreCalculator::CheckIfColor(std::vector<Card>& bestCards) const
{
	if (sortedHeartCardsByNumber.size() >= 5)
	{
		bestCards = sortedHeartCardsByNumber;
		colorPointer = &sortedHeartCardsByNumber;
		return true;
	}
	else if (sortedClubCardsByNumber.size() >= 5)
	{
		bestCards = sortedClubCardsByNumber;
		colorPointer = &sortedClubCardsByNumber;
		return true;
	}
	else if (sortedDiamondCardsByNumber.size() >= 5)
	{
		bestCards = sortedDiamondCardsByNumber;
		colorPointer = &sortedDiamondCardsByNumber;
		return true;
	}
	else if (sortedSpadeCardsByNumber.size() >= 5)
	{
		bestCards = sortedSpadeCardsByNumber;
		colorPointer = &sortedSpadeCardsByNumber;
		return true;
	}
	else
	{
		return false;
	}
}

bool ScoreCalculator::checkIfRoyalPoker(std::vector<Card>& bestCards) const
{
	std::vector<Card> cards;

	auto vector = *colorPointer;
	int number = 14;
	for (int i = 0; i < 5; i++)
	{
		if (vector[i].getNumber() != number)
		{
			return false;
		}
		number--;
		cards.push_back(vector[i]);
	}

	bestCards = std::move(cards);
	return true;
}

bool ScoreCalculator::checkIfPoker(std::vector<Card>& bestCards) const
{
	std::vector<Card> cards;

	if (colorPointer == nullptr)
	{
		throw std::exception("colorPointer is NULL");
	}
	auto vec = *colorPointer;
	int counter = 0;

	auto it1 = vec.begin();
	auto it2 = it1;
	it2++;
	while (it2 != vec.end())
	{
		if (it1->getNumber() - it2->getNumber() > 1)
		{
			return false;
		}
		cards.push_back(*it1);
		it2++;
		it1++;
	}

	cards.push_back(*it1);
	bestCards = std::move(cards);
	return true;
}

bool ScoreCalculator::checkIfStrit(std::vector<Card>& bestCards) const
{
	auto vec = sortedCardsByNumber;
	int counter = 0;
	auto itBegin = vec.begin();
	auto itEnd = vec.begin();
	std::advance(itEnd, 5);
	std::vector<Card> cards;
	while (itEnd != vec.end() and counter < 5)
	{
		counter = 0;
		cards.clear();
		auto it2 = itBegin;
		auto it3 = it2;
		it3++;
		while (it2 != itEnd)
		{
			if (it2->getNumber() - it3->getNumber() == 1)
			{
				counter++;
				cards.push_back(*it2);
			}
			else
			{
				break;
			}
			it2++;
			it3++;
		}
		itEnd++;
		itBegin++;
	}
	if (counter >= 5)
	{
		bestCards = std::move(cards);
		return { true };
	}
	else
	{
		return false;
	}
}

bool ScoreCalculator::checkIfFull(std::vector<Card>& bestCards, int x, int y) const
{
	auto pair = checkIfCardsHaveSameNumber(sortedCardsByNumber, x, bestCards);
	auto copy = bestCards;

	if (pair.first)
	{
		auto vec = sortedCardsByNumber;
		auto it = vec.begin();
		while (it != vec.end())
		{
			if (it->getNumber() == pair.second)
			{
				vec.erase(it);
				it = vec.begin();
			}
			else
			{
				it++;
			}
		}

		if (checkIfCardsHaveSameNumber(vec, y, bestCards).first)
		{
			for (auto& card : bestCards)
			{
				copy.push_back(std::move(card));
			}
			bestCards = std::move(copy);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


