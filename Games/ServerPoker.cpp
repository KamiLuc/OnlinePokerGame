#include "ServerPoker.h"

void GameInfo::updateGameInfo(ServerPoker& poker)
{
	passedPlayersIndexes.clear();
	eliminatedPlayersIndexes.clear();
	int iteration = 0;
	std::string temp;
	for (auto player : poker.players)
	{
		playersInfoTexts[iteration].setFillColor(sf::Color::White);
		playersInfoTexts[iteration].setOutlineColor(sf::Color::White);
		playersInfoTexts[iteration].setStyle(sf::Text::Regular);

		temp = "";
		temp += player.getPlayerName();
		if (poker.smallBlindPlayer != poker.players.end())
		{
			if (poker.smallBlindPlayer->getPlayerName() == player.getPlayerName())
			{
				temp += " [sB]";
			}
		}

		if (poker.bigBlindPlayer->getPlayerName() == player.getPlayerName())
		{
			temp += " [bB]";
		}

		temp += "\n";
		if (!player.getIsEliminated())
		{
			temp += player.getLastPlay();
			if (!player.getPassedThisRound())
			{
				temp += " [";
				temp += std::to_string(player.getLastPlayTokens());
				temp += "]\n";
				temp += "Tokens = ";
				temp += std::to_string(player.getTokens());
				temp += "\n";
				temp += "Tokens on Pile = ";
				temp += std::to_string(player.getTokensOnPile());
			}
		}

		if (player.getPlayerName() == poker.endOfQueuePlayer->getPlayerName())
		{
			playersInfoTexts[iteration].setStyle(sf::Text::Italic);
			endOfQueuePlayerIndex = iteration;
		}

		if (player.getIsEliminated())
		{
			playersInfoTexts[iteration].setFillColor(sf::Color::Red);
			playersInfoTexts[iteration].setOutlineColor(sf::Color::Red);
			eliminatedPlayersIndexes.push_back(iteration);
		}

		else if (player.getPassedThisRound())
		{
			passedPlayersIndexes.push_back(iteration);
			playersInfoTexts[iteration].setFillColor(sf::Color::Green);
			playersInfoTexts[iteration].setOutlineColor(sf::Color::Green);
		}

		else if (poker.playingPlayer->getPlayerName() == player.getPlayerName())
		{
			playersInfoTexts[iteration].setFillColor(sf::Color::Blue);
			playersInfoTexts[iteration].setOutlineColor(sf::Color::Blue);
			playingPlayerIndex = iteration;
		}

		playersInfoTexts[iteration].setString(temp);
		playersInfoStrings[iteration] = temp;

		GameInfo::playersInfoTexts[iteration];
		iteration++;
	}
}

ServerPoker::ServerPoker(Server& server, int startingTokens, int smallBlindValue, int bigBlindValue, std::vector<std::string> playersNamesVec, TextOptions textOptions)
	:
	startingTokens(startingTokens), binarySemaphore{ 1 },
	smallBlindValue(smallBlindValue), bigBlindValue(bigBlindValue), players(0), server(&server), startGameThread(&ServerPoker::start, &*this),
	gameInfo(textOptions, playersNamesVec.size())
{
	for (auto& name : playersNamesVec)
	{
		ServerPokerPlayer player(name, startingTokens);
		players.push_back(std::move(player));
	}
	playersNumber = players.size();
}

void ServerPoker::startGame(void)
{
	if (gameOn.load() == false)
	{
		gameOn.store(true);
		auto p = players;
		players = p;
		startGameThread.launch();
	}
}

void ServerPoker::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	binarySemaphore.acquire();
	target.draw(gameInfo);
	binarySemaphore.release();
}

void ServerPoker::start(void)
{
	int roundCounter = 0;
	initializeIterators();
	while (!checkIfGameEnd())
	{
		prepareCards();
		sendCardsToHands();
		putBlindTokensOnPile();
		updateGameInfo();

		bool allowBet = false;

		if (roundCounter != 0)
		{
			allowBet = true;
		}

		//Phase 1 - Preflop
		while (true)
		{
			phase(allowBet);
			allowBet = false;
			if (checkIfPhaseEnd())
			{
				rotatePlayerIterator(playingPlayer);
				updateGameInfo();
				break;
			}
			else
			{
				rotatePlayerIterator(playingPlayer);
				updateGameInfo();
			}
		}

		if (!checkIfRoundEnd())
		{
			sendCardsToBoard(3);
			updateGameInfo();

			//Phase 2 and 3
			for (int i = 0; i < 2; i++)
			{
				allowBet = true;
				do
				{
					phase(allowBet);
					allowBet = false;
					if (checkIfPhaseEnd())
					{
						rotatePlayerIterator(playingPlayer);
						updateGameInfo();
						break;
					}
					else
					{
						rotatePlayerIterator(playingPlayer);
						updateGameInfo();
					}
				} while (true);

				if (!checkIfGameEnd())
				{
					sendCardsToBoard(1);
					updateGameInfo();
				}
				else
				{
					break;
				}
			}

			if (!checkIfRoundEnd())
			{
				allowBet = true;
				do
				{
					phase(allowBet);
					allowBet = false;
					if (checkIfPhaseEnd())
					{
						updateGameInfo();
						break;
					}
					else
					{
						rotatePlayerIterator(playingPlayer);
						updateGameInfo();
					}
				} while (true);
			}

		}

		auto winner = findRoundWinner();
		winner->giveTokens(tokensOnPile);
		tryToEliminatePlayers();
		sendEndOfRoundInfo(winner);
		resetForNextRound();
		sf::sleep(sf::Time(sf::seconds(5.0f)));
		roundCounter++;
	}

	auto gameWinner = findGameWinner();
	sendEndOfGameInfo(gameWinner);
	gameOn.store(false);
}

void ServerPoker::prepareCards(void)
{
	cardsPile.clear();
	char colors[] = { 's', 'h', 'd', 'c' };
	for (int i = 2; i < 15; i++)
	{
		for (auto el : colors)
		{
			cardsPile.push_back(Card(i, el));
		}
	}
	auto rng = std::default_random_engine{};
	rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
	std::shuffle(std::begin(cardsPile), std::end(cardsPile), rng);
}

void ServerPoker::sendCardsToHands(void)
{
	auto view = players | std::views::filter([](ServerPokerPlayer& player) {return !player.getIsEliminated(); });
	for (int i = 0; i < 2; i++)
	{
		sf::sleep(sf::Time(sf::seconds(0.5f)));
		for (auto& player : view)
		{
			Card card = cardsPile.back();
			sf::Packet packet(packetCreator.createPacketWithCardToHand(card.getNumber(), card.getColor()));
			server->sendPacket(player.getPlayerName(), packet);
			player.placeCardInHand(card);
			cardsPile.pop_back();
		}
	}
}

void ServerPoker::sendCardsToBoard(int numberOfCards)
{
	for (int i = 0; i < numberOfCards; i++)
	{
		sf::sleep(sf::Time(sf::seconds(0.5f)));
		Card card = cardsPile.back();
		sf::Packet packet(packetCreator.createPacketWithCardToBoard(card.getNumber(), card.getColor()));
		for (auto player : players)
		{

			server->sendPacket(player.getPlayerName(), packet);
		}
		cardsOnBoard.push_back(card);
		cardsPile.pop_back();
	}
}

void ServerPoker::putBlindTokensOnPile(void)
{
	if (smallBlindPlayer != players.end())
	{
		smallBlindPlayer->putTokensOnPile(smallBlindValue);
		tokensOnPile += smallBlindValue;
	}

	if (bigBlindPlayer != players.end())
	{
		bigBlindPlayer->putTokensOnPile(bigBlindValue);
		tokensOnPile += bigBlindValue;
	}
}

void ServerPoker::tryToEliminatePlayers()
{
	for (auto& player : players)
	{
		if (player.getTokens() < bigBlindValue)
		{
			player.eliminatePlayer();
		}
	}
}

void ServerPoker::resetForNextRound(void)
{
	for (auto& player : players)
	{
		player.resetForNextRound();
	}
	tokensOnPile = 0;
	cardsOnBoard.clear();

	rotatePlayerIterator(bigBlindPlayer);
	while (&*playingPlayer != &*bigBlindPlayer)
	{
		rotatePlayerIterator(playingPlayer);
	}
	rotatePlayerIterator(playingPlayer);

	makeEndOfQueuePlayerPlayingPlayer();

	if (players.size() - countEliminatedPlayers() <= 2)
	{
		smallBlindPlayer = players.end();
	}
	else
	{
		rotatePlayerIterator(smallBlindPlayer);
	}
}

void ServerPoker::resetForNextPhase(void)
{
	for (auto& player : players)
	{
		player.resetForNextPhase();
	}
}

void ServerPoker::tryToEliminatePlayingPlayer()
{
	if (playingPlayer->getTokens() < bigBlindValue)
	{
		playingPlayer->eliminatePlayer();
	}
}

bool ServerPoker::checkIfPhaseEnd()
{
	if (players.size() - countPassedPlayers() == 1)
	{
		return true;
	}

	int biggestPile = 0;
	for (auto& const player : players)
	{
		if (!player.getIsEliminated() and !player.getPassedThisRound())
		{
			if (player.getLastPlay() == "no play" and players.size() - countPassedPlayers() > 1)
			{
				return false;
			}
			else if (player.getTokensOnPile() > biggestPile)
			{
				biggestPile = player.getTokensOnPile();
			}
		}
	}

	auto view = players | std::views::filter([](ServerPokerPlayer& player) {return (!player.getIsEliminated() and !player.getPassedThisRound()); });
	for (auto& player : view)
	{
		if (player.getTokensOnPile() != biggestPile)
		{
			return false;
		}
	}

	if (&*playingPlayer != &*endOfQueuePlayer)
	{
		return false;
	}

	return true;
}

bool ServerPoker::checkIfRoundEnd()
{
	if (players.size() - countPassedPlayers() <= 1)
	{
		return true;
	}
	else
		return false;
}

int ServerPoker::countPassedPlayers(void) const
{
	int counter = 0;
	for (auto& const player : players)
	{
		if (player.getPassedThisRound())
		{
			counter++;
		}
	}
	return counter;
}

int ServerPoker::countEliminatedPlayers(void) const
{
	int counter = 0;
	for (auto& const player : players)
	{
		if (player.getIsEliminated())
		{
			counter++;
		}
	}
	return counter;
}

void ServerPoker::updateGameInfo()
{
	binarySemaphore.acquire();
	gameInfo.updateGameInfo(*this);
	binarySemaphore.release();
	sendGameInfo();
}

void ServerPoker::sendGameInfo(void)
{
	sf::Packet packet = packetCreator.createPacketWithGameInfo(gameInfo);
	server->sendPacketToAll(packet);
}

bool ServerPoker::checkIfGameEnd() const
{
	if (players.size() - countEliminatedPlayers() <= 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ServerPoker::initializeIterators(void)
{
	std::vector<ServerPokerPlayer>::iterator tempIt = players.begin();
	smallBlindPlayer = tempIt;
	tempIt++;
	bigBlindPlayer = tempIt;
	tempIt++;
	playingPlayer = tempIt;
	endOfQueuePlayer = players.rbegin();

	//end of queue player is big blind for now
	while (&*endOfQueuePlayer != &*bigBlindPlayer)
	{
		endOfQueuePlayer++;
	}
}

void ServerPoker::sendEndOfRoundInfo(const ServerPokerPlayer* winner)
{
	sf::Packet packet = packetCreator.createPacketWithRoundEndInfo(winner->getPlayerName(), winner->getBestCards());
	server->sendPacketToAll(packet);
}

void ServerPoker::sendEndOfGameInfo(const ServerPokerPlayer* winner)
{
	sf::Packet packet = packetCreator.createPacketWithGameEndInfo(winner->getPlayerName());
	server->sendPacketToAll(packet);
}

void ServerPoker::rotatePlayerIterator(std::vector<ServerPokerPlayer>::iterator& it)
{
	it++;
	if (it == players.end())
	{
		it = players.begin();
	}
	while (it->getIsEliminated() == true or it->getPassedThisRound() == true)
	{
		it++;
		if (it == players.end())
		{
			it = players.begin();
		}
	}
}

void ServerPoker::rotateEndOfQueueIterator()
{
	endOfQueuePlayer++;
	if (endOfQueuePlayer == players.rend())
	{
		endOfQueuePlayer = players.rbegin();
	}
	while (endOfQueuePlayer->getIsEliminated() == true or endOfQueuePlayer->getPassedThisRound() == true)
	{
		endOfQueuePlayer++;
		if (endOfQueuePlayer == players.rend())
		{
			endOfQueuePlayer = players.rbegin();
		}
	}
}

void ServerPoker::makeEndOfQueuePlayerPlayingPlayer()
{
	endOfQueuePlayer = players.rbegin();
	while (&*endOfQueuePlayer != &*playingPlayer)
	{
		endOfQueuePlayer++;
	}
}

void ServerPoker::phase(bool allowBet)
{

	biggestPile = bigBlindValue;

	for (auto& player : players)
	{
		if (player.getTokensOnPile() > biggestPile)
		{
			biggestPile = player.getTokensOnPile();
		}
	}

	bool serverRecivedAnswer = false;

	while (!serverRecivedAnswer)
	{
		sf::Packet packet = packetCreator.createPacketWithPlayRequest();
		server->sendPacket(playingPlayer->getPlayerName(), packet);
		packet.clear();
		packet = server->getAnswerFromClient(playingPlayer->getPlayerName());

		int code = 0;
		packet >> code;

		if (code == packetCode::clientPlay)
		{
			std::string play = "";
			int tokensConnectedWithPlay = 0;
			packet >> play;
			packet >> tokensConnectedWithPlay;

			if (play == "Fold")
			{
				fold();
				serverRecivedAnswer = true;
			}

			else if (play == "Raise" and
				!allowBet and
				tokensConnectedWithPlay > biggestPile - playingPlayer->getTokensOnPile() and
				playingPlayer->getTokens() >= tokensConnectedWithPlay)
			{
				raise(tokensConnectedWithPlay);
				serverRecivedAnswer = true;
			}

			else if (play == "Call" and
				playingPlayer->getTokens() >= biggestPile - playingPlayer->getTokensOnPile() and
				biggestPile != playingPlayer->getTokensOnPile())
			{
				call();
				serverRecivedAnswer = true;
			}

			else if (play == "Check" and
				playingPlayer->getTokensOnPile() == biggestPile or playingPlayer->getTokens() == 0)
			{
				check();
				serverRecivedAnswer = true;
			}

			else if (play == "Bet" and
				allowBet and
				tokensConnectedWithPlay > 0
				and tokensConnectedWithPlay > biggestPile - playingPlayer->getTokensOnPile())
			{
				bet(tokensConnectedWithPlay);
				serverRecivedAnswer = true;
			}

			else
			{
				serverRecivedAnswer = false;
			}
		}
		else
		{
			throw std::exception("Invalid answer");
		}
	}
}

const ServerPokerPlayer* ServerPoker::findRoundWinner(void)
{
	const ServerPokerPlayer* winner;
	if (countPassedPlayers() == players.size() - 1)
	{
		for (auto& player : players)
		{
			if (!player.getPassedThisRound() and !player.getIsEliminated())
			{
				player.setBestCards(player.getHand());
				winner = &player;
				return winner;
			}
		}
	}
	else
	{
		scoreCalculator.calculatePlayersPoints(players, cardsOnBoard);
	}
	return scoreCalculator.findPokerWinner(players);
}

const ServerPokerPlayer* ServerPoker::findGameWinner(void)
{
	ServerPokerPlayer* winner;

	for (auto& player : players)
	{
		if (!player.getIsEliminated())
		{
			winner = &player;
			return winner;
		}
	}
}

void ServerPoker::findNewEndOfQueuePlayer(void)
{
	rotateEndOfQueueIterator();
}

void ServerPoker::fold(void)
{
	tryToEliminatePlayingPlayer();
	playingPlayer->setPassedThisRound(true);
	playingPlayer->setLastPlayAndTokens("Fold", 0);
	if (&*playingPlayer == &*endOfQueuePlayer)
	{
		findNewEndOfQueuePlayer();
	}
}

void ServerPoker::raise(int tokensConnectedWithPlay)
{
	playingPlayer->setLastPlayAndTokens("Raise", tokensConnectedWithPlay);
	tokensOnPile += tokensConnectedWithPlay;
}

void ServerPoker::call(void)
{
	int tokensNeededToEqualBiggestPile = biggestPile - playingPlayer->getTokensOnPile();
	tokensOnPile += tokensNeededToEqualBiggestPile;
	playingPlayer->setLastPlayAndTokens("Call", tokensNeededToEqualBiggestPile);
}

void ServerPoker::check(void)
{
	playingPlayer->setLastPlayAndTokens("Check", 0);
}

void ServerPoker::bet(int tokensConnectedWithPlay)
{
	playingPlayer->setLastPlayAndTokens("Bet", tokensConnectedWithPlay);
	tokensOnPile += tokensConnectedWithPlay;
}
