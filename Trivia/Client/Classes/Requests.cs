using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Classes
{
    public class Requests
    {
    }

    public class LoginRequest
    {
		private string username;
		private string password;

        public LoginRequest(string username, string password)
        {
            this.username = username;
            this.password = password;
        }

        public string Username { get => username; set => username = value; }
        public string Password { get => password; set => password = value; }
    }

    public class SignupRequest
    {
		private string username;
		private string password;
		private string email;

        public SignupRequest(string username, string password, string email)
        {
            this.username = username;
            this.password = password;
            this.email = email;
        }

        public string Username { get => username; set => username = value; }
        public string Password { get => password; set => password = value; }
        public string Email { get => email; set => email = value; }
    }
    public class GetPlayersInRoomRequest
    {
		private int roomId;

        public GetPlayersInRoomRequest(int roomId)
        {
            this.roomId = roomId;
        }

        public int RoomId { get => roomId; set => roomId = value; }
    }

    public class JoinRoomRequest
    {
		private int roomId;

        public JoinRoomRequest(int roomId)
        {
            this.roomId = roomId;
        }

        public int RoomId { get => roomId; set => roomId = value; }
    }

    public class CreateRoomRequest
    {
        private string roomName;
        private int maxUsers;
        private int questionCount;
        private int answerTimeout;

        public CreateRoomRequest(string roomName, int maxUsers, int questionCount, int answerTimeout)
        {
            this.roomName = roomName;
            this.maxUsers = maxUsers;
            this.questionCount = questionCount;
            this.answerTimeout = answerTimeout;
        }

        public string RoomName { get => roomName; set => roomName = value; }
        public int MaxUsers { get => maxUsers; set => maxUsers = value; }
        public int QuestionCount { get => questionCount; set => questionCount = value; }
        public int AnswerTimeout { get => answerTimeout; set => answerTimeout = value; }
    }

    public class SubmitAnswerRequest
    {
        private int asnwerId;
        private int timeToAnswer;

        public SubmitAnswerRequest(int asnwerId, int timeToAnswer)
        {
            this.asnwerId = asnwerId;
            this.timeToAnswer = timeToAnswer;
        }

        public int AsnwerId { get => asnwerId; set => asnwerId = value; }
        public int TimeToAnswer { get => timeToAnswer; set => timeToAnswer = value; }
    }

    public class AddQuestionRequest
    {
        private string question;
        private string correctAns;
        private string ans2;
        private string ans3;
        private string ans4;

        public AddQuestionRequest(string question, string correctAns, string ans2, string ans3, string ans4)
        {
            this.question = question;
            this.correctAns = correctAns;
            this.ans2 = ans2;
            this.ans3 = ans3;
            this.ans4 = ans4;
        }

        public string Question { get => question; set => question = value; }
        public string CorrectAns { get => correctAns; set => correctAns = value; }
        public string Ans2 { get => ans2; set => ans2 = value; }
        public string Ans3 { get => ans3; set => ans3 = value; }
        public string Ans4 { get => ans4; set => ans4 = value; }
    }
}
