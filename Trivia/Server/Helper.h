#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "json.hpp"
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <chrono>

#define DATA_START 5
#define BITS_ARRAY 32
#define BYTE_LEN 8
#define LOGIN_ID 1
#define MENU_ID 2
#define ROOM_ADMIN_ID 3
#define ROOM_MEMBER_ID 4
#define GAME_ID 5
using json = nlohmann::json;

typedef unsigned char BYTE;
typedef std::vector<BYTE> Buffer;

enum PacketCodes
{
	Login = 1,
	Signup = 2,
	Error = 99,

	Logout = 11,
	GetRooms = 12,
	GetPlayersInRoom = 13,
	JoinRoom = 14,
	CreateRoom = 15,
	GetHighScore = 16,
	GetPersonalStatus = 17,
	CloseRoom = 18,
	StartGame = 19,
	GetRoomStateRequest = 20,
	LeaveRoomRequest = 21,
	LeaveGame = 22,
	GetQuestion = 23,
	SubmitAnswer = 24,
	GetGameResults = 25,

	AddQuestionCode = 26
};

class Helper
{
public:
	//Serializer
	static void createBuffer(Buffer& buffer, json& jsonObj, BYTE code);
	static void fillBufferField(Buffer& bf, std::string str);
	static std::string lenPaddZero(int len);

	//Deserializer
	static void arrToVector(Buffer& bf, char* toCopy);
	static std::string bufferToJson(Buffer& buffer);
	static size_t lenFromBuffer(Buffer& buffer);

	//Byte convertor
	static std::string makeBinary32(size_t num);
	static BYTE binToDec8(std::string byte);
	static size_t binToDec32(std::string byte);
	static std::string makeBinary8(BYTE num);

	// Codes
	static bool isCodeInEnum(int code);

};