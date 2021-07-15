using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Client.Classes
{
    public enum PacketCodes
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

        AddQuestionCode=26
    };

    public class PacketHelper
    {
        public static byte[] LenToByte(uint len)
        {
            byte[] byteArr = BitConverter.GetBytes(len);
            Array.Reverse(byteArr);
            return byteArr;
        }

        public static uint ByteToLen(byte[] byteArr)
        {
            Array.Reverse(byteArr);
            uint len = BitConverter.ToUInt32(byteArr, 0);
            return len;
        }

        public static byte[] CreatePacket(byte code, string message)
        {
            //Calculate the total length(1 for code, 4 for length)
            uint bufferLen = 1 + 4 + (uint)message.Length;
            byte[] len = PacketHelper.LenToByte((uint)message.Length);
            byte[] buffer = new byte[bufferLen];
            byte[] messageArr = new ASCIIEncoding().GetBytes(message);
            //Set the code
            buffer[0] = code;
            //Set the length
            len.CopyTo(buffer, 1);
            //Set the message
            messageArr.CopyTo(buffer, 5);

            //DEBUG
            /*string s = "code:" + (int)code + "\nlen:" + message.Length + "\n";
            for (int i = 5; i < buffer.Length; i++)
            {
                s += (char)buffer[i];
            }
            MessageBox.Show(s);*/
            return buffer;
        }
    }
}
