using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Windows;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace Demo
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        /// <summary>
        /// Application Entry Point.
        /// </summary>
        [System.STAThreadAttribute()]
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        public static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                Demo.App app = new Demo.App();
                app.InitializeComponent();
                app.Run();
            }
            else
            {
                Process p = new Process();
                p.StartInfo.FileName = "cmd";
                p.Start();
            }
        }
    }
}
