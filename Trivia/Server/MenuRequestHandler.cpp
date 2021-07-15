#include "MenuRequestHandler.h"
MenuRequestHandler::MenuRequestHandler(LoggedUser& usr, IDatabase* db, RoomManager* roomManager, StatisticsManager& statsManager, LoginManager* login)
	: m_user(usr), m_statisticsManager(statsManager), m_dataBase(db)
{
	this->m_id = MENU_ID;
	m_roomManager = roomManager;
	m_loginManager = login;
}

MenuRequestHandler::MenuRequestHandler(LoggedUser& usr, IDatabase* db, RoomManager* roomManager, LoginManager* login)
	: m_user(usr), m_dataBase(db), m_statisticsManager(db)
{
	this->m_id = MENU_ID;
	m_roomManager = roomManager;
	m_loginManager = login;
}

MenuRequestHandler::~MenuRequestHandler()
{
}

bool MenuRequestHandler::isRequestRelevant(RequestInfo reqInfo)
{		
	return (reqInfo.id <= GetPersonalStatus && reqInfo.id >= Logout )|| reqInfo.id == AddQuestionCode;
}

void MenuRequestHandler::handleRequest(RequestResult& rr, RequestInfo reqInfo)
{
	switch (reqInfo.id)
	{
		case PacketCodes::Error:
			break;
		case PacketCodes::Logout:
			this->signout(rr, reqInfo);
			break;
		case PacketCodes::GetRooms:
			this->getRooms(rr, reqInfo);
			break;
		case PacketCodes::GetPlayersInRoom:
			this->getPlayers(rr, reqInfo);
			break;
		case PacketCodes::JoinRoom:
			this->joinRoom(rr, reqInfo);
			break;
		case PacketCodes::CreateRoom:
			this->createRoom(rr, reqInfo);
			break;
		case PacketCodes::GetHighScore:
			this->getHighScore(rr, reqInfo);
			break;
		case PacketCodes::GetPersonalStatus:
			this->getPersonalStats(rr, reqInfo);
			break;
		case PacketCodes::AddQuestionCode:
			this->addQuestion(rr, reqInfo);
			break;
		default:
			break;
	}
}

int MenuRequestHandler::getId()
{
	return this->m_id;
}

StatisticsManager& MenuRequestHandler::getStatManager()
{
	return this->m_statisticsManager;
}

RoomManager& MenuRequestHandler::getRoomManager()
{
	return *this->m_roomManager;
}

void MenuRequestHandler::signout(RequestResult& rr, RequestInfo ri)
{

	try
	{
		this->m_loginManager->logout(m_user.getUsernameR());
		LogoutResponse logoutRes;
		logoutRes.status = 1;
		rr.response = JsonResponsePacketSerializer::serializeResponse(logoutRes);
		rr.newHandler = new LoginRequestHandler(this->m_dataBase, this->m_loginManager);
	}
	catch (std::exception& err)
	{
		ErrorResponse er;

		er.message = "Error, canno't logout";
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}

void MenuRequestHandler::getRooms(RequestResult& rr, RequestInfo ri)
{
	try
	{
		GetRoomsResponse getRoomsRes;
		getRoomsRes.status = 1;
		getRoomsRes.rooms = this->m_roomManager->getRooms();
		rr.response = JsonResponsePacketSerializer::serializeResponse(getRoomsRes);
		rr.newHandler = this;
	}
	catch (std::exception& err)
	{
		ErrorResponse er;

		er.message = "Error, canno't getRoom";
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}

void MenuRequestHandler::getPlayers(RequestResult& rr , RequestInfo ri)
{

	GetPlayersInRoomRequest lr;
	JsonRequestPacketDeserializer::deserializeGetPlayersRequest(lr, ri.buffer);
	try
	{
		GetPlayersInRoomResponse playerRes;
		playerRes.players = this->m_roomManager->getRoomById(lr.roomId).getAllusers();
		playerRes.status = 1;
		rr.response = JsonResponsePacketSerializer::serializeResponse(playerRes);
		rr.newHandler = this;
	}
	catch (std::exception& err)
	{
		ErrorResponse er;

		er.message = "Error, canno't get players.";
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}

void MenuRequestHandler::getPersonalStats(RequestResult& rr , RequestInfo ri)
{
	try
	{
		GetPersonalStatusResponse statusRes;
		statusRes.status = 1;
		statusRes.statistics = m_statisticsManager.getUserStatistics(this->m_user.getUsername());
		rr.response = JsonResponsePacketSerializer::serializeResponse(statusRes);
		rr.newHandler = this;
	}
	catch (std::exception& err)
	{
		ErrorResponse er;

		er.message = "Error, canno't get status of given player.";
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}

void MenuRequestHandler::getHighScore(RequestResult& rr, RequestInfo ri)
{

	try
	{
		GetHighScoreResponse statusRes;
		statusRes.status = 1;
		statusRes.statistics = m_statisticsManager.getHighScore();
		rr.response = JsonResponsePacketSerializer::serializeResponse(statusRes);
		rr.newHandler = this;
	}
	catch (std::exception& err)
	{
		ErrorResponse er;

		er.message = "Error, canno't get status of given player.";
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}

void MenuRequestHandler::joinRoom(RequestResult& rr, RequestInfo ri)
{
	JoinRoomRequest lr;
	JsonRequestPacketDeserializer::deserializeJoinRoomRequest(lr, ri.buffer);

	try
	{
		JoinRoomResponse statusRes;
		statusRes.status = 1;
		rr.response = JsonResponsePacketSerializer::serializeResponse(statusRes);
		this->m_roomManager->joinRoom(this->m_user, lr.roomId);
		rr.newHandler = this;
	}
	catch (std::exception& err)
	{
		ErrorResponse er;

		er.message = "Error, canno't get join wanted room.";
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}

void MenuRequestHandler::createRoom(RequestResult& rr , RequestInfo ri)
{
	CreateRoomRequest lr;
	JsonRequestPacketDeserializer::deserializeCreateRoomRequest(lr, ri.buffer);
	// Setting the room data based on given msg
	RoomData rd;
	// Copying the room data.
	rd.id = this->m_dataBase->getAllGames().size() + 1;
	rd.name = lr.roomName;
	rd.maxPlayers = lr.maxUsers;
	rd.numOfQuestionsInGame = lr.questionCount;
	rd.timePerQuestion = lr.answerTimeout;
	rd.isActive = 1;
	try
	{
		CreateRoomResponse createRes;
		
		if (this->m_roomManager->doesRoomExists(rd.name))
		{
			throw std::exception("Error, Room with selected name already running ");
		}
		else if (this->m_dataBase->getAllQuestions().size() < rd.numOfQuestionsInGame)
		{
			std::string errM = "Error, Max number of questions is -" + std::to_string(this->m_dataBase->getAllQuestions().size());
			throw std::exception(errM.c_str());
		}
		else
		{
			createRes.status = 1;
			this->m_roomManager->createRoom(this->m_user, rd);
			this->m_dataBase->AddGame(rd.id);
		}
		rr.response = JsonResponsePacketSerializer::serializeResponse(createRes);
		rr.newHandler = this;
	}
	catch (std::exception& err)
	{
		ErrorResponse er;
		if (strlen(err.what()) > 1)
		{
			er.message = err.what();
		}
		else
		{
			er.message = "Error, canno't create Room.";
		}
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}

void MenuRequestHandler::addQuestion(RequestResult& rr, RequestInfo ri)
{
	AddQuestionRequest lr;
	JsonRequestPacketDeserializer::deserializeAddQuestionRequest(lr, ri.buffer);
	try
	{
		AddQuestionResponse statusRes;
		statusRes.status = 1;
		rr.response = JsonResponsePacketSerializer::serializeResponse(statusRes);
		rr.newHandler = this;
		//Creating a question to add to the db.
		QuestionS q{ this->m_dataBase->getAllQuestions().size() + 1, lr.question, lr.correctAns, lr.ans2, lr.ans3, lr.ans4 };
		this->m_dataBase->addQuestion(q);
	}
	catch (std::exception& err)
	{
		ErrorResponse er;
		er.message = "Error, could not add answer.";
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}
