#include "ClientPokerPlays.h"

ClientPokerPlays::ClientPokerPlays(std::vector<std::string>& pokerPlaysStringVector, sf::Font* fontPointer,
	float xPos, float yPos, float xFreeSpace, float yFreeSpace, int fontSize, sf::Color textColor, sf::Color markedTextColor,
	float markXSize, int maxTokens) :
	SwitchingMenu(pokerPlaysStringVector, fontPointer,
		xPos, yPos, xFreeSpace, yFreeSpace, fontSize,
		textColor, markedTextColor),
	tokensString("0"), maxTokens(maxTokens)
{
	tokensText.setFillColor(textColor);
	tokensText.setString(tokensString);
	tokensText.setFont(*fontPointer);
	tokensText.setOrigin(0.f, 0.f);
	tokensText.setCharacterSize(fontSize);
	tokensText.setPosition(float(xFreeSpace * pokerPlaysStringVector.size() + xPos), yPos);
	tokensRectangleMark.setSize(sf::Vector2f(markXSize, fontSize));
	tokensRectangleMark.setFillColor(markedTextColor);
	tokensRectangleMark.setPosition(tokensText.getPosition());
	tokensRectangleMark.move(0.f, 3.5f);
}

void ClientPokerPlays::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(tokensRectangleMark);
	target.draw(tokensText);
	SwitchingMenu::draw(target, states);
}

int ClientPokerPlays::returnTokens(void)
{
	if (tokensString == "" or tokensString == "0")
		return 0;
	return std::stoi(tokensString);
}

//changes input from keyboard into tokens
bool ClientPokerPlays::digitToTokens(std::string digit)
{
	if (tokensString.size() == 0 and digit == "0" or tokensString.size() > maxTokens)
	{
		return false;
	}
	else if (tokensString.size() == 1 and tokensString[0] == '0')
	{
		tokensString.clear();
	}
	tokensString += digit;
	tokensText.setString(tokensString);
	return true;
}

//shrinks tokens field
bool ClientPokerPlays::deleteDigit(void)
{
	if (tokensString.size() > 1)
	{
		tokensString.pop_back();
		tokensText.setString(tokensString);
		return true;
	}
	else if (tokensString.size() == 1)
	{
		tokensString = "0";
		tokensText.setString(tokensString);
		return true;
	}
	else
		return false;
}
