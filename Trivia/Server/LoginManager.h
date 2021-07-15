#pragma once

#include <string>
#include <vector>
#include <iterator>
#include "IDatabase.h"
#include "LoggedUser.h"

class LoginManager
{
public:
	LoginManager(IDatabase* db);

	void signup(std::string& username, std::string& password, std::string& email);
	void login(std::string& username, std::string& password);
	void logout(std::string& username);
	std::vector<LoggedUser> getLoggedUsers();
private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
};