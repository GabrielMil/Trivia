#include "GameRequestHandler.h"

GameRequestHandler::GameRequestHandler(LoggedUser& usr, IDatabase* db, GameManager* gm, Game* game)
	: m_user(usr), m_dataBase(db), m_gameManager(gm), m_game(game)
{
	this->m_id = GAME_ID;
}

GameRequestHandler::~GameRequestHandler()
{
}

bool GameRequestHandler::isRequestRelevant(RequestInfo reqInfo)
{
	return reqInfo.id == PacketCodes::LeaveGame || reqInfo.id == PacketCodes::GetQuestion || reqInfo.id == PacketCodes::SubmitAnswer || reqInfo.id == PacketCodes::GetGameResults;
}

void GameRequestHandler::handleRequest(RequestResult& rr, RequestInfo reqInfo)
{
	switch (reqInfo.id)
	{
	case PacketCodes::Error:
		break;
	case PacketCodes::LeaveGame:
		this->leaveGame(rr, reqInfo);
		break;
	case PacketCodes::GetQuestion:
		this->getQuestion(rr, reqInfo);
		break;
	case PacketCodes::SubmitAnswer:
		this->sumbitAnswer(rr, reqInfo);
		break;
	case PacketCodes::GetGameResults:
		this->getGameResult(rr, reqInfo);
		break;
	default:
		break;
	}
}

int GameRequestHandler::getId()
{
	return this->m_id;
}



void GameRequestHandler::sumbitAnswer(RequestResult& rr, RequestInfo ri)
{
	std::map<unsigned int, std::string> answers;
	int i = 1;
	SubmitAnswerRequest lr;
	JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(lr, ri.buffer);
	try
	{
		for (auto& answer : this->m_game->getQuestionForUser(this->m_user).getPossibleAnswers())
		{ // Converting from std::vector<std::string> to map<int, std::string>
			answers.insert({ i, answer });
			i++;
		}
		answers.insert({ i, "null" }); // Inserting fifth answer in case the user didn't answer any of the answers.
		// Fifth answer will always be wrong.
		this->m_game->getQuestionForUser(this->m_user).setTime(lr.timeToAnswer);
		SubmitAnswerResponse submitRes;
		submitRes.status = 1;
		this->m_game->submitAnswer(this->m_user, answers[lr.asnwerId]);
		submitRes.correctAnswerId = 1;
		rr.response = JsonResponsePacketSerializer::serializeResponse(submitRes);
		rr.newHandler = this;
	}
	catch (std::exception& err)
	{
		ErrorResponse er;

		er.message = "Error, canno't get submit answer.";
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}

void GameRequestHandler::getQuestion(RequestResult& rr, RequestInfo ri)
{
	std::map<unsigned int, std::string> answers;
	int i = 1;
	try
	{
		GetQuestionResponse getQueRes;
		getQueRes.status = 1;
		Question que = this->m_game->getQuestionForUser(this->m_user);
		if (que.getQuestion() == "null")
		{	
			PlayerResults a;
			std::vector<PlayerResults> gameRes;

			this->m_game->getGameRes(gameRes);
			for (auto& player : gameRes)
			{
				if (player.username == this->m_user.getUsername())
				{
					a = player;
				}
			}
			getQueRes.status = 0;
			this->m_dataBase->AddStat(a, this->m_game->getId());
			// Adding the client's entire game data if it reached and answered it's last question.

		}
		getQueRes.question = que.getQuestion();
		for (auto& answer : que.getPossibleAnswers())
		{ // Converting from std::vector<std::string> to map<int, std::string>
			answers.insert({ i, answer });
			i++;
		}
		getQueRes.answers = answers;

		rr.response = JsonResponsePacketSerializer::serializeResponse(getQueRes);
		rr.newHandler = this;

	}
	catch (std::exception& err)
	{
		ErrorResponse er;

		er.message = "Error, canno't get question for user.";
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}


void GameRequestHandler::getGameResult(RequestResult& rr, RequestInfo ri)
{
	try
	{
		GetGameResultsResponse getGameRes;
		getGameRes.status = 1;
		while (true)
		{
			if (this->allFalse())
			{
				break;
			}
		}
		this->m_game->getGameRes(getGameRes.results, this->m_dataBase);
		//Sort the results so the one in the first place is the winner
		this->m_gameManager->sortResults(getGameRes.results);
		rr.response = JsonResponsePacketSerializer::serializeResponse(getGameRes);
		rr.newHandler = this;
	}
	catch (std::exception& err)
	{
		ErrorResponse er;

		er.message = "Error, canno't get GAME RESULT";
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}

bool GameRequestHandler::allFalse()
{
	// The Function says if all the players reached the end of the game.
	std::vector<PlayerResults> gameRes;
	this->m_game->getGameRes(gameRes, this->m_dataBase);
	return gameRes.size() == this->m_game->getPlayers().size();
}

void GameRequestHandler::leaveGame(RequestResult& rr, RequestInfo ri)
{
	try
	{
		std::vector<PlayerResults> gameRes;
		this->m_game->getGameRes(gameRes, this->m_dataBase);
		LeaveGameResponse leaveGameRes;
		leaveGameRes.status = 1;
		Question que = this->m_game->getQuestionForUser(this->m_user);
		if (que.getQuestion() != "null")
		{
			PlayerResults a;
			for (auto& player : gameRes)
			{
				if (player.username == this->m_user.getUsername())
				{
					a = player;
				}
			}
			this->m_dataBase->AddStat(a, this->m_game->getId());
			// Adding the un-finished game stats to the db.

		}
		this->m_game->removePlayer(this->m_user);
		if (this->m_game->isEmpty())
		{
			this->m_gameManager->deleteGame(*this->m_game);
		}
		
		rr.response = JsonResponsePacketSerializer::serializeResponse(leaveGameRes);
		rr.newHandler = this;
	}
	catch (std::exception& err)
	{
		ErrorResponse er;

		er.message = "Error, canno't leave game";
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}
