#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "Server.h"

#include <iostream>

int main()
{
	SqliteDatabase db;
	try
	{
		WSAInitializer wsaInit;
		Server myServer(&db);

		myServer.run();
	}
	catch (std::exception& e)
	{
		std::cout << "Error occured: " << e.what() << std::endl;
	}
	return 0;
}