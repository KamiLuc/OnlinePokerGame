#include "GameInfo.h"

GameInfo::GameInfo(TextOptions& textOptions, int textsNumber) : playersInfoTexts(textsNumber), playersInfoStrings(textsNumber)
{
	int tempXFree = textOptions.getXFreeSpace();
	int tempYFree[3] = { 0, textOptions.getYFreeSpace(), textOptions.getYFreeSpace() * 2 };

	int iterations[3] = { 0,0,0 };
	if (textsNumber > 3)
	{
		iterations[0] = 3;
		if (textsNumber > 6)
		{
			iterations[1] = 3;
			iterations[2] = textsNumber - 6;
		}
		else
			iterations[1] = textsNumber - 3;
	}
	else iterations[0] = textsNumber;

	for (int i = 0; i < iterations[0]; i++)
	{
		textOptions.LoadTextOptions(playersInfoTexts[i]);
		playersInfoTexts[i].move(sf::Vector2f(tempXFree * i, 0));
	}

	for (int i = 0; i < iterations[1]; i++)
	{
		textOptions.LoadTextOptions(playersInfoTexts[i + 3]);
		playersInfoTexts[i + 3].move(sf::Vector2f(tempXFree * i, tempYFree[1]));
	}

	for (int i = 0; i < iterations[2]; i++)
	{
		textOptions.LoadTextOptions(playersInfoTexts[i + 6]);
		playersInfoTexts[i + 6].move(sf::Vector2f(tempXFree * i, tempYFree[2]));
	}
}

void GameInfo::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& info : playersInfoTexts)
	{
		target.draw(info);
	}
}

void GameInfo::createInfoTextesWithPacket(sf::Packet packet)
{
	playersInfoStrings.clear();
	passedPlayersIndexes.clear();
	eliminatedPlayersIndexes.clear();

	int loopsConditions = 0;
	//number of strings (players)
	packet >> loopsConditions;

	for (int i = 0; i < loopsConditions; i++)
	{
		std::string tempString;
		packet >> tempString;
		playersInfoStrings.push_back(tempString);
	}

	for (auto& text : playersInfoTexts)
	{
		text.setFillColor(sf::Color::Black);
		text.setOutlineColor(sf::Color::Black);
		text.setStyle(sf::Text::Regular);
	}

	for (int i = 0; i < playersInfoStrings.size(); i++)
	{
		playersInfoTexts[i].setString(playersInfoStrings[i]);
	}

	packet >> loopsConditions;
	int passedIndex = 0;
	for (int i = 0; i < loopsConditions; i++)
	{
		packet >> passedIndex;
		passedPlayersIndexes.push_back(passedIndex);
		playersInfoTexts[passedIndex].setFillColor(sf::Color::White);
		playersInfoTexts[passedIndex].setOutlineColor(sf::Color::White);
	}

	int eliminatedLoopConditions = 0;
	packet >> eliminatedLoopConditions;
	std::vector<int> eliminatedIndexes;
	int eliminatedIndex = 0;
	for (int i = 0; i < eliminatedLoopConditions; i++)
	{
		packet >> eliminatedIndex;
		eliminatedIndexes.push_back(eliminatedIndex);
	}

	int playingPlayerIndex = 0;
	packet >> playingPlayerIndex;

	playersInfoTexts[playingPlayerIndex].setFillColor(sf::Color::Blue);
	playersInfoTexts[playingPlayerIndex].setOutlineColor(sf::Color::Blue);

	int endOfQueuePlayerIndex = 0;
	packet >> endOfQueuePlayerIndex;

	playersInfoTexts[endOfQueuePlayerIndex].setStyle(sf::Text::Italic);

	for (auto& index : eliminatedIndexes)
	{
		playersInfoTexts[index].setFillColor(sf::Color::Red);
		playersInfoTexts[index].setOutlineColor(sf::Color::Red);
	}
}






