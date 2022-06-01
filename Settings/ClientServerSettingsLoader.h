#pragma once
#include <filesystem>
#include <regex>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

class ClientServerSettingsLoader : public sf::Drawable
{
private:

	//describes problem as drawable text
	mutable sf::Text errorMessageText;

	std::filesystem::path serverSettingsPath;
	std::filesystem::path clientSettingsPath;

	//describes problem
	std::string errorMessage = "";

	//typical ip regex
	std::regex ipRegex;

	//int <1000-9999>
	std::regex portRegex;

	//string with 10 chars
	std::regex playerNameRegex;

	//int <1000-999999>
	std::regex startTokensRegex;

	//big blind and small blind values
	//int <1-99999>
	std::regex blindRegex;

	//int <3-9>
	std::regex numberOfPlayersRegex;

public:
	sf::IpAddress ip;

	std::string playerName = "";

	int port = 0;
	int startTokens = 0;

	//should be smaller than 0.1 * tokens;
	int smallBlind = 0;

	//shoulde be bigger or equal smallBlind * 2
	int bigBlind = 0;
	int numberOfPlayers = 0;

private:
	bool validateIp(std::string ip);
	bool validatePort(std::string port);
	bool validateNumberOfPlayers(std::string numberOfPlayers);
	bool validateNumberOfStartTokens(std::string startTokens);
	bool validateSmallBlind(std::string smallBlind);
	bool validateBigBlind(std::string bigBlind);
	bool validatePlayerName(std::string playerName);

public:
	ClientServerSettingsLoader(std::string clientSettingsPath, std::string serverSettingsPath,
		sf::Font* fontPointer, sf::Color messageColor,
		float messageXpos, float messageYpos, int fontSize);

	std::string getErrorMessage(void) const { return errorMessage; }
	bool loadServerSettings(void);
	bool loadClientSettings(void);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
};