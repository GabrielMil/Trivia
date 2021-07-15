#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "LoggedUser.h"
typedef struct RoomData {
	size_t id;
	std::string name = "Room";
	size_t maxPlayers = 4;
	size_t numOfQuestionsInGame = 5;
	size_t timePerQuestion = 30;
	size_t isActive;
}RoomData;

class Room
{
private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
public:
	Room(RoomData&);
	Room();
	~Room();
	void addUser(LoggedUser);
	void removeUser(LoggedUser);
	RoomData& getRoomData();
	void activate();
	void deActivate();
	std::vector<std::string> getAllusers();

};