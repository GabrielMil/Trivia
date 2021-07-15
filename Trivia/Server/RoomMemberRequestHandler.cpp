#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(LoggedUser& usr, Room*  room, RoomManager* roomManager, IDatabase* db)
	:RoomRequestHandler(usr, room, roomManager,db)
{
	this->m_id = ROOM_MEMBER_ID;
}

RoomMemberRequestHandler::~RoomMemberRequestHandler()
{
}

bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo reqInfo)
{
	return reqInfo.id == PacketCodes::LeaveRoomRequest || reqInfo.id == PacketCodes::StartGame || reqInfo.id == PacketCodes::GetRoomStateRequest;
}

void RoomMemberRequestHandler::handleRequest(RequestResult& rr, RequestInfo reqInfo)
{

	switch (reqInfo.id)
	{
	case PacketCodes::GetRoomStateRequest:
		this->getRoomState(rr, reqInfo);
		break;
	case PacketCodes::LeaveRoomRequest:
		this->leaveRoom(rr, reqInfo);
		break;
	default:
		break;
	}
}

int RoomMemberRequestHandler::getId()
{
	return this->m_id;
}

void RoomMemberRequestHandler::leaveRoom(RequestResult& rr , RequestInfo ri)
{
	try
	{
		LeaveRoomResponse leaveRoom;
		leaveRoom.status = 1;
		rr.response = JsonResponsePacketSerializer::serializeResponse(leaveRoom);
		rr.newHandler = this;
		this->m_room->removeUser(this->m_user); //  Exiting the leaving user
	}
	catch (std::exception& err)
	{
		ErrorResponse er;

		er.message = "Error, canno't getRoom";
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}
