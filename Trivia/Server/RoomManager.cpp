#include "RoomManager.h"
#include "SqliteDatabase.h"
RoomManager::RoomManager()
{
}
RoomManager::~RoomManager()
{
	this->m_rooms.clear();
}
void RoomManager::createRoom(LoggedUser& loggedUser, RoomData& roomData)
{
	Room newRoom(roomData); // Creating the new room.
	newRoom.addUser(loggedUser); // Adding the creator of the room to the room.
	this->m_rooms.insert({ roomData.id, newRoom });
}

bool RoomManager::deleteRoom(int id)
{	// Deleting the room from the list.
	if (this->m_rooms.find(id) == this->m_rooms.end()) {
		std::cout << "User id was not found" << std::endl;
		return false;
	}
	this->m_rooms.erase(id);
	return true;
}

size_t RoomManager::getRoomState(int id)
{
	if (this->m_rooms.find(id) == this->m_rooms.end()) {
		std::cout << "User id was not found" << std::endl;
		return NULL;
	}
	return this->m_rooms[id].getRoomData().isActive;
}

std::vector<RoomData> RoomManager::getRooms()
{
	std::vector<RoomData> roomVector;
	int i = 0;
	for (auto& elem : this->m_rooms)
	{	//	Getting all the rooms's data
		roomVector.push_back(elem.second.getRoomData());
		//roomVector[i] = elem.second.getRoomData();	
		i++;
	}
	return roomVector;
}

Room RoomManager::getRoomById(int id)
{
	RoomData temp;
	if (this->m_rooms.find(id) == this->m_rooms.end()) {
		std::cout << "User id was not found" << std::endl;
		return Room(temp);
	}
	return this->m_rooms[id];
}

Room& RoomManager::getRoomByUser(LoggedUser& usr)
{
	int i = 0;
	RoomData temp;
	Room tempR(temp);
	for (auto& elem : this->m_rooms)
	{	//	Getting all the rooms's data
		for (i = 0; i < elem.second.getAllusers().size(); i++)
		{
			if (elem.second.getAllusers()[i] == usr.getUsername())
			{
				return elem.second;
			}
		}
	}
	return tempR;
}

void RoomManager::joinRoom(LoggedUser user, int roomId)
{
	this->m_rooms[roomId].addUser(user);
}

bool RoomManager::doesRoomExists(std::string roomName)
{
	for (auto& elem : this->m_rooms)
	{
		if (elem.second.getRoomData().name == roomName && elem.second.getRoomData().isActive == 1)
		{
			return true;
		}
	}
	return false;
}
