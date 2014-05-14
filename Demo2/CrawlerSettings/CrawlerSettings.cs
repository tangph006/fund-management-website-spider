using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Text.RegularExpressions;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;

namespace Settings
{
    [DataContract]
    public class CrawlerSettings : CrawlerCommon.CrawlerFileEntity
    {
		// MIME types string
        [DataMember(Order = 0)]
		private string MIMETypes;// = SettingLoader.GetMIMETypes();
		public string MIMETypes_
		{
			get	{	return MIMETypes;	}
			set	{	MIMETypes = value;	}
		}

		// encoding text that includes all settings types in one string
        [DataMember(Order = 1)]
        private Encoding encoding;// = SettingLoader.GetTextEncoding();
        public Encoding encoding_
		{
			get	{	return encoding;	}
			set	{	encoding = value;	}
		}

		// timeout of sockets send and receive
        [DataMember(Order = 2)]
		private int requestTimeout;
        public int requestTimeout_
		{
            get { return requestTimeout; }
            set { requestTimeout = value; }
		}

		// the time that each thread sleeps when the refs queue empty
        [DataMember(Order = 3)]
		private int sleepFetchTime;
        public int sleepFetchTime_
		{
            get { return sleepFetchTime; }
            set { sleepFetchTime = value; }
		}		
		
		// List of a user defined list of restricted words to enable user to prevent any bad pages
        [DataMember(Order = 4)]
		private string[] excludeWords;
        public string[] excludeWords_
		{
            get { return excludeWords; }
            set { excludeWords = value; }
		}

		// List of a user defined list of restricted files extensions to avoid paring non-text data 
        [DataMember(Order = 5)]
		private string[] excludeFiles;
        public string[] excludeFiles_
		{
            get { return excludeFiles; }
            set { excludeFiles = value; }
		}

		// List of a user defined list of restricted hosts extensions to avoid blocking by these hosts
        [DataMember(Order = 6)]
		private string[] excludeHosts;
        public string[] excludeHosts_
		{
            get { return excludeHosts; }
            set { excludeHosts = value; }
		}
		
		// the number of requests to keep in the requests view for review requests details
        [DataMember(Order = 7)]
		private int lastRequestCount;
        public int lastRequestCount_
		{
            get { return lastRequestCount; }
            set { lastRequestCount = value; }
		}
		
		// the time that each thread sleep after handling any request, 
		// which is very important value to prevent Hosts from blocking the crawler due to heavy load
        [DataMember(Order = 8)]
		private int sleepConnectTime;
        public int sleepConnectTime_
		{
            get { return sleepConnectTime; }
            set { sleepConnectTime = value; }
		}

		// represents the depth of navigation in the crawling process
        [DataMember(Order = 9)]
		private int webDepth;
        public int webDepth_
		{
            get { return webDepth; }
            set { webDepth = value; }
		}

		// MIME types are the types that are supported to be downloaded by the crawler 
		// and the crawler includes a default types to be used. 
        [DataMember(Order = 10)]
		private bool allMIMETypes;
        public bool allMIMETypes_
		{
            get { return allMIMETypes; }
            set { allMIMETypes = value; }
		}		

		// to limit crawling process to the same host of the original URL
        [DataMember(Order = 11)]
		private bool keepSameServer;
        public bool keepSameServer_
		{
            get { return keepSameServer; }
            set { keepSameServer = value; }
		}		
		
		// means keep socket connection opened for subsequent requests to avoid reconnect time
        [DataMember(Order = 12)]
		private bool keepAlive;
        public bool keepAlive_
		{
            get { return keepAlive; }
            set { keepAlive = value; }
		}			

        // download folder
        [DataMember(Order = 13)]
        private string downloadfolder;
        public string downloadfolder_
        {
            get { return downloadfolder; }
            set
            {
                downloadfolder = value;
                downloadfolder = downloadfolder.TrimEnd('\\');
            }
        }

        // set the threads count when engine is running
        [DataMember(Order = 14)]
        private int threadsCount;
        public int threadsCount_
        {
            get { return threadsCount; }
            set { threadsCount = value; }
        }

        // default constructor
        public CrawlerSettings()
        {
            this.dataTypeName_ = "CrawlerSettings";
            this.version_ = 1;
            this.filePath_ = "CrawlerSettings.json";
        }

        // generate an instance from Json string
        override public void FromJsonString(string jsonString)
        {
            var mStream = new MemoryStream(Encoding.Default.GetBytes(jsonString));
            var serializer = new DataContractJsonSerializer(typeof(CrawlerSettings));
            CopyFrom((CrawlerSettings)serializer.ReadObject(mStream));
        }

        // translate 'this' to Json string
        override public string ToJsonString()
        {
            try
            {
                var serializer = new DataContractJsonSerializer(typeof(CrawlerSettings));
                var stream = new MemoryStream();
                serializer.WriteObject(stream, this);
                byte[] dataBytes = new byte[stream.Length];
                stream.Position = 0;
                stream.Read(dataBytes, 0, (int)stream.Length);
                string jsonString = Encoding.UTF8.GetString(dataBytes);
                return jsonString;
            }
            catch (System.Exception ex)
            {
                // TODO: error handling.
                return "";
            }
        }

        // copy from
        public CrawlerSettings CopyFrom(CrawlerSettings other)
        {
            base.CopyFrom((CrawlerCommon.CrawlerFileEntity)other);
            MIMETypes_ = other.MIMETypes_;
            encoding_ = other.encoding_;
            requestTimeout_ = other.requestTimeout_;
            sleepFetchTime_ = other.sleepFetchTime_;
            excludeWords_ = other.excludeWords_;
            excludeFiles_ = other.excludeFiles_;
            excludeHosts_ = other.excludeHosts_;
            lastRequestCount_ = other.lastRequestCount_;
            sleepConnectTime_ = other.sleepConnectTime_;
            webDepth_ = other.webDepth_;
            keepSameServer_ = other.keepSameServer_;
            keepAlive_ = other.keepAlive_;
            downloadfolder_ = other.downloadfolder_;
            return this;
        }
    }
}
