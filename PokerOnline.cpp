#include <iostream>
#include "Settings/CardTexturesAndFontsContainer.h"
#include "Settings/ClientServerSettingsLoader.h"
#include "OtherComponents/SwitchingMenu.h"
#include "OtherComponents/TextOptions.h"
#include "Windows/ClientServerChooseWindow.h"
#include "Windows/ServerWindow.h"
#include "Windows/ClientWindow.h"
#include "Windows/GameEndWindow.h"
#include "Windows/ClientPokerWindow.h"
#include "Windows/LobbySnakeWindow.h"
#include "Windows/ServerPokerWindow.h"
#include "Network/Server.h"
#include "Network/Client.h"
#include "Games/ServerPoker.h"
#include "Games/ClientSnake.h"
#include "Games/ClientPokerPlays.h"
#include "Games/ClientPoker.h"


int main()
{
    CardTexturesAndFontsContainer::getInstance().loadTexturesAndFonts("CardsTextures",
        "ColorsTextures",
        "Fonts");

    auto CardTexturesAndFontsContainer = &CardTexturesAndFontsContainer::getInstance();
    auto font = CardTexturesAndFontsContainer->getFontPointer();

    auto windowPointer = std::make_shared<sf::RenderWindow>(sf::VideoMode(1080, 720),
        "Poker!", sf::Style::Titlebar | sf::Style::Close);

    ClientServerSettingsLoader clientServerSettingsLoader("ClientServerSettings/Client.txt",
        "ClientServerSettings/Server.txt",
        font,
        sf::Color::White,
        20.f, 625.f, 30);

    SwitchingMenu clientServerChooseMenu(std::vector<std::string>{ "Client", "Server" },
        font,
        410.f, 220.f, 0.f, 140.f, 80,
        sf::Color::White, sf::Color::Blue);

    ClientServerChooseWindow clientServerChooseWindow(windowPointer,
        sf::Color::Black,
        clientServerChooseMenu,
        clientServerSettingsLoader);

    clientServerChooseWindow.startDrawing();

    if (clientServerChooseMenu.getMarkedOption() == "Server")
    {
        TextOptions serverTextOptions(300.f, 240.f, font, 50);
        Server server(sf::IpAddress::getLocalAddress(),
            clientServerSettingsLoader.port,
            clientServerSettingsLoader.numberOfPlayers,
            serverTextOptions);

        server.run();

        ServerWindow serverWindow(windowPointer,
            sf::Color::Black,
            server);

        serverWindow.startDrawing();

        if (server.isGameReady())
        {
            TextOptions serverPokerTextOptions(25.f, 25.f, font, 30, sf::Color::White,"", 325, 215);
            ServerPoker serverPoker(server,
                clientServerSettingsLoader.startTokens,
                clientServerSettingsLoader.smallBlind,
                clientServerSettingsLoader.bigBlind,
                server.getPlayersNames(),
                serverPokerTextOptions);

            serverPoker.startGame();
           
            ServerPokerWindow serverPokerWindow(windowPointer, sf::Color::Black, serverPoker);
            serverPokerWindow.startDrawing();

            serverPoker.terminateThread();
        }

        server.terminateThread();
        
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if (clientServerChooseMenu.getMarkedOption() == "Client")
    {
        TextOptions clientTextOptions(290.f, 300.f, font, 50, sf::Color::White);
        TextOptions lobbyTextOptions(40.f, 20.f, font, 30, sf::Color::White);
        Client client(clientServerSettingsLoader.ip,
            clientServerSettingsLoader.port,
            clientServerSettingsLoader.playerName,
            clientTextOptions,
            lobbyTextOptions);

        ClientWindow clientWindow(windowPointer, sf::Color::Black, client);

        client.run();
        clientWindow.startDrawing();

        if (client.shouldLobbyStart())
        {
            ClientSnake snake(25, 15, 40, 100,
                sf::Color::Blue,
                sf::Color(0, 153, 204),
                sf::Color(238, 28, 18),
                sf::Color(134, 232, 30),
                sf::Color(113, 204, 16));

            LobbySnakeWindow lobbySnakeWindow(windowPointer, sf::Color(0, 128, 0), client, snake);

            lobbySnakeWindow.startDrawing();
        }

        if (client.shouldPokerStart())
        {
            std::vector<std::string> pokerPlaysStrings{ "Check", "Fold", "Bet", "Call", "Raise" };
            ClientPokerPlays clientPokerPlays(pokerPlaysStrings,
                font,
                70.f, 650.f, 160.f, 0.f,
                35, sf::Color::Black, sf::Color(153, 255, 255));

            TextOptions gameInfoTextOptions(465, 310, font, 17, sf::Color::White, "", 200, 90);
            TextOptions roundEndInfoTextOptions(165, 100, font, 45, sf::Color::White, "");

            ClientPoker clientPoker(client,
                CardTexturesAndFontsContainer,
                clientPokerPlays,
                218.f, 0.f, 15.f, 15.f,
                15.f, 330.f, 218.f, 0.f,
                gameInfoTextOptions,
                roundEndInfoTextOptions);

            ClientPokerWindow clientPokerWindow(windowPointer,
                sf::Color(0, 128, 0),
                &client,
                &clientPoker,
                &clientPokerPlays);

            clientPokerWindow.startDrawing();   

            std::string winner = clientPoker.getWinner();
            std::string endMessage = "Game Finished!\nThe Winner is - ";
            endMessage += winner;
            endMessage += "\nPress enter to exit.";

            TextOptions gameEndTextOptions(290, 250, font, 55, sf::Color::Black, endMessage);
            GameEndWindow gameEndWindow(windowPointer,
                sf::Color::Black,
                gameEndTextOptions);

            gameEndWindow.startDrawing();

           
        }
        client.terminateThread();
    }

    windowPointer->close();
    windowPointer = nullptr;

    

    return 0;

}



