#pragma once
#include <vector>
#include <random>
#include <ranges>
#include <semaphore>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "../Games/ServerPokerPlayer.h"
#include "../Cards/Card.h"
#include "../Network/PacketCodes.h"
#include "../Network/Server.h"
#include "../Network/PacketCreator.h"
#include "../Games/GameInfo.h"
#include "../Games/ScoreCalculator.h"

class ServerPoker : public sf::Drawable
{
private:
	friend class GameInfo;
	Server* server;
	PacketCreator packetCreator;
	ScoreCalculator scoreCalculator;
	std::atomic<bool> gameOn = false;
	sf::Thread startGameThread;
	int startingTokens;
	int smallBlindValue;
	int bigBlindValue;
	int biggestPile = 0;
	int tokensOnPile = 0;
	int playersNumber = 0;
	mutable GameInfo gameInfo;
	mutable std::binary_semaphore binarySemaphore;
	std::vector<ServerPokerPlayer> players;
	std::vector<Card> cardsPile;
	std::vector<Card> cardsOnBoard;
	std::vector<ServerPokerPlayer>::iterator smallBlindPlayer;
	std::vector<ServerPokerPlayer>::iterator bigBlindPlayer;
	std::vector<ServerPokerPlayer>::iterator playingPlayer;
	std::vector<ServerPokerPlayer>::reverse_iterator endOfQueuePlayer;

public:
	ServerPoker(Server& server, int startingTokens, int smallBlindValue, int bigBlindValue, std::vector<std::string> playersNamesVec, TextOptions textOptions);
	void startGame(void);
	bool getGameOn(void) const { return gameOn.load(); }
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
private:
	void start(void);
	void prepareCards(void);
	//------------------------------------------------------------------------------------------------------------------------------------
	void sendCardsToHands(void);
	//------------------------------------------------------------------------------------------------------------------------------------
	void sendCardsToBoard(int numberOfCards);
	void putBlindTokensOnPile(void);
	void tryToEliminatePlayers();
	void resetForNextRound(void);
	void resetForNextPhase(void);
	void tryToEliminatePlayingPlayer();
	bool checkIfPhaseEnd();
	bool checkIfRoundEnd();
	int countPassedPlayers(void) const;
	int countEliminatedPlayers(void) const;
	void updateGameInfo();
	void sendGameInfo(void);
	bool checkIfGameEnd() const;
	void initializeIterators(void);
	void sendEndOfRoundInfo(const ServerPokerPlayer* winner);
	void sendEndOfGameInfo(const ServerPokerPlayer* winner);
	void rotatePlayerIterator(std::vector<ServerPokerPlayer>::iterator& it);
	void rotateEndOfQueueIterator();
	void makeEndOfQueuePlayerPlayingPlayer();
	//---------------------------------------------------------------------------------Phases----------------------------------------------------------------------------------
	void phase(bool allowBet = true);
	//---------------------------------------------------------------------------------Phases----------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------Find Functions--------------------------------------------------------------------------
	const ServerPokerPlayer* findRoundWinner(void);
	const ServerPokerPlayer* findGameWinner(void);
	void findNewEndOfQueuePlayer(void);
	//---------------------------------------------------------------------------------Find Functions--------------------------------------------------------------------------
	//---------------------------------------------------------------------------------Plays-----------------------------------------------------------------------------------
	void fold(void);
	void raise(int tokensConnectedWithPlay);
	void call(void);
	void check(void);
	void bet(int tokensConnectedWithPlay);
	//---------------------------------------------------------------------------------Plays-----------------------------------------------------------------------------------


};

