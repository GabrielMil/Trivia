using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace Client.Classes
{

    public class Communicator
    {
        public static string ipAddr = "127.0.0.1";
        public static int port = 1227;

        private Mutex mWrite;
        private Mutex mRead;
        private TcpClient client;
        private IPEndPoint serverEndPoint;
        private NetworkStream clientStream;
        private string username;

        public string Username { get => username; set => username = value; }

        public Communicator()
        {
            this.mWrite = new Mutex();
            this.mRead = new Mutex();
            this.username = "";
            this.client = new TcpClient();
            this.serverEndPoint = new IPEndPoint(IPAddress.Parse(ipAddr), port);
            //Connect to the server
            this.client.Connect(this.serverEndPoint);
            this.clientStream = this.client.GetStream();
        }

        public void Write(byte[] buffer)
        {
            /*//DEBUG:
            string s = "";
            foreach (byte item in buffer)
            {
                if (item == 0) s += "NULL";
                else s += (char)item;
            }
            MessageBox.Show(s, "WRITE DEBUG");*/

            try
            {
                mWrite.WaitOne();
                this.clientStream.Write(buffer, 0, buffer.Length);
                this.clientStream.Flush();
                mWrite.ReleaseMutex();
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Error Detected While Sending Data.");
                Application.Current.Shutdown();
                return;
            }
        }

        public Responses Read()
        {
            try
            {
                mRead.WaitOne();
                byte[] buffer = null;
                int code = 0;
                uint len = 0;
                string data = "";
                //Get the message code
                buffer = new byte[1];
                this.clientStream.Read(buffer, 0, 1);
                code = (int)buffer[0];
                //Get the message length
                buffer = new byte[4];
                this.clientStream.Read(buffer, 0, 4);
                len = PacketHelper.ByteToLen(buffer);
                //Get the message itself
                buffer = new byte[len];
                this.clientStream.Read(buffer, 0, (int)len);
                data = Encoding.ASCII.GetString(buffer);
                mRead.ReleaseMutex();

                //MessageBox.Show(code.ToString() + "\n" + len.ToString() + "\n" + data, "READ DEBUG");
                
                return new Responses(code, len, data);
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Error Detected While Recieving Data.");
                Application.Current.Shutdown();
                return null;
            }
        }
    }
}
