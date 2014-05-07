using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.IO;

namespace JsonSample2
{
    class Program
    {
        static void Main(string[] args)
        {
            string jsonStr = MyWriteOjbect();
            Console.ReadLine();
            var config = new Config()
            {
                encoding_ = "UTF-8",
                plugins_ = new string[] { "python", "C++", "C#" },
                indent_ = new Indent() { length_ = 4, useSpace_ = false, test_ = 10000 }
            };
            string strTest = config.ToString();
        }

        static string MyWriteOjbect()
        {
            var config = new Config()
            {
                encoding_ = "UTF-8",
                plugins_ = new string[] { "python", "C++", "C#" },
                indent_ = new Indent() { length_ = 4, useSpace_ = false, test_ = 10000 }
            };
            var serializer = new DataContractJsonSerializer(typeof(Config));
            var stream = new MemoryStream();
            serializer.WriteObject(stream, config);

            byte[] dataBytes = new byte[stream.Length];

            stream.Position = 0;

            stream.Read(dataBytes, 0, (int)stream.Length);
            string dataString = Encoding.UTF8.GetString(dataBytes);
            Console.WriteLine("JSON string is:");
            Console.WriteLine(dataString);
            return dataString;
        }

        static void MyReadObject(string dataString)
        {
            var mStream = new MemoryStream(Encoding.Default.GetBytes(dataString));
            var serializer = new DataContractJsonSerializer(typeof(Config));
            Config readConfig = (Config)serializer.ReadObject(mStream);

            Console.WriteLine("encoding_: {0}", readConfig.encoding_);
            Console.WriteLine("indent.length_: {0}", readConfig.indent_.length_);
            foreach (string plugin in readConfig.plugins_)
            {
                Console.WriteLine("plugins: {0}", plugin);
            }
            
            Console.WriteLine("indent.useSpace_: {0}", readConfig.indent_.useSpace_);
            Console.WriteLine("indent.test_: {0}", readConfig.indent_.test_);
        }
    }

    [DataContract]
    class Config
    {
        private string encoding;
        [DataMember(Order = 0)]
        public string encoding_
        {
            get { return encoding; }
            set { encoding = value; }
        }

        private string[] plugins;
        [DataMember(Order = 1)]
        public string[] plugins_
        {
            get { return plugins; }
            set { plugins = value; }
        }

        private Indent indent;
        [DataMember(Order = 2)]
        public Indent indent_
        {
            get { return indent; }
            set { indent = value; }
        }
    }

    [DataContract]
    class Indent
    {
        private int length;
        [DataMember(Order = 0)]
        public int length_
        {
            get { return length; }
            set { length = value; }
        }

        private bool useSpace;
        [DataMember(Order = 1)]
        public bool useSpace_
        {
            get { return useSpace; }
            set { useSpace = value; } 
        }

        private int test;
        [DataMember(Order = 1)]
        public int test_
        {
            get { return test; }
            set { test = value; }
        }
    }
}
