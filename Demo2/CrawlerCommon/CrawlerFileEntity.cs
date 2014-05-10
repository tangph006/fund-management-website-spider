using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.IO;

namespace CrawlerCommon
{
    [DataContract]
    abstract public class CrawlerFileEntity
    {
        // file path saving the Json string.
        [DataMember(Order = 0)]
        private string filePath;
        public string filePath_
        {
            get { return filePath; }
            set { filePath = value; }
        }

        // version of this data
        [DataMember(Order = 1)]
        private int version;
        public int version_
        {
            get { return version; }
            set { version = value; }
        }

        // data name
        [DataMember(Order = 2)]
        private string dataTypeName;
        public string dataTypeName_
        {
            get { return dataTypeName; }
            set { dataTypeName = value; }
        }

        // time of last modified
        [DataMember(Order = 3)]
        private DateTime lastModified;
        public DateTime lastModified_
        {
            get { return lastModified; }
            set { lastModified = value; }
        }

        // translate 'this' to Json string
        abstract public string ToJsonString();

        // get instance from Json string
        abstract public void FromJsonString(string jsonString);

        // write to file
        public void WriteToFile()
        {
            lastModified_ = DateTime.Now;
            string jsonString = ToJsonString();
            try
            {
                FileStream fStream = new FileStream(filePath_, FileMode.OpenOrCreate);
                fStream.Position = 0;
                StreamWriter sWriter = new StreamWriter(fStream);
                sWriter.Write(jsonString);
                sWriter.Close();
                fStream.Close();
            }
            catch (System.Exception ex)
            {
                // TODO: error handling.
            }
        }

        // read from file
        public void ReadFromFile()
        {
            var fStream = new FileStream(filePath_, FileMode.Open);
            StreamReader sReader = new StreamReader(fStream);
            string jsonString = sReader.ReadToEnd();
            sReader.Close();
            fStream.Close();

            FromJsonString(jsonString);
        }

        // operator = , copy from
        public CrawlerFileEntity CopyFrom(CrawlerFileEntity other)
        {
            filePath_ = other.filePath_;
            version_ = other.version_;
            dataTypeName_ = other.dataTypeName_;
            lastModified_ = other.lastModified_;
            return this;
        }
    }
}
