#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(LoggedUser& usr, Room* room, RoomManager* roomManager, IDatabase* db)
	:RoomRequestHandler(usr,room,roomManager,db)
{	
	this->m_id = ROOM_ADMIN_ID;
}

RoomAdminRequestHandler::~RoomAdminRequestHandler()
{
}

bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo reqInfo)
{
	return reqInfo.id == PacketCodes::CloseRoom || reqInfo.id == PacketCodes::StartGame || reqInfo.id == PacketCodes::GetRoomStateRequest;

}

void RoomAdminRequestHandler::handleRequest(RequestResult& rr, RequestInfo reqInfo)
{
	switch (reqInfo.id)
	{
	case PacketCodes::GetRoomStateRequest:
		this->getRoomState(rr, reqInfo);
		break;
	case PacketCodes::CloseRoom:
		this->closeRoom(rr, reqInfo);
		break;
	case PacketCodes::StartGame:
		this->startGame(rr, reqInfo);
		break;
	default:
		break;
	}
}

void RoomAdminRequestHandler::startGame(RequestResult& rr, RequestInfo ri)
{
	try
	{
		StartGameResponse sGameRes;
		sGameRes.status = 1;
		rr.response = JsonResponsePacketSerializer::serializeResponse(sGameRes);
		this->m_room->activate();
		this->m_db->AddGame(this->m_db->getAllGames().size() + 1);
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

int RoomAdminRequestHandler::getId()
{
	return this->m_id;
}

void RoomAdminRequestHandler::closeRoom(RequestResult& rr, RequestInfo ri)
{
	try
	{
		CloseRoomResponse cGameRes;
		cGameRes.status = 1;
		rr.response = JsonResponsePacketSerializer::serializeResponse(cGameRes);
		this->m_room->deActivate();
		
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
