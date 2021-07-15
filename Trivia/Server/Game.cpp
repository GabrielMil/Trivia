#include "Game.h"
#define ANS_NUM 4
Game::Game(std::vector<QuestionS>& allQuestions, std::vector<LoggedUser>& usrs)
{
	
	std::vector<std::string> answers;
	// Converting all the questions from struct to class
	for (auto& q : allQuestions)
	{
		answers.push_back(q.correctAns);
		answers.push_back(q.ans2);
		answers.push_back(q.ans3);
		answers.push_back(q.ans4);
		this->m_questions.push_back(Question(q.question, answers));
		answers.clear();
	}

	for (auto& usr : usrs)
	{ // Intiliazing the players map with the started gameData

		this->m_players.insert({ usr, new GameData({0,0,0,&this->m_questions.front()})});
	}
}

Game::Game(Room& room, IDatabase* db)
{
	std::vector<LoggedUser> users;
	std::vector<std::string> answers;
	std::vector<QuestionS> all;
	for (auto& usr : room.getAllusers())
	{
		users.push_back(LoggedUser(usr));
	}
	// Converting all the questions from struct to class
	all = this->getRandomQuestions(room.getRoomData().numOfQuestionsInGame, db);
	for (auto& q : all)
	{
		// Inserting the questions to the questions vector while transfaring them to Question form.
		answers.push_back(q.correctAns);
		answers.push_back(q.ans2);
		answers.push_back(q.ans3);
		answers.push_back(q.ans4);
		this->m_questions.push_back(Question(q.question, answers));
		answers.clear();
	}

	for (auto& usr : users)
	{ // Intiliazing the players map with the started gameData
		this->m_players.insert({ usr, new GameData({0,0,0,&this->m_questions.front()}) });
	}
	this->id = room.getRoomData().id;
	
}

std::map<LoggedUser, GameData*, LoggedUserCompare>& Game::getPlayers()
{
	return this->m_players;
}

std::vector<QuestionS> Game::getRandomQuestions(int n, IDatabase* db)
{
	std::vector<QuestionS> allQuestions;
	allQuestions = db->getAllQuestions();
	std::vector<QuestionS> choices;
	std::set<int> indexes;
	int random_index = 0;
	int max_index = allQuestions.size();
	// Choosing numOfQuestionsInGame ransom question from the database.
	while (indexes.size() < max_index && indexes.size() < n)
	{
		random_index = rand() % max_index;
		if (indexes.find(random_index) == indexes.end())
		{
			choices.push_back(allQuestions[random_index]);
			indexes.insert(random_index);
		}
	}
	return choices;
}

int getIndex(std::vector<Question> v, std::string K)
{
	std::vector<std::string> questions;
	for (auto& q : v)
	{
		questions.push_back(q.getQuestion());
	}
	auto it = find(questions.begin(), questions.end(), K);

	// If element was found
	if (it != questions.end())
	{
		// calculating the index
		// of K
		int index = it - questions.begin();
		return index;
	}
	else {
		// If the element is not
		// present in the vector
		return -1;
	}
}

Game::~Game()
{
}

void Game::getGameRes(std::vector<PlayerResults>& gameRes)
{
	int total = 0;
	PlayerResults currPR;
	for (auto& player : this->m_players)
	{
		// For each player -> assembelying a PlayerResult struct.
		total = (player.second->correctAnswerCount + player.second->wrongAnswerCount) == 0 ? 1 : (player.second->correctAnswerCount + player.second->wrongAnswerCount);
		currPR = PlayerResults({ player.first.getUsername(), player.second->correctAnswerCount, player.second->wrongAnswerCount, player.second->averageAnswerTime/total });
		gameRes.push_back(currPR);
	}
}

void Game::getGameRes(std::vector<PlayerResults>& gameRes, IDatabase* db)
{
	gameRes = db->getGameResults(this->id);
}



Question& Game::getQuestionForUser(LoggedUser& usr)
{
	return *(this->m_players[usr]->currentQuestion);
}

bool Game::isEmpty()
{
	return this->m_players.size() == 0;
}


bool Game::submitAnswer(LoggedUser& usr, std::string answer)
{
	if (answer == (this->m_players[usr]->currentQuestion->getCorrectAnswer()))
	{// If the user submitted the correct answer
		this->m_players[usr]->correctAnswerCount++;
	}
	else
	{
		this->m_players[usr]->wrongAnswerCount++;
	}
	// If the player submmited the last answer to the question
	if (getIndex(this->m_questions, this->m_players[usr]->currentQuestion->getQuestion()) >= (this->m_questions.size() - 1))
	{
		std::vector<std::string> nullAns;
		for (int i = 0; i < ANS_NUM; i++)
		{
			nullAns.push_back("null");
		}
		this->m_players[usr]->currentQuestion = new Question("null", nullAns);
		return true;
	}
	// Adding the question's time to all avg time in order to calculate in the future.
	this->m_players[usr]->averageAnswerTime += this->m_players[usr]->currentQuestion->getTime();
	int index = getIndex(this->m_questions, this->m_players[usr]->currentQuestion->getQuestion()); // Getting thequestion's index in order to view the motion of the game
	this->m_players[usr]->currentQuestion = &this->m_questions[index + 1]; // Getting the net question.

	return false;
}

void Game::removePlayer(LoggedUser& usr)
{
	this->m_players.erase(usr);
	// Don't forget to send game data.
}

int Game::getId()
{
	return this->id;
}

void Game::setId(int id)
{
	this->id = id;
}


