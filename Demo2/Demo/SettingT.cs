using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Text.RegularExpressions;

namespace Demo
{
    public class SettingT
    {
		// MIME types string
		private string strMIMETypes = SettingLoader.GetMIMETypes();
		public string MIMETypes
		{
			get	{	return strMIMETypes;	}
			set	{	strMIMETypes = value;	}
		}

		// encoding text that includes all settings types in one string
        private Encoding encoding = SettingLoader.GetTextEncoding();
        public Encoding TextEncoding
		{
			get	{	return encoding;	}
			set	{	encoding = value;	}
		}

		// timeout of sockets send and receive
		private int nRequestTimeout;
        public int RequestTimeout
		{
			get	{	return nRequestTimeout;	}
			set	{	nRequestTimeout = value;	}
		}

		// the time that each thread sleeps when the refs queue empty
		private int nSleepFetchTime;
        public int SleepFetchTime
		{
			get	{	return nSleepFetchTime;	}
			set	{	nSleepFetchTime = value;	}
		}		
		
		// List of a user defined list of restricted words to enable user to prevent any bad pages 
		private string[] strExcludeWords;
        public string[] ExcludeWords
		{
			get	{	return strExcludeWords;	}
			set	{	strExcludeWords = value;	}
		}

		// List of a user defined list of restricted files extensions to avoid paring non-text data 
		private string[] strExcludeFiles;
        public string[] ExcludeFiles
		{
			get	{	return strExcludeFiles;	}
			set	{	strExcludeFiles = value;	}
		}

		// List of a user defined list of restricted hosts extensions to avoid blocking by these hosts
		private string[] strExcludeHosts;
        public string[] ExcludeHosts
		{
			get	{	return strExcludeHosts;	}
			set	{	strExcludeHosts = value;	}
		}
		
		// the number of requests to keep in the requests view for review requests details
		private int nLastRequestCount;
        public int LastRequestCount
		{
			get	{	return nLastRequestCount;	}
			set	{	nLastRequestCount = value;	}
		}
		
		// the time that each thread sleep after handling any request, 
		// which is very important value to prevent Hosts from blocking the crawler due to heavy load
		private int nSleepConnectTime;
        public int SleepConnectTime
		{
			get	{	return nSleepConnectTime;	}
			set	{	nSleepConnectTime = value;	}
		}

		// represents the depth of navigation in the crawling process
		private int nWebDepth;
        public int WebDepth
		{
			get	{	return nWebDepth;	}
			set	{	nWebDepth = value;	}
		}

		// MIME types are the types that are supported to be downloaded by the crawler 
		// and the crawler includes a default types to be used. 
		private bool bAllMIMETypes;
        public bool AllMIMETypes
		{
			get	{	return bAllMIMETypes;	}
			set	{	bAllMIMETypes = value;	}
		}		

		// to limit crawling process to the same host of the original URL
		private bool bKeepSameServer;
        public bool KeepSameServer
		{
			get	{	return bKeepSameServer;	}
			set	{	bKeepSameServer = value;	}
		}		
		
		// means keep socket connection opened for subsequent requests to avoid reconnect time
		private bool bKeepAlive;
        public bool KeepAlive
		{
			get	{	return bKeepAlive;	}
			set	{	bKeepAlive = value;	}
		}			

        // download folder
        private string strDownloadfolder;
        public string Downloadfolder
        {
            get { return strDownloadfolder; }
            set
            {
                strDownloadfolder = value;
                strDownloadfolder = strDownloadfolder.TrimEnd('\\');
            }
        }

        // constructor
        public SettingT()
        {
            InitValues();
        }

        // construct all from registry.
        public void InitValues()
        {
            WebDepth = SettingLoader.GetValue("Web depth", 3);
            RequestTimeout = SettingLoader.GetValue("Request timeout", 20);
            SleepFetchTime = SettingLoader.GetValue("Sleep fetch time", 2);
            SleepConnectTime = SettingLoader.GetValue("Sleep connect time", 1);
            KeepSameServer = SettingLoader.GetValue("Keep same URL server", false);
            AllMIMETypes = SettingLoader.GetValue("Allow all MIME types", true);
            KeepAlive = SettingLoader.GetValue("Keep connection alive", true);
            ExcludeHosts = SettingLoader.GetValue("Exclude Hosts", ".org; .gov;").Replace("*", "").ToLower().Split(';');
            ExcludeWords = SettingLoader.GetValue("Exclude words", "").Split(';');
            ExcludeFiles = SettingLoader.GetValue("Exclude files", "").Replace("*", "").ToLower().Split(';');
            LastRequestCount = SettingLoader.GetValue("View last requests count", 20);
            Downloadfolder = SettingLoader.GetValue("Download folder", System.Environment.GetFolderPath(System.Environment.SpecialFolder.Personal));
            MIMETypes = SettingLoader.GetMIMETypes();
            TextEncoding = SettingLoader.GetTextEncoding();
        }
    }
    public class SettingLoader
    {
        public string this[string name]
        {
            get
            {
                try
                {
                    return (string)Application.UserAppDataRegistry.GetValue(name);
                }
                catch (Exception)
                {
                    return null;
                }
            }
            set
            {
                try
                {
                    Application.UserAppDataRegistry.SetValue(name, value);
                }
                catch (Exception)
                {
                }
            }
        }
        // get functions
        public static string GetValue(string name)
        {
            return (string)Application.UserAppDataRegistry.GetValue(name);
        }
        public static string GetValue(string name, string defaultValue)
        {
            return (string)Application.UserAppDataRegistry.GetValue(name, defaultValue);
        }
        public static int GetValue(string name, int defaultValue)
        {
            string str = GetValue(name, defaultValue.ToString());
            if (str != "")
                return int.Parse(str);
            return -1;
        }
        public static bool GetValue(string name, bool defaultValue)
        {
            return GetValue(name, defaultValue.ToString()).ToLower() == "true";
        }
        // set functions
        public static void SetValue(string name, string value)
        {
            Application.UserAppDataRegistry.SetValue(name, value);
        }
        public static void SetValue(string name, bool value)
        {
            SetValue(name, value.ToString());
        }

        public static void SetValue(Form form)
        {
            Control ctrl = null;
            while ((ctrl = form.GetNextControl(ctrl, true)) != null)
                SettingLoader.SetValue(ctrl);
        }
        public static void SetValue(Control ctrl)
        {
            string Name = ctrl.FindForm().Name + '-' + ctrl.Name;
            if (ctrl.Tag != null && ctrl.Tag.ToString().Trim().Length > 0)
                switch (ctrl.GetType().Name)
                {
                    case "TextBox":
                        SettingLoader.SetValue((string)ctrl.Tag, ctrl.Text);
                        break;
                    case "CheckBox":
                        CheckBox check = (CheckBox)ctrl;
                        SettingLoader.SetValue((string)ctrl.Tag, check.Checked);
                        break;
                    case "RadioButton":
                        RadioButton radio = (RadioButton)ctrl;
                        SettingLoader.SetValue((string)ctrl.Tag, radio.Checked);
                        break;
                    case "NumericUpDown":
                        NumericUpDown numeric = (NumericUpDown)ctrl;
                        SettingLoader.SetValue((string)ctrl.Tag, numeric.Value.ToString());
                        break;
                    case "TabControl":
                        TabControl tab = (TabControl)ctrl;
                        SettingLoader.SetValue((string)ctrl.Tag, tab.SelectedIndex.ToString());
                        break;
                    case "ComboBox":
                        ComboBox combo = (ComboBox)ctrl;
                        try
                        {
                            string fileName = null;
                            XmlDocument doc = null;
                            XmlNode element = GetCtrlNode(ctrl, ref fileName, ref doc);
                            element.RemoveAll();
                            foreach (string item in combo.Items)
                            {
                                XmlNode node = doc.CreateNode(XmlNodeType.Element, "Item", "");
                                node.InnerText = item;
                                element.AppendChild(node);
                            }
                            doc.Save(fileName);
                        }
                        catch (XmlException)
                        {
                        }
                        SettingLoader.SetValue(Name, combo.Text);
                        break;
                    case "ListView":
                        ListView list = (ListView)ctrl;
                        try
                        {
                            string fileName = null;
                            XmlDocument doc = null;
                            XmlNode element = GetCtrlNode(ctrl, ref fileName, ref doc);
                            element.RemoveAll();
                            foreach (ListViewItem viewItem in list.Items)
                            {
                                XmlNode node = doc.CreateNode(XmlNodeType.Element, "Item", "");
                                if (list.CheckBoxes == true)
                                {
                                    XmlAttribute attribute = doc.CreateAttribute("Checked");
                                    attribute.Value = viewItem.Checked.ToString();
                                    node.Attributes.Append(attribute);
                                }
                                string InnerText = "";
                                foreach (ListViewItem.ListViewSubItem subItem in viewItem.SubItems)
                                    InnerText += subItem.Text + '\t';
                                node.InnerText = InnerText.TrimEnd('\t');
                                element.AppendChild(node);
                            }
                            doc.Save(fileName);
                        }
                        catch (XmlException)
                        {
                        }
                        break;
                }
        }

        static XmlNode GetCtrlNode(Control ctrl)
        {
            string fileName = null;
            XmlDocument doc = null;
            return GetCtrlNode(ctrl, ref fileName, ref doc);
        }

        static XmlNode GetCtrlNode(Control ctrl, ref string fileName, ref XmlDocument doc)
        {
            fileName = Application.StartupPath + '\\' + (string)ctrl.Tag + ".xml";
            doc = new XmlDocument();
            XmlNode node = null;
            string Name = ctrl.FindForm().Name + '-' + ctrl.Name;
            if (File.Exists(fileName))
            {
                doc.Load(fileName);
                node = doc.DocumentElement.SelectSingleNode(Name);
            }
            else
                doc.LoadXml("<Settings>\r\n</Settings>");
            if (node == null)
            {
                node = doc.CreateNode(XmlNodeType.Element, Name, "");
                doc.DocumentElement.AppendChild(node);
            }
            return node;
        }

        public static void GetValue(Form form)
        {
            Control ctrl = null;
            while ((ctrl = form.GetNextControl(ctrl, true)) != null)
                SettingLoader.GetValue(ctrl);
        }
        public static void GetValue(Control ctrl)
        {
            string Name = ctrl.FindForm().Name + '-' + ctrl.Name;
            if (ctrl.Tag != null && ctrl.Tag.ToString().Trim().Length > 0)
                switch (ctrl.GetType().Name)
                {
                    case "TextBox":
                        ctrl.Text = SettingLoader.GetValue((string)ctrl.Tag, ctrl.Text);
                        break;
                    case "CheckBox":
                        CheckBox check = (CheckBox)ctrl;
                        check.Checked = SettingLoader.GetValue((string)ctrl.Tag, check.Checked);
                        break;
                    case "RadioButton":
                        RadioButton radio = (RadioButton)ctrl;
                        radio.Checked = SettingLoader.GetValue((string)ctrl.Tag, radio.Checked);
                        break;
                    case "NumericUpDown":
                        NumericUpDown numeric = (NumericUpDown)ctrl;
                        numeric.Value = decimal.Parse(SettingLoader.GetValue((string)ctrl.Tag, numeric.Value.ToString()));
                        break;
                    case "TabControl":
                        TabControl tab = (TabControl)ctrl;
                        tab.SelectedIndex = int.Parse(SettingLoader.GetValue((string)ctrl.Tag, tab.SelectedIndex.ToString()));
                        break;
                    case "ComboBox":
                        ComboBox combo = (ComboBox)ctrl;
                        try
                        {
                            XmlNode element = GetCtrlNode(ctrl);
                            if (element != null && element.ChildNodes.Count > 0)
                            {
                                combo.Items.Clear();
                                foreach (XmlNode node in element.ChildNodes)
                                    combo.Items.Add(node.InnerText);
                            }
                        }
                        catch (XmlException)
                        {
                        }
                        combo.Text = SettingLoader.GetValue(Name, combo.Text);
                        break;
                    case "ListView":
                        ListView list = (ListView)ctrl;
                        try
                        {
                            XmlNode element = GetCtrlNode(ctrl);
                            if (element != null && element.ChildNodes.Count > 0)
                            {
                                list.Items.Clear();
                                foreach (XmlNode node in element.ChildNodes)
                                {
                                    string[] items = node.InnerText.Split('\t');
                                    ListViewItem viewItem = list.Items.Add(items[0]);
                                    for (int nIndex = 1; nIndex < items.Length; nIndex++)
                                        viewItem.SubItems.Add(items[nIndex]);
                                    if (list.CheckBoxes == true)
                                    {
                                        XmlAttribute attribute = node.Attributes["Checked"];
                                        if (attribute != null)
                                            viewItem.Checked = attribute.Value.ToLower() == "true";
                                    }
                                }
                            }
                        }
                        catch (XmlException)
                        {
                        }
                        break;
                }
        }

        // construct MIME types string from settings XML file
        public static string GetMIMETypes()
        {
            string str = "";
            // check for settings XML file existence
            if (File.Exists(Application.StartupPath + "\\Settings.xml"))
            {
                XmlDocument doc = new XmlDocument();
                doc.Load(Application.StartupPath + "\\Settings.xml");
                XmlNode element = doc.DocumentElement.SelectSingleNode("SettingsForm-listViewFileMatches");
                if (element != null)
                {
                    for (int n = 0; n < element.ChildNodes.Count; n++)
                    {
                        XmlNode xmlnode = element.ChildNodes[n];
                        XmlAttribute attribute = xmlnode.Attributes["Checked"];
                        if (attribute == null || attribute.Value.ToLower() != "true")
                            continue;
                        string[] items = xmlnode.InnerText.Split('\t');
                        if (items.Length > 1)
                        {
                            str += items[0];
                            if (items.Length > 2)
                                str += '[' + items[1] + ',' + items[2] + ']';
                            str += ';';
                        }
                    }
                }
            }
            return str;
        }

        // construct encoding from registry.
        public static Encoding GetTextEncoding()
        {
            Encoding code;
            if (SettingLoader.GetValue("Use windows default code page", true) == true)
                code = Encoding.Default;
            else
            {
                string strCodePage = SettingLoader.GetValue("Settings code page");
                Regex reg = new Regex(@"\([0-9]*\)");
                strCodePage = reg.Match(strCodePage).Value;
                code = Encoding.GetEncoding(int.Parse(strCodePage.Trim('(', ')')));
            }
            return code;
        }
    }
}
