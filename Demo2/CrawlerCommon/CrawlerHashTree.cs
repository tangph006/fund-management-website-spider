using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace CrawlerCommon
{
    /// <summary>
    /// Summary description for HashTree.
    /// </summary>
    public class HashTree
    {
        public HashTreeNode Root;
        public bool Modified;
        public int Count;

        public HashTree()
        {
        }
        public void Clear()
        {
            Root = null;
            Count = 0;
            Modified = false;
        }
        public bool Add(ref string str)
        {
            int Code = str.GetHashCode();
            HashTreeNode node;
            if (Root == null)
            {
                Root = new HashTreeNode();
                node = Root;
            }
            else
            {
                node = Root;
                while (true)
                {
                    if (node.Code == Code)
                        return false;
                    if (Code < node.Code)
                    {	// add the node at the small branch
                        if (node.Small == null)
                        {
                            node.Small = new HashTreeNode();
                            node = node.Small;
                            break;
                        }
                        node = node.Small;
                    }
                    else
                    {	// add the node at the great branch
                        if (node.Great == null)
                        {
                            node.Great = new HashTreeNode();
                            node = node.Great;
                            break;
                        }
                        node = node.Great;
                    }
                }
            }
            node.Code = Code;
            this.Modified = true;
            this.Count++;

            return true;
        }

        public void Open(string fileName, System.Text.Encoding code)
        {
            FileStream stream = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
            StreamReader reader = new StreamReader(stream, code);

            string str;
            while ((str = reader.ReadLine()) != null)
            {
                string[] strCols = str.Split('\t');
                if (strCols.Length > 0)
                    this.Add(ref strCols[0]);
            }
            reader.Close();
            stream.Close();
        }
    }

    public class HashTreeNode
    {
        public HashTreeNode Small;
        public HashTreeNode Great;
        public int Code;
    }
}
