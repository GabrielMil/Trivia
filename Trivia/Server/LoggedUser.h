#pragma once
#include <string>

class LoggedUser
{
public:
	LoggedUser(std::string username);
	~LoggedUser();
	std::string getUsername() const;
	std::string& getUsernameR();

private:
	std::string m_username;
};