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
    /// Interaction logic for AddQuestionWindow.xaml
    /// </summary>
    public partial class AddQuestionWindow : Window
    {
        Communicator communicator;
        public AddQuestionWindow(Communicator communicator)
        {
            InitializeComponent();
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

        private void btnAddQuestion_Click(object sender, RoutedEventArgs e)
        {
            string question = txtQuestion.Text;
            string correctAns = txtCorrectAns.Text;
            string ans2 = txtAns2.Text;
            string ans3 = txtAns3.Text;
            string ans4 = txtAns4.Text;

            if(question == null || question == "")
            {
                txtErrAddQuestion.Text = "Question is empty.";
                return;
            }
            if (correctAns == null || correctAns == "")
            {
                txtErrAddQuestion.Text = "Correct answer is empty.";
                return;
            }
            if (ans2 == null || ans2 == "")
            {
                txtErrAddQuestion.Text = "Answer 2 is empty.";
                return;
            }
            if (ans3 == null || ans3 == "")
            {
                txtErrAddQuestion.Text = "Answer 3 is empty.";
                return;
            }
            if (ans4 == null || ans4 == "")
            {
                txtErrAddQuestion.Text = "Answer 4 is empty.";
                return;
            }
            txtErrAddQuestion.Text = "";

            //Talk with the server
            AddQuestionRequest addQuestionRequest = new AddQuestionRequest(question, correctAns, ans2, ans3, ans4);
            this.communicator.Write(Serializer.SerializeAddQuestionRequest(addQuestionRequest));
            //Start handeling the response
            Responses answer = this.communicator.Read();
            Response response = Response.GetResponse(answer);
            if (response is ErrorResponse)
            {
                txtErrAddQuestion.Text = ((ErrorResponse)response).Message;
                return;
            }
            else if (response is AddQuestionResponse)
            {
                if (((AddQuestionResponse)response).Status == 1)
                {
                    MenuWindow menuWindow = new MenuWindow();
                    menuWindow.SetCommunicator(this.communicator);
                    menuWindow.Show();
                    this.Close();
                }
                else
                {
                    txtErrAddQuestion.Text = "Can't add question room";
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
