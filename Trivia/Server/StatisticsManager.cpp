#include "StatisticsManager.h"
#include <iostream>
#include <map>
#include <iterator>
#define MAX_IN_SCORE 5
StatisticsManager::StatisticsManager(IDatabase* db)
    : m_datatbase(db)
{
}
StatisticsManager::~StatisticsManager()
{
}

std::vector<std::string> StatisticsManager::getHighScore()
{
    std::vector<std::string> usernames = this->m_datatbase->getAllUsernames();
    std::multimap<int, std::string> usersAndScore;
    std::vector<std::string> toRet;
    int amountOfPlayers = MAX_IN_SCORE, i = 0;
    if(usernames.size() < MAX_IN_SCORE)
    {
        amountOfPlayers = usernames.size();
    }
    // Inserting to multimap in order to get in sorted by the key (score)
    for (i = 0; i < usernames.size(); i++)
    {
        usersAndScore.insert(std::pair<int, std::string>(calculateScore(usernames[i]),usernames[i]));
    }
    for (auto& it : usersAndScore) {
        if (0 == amountOfPlayers)
        {
            break;
        }
        // Inserting the usernames next to their score.
        toRet.push_back(it.second + " - " + std::to_string(it.first));
    }
    std::reverse(toRet.begin(), toRet.end()); // Reversing the map in order to help the gui.
    return toRet;

}

std::vector<std::string> StatisticsManager::getUserStatistics(std::string userName)
{
    //Getting all the user's statistics located in vector by chosen order.
    std::vector<std::string> arrOfStats;
    arrOfStats.push_back(std::to_string(m_datatbase->getNumOfPlayerGames(userName)));
    arrOfStats.push_back(std::to_string(m_datatbase->getNumOfCorrectAnswers(userName)));
    arrOfStats.push_back(std::to_string(m_datatbase->getNumOfTotalAnswers(userName) - m_datatbase->getNumOfCorrectAnswers(userName)));
    arrOfStats.push_back(std::to_string(m_datatbase->getPlayerAverageAnswerTime(userName)));
    return arrOfStats;
}

// Formula - (CorrectAnswers/TotalAnswers)*(1/AverageAnswerTime)*NumOfPlayerGames
float StatisticsManager::calculateScore(std::string userName)
{
    if (m_datatbase->getNumOfTotalAnswers(userName) == 0 || m_datatbase->getPlayerAverageAnswerTime(userName) == 0)
    {
        return 0.0; 
        // Incase The user didn't answered any questions.
    }
    return ((float)(m_datatbase->getNumOfCorrectAnswers(userName))/m_datatbase->getNumOfTotalAnswers(userName))*(1.0/ m_datatbase->getPlayerAverageAnswerTime(userName))* m_datatbase->getNumOfPlayerGames(userName)*10;
}

