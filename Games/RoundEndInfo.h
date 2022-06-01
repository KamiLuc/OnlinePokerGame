#pragma once
#include <SFML/Graphics.hpp>
#include "../Cards/ClientCard.h"
#include "../OtherComponents/TextOptions.h"

class RoundEndInfo : public sf::Drawable
{
private:
	
	bool isRoundEndInfoReady = false;
	sf::Text roundEndText;
	std::vector<ClientCard> cards;
	const std::string halfMessage = "The winner of the round is: ";

public:
	RoundEndInfo(TextOptions& textOptions)
	{
		textOptions.LoadTextOptions(roundEndText);
	};

	void cleanInfo(void)
	{
		roundEndText.setString("");
		cards.clear();
	}

	void setTextAndCards(std::vector<ClientCard> winnerCards, std::string winnerName)
	{
		auto temp = halfMessage;
		temp += winnerName;
		roundEndText.setString(temp);
		cards = std::move(winnerCards);

		for (auto& card : cards)
		{
			card.scale(0.6);
		}
	}

	void setIsRoundEndInfoReady(bool value)
	{
		isRoundEndInfoReady = value;
	}

	bool getIsRoundEndInfoReady(void) const
	{
		return isRoundEndInfoReady;
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const
	{
		if (isRoundEndInfoReady)
		{
			for (auto& element : cards)
			{
				target.draw(element);
			}
			target.draw(roundEndText);
		}
	}

};