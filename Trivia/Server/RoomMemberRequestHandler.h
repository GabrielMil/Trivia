#pragma once
#include "RoomRequestHandler.h"
class RoomMemberRequestHandler : public RoomRequestHandler
{
public:
	RoomMemberRequestHandler(LoggedUser& usr, Room* room, RoomManager* roomManager, IDatabase* db);
	virtual ~RoomMemberRequestHandler();
	bool isRequestRelevant(RequestInfo);
	void handleRequest(RequestResult& rr, RequestInfo reqInfo);
	int getId();

private:
	void leaveRoom(RequestResult&, RequestInfo);

};
