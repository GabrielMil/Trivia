#include "SqliteDatabase.h"

SqliteDatabase::SqliteDatabase()
{
	this->dbFileName = "Trivia.db";
	int res = sqlite3_open(dbFileName.c_str(), &(this->db));
	if (res != SQLITE_OK)
	{
		this->db = nullptr;
		throw std::exception("Failed to open DB!");
	}
	open();  // After creating the DB open it(Create tables if needed);
}

SqliteDatabase::~SqliteDatabase()
{
	sqlite3_close(this->db);
	this->db = nullptr;
}

bool SqliteDatabase::doesUserExist(std::string& username)
{
	int count = 0;
	std::string sql = "SELECT * FROM users WHERE username='" + username + "';";
	execQuery(sql, CallBackFunctions::countRows, &count);
	return count != 0;  // If count 0 user not exist(Not fount)
}

bool SqliteDatabase::doesPasswordMatch(std::string& username, std::string& password)
{
	StrField sqlPassword;
	std::string sql = "SELECT password FROM users WHERE username='" + username + "';";
	sqlPassword.field = "password";
	execQuery(sql, CallBackFunctions::getStrField, &sqlPassword);
	return password == sqlPassword.value;
}

void SqliteDatabase::addNewUser(std::string& username, std::string& password, std::string& email)
{
	std::string sql = "INSERT INTO users(username, password, email) VALUES(";
	sql += "'" + username + "', ";
	sql += "'" + password + "', ";
	sql += "'" + email + "'); ";
	execQuery(sql);
}

std::vector<QuestionS> SqliteDatabase::getQuestions(int numOfQuestion)
{
	std::string sql = "SELECT * FROM questions LIMIT " + std::to_string(numOfQuestion) + ";";
	std::vector<QuestionS> allQuestions;
	execQuery(sql, CallBackFunctions::getQuestion, &allQuestions);
	return allQuestions;
}

float SqliteDatabase::getPlayerAverageAnswerTime(std::string& userName)
{
	ForAvg toCalc{ 0,0 };
	std::string sql = "SELECT avg_ans_time FROM statistics WHERE username='" + userName + "';";
	execQuery(sql, CallBackFunctions::getDataForAvg, &toCalc);
	if (toCalc.count == 0)  // Check if there are any answers to calc the average
	{
		return 0.0f;
	}
	return (toCalc.total / toCalc.count);
}

int SqliteDatabase::getNumOfCorrectAnswers(std::string& userName)
{
	ForSum counter{ 0,0,"correct_answers" };
	std::string sql = "SELECT correct_answers FROM statistics WHERE username='" + userName + "';";
	execQuery(sql, CallBackFunctions::getDataForSum, &counter);
	return counter.total;
}

int SqliteDatabase::getNumOfTotalAnswers(std::string& userName)
{
	ForSum counter{ 0,0,"num_of_questions" };
	std::string sql = "SELECT num_of_questions FROM statistics WHERE username='" + userName + "';";
	execQuery(sql, CallBackFunctions::getDataForSum, &counter);
	return counter.total;
}

int SqliteDatabase::getNumOfPlayerGames(std::string& userName)
{
	int count = 0;;
	std::string sql = "SELECT COUNT(DISTINCT game_id) FROM statistics WHERE username='" + userName + "';";
	execQuery(sql, CallBackFunctions::getIntValue, &count);
	return count;
}

std::vector<std::string> SqliteDatabase::getAllUsernames()
{
	std::vector<std::string> allUsers;
	std::string sql = "SELECT username FROM users;";
	execQuery(sql, CallBackFunctions::getStrVector, &allUsers);
	return allUsers;
}

/*
Function : the function will return all the played games id's as a vector
Input : None
Output : Id's vector
*/
std::vector<int> SqliteDatabase::getAllGames()
{
	std::vector<int> allGamesId;
	std::string sql = "SELECT game_id FROM games;";
	execQuery(sql, CallBackFunctions::getIntVector, &allGamesId);
	return allGamesId;
}

void SqliteDatabase::addQuestion(QuestionS question)
{
	//using \" and not ' because the question can have '(In which year did Israel announced it's independence?)
	std::string sql = "INSERT INTO questions(question, correct_ans, ans2, ans3, ans4) VALUES(\"" +
		question.question + "\",\"" + question.correctAns + "\",\"" + question.ans2 + "\",\"" + question.ans3 + "\",\"" + question.ans4 + "\");";
	execQuery(sql);
}

std::vector<QuestionS> SqliteDatabase::getAllQuestions()
{
	std::vector<QuestionS> allQuestions;
	std::string sql = "SELECT * FROM questions;";
	execQuery(sql, CallBackFunctions::getQuestion, &allQuestions);
	return allQuestions;
}

void SqliteDatabase::AddGame(int id)
{
	//Insert game id
	std::string sql = "INSERT INTO games(game_id) VALUES(" + std::to_string(id) + ");";
	execQuery(sql);
}

void SqliteDatabase::AddStat(PlayerResults& res, int id)
{
	std::string sql = "INSERT INTO statistics(game_id, username, num_of_questions,correct_answers,avg_ans_time) VALUES(";
	sql += std::to_string(id) + ", '" + res.username + "'," + std::to_string(res.correctAnswerCount + res.wrongAnswerCount);
	sql += ", " + std::to_string(res.correctAnswerCount) + ", " + std::to_string(res.averageAnswerTime) + ");";
	execQuery(sql);
}

std::vector<PlayerResults> SqliteDatabase::getGameResults(int gameId)
{
	std::vector<StatisticData> allStats;
	std::vector<PlayerResults> allResults;
	//Get all the stats
	std::string sql = "SELECT * FROM statistics WHERE game_id=" + std::to_string(gameId) + ";";
	execQuery(sql, CallBackFunctions::getStatsVector, &allStats);
	//Fill the results
	for (int i = 0; i < allStats.size(); i++)
	{
		PlayerResults curr;
		curr.username = allStats[i].username;
		curr.correctAnswerCount = allStats[i].numOfCorrectAns;
		curr.wrongAnswerCount = allStats[i].numOfQuestions - allStats[i].numOfCorrectAns;
		curr.averageAnswerTime = allStats[i].avgAnsTime;
		allResults.push_back(curr);
	}
	return allResults;
}

void SqliteDatabase::execQuery(std::string& sqlStatement, int(*callBack)(void* givenParam, int argc, char** argv, char** azColName), void* param)
{
	//Helping Function to execute sql query
	char* errMessage = nullptr;
	int res = 0;
	try
	{
		res = sqlite3_exec(this->db, sqlStatement.c_str(), callBack, param, &errMessage);
		if (res != SQLITE_OK)
		{
			throw std::exception(errMessage);
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void SqliteDatabase::open()
{
	std::string sql = "";
	char* errMessage;
	int res = 0;

	//Make sure foreign-keys enabled
	sql = "PRAGMA FOREIGN_KEYS=true;";
	errMessage = nullptr;
	execQuery(sql);

	//Create the users
	sql = "CREATE TABLE IF NOT EXISTS users(username TEXT PRIMARY KEY NOT NULL, password TEXT NOT NULL, email TEXT NOT NULL);";
	execQuery(sql);
	//Create the questions table
	sql = "CREATE TABLE IF NOT EXISTS questions(question_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, question TEXT NOT NULL, correct_ans TEXT NOT NULL, ans2 TEXT NOT NULL, ans3 TEXT NOT NULL, ans4 TEXT NOT NULL);";
	execQuery(sql);
	//Create the games table
	sql = "CREATE TABLE IF NOT EXISTS games(game_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL);";
	execQuery(sql);
	//Create the statistics table
	sql = "CREATE TABLE IF NOT EXISTS statistics(game_id INTEGER NOT NULL, username TEXT NOT NULL, num_of_questions INTEGER NOT NULL, correct_answers INTEGER NOT NULL, avg_ans_time REAL NOT NULL, primary key(game_id, username), FOREIGN KEY(game_id) REFERENCES games(game_id), FOREIGN KEY(username) REFERENCES users(username));";
	execQuery(sql);
}
