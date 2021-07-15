#pragma once
#include "RoomRequestHandler.h"
class RoomAdminRequestHandler : public RoomRequestHandler
{
public:
	RoomAdminRequestHandler(LoggedUser& usr, Room* room, RoomManager* roomManager, IDatabase* db);
	virtual ~RoomAdminRequestHandler();
	bool isRequestRelevant(RequestInfo);
	void handleRequest(RequestResult& rr, RequestInfo reqInfo);
	int getId();


private:

	void closeRoom(RequestResult&, RequestInfo);
	void startGame(RequestResult& rr, RequestInfo ri);
	
};
