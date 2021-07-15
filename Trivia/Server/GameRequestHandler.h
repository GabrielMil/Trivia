#pragma once
#include "GameManager.h"
#include "LoggedUser.h"
#include "SqliteDatabase.h"
#include "IRequestHandler.h"
class GameRequestHandler : public IRequestHandler
{
public:
	GameRequestHandler(LoggedUser& usr, IDatabase* db, GameManager* gm, Game* game);
	virtual ~GameRequestHandler();
	bool isRequestRelevant(RequestInfo reqInfo);
	void handleRequest(RequestResult& rr, RequestInfo reqInfo);
	int getId();
private:
	Game* m_game;
	GameManager* m_gameManager;
	LoggedUser m_user;
	IDatabase* m_dataBase;
	
	//Functions
	void getQuestion(RequestResult&, RequestInfo); // done
	// Signout should be thougt over, Where to locate?
	void sumbitAnswer(RequestResult&, RequestInfo);
	void getGameResult(RequestResult&, RequestInfo); // done
	bool allFalse();
	void leaveGame(RequestResult&, RequestInfo); 


};