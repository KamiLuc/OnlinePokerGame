#pragma once
#include <semaphore>
#include "Window.h"
#include "../Network/Client.h"
#include "../Games/ClientSnake.h"

class LobbySnakeWindow : public Window
{
private:
	Client* client;
	ClientSnake* snake;
	mutable std::binary_semaphore binarySemaphore;

	std::atomic<bool> stopSnakeUpdate;

	//without this, player was able to make couple of moves before game updated
	std::atomic<bool> blockMove;

	sf::Thread snakeUpdateThread;

	void cdd(void);
	void snakeUpdate(void);
	void giveReactionTime();

public:
	LobbySnakeWindow(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor,
		Client& client, ClientSnake& clientSnake); 
	void startDrawing();
};