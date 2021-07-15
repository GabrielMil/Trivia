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
    /// Interaction logic for SignupWindow.xaml
    /// </summary>
    public partial class SignupWindow : Window
    {
        private Communicator communicator;
        public SignupWindow()
        {
            InitializeComponent();
        }
        public void SetCommunicator(Communicator communicator)
        {
            this.communicator = communicator;
        }

        private void btnBack_Click(object sender, RoutedEventArgs e)
        {
            LoginWindow loginWindow = new LoginWindow();
            loginWindow.SetCommunicator(this.communicator);
            loginWindow.Show();
            this.Close();
        }

        private void btnSignup_Click(object sender, RoutedEventArgs e)
        {
            string username = txtUsernameSignup.Text;
            string password = txtPasswordSignup.Text;
            string email = txtEmailSignup.Text;
            if(username == "" || password == "" || email == "")
            {
                txtErrSignup.Text = "One or more fields are missing.";
                return;
            }
            txtErrSignup.Text = "";

            //Talk with the server
            SignupRequest signupRequest = new SignupRequest(username, password, email);
            this.communicator.Write(Serializer.SerializeSignupRequest(signupRequest));
            //Start handeling the response
            Responses answer = this.communicator.Read();
            Response response = Response.GetResponse(answer);
            if (response is ErrorResponse)
            {
                txtErrSignup.Text = ((ErrorResponse)response).Message;
                return;
            }
            else if (response is SignupResponse)
            {
                if (((SignupResponse)response).Status == 1)
                {
                    this.communicator.Username = "";
                }
                else
                {
                    txtErrSignup.Text = "Can't signup";
                    return;
                }
            }
            else
            {
                MessageBox.Show("Unexpected Response.\nPlease contact us.\nResponse Code:" + answer.Code.ToString(), "Error Detected");
                this.Close();
                return;
            }

            LoginWindow loginWindow = new LoginWindow();
            loginWindow.SetCommunicator(this.communicator);
            loginWindow.Show();
            this.Close();
        }

        private void btnQuit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }
    }
}
