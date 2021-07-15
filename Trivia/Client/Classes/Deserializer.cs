using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace Client.Classes
{
    public class Deserializer
    {
        public static LoginResponse DeserializeLoginResponce(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<LoginResponse>(response.Data);
        }

        public static SignupResponse DeserializeSignupResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<SignupResponse>(response.Data);
        }

        public static ErrorResponse DeserializeErrorResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<ErrorResponse>(response.Data);
        }

        public static LogoutResponse DeserializeLogoutResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<LogoutResponse>(response.Data);
        }

        public static GetRoomsResponse DeserializeGetRoomsResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<GetRoomsResponse>(response.Data);
        }

        public static GetPlayersInRoomResponse DeserializeGetPlayersInRoomResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            object a = JsonConvert.DeserializeObject(response.Data);
            return JsonConvert.DeserializeObject<GetPlayersInRoomResponse>(response.Data);
        }

        public static JoinRoomResponse DeserializeJoinRoomResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<JoinRoomResponse>(response.Data);
        }

        public static CreateRoomResponse DeserializeCreateRoomResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<CreateRoomResponse>(response.Data);
        }

        public static GetHighScoreResponse DeserializeGetHighScoreResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<GetHighScoreResponse>(response.Data);
        }

        public static GetPersonalStatusResponse DeserializeGetPersonalStatusResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<GetPersonalStatusResponse>(response.Data);
        }

        public static CloseRoomResponse DeserializeCloseRoomResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<CloseRoomResponse>(response.Data);
        }

        public static LeaveRoomResponse DeserializeLeaveRoomResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<LeaveRoomResponse>(response.Data);
        }

        public static StartGameResponse DeserializeStartGameResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<StartGameResponse>(response.Data);
        }

        public static GetRoomStateResponse DeserializeGetRoomStateResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<GetRoomStateResponse>(response.Data);
        }

        public static SubmitAnswerResponse DeserializeSubmitAnswerResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<SubmitAnswerResponse>(response.Data);
        }

        public static GetQuestionResponse DeserializeGetQuestionResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);

            int status = (int)json["status"];
            string question = (string)json["question"];
            Dictionary<int, string> answers = JsonConvert.DeserializeObject<Dictionary<int, string>>(json["answers"].ToString());
            //Create the response and return it
            return new GetQuestionResponse(status, question, answers);
            //return JsonConvert.DeserializeObject<GetQuestionResponse>(response.Data);
        }

        public static GetGameResultsResponse DeserializeGetGameResultsResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<GetGameResultsResponse>(response.Data);
        }

        public static LeaveGameResponse DeserializeLeaveGameResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<LeaveGameResponse>(response.Data);
        }

        public static AddQuestionResponse DeserializeAddQuestionResponse(Responses response)
        {
            //Parse to json from string
            JObject json = JObject.Parse(response.Data);
            //Create the response and return it
            return JsonConvert.DeserializeObject<AddQuestionResponse>(response.Data);
        }
    }
}
