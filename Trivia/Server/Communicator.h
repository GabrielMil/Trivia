#pragma once

#include "Helper.h"
#include "JsonResponsePacketSerializer.h"
#include <string>
#include "RequestHandlerFactory.h" 
#include <iostream>
#include <map>
#include <thread>
#include <mutex>
#include <exception>

#define LOCAL_IP "127.0.0.1"
#define PORT 1227  // Just random port, hope it won't kill my computer

class Communicator
{
public:
	Communicator(RequestHandlerFactory& rhf);
	~Communicator();

	void startHandleRequests();

private:
	void bindAndListen();
	void handleNewClient(SOCKET newClient);
	std::vector<SOCKET> socketRoomFilter(std::map<SOCKET, IRequestHandler*>& m_clients, Room& room);
	void clientHandler(SOCKET client);

	void accept();

	int read(SOCKET newClient, RequestInfo& reqInfo);
	void write(SOCKET newClient, Buffer& buffer);

	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory& m_handlerFactory;
};