using Client.Classes;
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

namespace Client.Windows
{
    /// <summary>
    /// Interaction logic for HighScoresWindow.xaml
    /// </summary>
    public partial class HighScoresWindow : Window
    {
        private Communicator communicator;
        public HighScoresWindow()
        {
            InitializeComponent();
        }
        public void SetCommunicator(Communicator communicator)
        {
            this.communicator = communicator;
            txtUsername.Text = this.communicator.Username;

            //Talk with the server
            this.communicator.Write(PacketHelper.CreatePacket((byte)PacketCodes.GetHighScore, ""));
            //Start handeling the response
            Responses answer = this.communicator.Read();
            Response response = Response.GetResponse(answer);
            if (response is ErrorResponse)
            {
                MessageBox.Show(((ErrorResponse)response).Message, "Error Response");
                this.Close();
                return;
            }
            else if (response is GetHighScoreResponse)
            {
                if (((GetHighScoreResponse)response).Status == 1)
                {
                    foreach (string item in ((GetHighScoreResponse)response).Statistics)
                    {
                        //Get the best!
                        TextBlock textBlock = new TextBlock();
                        textBlock.Text = item;
                        textBlock.TextWrapping = TextWrapping.Wrap;
                        textBlock.FontSize = 30;
                        textBlock.FontWeight = FontWeights.Bold;
                        textBlock.FontFamily = new FontFamily("Britannic Bold");
                        stckPlayers.Children.Add(textBlock);
                    }
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

        private void btnBack_Click(object sender, RoutedEventArgs e)
        {
            MenuWindow menuWindow = new MenuWindow();
            menuWindow.SetCommunicator(this.communicator);
            menuWindow.Show();
            this.Close();
        }
    }
}
