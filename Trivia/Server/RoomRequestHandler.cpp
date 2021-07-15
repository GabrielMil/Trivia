#include "RoomRequestHandler.h"

RoomRequestHandler::RoomRequestHandler(LoggedUser& usr, Room* room, RoomManager* roomManager, IDatabase* db)
	: m_user(usr), m_room(room), m_roomManager(roomManager), m_db(db)
{
}

RoomRequestHandler::~RoomRequestHandler()
{
}

bool RoomRequestHandler::isRequestRelevant(RequestInfo reqInfo)
{
	return reqInfo.id == PacketCodes::StartGame || reqInfo.id == PacketCodes::GetRoomStateRequest;

}

void RoomRequestHandler::handleRequest(RequestResult&, RequestInfo reqInfo)
{
	
}


void RoomRequestHandler::getRoomState(RequestResult& rr, RequestInfo ri)
{
	RoomData rd;
	try
	{
		GetRoomStateResponse getStateRes;
		getStateRes.status = 1;
		// Getting al the roomStateData in form of 'GetRoomStateResponse' struct
		getStateRes.players = this->m_room->getAllusers();
		rd = this->m_room->getRoomData();
		getStateRes.questionCount = rd.numOfQuestionsInGame;
		getStateRes.answerTimeOut = rd.timePerQuestion;
		getStateRes.hasGameBegun = rd.isActive == 1 ? true : false;
		rr.response = JsonResponsePacketSerializer::serializeResponse(getStateRes);
		rr.newHandler = this;
	}
	catch (std::exception& err)
	{
		ErrorResponse er;

		er.message = "Error, canno't get Room state";
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}

RoomManager& RoomRequestHandler::getRoomManager()
{
	return *this->m_roomManager;
}

Room& RoomRequestHandler::getRoom()
{
	return *this->m_room;
}


