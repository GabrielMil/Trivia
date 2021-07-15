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
    /// Interaction logic for LoginWindow.xaml
    /// </summary>
    public partial class LoginWindow : Window
    {
        Communicator communicator;

        public LoginWindow()
        {
            InitializeComponent();
        }

        public void SetCommunicator(Communicator communicator)
        {
            this.communicator = communicator;
        }

        private void btnLogin_Click(object sender, RoutedEventArgs e)
        {
            string userName = txtUsernameLogin.Text;
            string password = txtPasswordLogin.Text;
            if (userName == "" || password == "")
            {
                txtErrLogin.Text = "One or more fields are empty.";
                return;
            }
            txtErrLogin.Text = "";

            //Talk with the server
            LoginRequest loginRequest = new LoginRequest(userName, password);
            this.communicator.Write(Serializer.SerializeLoginRequest(loginRequest));
            //Start handeling the response
            Responses answer = this.communicator.Read();
            Response response = Response.GetResponse(answer);
            if(response is ErrorResponse)
            {
                txtErrLogin.Text = ((ErrorResponse)response).Message;
                return;
            }
            else if(response is LoginResponse)
            {
                if(((LoginResponse)response).Status == 1)
                {
                    this.communicator.Username = loginRequest.Username;
                }
                else
                {
                    txtErrLogin.Text = "Can't login";
                    return;
                }
            }
            else
            {
                MessageBox.Show("Unexpected Response.\nPlease contact us.\nResponse Code:" + answer.Code.ToString(), "Error Detected");
                this.Close();
                return;
            }

            MenuWindow window = new MenuWindow();
            window.SetCommunicator(this.communicator);
            window.Show();
            this.Close();
        }

        private void btnQuit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void btnSignup_Click(object sender, RoutedEventArgs e)
        {
            SignupWindow signupWindow = new SignupWindow();
            signupWindow.SetCommunicator(this.communicator);
            signupWindow.Show();
            this.Close();
        }
    }
}
