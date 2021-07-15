#pragma once
#include "RoomManager.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "LoggedUser.h"
#include "StatisticsManager.h"
#include "SqliteDatabase.h"
class RoomRequestHandler : public IRequestHandler
{
public:
	RoomRequestHandler(LoggedUser& usr, Room* room, RoomManager* roomManager, IDatabase* db);
	virtual ~RoomRequestHandler();
	bool isRequestRelevant(RequestInfo);
	void handleRequest(RequestResult& rr, RequestInfo reqInfo);
	RoomManager& getRoomManager();
	Room& getRoom();

protected:
	IDatabase* m_db;
	Room* m_room;
	LoggedUser m_user;
	RoomManager* m_roomManager;
	void getRoomState(RequestResult&, RequestInfo);
};
