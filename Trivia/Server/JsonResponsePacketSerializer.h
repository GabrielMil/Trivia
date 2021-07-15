#pragma once
#include "Helper.h"
#include "Room.h"
#include <string>
#include <vector>
#include <map>

typedef struct LoginResponse
{
	unsigned int status;
}LoginResponse;

typedef struct SignupResponse
{
	unsigned int status;
}SignupResponse;

typedef struct ErrorResponse
{
	std::string message;
}ErrorResponse;

typedef struct LogoutResponse
{
	unsigned int status;
}LogoutResponse;

typedef struct GetRoomsResponse
{
	unsigned int status;
	std::vector<RoomData> rooms;
}GetRoomsResponse;

typedef struct GetPlayersInRoomResponse
{
	unsigned int status;
	std::vector<std::string> players;
}GetPlayersInRoomResponse;

typedef struct GetHighScoreResponse
{
	unsigned int status;
	std::vector<std::string> statistics;
}GetHighScoreResponse;

typedef struct GetPersonalStatusResponse
{
	unsigned int status;
	std::vector<std::string> statistics;
}GetPersonalStatusResponse;

typedef struct JoinRoomResponse
{
	unsigned int status;
}JoinRoomResponse;

typedef struct CreateRoomResponse
{
	unsigned int status;
}CreateRoomResponse;

typedef struct CloseRoomResponse
{
	unsigned int status;
}CloseRoomResponse;

typedef struct LeaveRoomResponse
{
	unsigned int status;
}LeaveRoomResponse;

typedef struct StartGameResponse
{
	unsigned int status;
}StartGameResponse;

typedef struct GetRoomStateResponse
{
	unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeOut;
}GetRoomStateResponse;

typedef struct LeaveGameResponse
{
	unsigned int status;
}LeaveGameResponse;

typedef struct GetQuestionResponse
{
	unsigned int status;
	std::string question;
	std::map<unsigned int, std::string> answers;
}GetQuestionResponse;

typedef struct SubmitAnswerResponse
{
	unsigned int status;
	unsigned int correctAnswerId;
}SubmitAnswerResponse;

typedef struct PlayerResults
{
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
}PlayerResults;

typedef struct GetGameResultsResponse
{
	unsigned int status;
	std::vector<PlayerResults> results;
}GetGameResultsResponse;

typedef struct AddQuestionResponse
{
	unsigned int status;
}AddQuestionResponse;

class JsonResponsePacketSerializer
{
public:
	static Buffer serializeResponse(ErrorResponse res);
	static Buffer serializeResponse(LoginResponse res);
	static Buffer serializeResponse(SignupResponse res);

	static Buffer serializeResponse(LogoutResponse res);
	static Buffer serializeResponse(GetRoomsResponse res);
	static Buffer serializeResponse(GetPlayersInRoomResponse res);
	static Buffer serializeResponse(JoinRoomResponse res);
	static Buffer serializeResponse(CreateRoomResponse res);
	static Buffer serializeResponse(GetHighScoreResponse res);
	static Buffer serializeResponse(GetPersonalStatusResponse res);

	static Buffer serializeResponse(CloseRoomResponse res);
	static Buffer serializeResponse(LeaveRoomResponse res);
	static Buffer serializeResponse(StartGameResponse res);
	static Buffer serializeResponse(GetRoomStateResponse res);

	static Buffer serializeResponse(GetGameResultsResponse res);
	static Buffer serializeResponse(SubmitAnswerResponse res);
	static Buffer serializeResponse(GetQuestionResponse res);
	static Buffer serializeResponse(LeaveGameResponse res);

	static Buffer serializeResponse(AddQuestionResponse res);
};