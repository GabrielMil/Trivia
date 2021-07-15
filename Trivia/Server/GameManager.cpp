#include "GameManager.h"
GameManager::GameManager(IDatabase* db)
{
	this->m_database = db;
}

GameManager::~GameManager()
{
}

Game& GameManager::createGame(Game& g)
{
    this->m_games.push_back(&g);
    return g;
}

void GameManager::deleteGame(Game& g)
{
    int i = 0;

    for (i = 0 ; i < this->m_games.size();i++)
    {
        if (g.getId() == (*(this->m_games.begin() + i))->getId())
        {

            this->m_games.erase(this->m_games.begin() + i);
        }
    }
}

void GameManager::sortResults(std::vector<PlayerResults>& original)
{
    std::sort(original.begin(), original.end(), [this](const PlayerResults& l, const PlayerResults& r) {
        return this->calculateScore(l) < this->calculateScore(r);
        });
    //Reverse so the first with the best score
    std::reverse(original.begin(), original.end());
}

float GameManager::calculateScore(const PlayerResults& result)
{
    if (result.correctAnswerCount + result.wrongAnswerCount == 0 || result.averageAnswerTime == 0)
    {
        return 0;
    }
    return ((float)(result.correctAnswerCount) / result.correctAnswerCount+result.wrongAnswerCount) * (1.0 / result.averageAnswerTime) * 10;
}
