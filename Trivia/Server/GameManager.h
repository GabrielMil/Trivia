#pragma once
#include "Room.h"
#include "Game.h"
#include "SqliteDatabase.h"
#include <vector>

class GameManager
{
public:
	GameManager(IDatabase* db);
	~GameManager();

	Game& createGame(Game& g);
	void deleteGame(Game& g);

	void sortResults(std::vector<PlayerResults>& original);
	

private:
	float calculateScore(const PlayerResults& result);

	IDatabase* m_database;
	std::vector<Game*> m_games;
};