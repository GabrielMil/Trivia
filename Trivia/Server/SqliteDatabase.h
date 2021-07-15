#pragma once
#include "sqlite3.h"
#include "IDatabase.h"
#include "CallBackFunctions.h"

class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase();
	virtual ~SqliteDatabase();

	//User related
	 bool doesUserExist(std::string& user);
	 bool doesPasswordMatch(std::string& user, std::string& password);
	 void addNewUser(std::string& user, std::string& password, std::string& email);

	 std::vector<QuestionS> getQuestions(int numOfQuestion);
	 float getPlayerAverageAnswerTime(std::string& userName);
	 int getNumOfCorrectAnswers(std::string& userName);
	 int getNumOfTotalAnswers(std::string& userName);
	 int getNumOfPlayerGames(std::string& userName);
	 std::vector<int> getAllGames();
	 std::vector<std::string> getAllUsernames();

	 void addQuestion(QuestionS question);
	 std::vector<QuestionS> getAllQuestions();

	 void AddGame(int id);
	 void AddStat(PlayerResults& res, int id);

	 std::vector<PlayerResults> getGameResults(int gameId);

private:
	sqlite3* db;
	std::string dbFileName;

	//This function excutes the query with the given string, optional callback functions and variable
	void execQuery(std::string& sqlStatement, int(*callBack)(void* givenParam, int argc, char** argv, char** azColName) = nullptr, void* param = nullptr);
	//This function opens all the tables(if they doesn't exist)
	void open();

};