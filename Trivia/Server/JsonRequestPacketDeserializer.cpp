#include "JsonRequestPacketDeserializer.h"

void JsonRequestPacketDeserializer::deserializeLoginRequest(LoginRequest& login, Buffer& buffer)
{
	json jsonObj = json::parse(Helper::bufferToJson(buffer));  // Get the data from the buffer and make as json
	login.password = jsonObj["password"];
	login.username = jsonObj["username"];
}

void JsonRequestPacketDeserializer::deserializeSignupRequest(SignupRequest& singup, Buffer& buffer)
{
	json jsonObj = json::parse(Helper::bufferToJson(buffer));  // Get the data from the buffer and make as json
	singup.password = jsonObj["password"];
	singup.username = jsonObj["username"];
	singup.email = jsonObj["email"];
}

void JsonRequestPacketDeserializer::deserializeGetPlayersRequest(GetPlayersInRoomRequest& getPlayers, Buffer& buffer)
{
	json jsonObj = json::parse(Helper::bufferToJson(buffer));  // Get the data from the buffer and make as json
	getPlayers.roomId = jsonObj["roomId"];
}

void JsonRequestPacketDeserializer::deserializeJoinRoomRequest(JoinRoomRequest& joinRoom, Buffer& buffer)
{
	json jsonObj = json::parse(Helper::bufferToJson(buffer));  // Get the data from the buffer and make as json
	joinRoom.roomId = jsonObj["roomId"];
}

void JsonRequestPacketDeserializer::deserializeCreateRoomRequest(CreateRoomRequest& createRoom, Buffer& buffer)
{
	json jsonObj = json::parse(Helper::bufferToJson(buffer));  // Get the data from the buffer and make as json
	createRoom.answerTimeout = jsonObj["answerTimeout"];
	createRoom.maxUsers = jsonObj["maxUsers"];
	createRoom.questionCount = jsonObj["questionCount"];
	createRoom.roomName = jsonObj["roomName"];
}

void JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(SubmitAnswerRequest& submitAnsReq, Buffer& buffer)
{
	json jsonObj = json::parse(Helper::bufferToJson(buffer));  // Get the data from the buffer and make as json
	submitAnsReq.asnwerId = jsonObj["answerId"];
	submitAnsReq.timeToAnswer = jsonObj["timeToAnswer"];
}

void JsonRequestPacketDeserializer::deserializeAddQuestionRequest(AddQuestionRequest& addQuestionReq, Buffer& buffer)
{
	json jsonObj = json::parse(Helper::bufferToJson(buffer));  // Get the data from the buffer and make as json
	addQuestionReq.question = jsonObj["question"];
	addQuestionReq.correctAns = jsonObj["correctAns"];
	addQuestionReq.ans2 = jsonObj["ans2"];
	addQuestionReq.ans3 = jsonObj["ans3"];
	addQuestionReq.ans4 = jsonObj["ans4"];
}
