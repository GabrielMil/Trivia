#pragma once
#include "RoomManager.h"
#include "LoginRequestHandler.h"
#include "StatisticsManager.h"
class MenuRequestHandler : public IRequestHandler
{
public:
	MenuRequestHandler(LoggedUser& usr, IDatabase* db, RoomManager* roomManager, StatisticsManager& statsManager,LoginManager* login);
	MenuRequestHandler(LoggedUser& usr, IDatabase* db, RoomManager* roomManager, LoginManager* login);
	virtual ~MenuRequestHandler();
	bool isRequestRelevant(RequestInfo reqInfo);
	void handleRequest(RequestResult& rr, RequestInfo reqInfo);
	int getId();

	// Getters 
	StatisticsManager& getStatManager();
	RoomManager& getRoomManager();
	
private:
	StatisticsManager m_statisticsManager;
	RoomManager* m_roomManager;
	LoggedUser m_user;
	LoginManager* m_loginManager;
	IDatabase* m_dataBase;

	//Functions
	void signout(RequestResult&, RequestInfo);
	// Signout should be thougt over, Where to locate?
	void getRooms(RequestResult&, RequestInfo);
	void getPlayers(RequestResult&, RequestInfo);
	void getPersonalStats(RequestResult&, RequestInfo); // Half way through
	void getHighScore(RequestResult&, RequestInfo);
	void joinRoom(RequestResult&, RequestInfo);
	void createRoom(RequestResult&, RequestInfo);
	void addQuestion(RequestResult&, RequestInfo);


};