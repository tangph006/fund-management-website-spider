using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.IO;
using System.Net;
using System.Text.RegularExpressions;
using System.Windows.Forms;

using mshtml;

using FM.Service.WebSpider;
using FM.Util;

namespace Spider
{
    /// <summary>
    /// Summary description for Form1.
    /// </summary>
    public class Form1 : System.Windows.Forms.Form
    {
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.StatusBar statusBar;
        private System.Windows.Forms.StatusBarPanel statusBarPanel;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Label lblFailedLinks;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtOutput;
        private System.Windows.Forms.TextBox txtBaseUri;
        private System.Windows.Forms.Button btnStartStop;
        private System.Windows.Forms.NumericUpDown spnProcessUriMax;
        private System.Windows.Forms.TextBox txtUri;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label lblBaseUrl;
        private System.Windows.Forms.Label lblUrl;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnImgStart;
        private System.Windows.Forms.TextBox txtUrlGraphicsRipper;
        private AxSHDocVw.AxWebBrowser axWebBrowser;
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.Container components = null;

        [STAThread]
        static void Main()
        {
            Application.Run(new Form1());
        }

        public Form1()
        {
            //
            // Required for Windows Form Designer support
            //
            InitializeComponent();

        }

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (components != null)
                {
                    components.Dispose();
                }
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.lblFailedLinks = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.txtOutput = new System.Windows.Forms.TextBox();
            this.txtBaseUri = new System.Windows.Forms.TextBox();
            this.btnStartStop = new System.Windows.Forms.Button();
            this.spnProcessUriMax = new System.Windows.Forms.NumericUpDown();
            this.txtUri = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.lblBaseUrl = new System.Windows.Forms.Label();
            this.lblUrl = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.axWebBrowser = new AxSHDocVw.AxWebBrowser();
            this.btnImgStart = new System.Windows.Forms.Button();
            this.txtUrlGraphicsRipper = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.statusBar = new System.Windows.Forms.StatusBar();
            this.statusBarPanel = new System.Windows.Forms.StatusBarPanel();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.spnProcessUriMax)).BeginInit();
            this.tabPage2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.axWebBrowser)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanel)).BeginInit();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(10, 9);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(700, 585);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.lblFailedLinks);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Controls.Add(this.txtOutput);
            this.tabPage1.Controls.Add(this.txtBaseUri);
            this.tabPage1.Controls.Add(this.btnStartStop);
            this.tabPage1.Controls.Add(this.spnProcessUriMax);
            this.tabPage1.Controls.Add(this.txtUri);
            this.tabPage1.Controls.Add(this.label4);
            this.tabPage1.Controls.Add(this.lblBaseUrl);
            this.tabPage1.Controls.Add(this.lblUrl);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Size = new System.Drawing.Size(692, 559);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Broken Links";
            // 
            // lblFailedLinks
            // 
            this.lblFailedLinks.Location = new System.Drawing.Point(10, 112);
            this.lblFailedLinks.Name = "lblFailedLinks";
            this.lblFailedLinks.Size = new System.Drawing.Size(120, 25);
            this.lblFailedLinks.TabIndex = 23;
            this.lblFailedLinks.Text = "Failed Links";
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(259, 78);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(87, 24);
            this.label1.TabIndex = 22;
            this.label1.Text = "-1 Unlimited";
            // 
            // txtOutput
            // 
            this.txtOutput.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtOutput.Location = new System.Drawing.Point(10, 138);
            this.txtOutput.Multiline = true;
            this.txtOutput.Name = "txtOutput";
            this.txtOutput.Size = new System.Drawing.Size(672, 370);
            this.txtOutput.TabIndex = 21;
            // 
            // txtBaseUri
            // 
            this.txtBaseUri.Location = new System.Drawing.Point(182, 43);
            this.txtBaseUri.Name = "txtBaseUri";
            this.txtBaseUri.Size = new System.Drawing.Size(500, 21);
            this.txtBaseUri.TabIndex = 20;
            this.txtBaseUri.Text = "http://www.holidaycoast.net.au/";
            // 
            // btnStartStop
            // 
            this.btnStartStop.Location = new System.Drawing.Point(595, 517);
            this.btnStartStop.Name = "btnStartStop";
            this.btnStartStop.Size = new System.Drawing.Size(90, 25);
            this.btnStartStop.TabIndex = 19;
            this.btnStartStop.Text = "Start";
            this.btnStartStop.Click += new System.EventHandler(this.btnStartStop_Click);
            // 
            // spnProcessUriMax
            // 
            this.spnProcessUriMax.Location = new System.Drawing.Point(182, 78);
            this.spnProcessUriMax.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.spnProcessUriMax.Name = "spnProcessUriMax";
            this.spnProcessUriMax.Size = new System.Drawing.Size(68, 21);
            this.spnProcessUriMax.TabIndex = 18;
            this.spnProcessUriMax.Value = new decimal(new int[] {
            100,
            0,
            0,
            0});
            // 
            // txtUri
            // 
            this.txtUri.Location = new System.Drawing.Point(182, 9);
            this.txtUri.Name = "txtUri";
            this.txtUri.Size = new System.Drawing.Size(500, 21);
            this.txtUri.TabIndex = 17;
            this.txtUri.Text = "http://www.holidaycoast.net.au/coffsharbour/";
            // 
            // label4
            // 
            this.label4.Location = new System.Drawing.Point(10, 78);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(163, 24);
            this.label4.TabIndex = 16;
            this.label4.Text = "Process Urls (Max)";
            // 
            // lblBaseUrl
            // 
            this.lblBaseUrl.Location = new System.Drawing.Point(10, 43);
            this.lblBaseUrl.Name = "lblBaseUrl";
            this.lblBaseUrl.Size = new System.Drawing.Size(163, 25);
            this.lblBaseUrl.TabIndex = 15;
            this.lblBaseUrl.Text = "Base Url";
            // 
            // lblUrl
            // 
            this.lblUrl.Location = new System.Drawing.Point(10, 9);
            this.lblUrl.Name = "lblUrl";
            this.lblUrl.Size = new System.Drawing.Size(163, 24);
            this.lblUrl.TabIndex = 14;
            this.lblUrl.Text = "Url";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.axWebBrowser);
            this.tabPage2.Controls.Add(this.btnImgStart);
            this.tabPage2.Controls.Add(this.txtUrlGraphicsRipper);
            this.tabPage2.Controls.Add(this.label2);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Size = new System.Drawing.Size(692, 559);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Graphics Ripper";
            // 
            // axWebBrowser
            // 
            this.axWebBrowser.Enabled = true;
            this.axWebBrowser.Location = new System.Drawing.Point(10, 43);
            this.axWebBrowser.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("axWebBrowser.OcxState")));
            this.axWebBrowser.Size = new System.Drawing.Size(1113, 474);
            this.axWebBrowser.TabIndex = 21;
            // 
            // btnImgStart
            // 
            this.btnImgStart.Location = new System.Drawing.Point(595, 526);
            this.btnImgStart.Name = "btnImgStart";
            this.btnImgStart.Size = new System.Drawing.Size(87, 24);
            this.btnImgStart.TabIndex = 20;
            this.btnImgStart.Text = "Start";
            this.btnImgStart.Click += new System.EventHandler(this.btnImgStart_Click);
            // 
            // txtUrlGraphicsRipper
            // 
            this.txtUrlGraphicsRipper.Location = new System.Drawing.Point(182, 9);
            this.txtUrlGraphicsRipper.Name = "txtUrlGraphicsRipper";
            this.txtUrlGraphicsRipper.Size = new System.Drawing.Size(500, 21);
            this.txtUrlGraphicsRipper.TabIndex = 19;
            this.txtUrlGraphicsRipper.Text = "http://images.google.com.au/images?q=funny+sex&ie=UTF-8&oe=UTF-8&hl=en";
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(10, 9);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(163, 24);
            this.label2.TabIndex = 18;
            this.label2.Text = "Url";
            // 
            // statusBar
            // 
            this.statusBar.Location = new System.Drawing.Point(0, 602);
            this.statusBar.Name = "statusBar";
            this.statusBar.Panels.AddRange(new System.Windows.Forms.StatusBarPanel[] {
            this.statusBarPanel});
            this.statusBar.Size = new System.Drawing.Size(712, 24);
            this.statusBar.TabIndex = 1;
            // 
            // statusBarPanel
            // 
            this.statusBarPanel.Name = "statusBarPanel";
            // 
            // Form1
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
            this.ClientSize = new System.Drawing.Size(712, 626);
            this.Controls.Add(this.statusBar);
            this.Controls.Add(this.tabControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "Form1";
            this.Text = "Form1";
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.spnProcessUriMax)).EndInit();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.axWebBrowser)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanel)).EndInit();
            this.ResumeLayout(false);

        }
        #endregion

        #region Broken Link Checker
        private void btnStartStop_Click(object sender, System.EventArgs e)
        {
            statusBar.Text = "Processed Uri: " + txtUri.Text;

            btnStartStop.Enabled = false;
            txtOutput.Lines = null;

            WebSpider spider = new WebSpider(txtUri.Text, txtBaseUri.Text, (int)spnProcessUriMax.Value);

            spider.webPageProcessor_.contentHandler_ += new WebPageContentDelegate(HandleContent);
            spider.Execute();

            statusBar.Text = "Finished Processing";

            ICollection webPageStateList = spider.webPages_.Values;
            ArrayList failedPageList = new ArrayList();
            int pagesNotProcessed = 0;

            foreach (WebPageState webPageState in webPageStateList)
            {
                if (!webPageState.processStarted_)
                {
                    pagesNotProcessed++;
                }
                else if (!webPageState.processSuccessfull_)
                {
                    failedPageList.Add(webPageState);
                }
            }

            string[] lines = new string[failedPageList.Count * 3];
            int index = 0;

            foreach (WebPageState webPageState in failedPageList)
            {
                lines[index++] = "Uri        : " + webPageState.uri_.AbsoluteUri;
                lines[index++] = "Description: " + webPageState.statusDescription_;
            }

            txtOutput.Lines = lines;
            btnStartStop.Enabled = true;
        }

        private void HandleContent(WebPageState webPageState)
        {
            statusBar.Text = "Processed Uri: " + webPageState.uri_;

            Application.DoEvents();
        }
        #endregion

        #region Image Ripper
        private Uri m_baseUri;
        private StreamWriter m_graphicViewerWriter;
        private string m_outFolder;
        private string m_outFile;
        private int m_fileId;

        private void btnImgStart_Click(object sender, System.EventArgs e)
        {
            statusBar.Text = "Process Uri: " + txtUrlGraphicsRipper.Text;

            m_baseUri = new Uri(txtUrlGraphicsRipper.Text);

            try
            {
                SetupOutputFile();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to setup output file\n" + ex.ToString());
                return;
            }

            IWebPageProcessor webPageProcessor = new WebPageProcessor();
            webPageProcessor.contentHandler_ += new WebPageContentDelegate(GraphicsLinkHandler);
            webPageProcessor.Process(new WebPageState(txtUrlGraphicsRipper.Text));

            CloseOutputFile();

            statusBar.Text = "Finished Processing";
        }

        private void GraphicsLinkHandler(WebPageState state)
        {
            Match m = RegExUtil.GetMatchRegEx(RegularExpression.SrcExtractor, state.content_);
            string image;

            while (m.Success)
            {
                m = m.NextMatch();
                image = m.Groups[1].ToString();

                statusBar.Text = "Image: " + image;
                Application.DoEvents();

                DownloadImage(image);
            }

        }

        private void SetupOutputFile()
        {
            m_outFolder = Application.StartupPath + "\\temp";
            m_outFile = m_outFolder + "\\viewer.html";

            m_fileId = 0;

            try
            {
                DirectoryInfo dirInfo = new DirectoryInfo(m_outFolder);
                dirInfo.Delete(true);
            }
            catch { }

            Directory.CreateDirectory(m_outFolder);

            m_graphicViewerWriter = File.CreateText(m_outFile);
            m_graphicViewerWriter.WriteLine("<html><body>");
        }

        private void CloseOutputFile()
        {
            m_graphicViewerWriter.WriteLine("</body></html>");
            m_graphicViewerWriter.Close();

            object empty = null;
            axWebBrowser.Navigate(m_outFile, ref empty, ref empty, ref empty, ref empty);
        }

        private void DownloadImage(string imgUri)
        {
            Uri imageUri = null;
            string ext = null;
            string outFile = null;
            try
            {
                imageUri = new Uri(m_baseUri, imgUri);

                ext = StrUtil.RightLastIndexOf(imageUri.AbsoluteUri, ".").ToLower();
                outFile = m_outFolder + "\\img" + (m_fileId++) + "." + ext;

                if ("png|gif|jpg|jpeg|swf".IndexOf(ext) > -1)
                {
                    WebClient web = new WebClient();
                    web.DownloadFile(imageUri.AbsoluteUri, outFile);

                    if (ext == "swf")
                    {
                        m_graphicViewerWriter.WriteLine("<object classid='clsid:D27CDB6E-AE6D-11cf-96B8-444553540000' codebase='http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,29,0' width='150' height='100'>");
                        m_graphicViewerWriter.WriteLine("<param name='movie' value='" + outFile + "'>");
                        m_graphicViewerWriter.WriteLine("<param name=quality value=high>");
                        m_graphicViewerWriter.WriteLine("<embed src='" + outFile + "' quality=high pluginspage='http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash' type='application/x-shockwave-flash' width='150' height='100'></embed>");
                        m_graphicViewerWriter.WriteLine("</object>");
                    }
                    else
                    {
                        m_graphicViewerWriter.WriteLine("<img src='file://" + outFile + "' /><br />");
                    }
                }
            }
            catch (Exception)
            {
                m_graphicViewerWriter.WriteLine("could not download img: " + imageUri.AbsoluteUri);
            }
        }
        #endregion
    }
}
