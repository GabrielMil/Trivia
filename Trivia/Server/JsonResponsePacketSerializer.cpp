#include "JsonResponsePacketSerializer.h"

Buffer JsonResponsePacketSerializer::serializeResponse(ErrorResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["message"] = res.message;
	Helper::createBuffer(buffer, jsonObj, (BYTE)Error);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(LoginResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	Helper::createBuffer(buffer, jsonObj, (BYTE)Login);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(SignupResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	Helper::createBuffer(buffer, jsonObj, (BYTE)Signup);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(LogoutResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	Helper::createBuffer(buffer, jsonObj, (BYTE)Logout);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	for (int i = 0; i < res.rooms.size(); i++)
	{
		jsonObj["rooms"].push_back(json{
			{"id", res.rooms[i].id },
			{"isActive", res.rooms[i].isActive },
			{"maxPlayers", res.rooms[i].maxPlayers},
			{"name", res.rooms[i].name},
			{"numOfQuestionsInGame", res.rooms[i].numOfQuestionsInGame},
			{"timePerQuestion", res.rooms[i].timePerQuestion}
			});
	}
	Helper::createBuffer(buffer, jsonObj, (BYTE)GetRooms);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	for (int i = 0; i < res.players.size(); i++)
	{
		jsonObj["playersInRoom"].push_back(res.players[i]);
	}
	Helper::createBuffer(buffer, jsonObj, (BYTE)GetPlayersInRoom);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	Helper::createBuffer(buffer, jsonObj, (BYTE)JoinRoom);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	Helper::createBuffer(buffer, jsonObj, (BYTE)CreateRoom);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetHighScoreResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	for (int i = 0; i < res.statistics.size(); i++)
	{
		jsonObj["statistics"].push_back(res.statistics[i]);
	}
	Helper::createBuffer(buffer, jsonObj, (BYTE)GetHighScore);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetPersonalStatusResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	for (int i = 0; i < res.statistics.size(); i++)
	{
		jsonObj["statistics"].push_back(res.statistics[i]);
	}
	Helper::createBuffer(buffer, jsonObj, (BYTE)GetPersonalStatus);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	Helper::createBuffer(buffer, jsonObj, (BYTE)PacketCodes::CloseRoom);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	Helper::createBuffer(buffer, jsonObj, (BYTE)PacketCodes::LeaveRoomRequest);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(StartGameResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	Helper::createBuffer(buffer, jsonObj, (BYTE)PacketCodes::StartGame);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	jsonObj["hasGameBegun"] = res.hasGameBegun;
	for (int i = 0; i < res.players.size(); i++)
	{
		jsonObj["players"].push_back(res.players[i]);
	}
	jsonObj["questionCount"] = res.questionCount;
	jsonObj["answerTimeOut"] = res.answerTimeOut;
	Helper::createBuffer(buffer, jsonObj, (BYTE)PacketCodes::GetRoomStateRequest);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetGameResultsResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	for (int i = 0; i < res.results.size(); i++)
	{
		json temp;
		temp["username"] = res.results[i].username;
		temp["wrongAnswerCount"] = res.results[i].wrongAnswerCount;
		temp["correctAnswerCount"] = res.results[i].correctAnswerCount;
		temp["averageAnswerTime"] = res.results[i].averageAnswerTime;
		jsonObj["results"].push_back(temp);
	}
	Helper::createBuffer(buffer, jsonObj, (BYTE)PacketCodes::GetGameResults);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(SubmitAnswerResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	jsonObj["correctAnswerId"] = res.correctAnswerId;
	Helper::createBuffer(buffer, jsonObj, (BYTE)PacketCodes::SubmitAnswer);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetQuestionResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	jsonObj["question"] = res.question;
	//Create something like this for the cpp code
	//{"1":"A","2":"B","3":"C","4":"D"}
	std::string ansStr = "{";
	for (auto& ans : res.answers)
	{
		ansStr += "\"" + std::to_string(ans.first) + "\":\"" + ans.second + "\",";
	}
	//Remove the last ','
	ansStr = ansStr.substr(0, ansStr.size() - 1);
	ansStr += "}";
	jsonObj["answers"] = ansStr;
	Helper::createBuffer(buffer, jsonObj, (BYTE)PacketCodes::GetQuestion);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(LeaveGameResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	Helper::createBuffer(buffer, jsonObj, (BYTE)PacketCodes::LeaveGame);
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(AddQuestionResponse res)
{
	Buffer buffer;
	json jsonObj;
	jsonObj["status"] = res.status;
	Helper::createBuffer(buffer, jsonObj, (BYTE)PacketCodes::AddQuestionCode);
	return buffer;
}
