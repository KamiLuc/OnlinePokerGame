#pragma once
#include "../OtherComponents/SwitchingMenu.h"
#include "../Windows/Window.h"
#include "../Settings/ClientServerSettingsLoader.h"

class ClientServerChooseWindow : Window
{
private:
	std::string confrimedOption;
	ClientServerSettingsLoader* loader;
	SwitchingMenu* menu;

	void cdd(void) override;
	
public:
	ClientServerChooseWindow(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor,
		SwitchingMenu &menu,
		ClientServerSettingsLoader &loader);

	void startDrawing(void);
};