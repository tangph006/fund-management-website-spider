using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.IO;
using CrawlerCommon;

namespace Input
{
    public class CrawlerInput : CrawlerCommon.CrawlerFileEntity
    {
        // the url going to parse
        private string fullUrl;
        public string fullUrl_
        {
            get { return fullUrl; }
            set { fullUrl = value; }
        }

        // domain name
        private string domain;
        public string domain_
        {
            get { return domain; }
            set { domain = value; }
        }
        
        // default constructor
        public CrawlerInput()
        {
            this.dataTypeName_ = "CrawlerInput";
            this.version_ = 1;
            this.filePath_ = "CrawlerInput.json";
        }

        // generate an instance from Json string
        override public void FromJsonString(string jsonString)
        {
            var mStream = new MemoryStream(Encoding.Default.GetBytes(jsonString));
            var serializer = new DataContractJsonSerializer(typeof(CrawlerInput));
            CopyFrom((CrawlerInput)serializer.ReadObject(mStream));
        }

        // translate 'this' to Json string
        override public string ToJsonString()
        {
            var serializer = new DataContractJsonSerializer(typeof(CrawlerInput));
            try
            {
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
        public CrawlerInput CopyFrom(CrawlerInput other)
        {
            base.CopyFrom(other);
            domain_ = other.domain_;
            fullUrl_ = other.fullUrl_;
            return this;
        }
    }
}
