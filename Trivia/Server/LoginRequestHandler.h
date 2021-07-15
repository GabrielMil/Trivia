#pragma once
#include "Helper.h"
#include "IRequestHandler.h"
#include "LoginManager.h"
class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(IDatabase* db, LoginManager* loginManager);
	virtual ~LoginRequestHandler();
	bool isRequestRelevant(RequestInfo reqInfo) ;
	void handleRequest(RequestResult& rr, RequestInfo reqInfo) ;
	void setUsr(LoggedUser&);
	int getId();

private:
	LoginManager* m_loginManager;
	LoggedUser m_LoggedUsr;
	void login(RequestResult&, RequestInfo);
	void signup(RequestResult&, RequestInfo);
};