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

namespace Client.Windows
{
    /// <summary>
    /// Interaction logic for MenuWindow.xaml
    /// </summary>
    public partial class MenuWindow : Window
    {
        private Communicator communicator;

        public MenuWindow()
        {
            InitializeComponent();
        }

        public void SetCommunicator(Communicator communicator)
        {
            this.communicator = communicator;
            txtUsername.Text = this.communicator.Username;
            txtHelloUser.Text = this.communicator.Username;
        }

        private void btnJoinRoom_Click(object sender, RoutedEventArgs e)
        {
            JoinRoomWindow joinRoomWindow = new JoinRoomWindow();
            joinRoomWindow.SetCommunicator(this.communicator);
            joinRoomWindow.Show();
            this.Close();
        }

        private void btnCreateRoom_Click(object sender, RoutedEventArgs e)
        {
            CreateRoomWindow createRoomWindow = new CreateRoomWindow();
            createRoomWindow.SetCommunicator(this.communicator);
            createRoomWindow.Show();
            this.Close();
        }

        private void btnMyStatus_Click(object sender, RoutedEventArgs e)
        {
            MyStatusWindow myStatusWindow = new MyStatusWindow();
            myStatusWindow.SetCommunicator(this.communicator);
            myStatusWindow.Show();
            this.Close();
        }

        private void btnBestScores_Click(object sender, RoutedEventArgs e)
        {
            HighScoresWindow highScoresWindow = new HighScoresWindow();
            highScoresWindow.SetCommunicator(this.communicator);
            highScoresWindow.Show();
            this.Close();
        }

        private void btnQuit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void btnSignout_Click(object sender, RoutedEventArgs e)
        {
            //Talk with the server
            this.communicator.Write(PacketHelper.CreatePacket((byte)PacketCodes.Logout, ""));
            //Start handeling the response
            Responses answer = this.communicator.Read();
            Response response = Response.GetResponse(answer);
            if (response is ErrorResponse)
            {
                MessageBox.Show(((ErrorResponse)response).Message, "Error Response");
                this.Close();
                return;
            }
            else if (response is LogoutResponse)
            {
                if (((LogoutResponse)response).Status == 1)
                {
                    this.communicator.Username = "";
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
                this.Close();
                return;
            }

            LoginWindow loginWindow = new LoginWindow();
            loginWindow.SetCommunicator(this.communicator);
            loginWindow.Show();
            this.Close();
        }

        private void btnAddQuestion_Click(object sender, RoutedEventArgs e)
        {
            AddQuestionWindow addQuestionWindow = new AddQuestionWindow(this.communicator);
            addQuestionWindow.Show();
            this.Close();
        }
    }
}
