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
    /// Interaction logic for JoinRoomWindow.xaml
    /// </summary>
    public partial class JoinRoomWindow : Window
    {
        private Communicator communicator;
        private RoomData[] rooms;
        private bool isRoomFull;
        private int idChosenRoom;
        private RoomData roomToGo;
        private BackgroundWorker refreshWorker;

        public JoinRoomWindow()
        {
            InitializeComponent();
            this.rooms = null;
            this.isRoomFull = false;
            this.idChosenRoom = -1;
            this.refreshWorker = new BackgroundWorker();
            this.refreshWorker.DoWork += RefreshWorker_DoWork;
            this.refreshWorker.ProgressChanged += RefreshWorker_ProgressChanged;
            this.refreshWorker.WorkerReportsProgress = true;
            this.refreshWorker.WorkerSupportsCancellation = true;
            this.refreshWorker.RunWorkerAsync();
        }

        private void RefreshWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            Refresh();
        }

        private void RefreshWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            while(!this.refreshWorker.CancellationPending)
            {
                this.refreshWorker.ReportProgress(1);  // just notify so the progress change will be triggered
                Thread.Sleep(15000);
            }
        }

        public void SetCommunicator(Communicator communicator)
        {
            this.communicator = communicator;
            txtUsername.Text = this.communicator.Username;
        }

        private RoomData GetRoomDataByName(string name)
        {
            RoomData room = null;
            foreach (RoomData roomData in this.rooms)
            {
                if (roomData.Name == name)
                {
                    room = roomData;
                    break;
                }
            }
            return room;
        }

        private void UpdateRooms()
        {
            //Talk with the server
            this.communicator.Write(PacketHelper.CreatePacket((byte)PacketCodes.GetRooms, ""));
            //Start handeling the response
            Responses answer = this.communicator.Read();
            Response response = Response.GetResponse(answer);
            if (response is ErrorResponse)
            {
                MessageBox.Show(((ErrorResponse)response).Message, "Error Response");
                this.Close();
                return;
            }
            else if (response is GetRoomsResponse)
            {
                if (((GetRoomsResponse)response).Status == 1)
                {
                    this.rooms = ((GetRoomsResponse)response).Rooms;
                }
                else
                {
                    MessageBox.Show("Some error occured.. See ya", "Error Response");
                    this.Close();
                    return;
                }
            }
            else
            {
                MessageBox.Show("Unexpected Response.\nPlease contact us.\nResponse Code:" + answer.Code.ToString(), "Error Detected");
                this.Close();
                return;
            }
        }

        private void AddRoom(string name)
        {
            ListBoxItem listBoxItem = new ListBoxItem();
            listBoxItem.Content = name;
            listBoxItem.FontSize = 28;
            listBoxItem.FontWeight = FontWeights.Bold;
            listBoxItem.FontFamily = new FontFamily("Britannic Bold");
            listBoxItem.Selected += Room_Selected;
            lstRooms.Items.Add(listBoxItem);
        }

        private void AddPlayer(string name)
        {
            ListBoxItem listBoxItem = new ListBoxItem();
            listBoxItem.Content = name;
            listBoxItem.FontSize = 25;
            listBoxItem.FontWeight = FontWeights.Bold;
            listBoxItem.FontFamily = new FontFamily("Britannic Bold");
            lstPlayers.Items.Add(listBoxItem);
        }

        private void Room_Selected(object sender, RoutedEventArgs e)
        {
            lstPlayers.Items.Clear();
            ListBoxItem room = e.Source as ListBoxItem;
            //Get the chosen room by the item box name
            RoomData chosenRoom = GetRoomDataByName(room.Content.ToString());

            //Talk with the server
            GetPlayersInRoomRequest getPlayersInRoomRequest = new GetPlayersInRoomRequest(chosenRoom.Id);
            this.communicator.Write(Serializer.SerializeGetPlayersInRoomRequest(getPlayersInRoomRequest));
            //Start handeling the response
            Responses answer = this.communicator.Read();
            Response response = Response.GetResponse(answer);
            if (response is ErrorResponse)
            {
                MessageBox.Show(((ErrorResponse)response).Message, "Error Detected");
                return;
            }
            else if (response is GetPlayersInRoomResponse)
            {
                if (((GetPlayersInRoomResponse)response).Status == 1)
                {
                    foreach (string player in ((GetPlayersInRoomResponse)response).Players)
                    {
                        AddPlayer(player);
                    }
                    //Check if there is place
                    if(((GetPlayersInRoomResponse)response).Players.Length >= chosenRoom.MaxPlayers)
                    {
                        this.isRoomFull = true;
                        txtErrRooms.Text = "Room is Full.";
                        this.idChosenRoom = -1;
                        return;
                    }
                    this.isRoomFull = false;
                    txtErrRooms.Text = "";
                    this.idChosenRoom = chosenRoom.Id;
                    this.roomToGo = chosenRoom;

                }
                else
                {
                    MessageBox.Show("Oh Oh... Something went wrong. Try again.", "SAD");
                    return;
                }
            }
            else
            {
                MessageBox.Show("Unexpected Response.\nPlease contact us.\nResponse Code:" + answer.Code.ToString(), "Error Detected");
                this.Close();
                return;
            }
        }

        private void btnBack_Click(object sender, RoutedEventArgs e)
        {
            MenuWindow menuWindow = new MenuWindow();
            menuWindow.SetCommunicator(this.communicator);
            menuWindow.Show();
            this.refreshWorker.CancelAsync();
            this.Close();
        }

        private void btnJoin_Click(object sender, RoutedEventArgs e)
        {
            if(lstRooms.SelectedItem == null)
            {
                return;
            }
            RoomData chosenRoom = GetRoomDataByName(lstRooms.SelectedItem.ToString().Replace("System.Windows.Controls.ListBoxItem: ", ""));
            if(chosenRoom == null || txtErrRooms.Text != "")
            {
                return;
            }

            //Start talking with the server
            JoinRoomRequest joinRoomRequest = new JoinRoomRequest(chosenRoom.Id);
            this.communicator.Write(Serializer.SerializeJoinRoomRequest(joinRoomRequest));
            //Start handeling the response
            Responses answer = this.communicator.Read();
            Response response = Response.GetResponse(answer);
            if (response is ErrorResponse)
            {
                txtErrRooms.Text = ((ErrorResponse)response).Message;
                return;
            }
            else if (response is JoinRoomResponse)
            {
                if (((JoinRoomResponse)response).Status == 1)
                {
                    WaitingRoomWindow waitingRoomWindow = new WaitingRoomWindow(this.communicator, false, chosenRoom.Name);
                    waitingRoomWindow.SetCommunicator(this.communicator);
                    waitingRoomWindow.Show();
                    this.refreshWorker.CancelAsync();
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Some error occured.. See ya", "Error Response");
                    this.Close();
                    return;
                }
            }
            else
            {
                MessageBox.Show("Unexpected Response.\nPlease contact us.\nResponse Code:" + answer.Code.ToString(), "Error Detected");
                this.Close();
                return;
            }
        }

        private void btnRefresh_Click(object sender, RoutedEventArgs e)
        {
            Refresh();
        }

        private void Refresh()
        {
            UpdateRooms();
            lstRooms.Items.Clear();
            lstPlayers.Items.Clear();
            if (this.rooms == null || this.rooms.Length == 0)
            {
                txtErrRooms.Text = "No rooms were found.";
                return;
            }
            txtErrRooms.Text = "";
            foreach (RoomData room in this.rooms)
            {
                AddRoom(room.Name);
            }
        }
    }
}
