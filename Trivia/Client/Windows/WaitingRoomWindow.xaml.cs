using Client.Classes;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Client.Windows
{
    /// <summary>
    /// Interaction logic for WaitingRoomWindow.xaml
    /// </summary>
    public partial class WaitingRoomWindow : Window
    {
        private Communicator communicator;
        private bool isAdmin;
        private BackgroundWorker playersWorker;
        private BackgroundWorker listenerWorker;
        private Response currentResponse;
        private string roomName;

        private int timeForQuestions;
        private int totalQuestions;

        private bool isFinished;
        

        public WaitingRoomWindow(Communicator communicator, bool isAdmin, string roomName, int maxPlayers = 0)
        {
            InitializeComponent();
            this.isAdmin = isAdmin;
            this.roomName = roomName;
            this.communicator = communicator;
            this.currentResponse = null;

            this.isFinished = false;

            this.listenerWorker = new BackgroundWorker();
            this.listenerWorker.DoWork += ListenerWorker_DoWork;
            this.listenerWorker.ProgressChanged += ListenerWorker_ProgressChanged;
            this.listenerWorker.WorkerReportsProgress = true;
            this.listenerWorker.WorkerSupportsCancellation = true;

            this.playersWorker = new BackgroundWorker();
            this.playersWorker.DoWork += PlayersWorker_DoWork;
            this.playersWorker.WorkerSupportsCancellation = true;

            this.listenerWorker.RunWorkerAsync();
            this.playersWorker.RunWorkerAsync();

            txtRoomName.Text += roomName;
            txtRoomNameInStack.Text += roomName;
            if(this.isAdmin)
            {
                txtMaxPlayers.Text = "Max Number of Players: " + maxPlayers + " ";
                gridMain.Children.Remove(btnLeaveRoom);
            }
            else
            {
                gridMain.Children.Remove(btnCloseRoom);
                gridMain.Children.Remove(btnStartGame);
            }

        }

        private void ListenerWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            switch (e.ProgressPercentage)
            {
                case (int)PacketCodes.StartGame:
                    GotStartGame();
                    break;
                case (int)PacketCodes.LeaveRoomRequest:
                    GotLeaveRoom();
                    break;
                case (int)PacketCodes.CloseRoom:
                    GotCloseRoom();
                    break;
                case (int)PacketCodes.GetRoomStateRequest:
                    GotRoomState();
                    break;
                case (int)PacketCodes.Error:
                    GotError();
                    break;
                default:
                    break;
            }
        }

        private void ListenerWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            while(!this.isFinished)
            {
                Responses answer = this.communicator.Read();  // Keep getting the answers
                this.currentResponse = Response.GetResponse(answer);
                if(this.currentResponse == null)
                {
                    MessageBox.Show("Error happened while getting answer.");
                    return;
                }
                //Notify that got message and the percentage are the code of the packets
                this.listenerWorker.ReportProgress(answer.Code);
                //The only code that doesn't change window is the roomstate
                if(answer.Code != (int)PacketCodes.GetRoomStateRequest)
                {
                    this.isFinished = true;
                }
            }
            e.Cancel = true;
        }

        private void PlayersWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            while(!this.isFinished)
            {
                this.communicator.Write(PacketHelper.CreatePacket((byte)PacketCodes.GetRoomStateRequest, ""));
                Thread.Sleep(1000);  // Wait  1 second
            }
            e.Cancel = true;
        }

        public void SetCommunicator(Communicator communicator)
        {
            this.communicator = communicator;
            txtUsername.Text = this.communicator.Username;
        }

        private void btnCloseRoom_Click(object sender, RoutedEventArgs e)
        {
            this.communicator.Write(PacketHelper.CreatePacket((byte)PacketCodes.CloseRoom, ""));
        }

        private void btnStartGame_Click(object sender, RoutedEventArgs e)
        {
            this.communicator.Write(PacketHelper.CreatePacket((byte)PacketCodes.StartGame, ""));
        }

        private void btnLeaveRoom_Click(object sender, RoutedEventArgs e)
        {
            this.communicator.Write(PacketHelper.CreatePacket((byte)PacketCodes.LeaveRoomRequest, ""));
        }

        private void GotRoomState()
        {
            GetRoomStateResponse response = (GetRoomStateResponse)this.currentResponse;
            if(response.Status == 0)
            {
                MessageBox.Show("Can't get room status.");
                return;
            }
            updatePlayers(response.Players);
            this.totalQuestions = response.QuestionCount;
            this.timeForQuestions = response.AnswerTimeOut;
            txtQuestionCount.Text = "Number of Questions: " + response.QuestionCount.ToString();
            txtTimePerQuestion.Text = "Time per Question: " + response.AnswerTimeOut.ToString();
        }
        private void GotStartGame()
        {
            StartGameResponse response = (StartGameResponse)this.currentResponse;
            if (response.Status == 0)
            {
                MessageBox.Show("Can't start game. Try again.");
                return;
            }
            MoveToTheNext();
        }
        private void GotLeaveRoom()
        {
            LeaveRoomResponse response = (LeaveRoomResponse)this.currentResponse;
            if (response.Status == 0)
            {
                MessageBox.Show("Can't leave room. Try again.");
                return;
            }
            BackToMenu();
        }
        private void GotCloseRoom()
        {
            CloseRoomResponse response = (CloseRoomResponse)this.currentResponse;
            if(response.Status == 0)
            {
                MessageBox.Show("Can't close room. Try again.");
                return;
            }
            BackToMenu();
        }
        private void GotError()
        {
            ErrorResponse response = (ErrorResponse)this.currentResponse;
            MessageBox.Show(response.Message);
            BackToMenu();
        }

        private void MoveToTheNext()
        {
            GameWindow gameWindow = new GameWindow(this.roomName, this.communicator, this.totalQuestions, this.timeForQuestions);
            gameWindow.Show();
            this.Close();
        }

        private void BackToMenu()
        {
            MenuWindow menuWindow = new MenuWindow();
            menuWindow.SetCommunicator(this.communicator);
            menuWindow.Show();
            this.Close();
        }

        private void updatePlayers(string[] players)
        {
            listPlayers.Items.Clear();
            foreach (string player in players)
            {
                ListBoxItem listBoxItem = new ListBoxItem();
                listBoxItem.Content = player;
                listPlayers.Items.Add(listBoxItem);
            }
        }
    }
}
