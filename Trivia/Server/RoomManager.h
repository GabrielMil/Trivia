#pragma once
#include "Room.h"
#include "SqliteDatabase.h"
#include <map>
class RoomManager
{
public:
	RoomManager();
	~RoomManager();
	void createRoom(LoggedUser&, RoomData&);
	bool deleteRoom(int id);
	size_t getRoomState(int id);
	std::vector<RoomData> getRooms();
	Room getRoomById(int id);
	Room& getRoomByUser(LoggedUser&);
	void joinRoom(LoggedUser, int);
	bool doesRoomExists(std::string roomName);
private:
	std::map<int, Room> m_rooms;
};