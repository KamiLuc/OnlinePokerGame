#include "CardTexturesAndFontsContainer.h"
#include <iostream>

void CardTexturesAndFontsContainer::loadTexturesAndFonts(std::filesystem::path cardTexturesPath,
	std::filesystem::path colorTexturesPath,
	std::filesystem::path fontsPath)
{
	using std::filesystem::exists;

	if (exists(cardTexturesPath))
	{
		for (auto& file : std::filesystem::directory_iterator(cardTexturesPath))
		{
			cardTextures[std::stoi(file.path().filename().string())] = sf::Texture();
			cardTextures[std::stoi(file.path().filename().string())].loadFromFile(file.path().string());
		}
	}
	else
		throw std::exception("Card textures path doesn't exist!");

	if (exists(colorTexturesPath))
	{
		for (auto& file : std::filesystem::directory_iterator(colorTexturesPath))
		{
			colorTextures[(file.path().filename().string())[0]] = sf::Texture();
			colorTextures[(file.path().filename().string())[0]].loadFromFile(file.path().string());
		}
	}
	else
		throw std::exception("Color textures path doesn't exist!");

	if (exists(fontsPath))
	{
		for (auto& file : std::filesystem::directory_iterator(fontsPath))
		{
			std::string temp = file.path().filename().stem().string();
			fonts[temp] = sf::Font();
			fonts[temp].loadFromFile(file.path().string());
			fontsPointers[temp] = &fonts[temp];
		}
	}
	else
		throw std::exception("Font path doesn't exist!");
}

sf::Texture& CardTexturesAndFontsContainer::getCardTexture(int cardNumber)
{
	return cardTextures[cardNumber];
}

sf::Texture& CardTexturesAndFontsContainer::getColorTexture(char cardColor)
{
	return colorTextures[cardColor];
}


sf::Font* CardTexturesAndFontsContainer::getFontPointer(std::string fontName)
{
	return fontsPointers[fontName];
}

sf::Font* CardTexturesAndFontsContainer::getFontPointer(void)
{
	std::map<std::string, sf::Font*>::iterator iterator = fontsPointers.begin();
	return (*iterator).second;
}
