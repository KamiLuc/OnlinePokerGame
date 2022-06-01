#include "ClientServerSettingsLoader.h"
#include <string>
#include <fstream>

ClientServerSettingsLoader::ClientServerSettingsLoader(std::string clientSettingsPath, std::string serverSettingsPath,
	sf::Font* fontPointer, sf::Color messageColor, float messageXPos, float messageYPos, int fontSize) :
	serverSettingsPath(serverSettingsPath),
	clientSettingsPath(clientSettingsPath),
	ipRegex("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])"),
	portRegex("^[1-9][0-9]{3}$"),
	playerNameRegex("^[a-zA-Z0-9]{1,10}$"),
	startTokensRegex("^[1-9][0-9]{3,5}$"),
	blindRegex("^[1-9][0-9]{0,4}$"),
	numberOfPlayersRegex("^[3-9]{1}$"),
	bigBlind(0), smallBlind(0), numberOfPlayers(0), startTokens(0), port(0), ip(""), playerName("")
{
	errorMessageText.setFont(*fontPointer);
	errorMessageText.setString("");
	errorMessageText.setPosition(sf::Vector2f(messageXPos, messageYPos));
	errorMessageText.setCharacterSize(fontSize);
}

bool ClientServerSettingsLoader::loadServerSettings(void)
{
	if (!std::filesystem::exists(serverSettingsPath))
	{
		errorMessage = serverSettingsPath.string() + " " + "doesn't exist.";
		return false;
	}
	else
	{
		std::ifstream file;
		std::string tempString;
		file.open(serverSettingsPath.string());
		std::getline(file, tempString);
		if (validatePort(tempString))
		{
			port = std::stoi(tempString);
			std::getline(file, tempString);
			if (validateNumberOfPlayers(tempString))
			{
				numberOfPlayers = std::stoi(tempString);
				std::getline(file, tempString);
				if (validateNumberOfStartTokens(tempString))
				{
					startTokens = std::stoi(tempString);
					std::getline(file, tempString);
					if (validateSmallBlind(tempString))
					{
						smallBlind = std::stoi(tempString);
						std::getline(file, tempString);
						if (validateBigBlind(tempString))
						{
							bigBlind = std::stoi(tempString);
							errorMessage = "";
							file.close();
							return true;
						}
						else
						{
							errorMessage = "Invalid big blind value.\nValue must be greater than 2 * big blind value.";
							file.close();
							return false;
						}
					}
					else
					{
						errorMessage = "Invalid small blind value.\nValue must be smaller than 10% of number of tokens.";
						file.close();
						return false;
					}
				}
				else
				{
					errorMessage = "Invalid number of start tokens, must be in range <1000-999999>";
					file.close();
					return false;
				}
			}
			else
			{
				errorMessage = "Invalid number of players, must be in range <3-8>";
				file.close();
				return false;
			}
		}
		else
		{
			errorMessage = "Invalid port, must be in range <1000-9000>";
			file.close();
			return false;
		}
	}
}

bool ClientServerSettingsLoader::loadClientSettings(void)
{
	if (!std::filesystem::exists(clientSettingsPath))
	{
		errorMessage = clientSettingsPath.string() + " " + "doesn't exist.";
		return false;
	}
	else
	{
		std::string tempString;
		std::ifstream file;
		file.open(clientSettingsPath.string());
		std::getline(file, tempString);
		if (validatePlayerName(tempString))
		{
			playerName = tempString;
			std::getline(file, tempString);
			if (validateIp(tempString))
			{
				ip = tempString;
				std::getline(file, tempString);
				if (validatePort(tempString))
				{
					port = std::stoi(tempString);
					errorMessage = "";
					file.close();
					return true;
				}
				else
				{
					errorMessage = "Invalid port, must be in range <1000-9000>";
					file.close();
					return false;
				}
			}
			else
			{
				errorMessage = "Invalid ip address.";
				file.close();
				return false;
			}
		}
		else
		{
			errorMessage = "Invalid player name, must be a string of 1 to 10 characters.";
			file.close();
			return false;
		}
	}
}

void ClientServerSettingsLoader::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	errorMessageText.setString(errorMessage);
	target.draw(errorMessageText, states);
}

bool ClientServerSettingsLoader::validateIp(std::string ip)
{
	if (std::regex_match(ip, ipRegex))
	{
		return true;
	}
	else
		return false;
}

bool ClientServerSettingsLoader::validatePort(std::string port)
{
	if (std::regex_match(port, portRegex))
	{
		return true;
	}
	else
		return false;
}

bool ClientServerSettingsLoader::validateNumberOfPlayers(std::string numberOfPlayers)
{
	if (std::regex_match(numberOfPlayers, numberOfPlayersRegex))
	{
		return true;
	}
	else
		return false;
}

bool ClientServerSettingsLoader::validateNumberOfStartTokens(std::string startTokens)
{
	if (std::regex_match(startTokens, startTokensRegex))
	{
		return true;
	}
	else
		return false;
}

bool ClientServerSettingsLoader::validateSmallBlind(std::string smallBlind)
{
	if (std::regex_match(smallBlind, blindRegex))
	{
		if (std::stoi(smallBlind) <= 0.1 * startTokens)
		{
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool ClientServerSettingsLoader::validateBigBlind(std::string bigBlind)
{
	if (std::regex_match(bigBlind, blindRegex))
	{
		if (std::stoi(bigBlind) >= 2 * smallBlind)
		{
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool ClientServerSettingsLoader::validatePlayerName(std::string playerName)
{
	if (std::regex_match(playerName, playerNameRegex))
	{
		return true;
	}
	else
		return false;
}


