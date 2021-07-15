using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Client.Classes;

using Newtonsoft.Json;

namespace Client.Windows
{

    /// <summary>
    /// Interaction logic for CreateRoomWindow.xaml
    /// </summary>
    public partial class CreateRoomWindow : Window
    {
        private Communicator communicator;
        public CreateRoomWindow()
        {
            InitializeComponent();
        }

        public void SetCommunicator(Communicator communicator)
        {
            this.communicator = communicator;
            txtUsername.Text = this.communicator.Username;
        }

        private void btnBack_Click(object sender, RoutedEventArgs e)
        {
            MenuWindow menuWindow = new MenuWindow();
            menuWindow.SetCommunicator(this.communicator);
            menuWindow.Show();
            this.Close();
        }

        private void btnCreateRoom_Click(object sender, RoutedEventArgs e)
        {
            int numOfPlayers = -1;//txtNumberOfPlayers.Text;
            int numOfQuestions = -1;// txtNumberOfQuestions.Text;
            int time = -1;// txtTimeForQuestion.Text;

            //Make sure all the fields are full
            if (txtRoomName.Text == "" || txtNumberOfPlayers.Text == "" || txtNumberOfPlayers.Text == "" || txtTimeForQuestion.Text == "") 
            {
                txtErrCreateRoom.Text = "One or more fields are missing!";
                return;
            }

            //Try to convert the input, if not secceeded let the user know
            if (!int.TryParse(txtNumberOfPlayers.Text, out numOfPlayers))
            {
                txtErrCreateRoom.Text = "Num of Players is not a number!";
                return;
            }
            txtErrCreateRoom.Text = "";

            //Try to convert the input, if not secceeded let the user know
            if (!int.TryParse(txtNumberOfQuestions.Text, out numOfQuestions))
            {
                txtErrCreateRoom.Text = "Num of Questions is not a number!";
                return;
            }
            txtErrCreateRoom.Text = "";

            //Try to convert the input, if not secceeded let the user know
            if (!int.TryParse(txtTimeForQuestion.Text, out time))
            {
                txtErrCreateRoom.Text = "Time is not a number!";
                return;
            }
            txtErrCreateRoom.Text = "";

            //Talk with the server
            CreateRoomRequest createRoomRequest = new CreateRoomRequest(txtRoomName.Text, numOfPlayers, numOfQuestions, time);
            this.communicator.Write(Serializer.SerializeCreateRoomRequest(createRoomRequest));
            //Start handeling the response
            Responses answer = this.communicator.Read();
            Response response = Response.GetResponse(answer);
            if (response is ErrorResponse)
            {
                txtErrCreateRoom.Text = ((ErrorResponse)response).Message;
                return;
            }
            else if (response is CreateRoomResponse)
            {
                if (((CreateRoomResponse)response).Status == 1)
                {
                    WaitingRoomWindow waitingRoomWindow = new WaitingRoomWindow(this.communicator, true, createRoomRequest.RoomName, numOfPlayers);
                    waitingRoomWindow.SetCommunicator(this.communicator);
                    waitingRoomWindow.Show();
                    this.Close();
                }
                else
                {
                    txtErrCreateRoom.Text = "Can't create room";
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
    }
}
