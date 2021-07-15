using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Newtonsoft.Json.Linq;

namespace Client.Classes
{
    public class Serializer
    {
        public static byte[] SerializeLoginRequest(LoginRequest loginRequest)
        {
            //Set the data
            JObject obj = new JObject();
            obj.Add("username", loginRequest.Username);
            obj.Add("password", loginRequest.Password);
            //Make the buffer with helping function
            byte[] buffer = PacketHelper.CreatePacket((byte)PacketCodes.Login, obj.ToString());
            return buffer;
        }
        public static byte[] SerializeSignupRequest(SignupRequest signupRequest)
        {
            //Set the data
            JObject obj = new JObject();
            obj.Add("username", signupRequest.Username);
            obj.Add("password", signupRequest.Password);
            obj.Add("email", signupRequest.Email);
            //Make the buffer with helping function
            byte[] buffer = PacketHelper.CreatePacket((byte)PacketCodes.Signup, obj.ToString());
            return buffer;
        }

        public static byte[] SerializeGetPlayersInRoomRequest(GetPlayersInRoomRequest getPlayersInRoomRequest)
        {
            //Set the data
            JObject obj = new JObject();
            obj.Add("roomId", getPlayersInRoomRequest.RoomId);
            //Make the buffer with helping function
            byte[] buffer = PacketHelper.CreatePacket((byte)PacketCodes.GetPlayersInRoom, obj.ToString());
            return buffer;
        }

        public static byte[] SerializeJoinRoomRequest(JoinRoomRequest joinRoomRequest)
        {
            //Set the data
            JObject obj = new JObject();
            obj.Add("roomId", joinRoomRequest.RoomId);
            //Make the buffer with helping function
            byte[] buffer = PacketHelper.CreatePacket((byte)PacketCodes.JoinRoom, obj.ToString());
            return buffer;
        }

        public static byte[] SerializeCreateRoomRequest(CreateRoomRequest createRoomRequest)
        {
            //Set the data
            JObject obj = new JObject();
            obj.Add("answerTimeout", createRoomRequest.AnswerTimeout);
            obj.Add("maxUsers", createRoomRequest.MaxUsers);
            obj.Add("questionCount", createRoomRequest.QuestionCount);
            obj.Add("roomName", createRoomRequest.RoomName);
            //Make the buffer with helping function
            byte[] buffer = PacketHelper.CreatePacket((byte)PacketCodes.CreateRoom, obj.ToString());
            return buffer;
        }

        public static byte[] SerializeSubmitAnswerRequest(SubmitAnswerRequest submitAnswerRequest)
        {
            //Set the data
            JObject obj = new JObject();
            obj.Add("answerId", submitAnswerRequest.AsnwerId);
            obj.Add("timeToAnswer", submitAnswerRequest.TimeToAnswer);
            //Make the buffer with helping function
            byte[] buffer = PacketHelper.CreatePacket((byte)PacketCodes.SubmitAnswer, obj.ToString());
            return buffer;
        }

        public static byte[] SerializeAddQuestionRequest(AddQuestionRequest addQuestionRequest)
        {
            //Set the data
            JObject obj = new JObject();
            obj.Add("question", addQuestionRequest.Question);
            obj.Add("correctAns", addQuestionRequest.CorrectAns);
            obj.Add("ans2", addQuestionRequest.Ans2);
            obj.Add("ans3", addQuestionRequest.Ans3);
            obj.Add("ans4", addQuestionRequest.Ans4);
            //Make the buffer with helping function
            byte[] buffer = PacketHelper.CreatePacket((byte)PacketCodes.AddQuestionCode, obj.ToString());
            return buffer;
        }
    }
}
