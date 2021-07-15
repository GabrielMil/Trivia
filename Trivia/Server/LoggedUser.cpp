#include "LoggedUser.h"

LoggedUser::LoggedUser(std::string username):
	m_username(username)
{}

LoggedUser::~LoggedUser()
{
}

std::string LoggedUser::getUsername() const
{
	return this->m_username;
}

std::string& LoggedUser::getUsernameR()
{
	return this->m_username;
}
