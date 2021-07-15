#pragma once
#include "IDatabase.h"
#include "LoginManager.h"
#include "LoggedUser.h"
#include "LoginRequestHandler.h"
#include "RoomManager.h"
#include "MenuRequestHandler.h"
#include "StatisticsManager.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"
class RequestHandlerFactory
{
public:
	RequestHandlerFactory(IDatabase* db);
	~RequestHandlerFactory();
	MenuRequestHandler * createMenuRequestHandler(LoggedUser&);
	LoginRequestHandler* createLoginRequestHandler();
	RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser&);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(LoggedUser&);
	GameRequestHandler* createGameRequestHandler(LoggedUser&, Game*);
	Game* createGame(Room& r);


	LoginManager& getLoginManager();
	GameManager& getGameManager();
	IDatabase* getDB();
	Game* getGame();

private:
	StatisticsManager m_statisticsManagay;
	RoomManager m_roomManager;
	LoginManager m_loginManager;
	GameManager m_gameManager;
	IDatabase* m_database;
	Game* m_game;
};