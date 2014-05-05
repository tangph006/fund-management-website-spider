using System;
using System.Drawing;
using System.Xml;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Resources;
using System.Threading;
using System.Runtime.InteropServices;
using System.Net;
using System.Net.Sockets;
using LiteLib;

namespace Crawler
{
    public class OutputT
    {		// thread that take the browse editor text to parse it
        private Thread threadParse;
        // binary tree to keep unique Uri's
        private SortTree urlStorage;
        // Performance Counter to measure CPU usage
        private System.Diagnostics.PerformanceCounter cpuCounter;
        // Performance Counter to measure memory usage
        private System.Diagnostics.PerformanceCounter ramCounter;

        // number of bytes downloaded
        private int nByteCount;
        private int ByteCount
        {
            get { return nByteCount; }
            set
            {
                nByteCount = value;
                this.statusBarPanelByteCount.Text = Commas(nByteCount / 1024 + 1) + " KB";
            }
        }

        // number of errors during the download process
        private int nErrorCount;
        private int ErrorCount
        {
            get { return nErrorCount; }
            set
            {
                nErrorCount = value;
                this.statusBarPanelErrors.Text = Commas(nErrorCount) + " errors";
            }
        }

        // number of Uri's found
        private int nURLCount;
        private int URLCount
        {
            get { return nURLCount; }
            set
            {
                nURLCount = value;
                this.statusBarPanelURLs.Text = Commas(nURLCount) + " URL found";
            }
        }

        // available memory
        private float nFreeMemory;
        private float FreeMemory
        {
            get { return nFreeMemory; }
            set
            {
                nFreeMemory = value;
                this.statusBarPanelMem.Text = nFreeMemory + " Mb Available";
            }
        }

        // CPU usage
        private int nCPUUsage;
        private int CPUUsage
        {
            get { return nCPUUsage; }
            set
            {
                nCPUUsage = value;
                this.statusBarPanelCPU.Text = "CPU usage " + nCPUUsage + "%";

                Icon icon = Icon.FromHandle(((Bitmap)imageListPercentage.Images[value / 10]).GetHicon());
                this.statusBarPanelCPU.Icon = icon;
            }
        }

        // number of files downloaded
        private int nFileCount;
        private int FileCount
        {
            get { return nFileCount; }
            set
            {
                nFileCount = value;
                this.statusBarPanelFiles.Text = Commas(nFileCount) + " file(s) downloaded";
            }
        }

        // threads array
        private Thread[] threadsRun;

        // number of running threads
        private int nThreadCount;
        private int ThreadCount
        {
            get { return nThreadCount; }
            set
            {
                Monitor.Enter(this.listViewThreads);
                try
                {
                    for (int nIndex = 0; nIndex < value; nIndex++)
                    {
                        // check if thread not created or not suspended
                        if (threadsRun[nIndex] == null || threadsRun[nIndex].ThreadState != ThreadState.Suspended)
                        {
                            // create new thread
                            threadsRun[nIndex] = new Thread(new ThreadStart(ThreadRunFunction));
                            // set thread name equal to its index
                            threadsRun[nIndex].Name = nIndex.ToString();
                            // start thread working function
                            threadsRun[nIndex].Start();
                            // check if thread doesn't added to the view
                            if (nIndex == this.listViewThreads.Items.Count)
                            {
                                // add a new line in the view for the new thread
                                ListViewItem item = this.listViewThreads.Items.Add((nIndex + 1).ToString(), 0);
                                string[] subItems = { "", "", "", "0", "0%" };
                                item.SubItems.AddRange(subItems);
                            }
                        }
                        // check if the thread is suspended
                        else if (threadsRun[nIndex].ThreadState == ThreadState.Suspended)
                        {
                            // get thread item from the list
                            ListViewItem item = this.listViewThreads.Items[nIndex];
                            item.ImageIndex = 1;
                            item.SubItems[2].Text = "Resume";
                            // resume the thread
                            threadsRun[nIndex].Resume();
                        }
                    }
                    // change thread value
                    nThreadCount = value;
                }
                catch (Exception)
                {
                }
                Monitor.Exit(this.listViewThreads);
            }
        }
    }
}
