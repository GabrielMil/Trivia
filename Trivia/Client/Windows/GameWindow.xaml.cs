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
    /// Interaction logic for GameWindow.xaml
    /// </summary>
    public partial class GameWindow : Window
    {
        private Communicator communicator;
        private GameResult playerResult;
        private BackgroundWorker gameResultWorker;
        private BackgroundWorker submitAnsWorker;
        private BackgroundWorker managerWorker;

        private int timeForQuestion;
        private int timeLeft;
        private int totalQuestions;

        private bool isAnswered;
        private bool isExit;

        public GameWindow(string roomName, Communicator communicator, int totalQuestions, int time)
        {
            InitializeComponent();

            this.communicator = communicator;

            this.totalQuestions = totalQuestions;
            this.timeForQuestion = time;
            this.isAnswered = false;
            this.isExit = false;

            txtUsername.Text = communicator.Username;
            txtRoomName.Text = roomName;

            this.playerResult = new GameResult(this.communicator.Username, 0, 0, 0);

            this.gameResultWorker = new BackgroundWorker();
            this.gameResultWorker.DoWork += GameResultWorker_DoWork;
            this.gameResultWorker.RunWorkerCompleted += GameResultWorker_RunWorkerCompleted;
            this.gameResultWorker.WorkerSupportsCancellation = true;

            this.submitAnsWorker = new BackgroundWorker();
            this.submitAnsWorker.DoWork += SubmitAnsWorker_DoWork;
            this.submitAnsWorker.ProgressChanged += SubmitAnsWorker_ProgressChanged;
            this.submitAnsWorker.WorkerReportsProgress = true;

            this.managerWorker = new BackgroundWorker();
            this.managerWorker.DoWork += ManagerWorker_DoWork;
            this.managerWorker.ProgressChanged += ManagerWorker_ProgressChanged;
            this.managerWorker.RunWorkerCompleted += ManagerWorker_RunWorkerCompleted;
            this.managerWorker.WorkerReportsProgress = true;

            txtCorrectAns.Text = "Correct: " + this.playerResult.CorrectAnswerCount;
            txtAnswered.Text = "" + (this.playerResult.CorrectAnswerCount + this.playerResult.WrongAnswerCount) + "/" + this.totalQuestions;

            this.managerWorker.RunWorkerAsync();
        }

        private void GameResultWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            string result = (string)e.Result;
            MessageBox.Show(result);
            //To remove from server
            this.communicator.Write(PacketHelper.CreatePacket((byte)PacketCodes.LeaveGame, ""));
            Responses answer = this.communicator.Read();
            //
            MenuWindow menuWindow = new MenuWindow();
            menuWindow.SetCommunicator(this.communicator);
            menuWindow.Show();
            this.Close();
        }

        private void ManagerWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if(!this.isExit)
            {
                txtQuestions.Text = "Wait for everyone to finish.";
                this.gameResultWorker.RunWorkerAsync();
            }
        }

        private void ManagerWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            if(e.ProgressPercentage == 1)
            {
                txtTimeLeft.Text = "Time Left: " + this.timeLeft;
            }
            else if(e.ProgressPercentage == 0)
            {
                GetQuestionResponse questionRes = (GetQuestionResponse)e.UserState;
                //Set Question
                txtQuestions.Text = questionRes.Question;

                txtTimeLeft.Text = "Time Left: " + this.timeLeft;

                //Set answers
                Random random = new Random();
                Dictionary<int, string> answers = questionRes.Answers;

                //Generate answer
                int index = random.Next(answers.Count);
                int key = answers.Keys.ElementAt(index);
                btnAns1.Background = Brushes.AliceBlue;
                btnAns1.Content = answers[key];
                btnAns1.Tag = key;
                answers.Remove(key);

                //Generate answer
                index = random.Next(answers.Count);
                key = answers.Keys.ElementAt(index);
                btnAns2.Background = Brushes.AliceBlue;
                btnAns2.Content = answers[key];
                btnAns2.Tag = key;
                answers.Remove(key);

                //Generate answer
                index = random.Next(answers.Count);
                key = answers.Keys.ElementAt(index);
                btnAns3.Background = Brushes.AliceBlue;
                btnAns3.Content = answers[key];
                btnAns3.Tag = key;
                answers.Remove(key);

                //Generate answer
                index = random.Next(answers.Count);
                key = answers.Keys.ElementAt(index);
                btnAns4.Background = Brushes.AliceBlue;
                btnAns4.Content = answers[key];
                btnAns4.Tag = key;
                answers.Remove(key);
            }
        }

        private void ManagerWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            this.communicator.Write(PacketHelper.CreatePacket((byte)PacketCodes.GetQuestion, ""));
            GetQuestionResponse questionRes = (GetQuestionResponse)Response.GetResponse(this.communicator.Read());
            while(!this.isExit && questionRes.Status == 1)
            {
                this.isAnswered = false;
                for (this.timeLeft = this.timeForQuestion; this.timeLeft > 0 && !this.isAnswered && !this.isExit; this.timeLeft--) 
                {
                    if(!this.isAnswered && this.timeLeft == this.timeForQuestion)
                    {
                        //Init
                        this.managerWorker.ReportProgress(0, questionRes);
                    }
                    else
                    {
                        //Update time
                        this.managerWorker.ReportProgress(1, questionRes);

                    }
                    Thread.Sleep(1000);
                }
                if(!this.isExit)
                {
                    if (!this.isAnswered)
                    {
                        this.submitAnsWorker.RunWorkerAsync(5);
                    }
                    this.communicator.Write(PacketHelper.CreatePacket((byte)PacketCodes.GetQuestion, ""));
                    questionRes = (GetQuestionResponse)Response.GetResponse(this.communicator.Read());
                }
            }
        }

        private void SubmitAnsWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            this.Dispatcher.Invoke(() =>
            {
                txtCorrectAns.Text = "Correct: " + this.playerResult.CorrectAnswerCount;
                txtAnswered.Text = "" + (this.playerResult.CorrectAnswerCount + this.playerResult.WrongAnswerCount) + "/" + this.totalQuestions;

                //Update the colors
                btnAns1.Background = (int)btnAns1.Tag == (int)e.UserState ? Brushes.Green : Brushes.Red;
                btnAns2.Background = (int)btnAns2.Tag == (int)e.UserState ? Brushes.Green : Brushes.Red;
                btnAns3.Background = (int)btnAns3.Tag == (int)e.UserState ? Brushes.Green : Brushes.Red;
                btnAns4.Background = (int)btnAns4.Tag == (int)e.UserState ? Brushes.Green : Brushes.Red;

            });
            
            this.isAnswered = true;
        }

        private void SubmitAnsWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            //Submit answer
            int timeAnswerd = this.timeForQuestion - this.timeLeft + 1;
            this.communicator.Write(Serializer.SerializeSubmitAnswerRequest(new SubmitAnswerRequest((int)e.Argument, timeAnswerd)));
            //Get response
            SubmitAnswerResponse response = (SubmitAnswerResponse)Response.GetResponse(this.communicator.Read());

            if ((int)e.Argument == response.CorrectAnswerId)
            {
                this.playerResult.CorrectAnswerCount++;
            }
            else
            {
                this.playerResult.WrongAnswerCount++;
            }

            this.playerResult.AverageAnswerTime += ((timeAnswerd - this.playerResult.AverageAnswerTime) / (this.playerResult.CorrectAnswerCount + this.playerResult.WrongAnswerCount));
            this.submitAnsWorker.ReportProgress(100, response.CorrectAnswerId);
        }

        private void GameResultWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            string result = "";
            bool isFinished = false;
            while(!isFinished && !e.Cancel)
            {
                this.communicator.Write(PacketHelper.CreatePacket((byte)PacketCodes.GetGameResults, ""));
                GetGameResultsResponse gameResultsResponse = (GetGameResultsResponse)Response.GetResponse(this.communicator.Read());
                //Check if game finished
                if(gameResultsResponse.Status == 1)
                {
                    foreach (GameResult res in gameResultsResponse.Results)
                    {
                        result += res.ToString() + "\n";
                    }
                    isFinished = true;
                }
                Thread.Sleep(1000);
            }
            //////////////////////////////////////
            ///
            e.Result = result;
        }

        private void btnExit_Click(object sender, RoutedEventArgs e)
        {
            this.isExit = true;
            if(this.gameResultWorker.IsBusy)
            {
                this.gameResultWorker.CancelAsync();
                Thread.Sleep(1000);
            }
            this.communicator.Write(PacketHelper.CreatePacket((byte)PacketCodes.LeaveGame, ""));
            LeaveGameResponse leaveGameResponse = (LeaveGameResponse)Response.GetResponse(this.communicator.Read());
            if (leaveGameResponse.Status !=1)
            {
                MessageBox.Show("Error Happened, Can't leave room.");
                return;
            }
            MenuWindow menuWindow = new MenuWindow();
            menuWindow.SetCommunicator(this.communicator);
            menuWindow.Show();
            this.Close();
        }

        private void btnAns1_Click(object sender, RoutedEventArgs e)
        {
            this.submitAnsWorker.RunWorkerAsync((int)((Button)e.Source).Tag);
        }

        private void btnAns2_Click(object sender, RoutedEventArgs e)
        {
            this.submitAnsWorker.RunWorkerAsync((int)((Button)e.Source).Tag);
        }

        private void btnAns3_Click(object sender, RoutedEventArgs e)
        {
            this.submitAnsWorker.RunWorkerAsync((int)((Button)e.Source).Tag);
        }

        private void btnAns4_Click(object sender, RoutedEventArgs e)
        {
            this.submitAnsWorker.RunWorkerAsync((int)((Button)e.Source).Tag);
        }
    }
}
