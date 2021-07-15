#pragma once
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "Helper.h"
#include <ctime>
#include <iostream>
#include <vector>

class IRequestHandler;

typedef struct RequestResult {
	Buffer response;
	IRequestHandler* newHandler;
}RequestResult;

typedef struct RequestInfo
{
	int id;
	std::time_t receivalTime;
	Buffer buffer;
}RequestInfo;

class IRequestHandler
{
public:
	IRequestHandler();
	virtual ~IRequestHandler();
	virtual bool isRequestRelevant(RequestInfo reqInfo) = 0;
	virtual void handleRequest(RequestResult& rr, RequestInfo reqInfo) = 0;
	virtual int getId() = 0;
protected:
	int m_id;
};