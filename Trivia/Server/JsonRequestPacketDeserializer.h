#pragma once
#include <iostream>
#include <vector>
#include "Helper.h"

#define CODE_LEN 4

typedef struct LoginRequest {
	std::string username;
	std::string password;
}LoginRequest;

typedef struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
}SignupRequest;

typedef struct GetPlayersInRoomRequest
{
	size_t roomId;
}GetPlayersInRoomRequest;

typedef struct JoinRoomRequest
{
	size_t roomId;
}JoinRoomRequest;

typedef struct CreateRoomRequest
{
	std::string roomName;
	size_t maxUsers;
	size_t questionCount;
	size_t answerTimeout;
}CreateRoomRequest;

typedef struct SubmitAnswerRequest
{
	unsigned int asnwerId;
	unsigned int timeToAnswer;
}SubmitAnswerRequest;

typedef struct AddQuestionRequest
{
	std::string question;
	std::string correctAns;
	std::string ans2;
	std::string ans3;
	std::string ans4;
}AddQuestionRequest;

class JsonRequestPacketDeserializer
{

public:
	static void deserializeLoginRequest(LoginRequest& login, Buffer& buffer);
	static void deserializeSignupRequest(SignupRequest& singup, Buffer& buffer);

	static void deserializeGetPlayersRequest(GetPlayersInRoomRequest& getPlayers, Buffer& buffer);
	static void deserializeJoinRoomRequest(JoinRoomRequest& joinRoom, Buffer& buffer);
	static void deserializeCreateRoomRequest(CreateRoomRequest& createRoom, Buffer& buffer);

	static void deserializeSubmitAnswerRequest(SubmitAnswerRequest& submitAnsReq, Buffer& buffer);

	static void deserializeAddQuestionRequest(AddQuestionRequest& addQuestionReq, Buffer& buffer);
};