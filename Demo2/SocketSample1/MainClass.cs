using System;
using System.Text;
using System.IO;
using System.Net;
using System.Net.Sockets;

public class MainClass
{
    private static Socket ConnectSocket(string serverName, int port)
    {
        Socket s = null;
        IPHostEntry hostEntry = null;

        // Get host related information.
        hostEntry = Dns.GetHostEntry(serverName);

        // Loop through the AddressList to obtain the supported AddressFamily. This is to avoid 
        // an exception that occurs when the host IP Address is not compatible with the address family 
        // (typical in the IPv6 case). 
        foreach (IPAddress address in hostEntry.AddressList)
        {
            IPEndPoint ipe = new IPEndPoint(address, port);
            Socket tempSocket =
                new Socket(ipe.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

            tempSocket.Connect(ipe);

            if (tempSocket.Connected)
            {
                s = tempSocket;
                break;
            }
        }
        return s;
    }

    // This method requests the home page content for the specified server. 
    private static string SocketSendReceive(string serverName, int port)
    {
        string request = "GET / HTTP/1.1\r\nHost: " + serverName + "\r\nConnection: Close\r\n\r\n";
        Byte[] bytesSent = Encoding.ASCII.GetBytes(request);
        Byte[] bytesReceived = new Byte[256];

        // Create a socket connection with the specified server and port.
        Socket s = ConnectSocket(serverName, port);

        if (s == null)
            return ("Connection failed");

        // Send request to the server.
        s.Send(bytesSent, bytesSent.Length, 0);

        // Receive the server home page content. 
        int bytes = 0;
        string page = "Default HTML page on " + serverName + ":\r\n";

        // The following will block until te page is transmitted. 
        do
        {
            bytes = s.Receive(bytesReceived, bytesReceived.Length, 0);
            page = page + Encoding.UTF8.GetString(bytesReceived, 0, bytes);
        }
        while (bytes > 0);

        return page;
    }

    public static void Main(string[] args)
    {
        string host;
        int port = 80;

        if (args.Length == 0)
            // If no server name is passed as argument to this program,  
            // use the current host name as the default.
            host = Dns.GetHostName();
        else
            host = args[0];

        string result = SocketSendReceive(host, port);
        FileStream fs = new FileStream(@"SocketSample1.txt", FileMode.OpenOrCreate);
        StreamWriter sw = new StreamWriter(fs);
        sw.WriteLine();

        sw.Write(result);
        sw.Close();
        fs.Close();
    }
}