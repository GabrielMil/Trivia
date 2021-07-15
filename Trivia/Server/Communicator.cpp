#include "Communicator.h"
#include <mutex>          // std::mutex

template<typename Base, typename T>
inline bool instanceof(const T*) {
	return std::is_base_of<Base, T>::value;
}

Communicator::Communicator(RequestHandlerFactory& rhf):
	m_handlerFactory(rhf)
{
	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");

	this->bindAndListen();
}

Communicator::~Communicator()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		closesocket(m_serverSocket);

		//Clear all clients data
		for (auto client : m_clients)
		{
			closesocket(client.first);
			delete client.second;
			client.second = nullptr;
		}
	}
	catch (...) {}

	m_clients.clear();
}

std::vector<SOCKET> Communicator::socketRoomFilter(std::map<SOCKET, IRequestHandler*>& m_clients, Room& room)
{
	std::vector<SOCKET> filteredSockets;
	for (auto const& client : m_clients)
	{
		try
		{

			if (((RoomMemberRequestHandler*)client.second)->getRoom().getRoomData().id == room.getRoomData().id)
			{
				filteredSockets.push_back(client.first);
			}
		}
		catch(...)
		{

		}
	}
	return filteredSockets;
}
void Communicator::startHandleRequests()
{
	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		//std::cout << "accepting client..." << std::endl;
		accept();
	}
}

void Communicator::bindAndListen()
{
	//std::cout << "Starting..." << std::endl;
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(PORT); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	inet_pton(AF_INET, LOCAL_IP, &sa.sin_addr.s_addr);  // Set the IP

	// again stepping out to the global namespace
	// Connects between the socket and the configuration (port and etc..)
	if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");
	std::cout << "binded" << std::endl;

	// Start listening for incoming requests of clients
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "listening..." << std::endl;
}

void Communicator::handleNewClient(SOCKET newClient)
{

	//Here we start treating the client with all the other
	try
	{
		clientHandler(newClient);
	}
	catch (const std::exception& err)
	{
		std::cout << "Exception was catch in function handleNewClient(HI). " << newClient << " what=" << err.what() << std::endl;
	}	
}

void Communicator::clientHandler(SOCKET client)
{
	bool isRunning = true,  isLoggedIn = false;
	RequestInfo getData;
	RequestResult reqRes;
	Buffer sendBuffer;
	LoggedUser loggedUsr("null");
	LoginRequestHandler* loginHandler = m_handlerFactory.createLoginRequestHandler();
	MenuRequestHandler* menuHandler = NULL;
	RoomRequestHandler* roomMHandler = NULL;
	RoomRequestHandler* roomAHandler = NULL;
	GameRequestHandler* gameHandler = NULL;
	Game* g;
	int i = 0;
	bool isGameStarted = false;
	std::vector<SOCKET> currRoomSockets;
	while (isRunning)
	{
		//Get at first the data
		//May happen problem with socket so catch it
		try
		{
			while (read(client, getData) == INVALID_SOCKET)
			{
				if (read(client, getData) == INVALID_SOCKET)
				{
					throw std::exception("client signout");
				}
			};  
	
		}
		catch (std::exception& err)
		{
			std::cout << "Exception was catch in function clientHandler. " << client << " what=" << err.what() << std::endl;
			isRunning = false;
			if (isLoggedIn)
			{
				if (((RoomAdminRequestHandler*)reqRes.newHandler)->getId() == ROOM_ADMIN_ID)
				{// In case an error ocurred whilst the client was  an admin of an un-started game.
					getData.id = PacketCodes::CloseRoom;
					((RoomAdminRequestHandler*)roomAHandler)->handleRequest(reqRes, getData);
					sendBuffer = reqRes.response;

					// A leave room response will be assemebled in order to send to all member clients.
					LeaveRoomResponse leaveRoom;
					leaveRoom.status = 1;
					sendBuffer = JsonResponsePacketSerializer::serializeResponse(leaveRoom);
					currRoomSockets = socketRoomFilter(this->m_clients, roomAHandler->getRoom());
					for (i = 0; i < currRoomSockets.size(); i++)
					{ // Sending the start announcment to each of the room members.
						write(currRoomSockets[i], sendBuffer);
					}
				}
				else if (((RoomMemberRequestHandler*)reqRes.newHandler)->getId() == ROOM_MEMBER_ID)
				{// In case an error ocurred whilst the client was a member of an un-started game.
					getData.id = PacketCodes::LeaveRoomRequest;
					((RoomMemberRequestHandler*)roomMHandler)->handleRequest(reqRes, getData);

				}
				getData.id = PacketCodes::Logout;
			}
		}

		//TODO: use the data we got and start handeling it
		if (loginHandler != NULL && loginHandler->isRequestRelevant(getData))
		{
			loginHandler->handleRequest(reqRes, getData);
			loginHandler->setUsr(loggedUsr);
			sendBuffer = reqRes.response;
			if (loggedUsr.getUsername() == "null")
			{ // In case the user closed the App before login in.
				isLoggedIn = true;
			}
			if (reqRes.newHandler != nullptr)
			{
				menuHandler = this->m_handlerFactory.createMenuRequestHandler(loggedUsr);
				reqRes.newHandler = menuHandler;
				this->m_clients[client] = reqRes.newHandler;
				isLoggedIn = true;
			}
		}
		
		else if (menuHandler != NULL &&  menuHandler->isRequestRelevant(getData))
		{
			menuHandler->handleRequest(reqRes, getData);
			sendBuffer = reqRes.response;
			if (getData.id == PacketCodes::JoinRoom)
			{ // Metching each Handler depending on the client's actions.

				roomMHandler = this->m_handlerFactory.createRoomMemberRequestHandler(loggedUsr);
				reqRes.newHandler = roomMHandler;
			}
			else if (getData.id == PacketCodes::CreateRoom)
			{
				roomAHandler = this->m_handlerFactory.createRoomAdminRequestHandler(loggedUsr);
				reqRes.newHandler = roomAHandler;
			}
			if (reqRes.newHandler != nullptr)
			{
				this->m_clients[client] = reqRes.newHandler;
				
			}
		}
		
		else if (roomMHandler != NULL && roomMHandler->isRequestRelevant(getData))
		{
			roomMHandler->handleRequest(reqRes, getData);
			sendBuffer = reqRes.response;
			if (getData.id == PacketCodes::LeaveRoomRequest)
			{
				if (reqRes.newHandler != nullptr)
				{
					reqRes.newHandler = menuHandler;
					this->m_clients[client] = reqRes.newHandler;
				}
			}
			else
			{
				g = this->m_handlerFactory.createGame(((RoomRequestHandler*)reqRes.newHandler)->getRoom());

				gameHandler = this->m_handlerFactory.createGameRequestHandler(loggedUsr, g);
			}

		}
		else if(roomAHandler != NULL &&  roomAHandler->isRequestRelevant(getData))
		{
			roomAHandler->handleRequest(reqRes, getData);
			sendBuffer = reqRes.response;
			if (getData.id == PacketCodes::StartGame)
			{
				if (reqRes.newHandler != nullptr)
				{
					((RoomRequestHandler*)reqRes.newHandler)->getRoom().deActivate();
					g = this->m_handlerFactory.createGame(((RoomRequestHandler*)reqRes.newHandler)->getRoom());
					gameHandler = this->m_handlerFactory.createGameRequestHandler(loggedUsr, g);
					this->m_handlerFactory.getGameManager().createGame(*g);
					reqRes.newHandler = gameHandler;
					this->m_clients[client] = reqRes.newHandler;
					currRoomSockets = socketRoomFilter(this->m_clients, roomAHandler->getRoom());
					for (i = 0; i < currRoomSockets.size(); i++)
					{ // Sending the start announcment to each of the room members.
						write(currRoomSockets[i], sendBuffer = reqRes.response);
					}
					isGameStarted = true;
				}
			}
			else if (getData.id == PacketCodes::CloseRoom)
			{
				// Returning to menu handler
				reqRes.newHandler = menuHandler;
				this->m_clients[client] = reqRes.newHandler;
				LeaveRoomResponse leaveRoom;
				leaveRoom.status = 1;
				sendBuffer = JsonResponsePacketSerializer::serializeResponse(leaveRoom);
				currRoomSockets = socketRoomFilter(this->m_clients, roomAHandler->getRoom());
				for (i = 0; i < currRoomSockets.size(); i++)
				{ // Sending the leave announcment to each of the room members.
					write(currRoomSockets[i], sendBuffer);
				}
				roomAHandler->getRoomManager().deleteRoom(roomAHandler->getRoom().getRoomData().id);
				sendBuffer = reqRes.response;
			}
		}
		else if (gameHandler != NULL && gameHandler->isRequestRelevant(getData))
		{
			if (isGameStarted)
			{
				roomAHandler->getRoomManager().deleteRoom(roomAHandler->getRoom().getRoomData().id);
				isGameStarted = false;
			}
			gameHandler->handleRequest(reqRes, getData);
			sendBuffer = reqRes.response;
			if (getData.id == PacketCodes::LeaveGame)
			{
				if (reqRes.newHandler != nullptr)
				{ // Then leaving the game
					reqRes.newHandler = menuHandler;
					this->m_clients[client] = reqRes.newHandler;
				}
			}

		}
		else if (getData.id == Error)
		{
			std::cout << "An error occured, Unknown code1" << std::endl;
		}
		else
		{
			std::cout << "An error occured, Unknown code2" << std::endl;
		}
		//May happen problem with socket so catch it
		try
		{
			if (isLoggedIn)
			{
				write(client, sendBuffer);  // send the data
			}

		}
		catch (std::exception& err)
		{

			std::cout << "Exception was catch in function clientHandler. " << client << " what=" << err.what() << std::endl;
			isRunning = false;
		}
	}
}

void Communicator::accept()
{
	// notice that we step out to the global namespace
	// for the resolution of the function accept

	// this accepts the client and create a specific socket from server to this client
	SOCKET client_socket = ::accept(m_serverSocket, NULL, NULL);

	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	//std::cout << "Client accepted !" << std::endl;

	//Create a new "client" thread
	std::thread t_client(&Communicator::handleNewClient, this, client_socket);
	t_client.detach();

	m_clients.insert(std::pair<SOCKET, IRequestHandler*>(client_socket, m_handlerFactory.createLoginRequestHandler()));
}

int Communicator::read(SOCKET newClient, RequestInfo& msg)
{
	//Get message
	size_t len = 0;
	int minBytes = 5; // 1 - code, 4 - length of data
	char* dataRecv = new char[minBytes];
	Buffer buffer;

	//Get the minimal data required
	int res = recv(newClient, dataRecv, minBytes, 0);
	msg.receivalTime = std::time(0);
	if (res == INVALID_SOCKET)
	{
		std::string s = "Error while recieving from socket: ";
		s += std::to_string(newClient);
		return INVALID_SOCKET;
	}

	buffer.push_back((BYTE)dataRecv[0]);  // Insert the code
	buffer.push_back((BYTE)dataRecv[1]);  // Insert the length byte number: 1
	buffer.push_back((BYTE)dataRecv[2]);  // Insert the length byte number: 2
	buffer.push_back((BYTE)dataRecv[3]);  // Insert the length byte number: 3
	buffer.push_back((BYTE)dataRecv[4]);  // Insert the length byte number: 4
	len = Helper::lenFromBuffer(buffer);  // get the data len
	msg.id = (int)buffer[0];

	//Get the rest data
	delete[] dataRecv;
	dataRecv = new char[len + 1];
	///if(len != 0 && FirstStep)
	if(len != 0)
	{
		res = recv(newClient, dataRecv, len, 0);
		if (res == INVALID_SOCKET)
		{
			std::string s = "Error while recieving from socket: ";
			s += std::to_string(newClient);
			return INVALID_SOCKET;
		}
	}
	
	dataRecv[len] = 0;  // end of string
	Helper::fillBufferField(buffer, dataRecv);
	msg.buffer = buffer;

	delete[] dataRecv;
	return 1;
}

void Communicator::write(SOCKET newClient, Buffer& buffer)
{
	size_t length = Helper::lenFromBuffer(buffer) + DATA_START;
	char* data = new char[length];
	//Put from buffer to char array to send it
	for (int i = 0; i < length; i++)
	{
		data[i] = buffer[i];
	}
	if (send(newClient, data, length, 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}

	delete[] data;
}
