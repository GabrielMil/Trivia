#pragma once

#include "Communicator.h"
#include "IRequestHandler.h"
#include "IDatabase.h"

#include <iostream>
#include <string>

class Server
{
public:
	Server(IDatabase* db);
	~Server();
	void run();

private:
	QuestionS getQuestion();
	void printQuestion(QuestionS& question);

	Communicator m_communicator;
	IDatabase* m_dataBase;
	RequestHandlerFactory m_handlerFactory;
};