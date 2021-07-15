#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"

LoginRequestHandler::LoginRequestHandler(IDatabase* db, LoginManager* loginManager)
	: m_loginManager(loginManager), m_LoggedUsr("null")
{
	this->m_id = LOGIN_ID;
}

LoginRequestHandler::~LoginRequestHandler()
{
}

bool LoginRequestHandler::isRequestRelevant(RequestInfo reqInfo)
{
	return reqInfo.id == PacketCodes::Signup || reqInfo.id == PacketCodes::Login;
}

void LoginRequestHandler::handleRequest(RequestResult& rr, RequestInfo reqInfo)
{
	//TODO: Menu Handler and complete this one
	Buffer toSend;
	if (reqInfo.id == Signup)
	{
		this->signup(rr, reqInfo);

	}
	else  // Login
	{
		this->login(rr, reqInfo);
	}
}

void LoginRequestHandler::setUsr(LoggedUser& loggedUsr)
{
	loggedUsr = LoggedUser(this->m_LoggedUsr.getUsername());
}

int LoginRequestHandler::getId()
{
	return this->m_id;
}

void LoginRequestHandler::login(RequestResult& rr, RequestInfo ri)
{
	LoginRequest lr;
	
	JsonRequestPacketDeserializer::deserializeLoginRequest(lr, ri.buffer);
	// Login to database
	try
	{
		this->m_loginManager->login(lr.username, lr.password);
		LoginResponse loginRes;
		loginRes.status = 1;
		rr.response = JsonResponsePacketSerializer::serializeResponse(loginRes);
		this->m_LoggedUsr = LoggedUser(lr.username);
		rr.newHandler = this;
	}
	catch (std::exception& err)
	{		// Assembelying an {message:ERROR} to send

		ErrorResponse er;
		er.message = err.what();
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}

void LoginRequestHandler::signup(RequestResult& rr, RequestInfo ri)
{
	SignupRequest sr;
	JsonRequestPacketDeserializer::deserializeSignupRequest(sr, ri.buffer);
	// Signup to database
	try
	{
		this->m_loginManager->signup(sr.username, sr.password, sr.email);
		// Assembelying an {status:1} to send
		SignupResponse signUpRes;
		signUpRes.status = 1;
		rr.response = JsonResponsePacketSerializer::serializeResponse(signUpRes);
		this->m_LoggedUsr = LoggedUser(sr.username);
		rr.newHandler = this;
	}
	catch (std::exception& err)
	{
		// Assembelying an {message:ERROR} to send
		ErrorResponse er;
		er.message = err.what();
		rr.response = JsonResponsePacketSerializer::serializeResponse(er);
		rr.newHandler = nullptr;
	}
}
