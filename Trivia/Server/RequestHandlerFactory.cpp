#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* db)
	: m_loginManager(db), m_roomManager(), m_statisticsManagay(db), m_gameManager(db)
{
	m_database = db;
}

RequestHandlerFactory::~RequestHandlerFactory()
{
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(this->m_database, &this->m_loginManager);
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser& usr)
{
	return new RoomAdminRequestHandler(usr, &this->m_roomManager.getRoomByUser(usr) ,&this->m_roomManager, this->m_database);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser& usr)
{
	return new RoomMemberRequestHandler(usr, &this->m_roomManager.getRoomByUser(usr), &this->m_roomManager, this->m_database);
}

GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(LoggedUser& usr, Game* g)
{
	return new GameRequestHandler(usr, this->m_database, &this->m_gameManager,g);
}

Game* RequestHandlerFactory::createGame(Room& r)
{
	return new Game(r, this->m_database);
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser& usr)
{
	return new MenuRequestHandler(usr, this->m_database, &this->m_roomManager,&this->m_loginManager);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return this->m_loginManager;
}

GameManager& RequestHandlerFactory::getGameManager()
{
	return this->m_gameManager;
}

IDatabase* RequestHandlerFactory::getDB()
{
	return this->m_database;
}


