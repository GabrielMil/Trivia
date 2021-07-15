#pragma once
#include "Question.h"
#include "LoggedUser.h"
#include "SqliteDatabase.h"
#include <map>
#include <set>
#include "Room.h"

typedef struct GameData
{
	unsigned int correctAnswerCount=0;
	unsigned int wrongAnswerCount=0;
	unsigned int averageAnswerTime = 0;
	Question* currentQuestion;
}GameData;

struct LoggedUserCompare
{
	bool operator() (const LoggedUser& lhs, const LoggedUser& rhs) const
	{
		return lhs.getUsername() < rhs.getUsername();
	}
};

class Game
{
public:

	Game(std::vector<QuestionS>& allQuestions, std::vector<LoggedUser>& usrs);
	Game(Room& room, IDatabase* db);
	~Game();
	void getGameRes(std::vector<PlayerResults>& gameRes, IDatabase* db);
	void getGameRes(std::vector<PlayerResults>& gameRes);
	Question& getQuestionForUser(LoggedUser& usr);
	bool isEmpty();
	bool submitAnswer(LoggedUser& usr, std::string answer);
	void removePlayer(LoggedUser& usr);
	int getId();
	void setId(int id);
	std::map<LoggedUser, GameData*, LoggedUserCompare>& getPlayers();
private:
	int id;
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData*, LoggedUserCompare> m_players;
	std::vector<QuestionS> getRandomQuestions(int n, IDatabase* db);

};