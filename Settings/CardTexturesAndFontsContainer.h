#pragma once
#include <map>
#include <SFML/Graphics.hpp>
#include <filesystem>

class CardTexturesAndFontsContainer
{
private:
	//-1 reverse, 2, 10, 11 - jack, 12 - Queen, 13 - King, 14 - Ace...
	std::map<int, sf::Texture> cardTextures;

	//c - clubs, d - diamonds, h - hearts, s - spades
	std::map<char, sf::Texture> colorTextures;

	//arial - arial
	std::map<std::string, sf::Font> fonts;

	//font pointers
	std::map<std::string, sf::Font*> fontsPointers;

	CardTexturesAndFontsContainer(void) {};

public:
	//Singleton stuff
	static CardTexturesAndFontsContainer& getInstance()
	{
		static CardTexturesAndFontsContainer instance;
		return instance;
	}

	CardTexturesAndFontsContainer(const CardTexturesAndFontsContainer& x) = delete;
	CardTexturesAndFontsContainer(const CardTexturesAndFontsContainer&& x) = delete;

	void loadTexturesAndFonts(std::filesystem::path cardTexturesPath,
		std::filesystem::path colorTexturesPath,
		std::filesystem::path fontsPath);

	//Operators
	void operator=(const CardTexturesAndFontsContainer& x) = delete;
	sf::Texture& getCardTexture(int cardNumber);
	sf::Texture& getColorTexture(char cardColor);
	sf::Font* getFontPointer(std::string fontName);
	sf::Font* getFontPointer(void);
};
