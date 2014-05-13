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
            FileStream fStream = new FileStream("json.txt", FileMode.OpenOrCreate);
            fStream.Position = 0;
            StreamWriter sWriter = new StreamWriter(fStream);
            sWriter.Write(jsonStr);
            sWriter.Close();
            fStream.Close();

            jsonStr = "";
            fStream = new FileStream("json.txt", FileMode.Open);
            StreamReader sReader = new StreamReader(fStream);
            jsonStr = sReader.ReadToEnd();
            sReader.Close();

            MyReadObject(jsonStr);
            Console.ReadLine();
        }

        static string MyWriteOjbect()
        {
            var config = new Config()
            {
                encoding_ = "UTF-8",
                plugins_ = new string[] { "python", "C++", "C#" },
                indent_ = new Indent() { length_ = 4, useSpace_ = false, strTest_ = "1234123412341" }
            };

            var serializer = new DataContractJsonSerializer(typeof(Config));
            var stream = new MemoryStream();
            serializer.WriteObject(stream, config);
            byte[] dataBytes = new byte[stream.Length];
            stream.Position = 0;
            stream.Read(dataBytes, 0, (int)stream.Length);
            string dataString = Encoding.UTF8.GetString(dataBytes);

            Console.WriteLine();
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
            int nPluginCount = readConfig.plugins_.Length;
            foreach (string plugin in readConfig.plugins_)
            {
                Console.WriteLine("plugins: {0}", plugin);
            }
            
            Console.WriteLine("indent.useSpace_: {0}", readConfig.indent_.useSpace_);
            Console.WriteLine("indent.test_: {0}", readConfig.indent_.strTest_);
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

        private string strTest;
        [DataMember(Order = 2)]
        public string strTest_
        {
            get { return strTest; }
            set { strTest = value; }
        }

        public Indent()
        {
            length_ = -1;
            useSpace_ = false;
            strTest_ = "default Indent test string";
        }
    }
}
