#pragma once

enum packetCode
{
	empty = 0,
	playerName = 1,
	lobbyInfo = 2,
	startPoker = 3,
	playRequired = 4,
	clientPlay = 5,
	cardToBoard = 6,
	cardToHand = 7,
	resetForNextRound = 8,
	eliminatePlayer = 9,
	gameInfo = 10,
	gameEnd = 11,
	roundEndInfo = 12,
};