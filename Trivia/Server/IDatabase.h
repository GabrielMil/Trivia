#pragma once
#include <string>
#include <list>
#include <vector>
#include "JsonResponsePacketSerializer.h"

typedef struct QuestionS
{
	size_t questionId;
	std::string question;
	std::string correctAns;
	std::string ans2;
	std::string ans3;
	std::string ans4;
}QuestionS;

typedef struct StatisticData
{
	size_t gameId;
	std::string username;
	int numOfQuestions;
	int numOfCorrectAns;
	int avgAnsTime;
}StatisticData;

class IDatabase
{
public:
	virtual ~IDatabase() = default;

	//User related
	virtual bool doesUserExist(std::string& user) = 0;
	virtual bool doesPasswordMatch(std::string& user, std::string& password) = 0;
	virtual void addNewUser(std::string& user, std::string& password, std::string& email) = 0;

	virtual std::vector<QuestionS> getQuestions(int numOfQuestion) = 0;
	virtual float getPlayerAverageAnswerTime(std::string& userName) = 0;
	virtual int getNumOfCorrectAnswers(std::string& userName) = 0;
	virtual int getNumOfTotalAnswers(std::string& userName) = 0;
	virtual int getNumOfPlayerGames(std::string& userName) = 0;
	virtual std::vector<int> getAllGames() = 0;
	virtual std::vector<std::string> getAllUsernames() = 0;

	virtual void addQuestion(QuestionS question) = 0;
	virtual std::vector<QuestionS> getAllQuestions() = 0;

	virtual void AddGame(int id) = 0;
	virtual void AddStat(PlayerResults& res, int id) = 0;

	virtual std::vector<PlayerResults> getGameResults(int gameId) = 0;

};