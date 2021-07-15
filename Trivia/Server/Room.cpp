#include "Room.h"

Room::Room(RoomData& rd)
{
	this->m_metadata.id = rd.id;
	this->m_metadata.isActive = rd.isActive;
	this->m_metadata.maxPlayers = rd.maxPlayers;
	this->m_metadata.name = rd.name;
	this->m_metadata.numOfQuestionsInGame = rd.numOfQuestionsInGame;
	this->m_metadata.timePerQuestion = rd.timePerQuestion;
}

Room::Room()
{
}

Room::~Room()
{
	this->m_users.clear();
	//delete &this->m_metadata;
}

void Room::addUser(LoggedUser usr)
{
	this->m_users.push_back(usr);
}

void Room::removeUser(LoggedUser usr )
{
	int i = 0;
	for (i = 0; i < this->m_users.size(); i++)
	{
		if (this->m_users[i].getUsername().compare(usr.getUsername()) == 0)
		{
			this->m_users.erase(this->m_users.begin() + i);
			break;
		}
	}

}

RoomData& Room::getRoomData()
{
	return this->m_metadata;
}

void Room::activate()
{
	this->m_metadata.isActive = 1;
}

void Room::deActivate()
{
	this->m_metadata.isActive = 0;
}

std::vector<std::string> Room::getAllusers()
{
	std::vector<std::string> toRet;
	int i = 0;
	for (i = 0; i < this->m_users.size(); i++)
	{
		toRet.push_back(this->m_users[i].getUsername());
	}
	return toRet;
}
