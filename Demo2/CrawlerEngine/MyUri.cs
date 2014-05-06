using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Engine
{
    public class MyUri : System.Uri
    {
        // parsing depth limit
        private int nDepth;
        public int Depth
        {
            get { return nDepth; }
            set { nDepth = value; }
        }

        // 0: not handled; 1: handle_successed; 2: handle_failed; 3: other
        private int nState;
        public int State
        {
            get { return nState; }
            set { nState = value; }
        }

        // domain name
        private string strDomain;
        public string Domain
        {
            get { return strDomain; }
            set { strDomain = value; }
        }

        public MyUri(string uriString) : base(uriString) { }
    }
}
