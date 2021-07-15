#include "LoginManager.h"

LoginManager::LoginManager(IDatabase* db) :
	m_database(db)
{}

void LoginManager::signup(std::string & username, std::string & password, std::string & email)
{
	if (this->m_database->doesUserExist(username))
	{
		throw std::exception("User already exist!");
		return;
	}
	this->m_database->addNewUser(username, password, email);
}

void LoginManager::login(std::string& username, std::string& password)
{
	if (!this->m_database->doesUserExist(username))  // if user not exist stop
	{
		throw std::exception("User not exist!");
		return;
	}
	if (!this->m_database->doesPasswordMatch(username, password))  // if password doesn't match not exist stop
	{
		throw std::exception("Password doesn't match!");
		return;
	}
	for (auto& elem : this->m_loggedUsers)
	{	//	Checking if the user is already connected
		if (username == elem.getUsername())
		{
			throw std::exception("Username is already connected!");
			return;
		}
	}

	//If everything ok then add to vector of logged users
	this->m_loggedUsers.push_back(LoggedUser(username));
}

void LoginManager::logout(std::string& username)
{
	std::vector<LoggedUser>::iterator toRemove;
	bool isFound = false;
	//Search for the user in the vector
	for (auto usr = this->m_loggedUsers.begin(); usr != this->m_loggedUsers.end(); usr++)
	{
		if ((*usr).getUsername() == username)
		{
			toRemove = usr;
			isFound = true;
		}
	}
	if (!isFound)
	{
		throw std::exception("User to logout not found!");
		return;
	}
	this->m_loggedUsers.erase(toRemove);  // remove from vector
}

std::vector<LoggedUser> LoginManager::getLoggedUsers()
{
	return this->m_loggedUsers;
}
