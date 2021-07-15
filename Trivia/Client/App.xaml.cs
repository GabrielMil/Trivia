using Client.Classes;
using Client.Windows;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

using Newtonsoft.Json;

namespace Client
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            Communicator communicator = null;
        //Try to make an connection, if failed let the user choose what to do
        //Use label
        Create:
            try
            {
                communicator = new Communicator();
            }
            catch
            {
                if (MessageBox.Show("You do not have a server running\n\n Ok - try again.\nCancel - close client", "Error Detected", MessageBoxButton.OKCancel) == MessageBoxResult.Cancel)
                {
                    Application.Current.Shutdown();
                    return;
                }
                //If not closed try again
                goto Create;
            }
            LoginWindow window = new LoginWindow();
            window.SetCommunicator(communicator);
            window.Show();
        }
    }
}
