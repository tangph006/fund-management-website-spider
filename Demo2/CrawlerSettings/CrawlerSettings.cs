using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Text.RegularExpressions;
using System.Runtime.Serialization.Json.DataContractJsonSerializer;

namespace Engine
{
    public class CrawlerSettings
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
        public CrawlerSettings()
        {
            InitValues();
        }

        // construct all.
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
}
