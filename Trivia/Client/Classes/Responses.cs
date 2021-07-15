using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Classes
{
    public class Responses
    {
        private int code;
        private uint length;
        private string data;

        public Responses(int code, uint length, string data)
        {
            this.code = code;
            this.length = length;
            this.data = data;
        }

        public int Code { get => code; set => code = value; }
        public uint Length { get => length; set => length = value; }
        public string Data { get => data; set => data = value; }

    }

    public class Response
    {

        //This function returns the correct Response
        public static Response GetResponse(Responses res)
        {
            Response response = null;
            if (res == null)
            {
                return null;
            }

            switch ((PacketCodes)res.Code)
            {
                case PacketCodes.Login:
                    response = Deserializer.DeserializeLoginResponce(res);
                    break;
                case PacketCodes.Signup:
                    response = Deserializer.DeserializeSignupResponse(res);
                    break;
                case PacketCodes.Error:
                    response = Deserializer.DeserializeErrorResponse(res);
                    break;
                case PacketCodes.Logout:
                    response = Deserializer.DeserializeLogoutResponse(res);
                    break;
                case PacketCodes.GetRooms:
                    response = Deserializer.DeserializeGetRoomsResponse(res);
                    break;
                case PacketCodes.GetPlayersInRoom:
                    response = Deserializer.DeserializeGetPlayersInRoomResponse(res);
                    break;
                case PacketCodes.JoinRoom:
                    response = Deserializer.DeserializeJoinRoomResponse(res);
                    break;
                case PacketCodes.CreateRoom:
                    response = Deserializer.DeserializeCreateRoomResponse(res);
                    break;
                case PacketCodes.GetHighScore:
                    response = Deserializer.DeserializeGetHighScoreResponse(res);
                    break;
                case PacketCodes.GetPersonalStatus:
                    response = Deserializer.DeserializeGetPersonalStatusResponse(res);
                    break;
                case PacketCodes.CloseRoom:
                    response = Deserializer.DeserializeCloseRoomResponse(res);
                    break;
                case PacketCodes.StartGame:
                    response = Deserializer.DeserializeStartGameResponse(res);
                    break;
                case PacketCodes.GetRoomStateRequest:
                    response = Deserializer.DeserializeGetRoomStateResponse(res);
                    break;
                case PacketCodes.LeaveRoomRequest:
                    response = Deserializer.DeserializeLeaveRoomResponse(res);
                    break;
                case PacketCodes.LeaveGame:
                    response = Deserializer.DeserializeLeaveGameResponse(res);
                    break;
                case PacketCodes.GetQuestion:
                    response = Deserializer.DeserializeGetQuestionResponse(res);
                    break;
                case PacketCodes.SubmitAnswer:
                    response = Deserializer.DeserializeSubmitAnswerResponse(res);
                    break;
                case PacketCodes.GetGameResults:
                    response = Deserializer.DeserializeGetGameResultsResponse(res);
                    break;
                case PacketCodes.AddQuestionCode:
                    response = Deserializer.DeserializeAddQuestionResponse(res);
                    break;
                default:
                    response = null;
                    break;
            }
            return response;
        }
    }

    public class LoginResponse : Response
    {
        private int status;

        public LoginResponse(int status)
        {
            this.status = status;
        }

        public int Status { get => status; set => status = value; }
    }
    public class SignupResponse : Response
    {
        private int status;

        public SignupResponse(int status)
        {
            this.status = status;
        }

        public int Status { get => status; set => status = value; }
    }

    public class ErrorResponse : Response
    {
        private string message;

        public ErrorResponse(string message)
        {
            this.message = message;
        }

        public string Message { get => message; set => message = value; }
    }

    public class LogoutResponse : Response
    {
        private int status;

        public LogoutResponse(int status)
        {
            this.status = status;
        }

        public int Status { get => status; set => status = value; }
    }

    public class GetRoomsResponse : Response
    {
        private int status;
        private RoomData[] rooms;

        public GetRoomsResponse(int status, RoomData[] rooms)
        {
            this.status = status;
            this.rooms = rooms;
        }

        public int Status { get => status; set => status = value; }
        public RoomData[] Rooms { get => rooms; set => rooms = value; }
    }

    public class RoomData : Response
    {
        private int id;
        private bool isActive;
        private int maxPlayers;
        private string name;
        private int numOfQuestionsInGame;
        private int timePerQuestion;

        public int Id { get => id; set => id = value; }
        public bool IsActive { get => isActive; set => isActive = value; }
        public int MaxPlayers { get => maxPlayers; set => maxPlayers = value; }
        public string Name { get => name; set => name = value; }
        public int NumOfQuestionsInGame { get => numOfQuestionsInGame; set => numOfQuestionsInGame = value; }
        public int TimePerQuestion { get => timePerQuestion; set => timePerQuestion = value; }
    }

    public class GetPlayersInRoomResponse : Response
    {
        private int status;
        private string[] playersInRoom;

        public GetPlayersInRoomResponse(int status, string[] playersInRoom)
        {
            this.status = status;
            this.playersInRoom = playersInRoom;
        }

        public int Status { get => status; set => status = value; }
        public string[] Players { get => playersInRoom; set => playersInRoom = value; }
    }
    public class GetHighScoreResponse : Response
    {
        private int status;
        private string[] statistics;

        public GetHighScoreResponse(int status, string[] statistics)
        {
            this.status = status;
            this.statistics = statistics;
        }

        public int Status { get => status; set => status = value; }
        public string[] Statistics { get => statistics; set => statistics = value; }
    }

    public class GetPersonalStatusResponse : Response
    {
        private int status;
        private string[] statistics;

        public GetPersonalStatusResponse(int status, string[] statistics)
        {
            this.status = status;
            this.statistics = statistics;
        }

        public int Status { get => status; set => status = value; }
        public string[] Statistics { get => statistics; set => statistics = value; }
    }

    public class JoinRoomResponse : Response
    {
        private int status;

        public JoinRoomResponse(int status)
        {
            this.status = status;
        }

        public int Status { get => status; set => status = value; }
    }

    public class CreateRoomResponse : Response
    {
        private int status;

        public CreateRoomResponse(int status)
        {
            this.status = status;
        }

        public int Status { get => status; set => status = value; }
    }

    public class CloseRoomResponse : Response
    {
        private int status;

        public CloseRoomResponse(int status)
        {
            this.status = status;
        }

        public int Status { get => status; set => status = value; }
    }

    public class LeaveRoomResponse : Response
    {
        private int status;

        public LeaveRoomResponse(int status)
        {
            this.status = status;
        }

        public int Status { get => status; set => status = value; }
    }

    public class StartGameResponse : Response
    {
        private int status;

        public StartGameResponse(int status)
        {
            this.status = status;
        }

        public int Status { get => status; set => status = value; }
    }

    public class GetRoomStateResponse : Response
    {
        private int status;
        private bool hasGameBegun;
        private string[] players;
        private int questionCount;
        private int answerTimeOut;

        public GetRoomStateResponse(int status, bool hasGameBegun, string[] players, int questionCount, int answerTimeOut)
        {
            this.status = status;
            this.hasGameBegun = hasGameBegun;
            this.players = players;
            this.questionCount = questionCount;
            this.answerTimeOut = answerTimeOut;
        }

        public int Status { get => status; set => status = value; }
        public bool HasGameBegun { get => hasGameBegun; set => hasGameBegun = value; }
        public string[] Players { get => players; set => players = value; }
        public int QuestionCount { get => questionCount; set => questionCount = value; }
        public int AnswerTimeOut { get => answerTimeOut; set => answerTimeOut = value; }
    }

    public class GetGameResultsResponse : Response
    {
        private int status;
        private GameResult[] results;

        public GetGameResultsResponse(int status, GameResult[] results)
        {
            this.status = status;
            this.results = results;
        }

        public int Status { get => status; set => status = value; }
        public GameResult[] Results { get => results; set => results = value; }
    }

    public class GameResult
    {
        private string username;
        private int wrongAnswerCount;
        private int correctAnswerCount;
        private int averageAnswerTime;

        public GameResult(string username, int wrongAnswerCount, int correctAnswerCount, int averageAnswerTime)
        {
            this.username = username;
            this.wrongAnswerCount = wrongAnswerCount;
            this.correctAnswerCount = correctAnswerCount;
            this.averageAnswerTime = averageAnswerTime;
        }

        public string Username { get => username; set => username = value; }
        public int WrongAnswerCount { get => wrongAnswerCount; set => wrongAnswerCount = value; }
        public int CorrectAnswerCount { get => correctAnswerCount; set => correctAnswerCount = value; }
        public int AverageAnswerTime { get => averageAnswerTime; set => averageAnswerTime = value; }

        public override string ToString()
        {
            return "Username: " + this.username + "\n" +
                "Wrong: " + this.wrongAnswerCount + "\n" +
                "Correct: " + this.correctAnswerCount + "\n" +
                "Avg Time: " + this.averageAnswerTime;
        }
    }

    public class SubmitAnswerResponse : Response
    {
        private int status;
        private int correctAnswerId;

        public SubmitAnswerResponse(int status, int correctAnswerId)
        {
            this.status = status;
            this.correctAnswerId = correctAnswerId;
        }

        public int Status { get => status; set => status = value; }
        public int CorrectAnswerId { get => correctAnswerId; set => correctAnswerId = value; }
    }

    public class GetQuestionResponse : Response
    {
        private int status;
        private string question;
        private Dictionary<int, string> answers;

        public GetQuestionResponse(int status, string question, Dictionary<int, string> answers)
        {
            this.status = status;
            this.question = question;
            this.answers = answers;
        }

        public int Status { get => status; set => status = value; }
        public string Question { get => question; set => question = value; }
        public Dictionary<int, string> Answers { get => answers; set => answers = value; }
    }

    public class LeaveGameResponse : Response
    {
        private int status;

        public LeaveGameResponse(int status)
        {
            this.status = status;
        }

        public int Status { get => status; set => status = value; }
    }

    public class AddQuestionResponse : Response
    {
        private int status;

        public AddQuestionResponse(int status)
        {
            this.status = status;
        }

        public int Status { get => status; set => status = value; }
    }
}
