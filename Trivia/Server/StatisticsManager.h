#pragma once
#include "Helper.h"
#include "SqliteDatabase.h"
#include <vector>
#include <string>

class StatisticsManager
{
private:
	IDatabase* m_datatbase;
	float calculateScore(std::string userName);
public:
	StatisticsManager(IDatabase* db);
	~StatisticsManager();
	std::vector<std::string> getHighScore();
	std::vector<std::string> getUserStatistics(std::string userName);


};