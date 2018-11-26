using System;
using System.Text;
using System.Net.Sockets;
using bjs;
using bjs.net;

namespace bjs.link
{
    public enum LogType
    {
        Information,
        Warning,
        Error,
        CriticalError,
    }

    public static class Link
    {
        public static int BrewPort = 42760;

        public static TcpClient tcp;

        public static void Log(string LogText, LogType Type, bool NewLine = true)
        {
            Console.ForegroundColor = ConsoleColor.Gray;
            Console.Write("<");
            switch(Type)
            {
                case LogType.Error:
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.Write("Error");
                    break;
                case LogType.Information:
                    Console.ForegroundColor = ConsoleColor.Green;
                    Console.Write("Log");
                    break;
                case LogType.Warning:
                    Console.ForegroundColor = ConsoleColor.Yellow;
                    Console.Write("Warn");
                    break;
            }
            
            Console.ForegroundColor = ConsoleColor.Gray;
            Console.Write("> ");
            Console.ForegroundColor = ConsoleColor.White;
            if(Type == LogType.CriticalError)
            {

            }
            if(NewLine) Console.WriteLine(LogText);
            else Console.Write(LogText);
        }

        public static void CheckReads()
        {
            NetworkStream nets = tcp.GetStream();
            if(nets.DataAvailable)
            {
                byte[] rbuf = new byte[4096];
                StringBuilder sb = new StringBuilder();
                int read = 0;
                do
                {
                    read = nets.Read(rbuf, 0, rbuf.Length);
                    sb.AppendFormat("{0}", Encoding.UTF8.GetString(rbuf, 0, read));
                } while(nets.DataAvailable);
                string data = sb.ToString();
                EvaluationResult ev = EvaluationResultParser.Parse(data);
                Console.WriteLine();
                Console.WriteLine(ev.ToString());
            }
        }

        public static void Main(string[] Arguments)
        {
            try
            {
                Console.Title = "Brew.js link - Windows client";
                Log("Which is the Nintendo Switch's IP address? ", LogType.Information, false);
                string ipadd = Console.ReadLine();
                tcp = new TcpClient();
                Log("Connecting to Brew.js link homebrew with IP address " + ipadd + "...", LogType.Information);
                tcp.Connect(ipadd, BrewPort);
                Log("Connected to Brew.js link homebrew interpreter!", LogType.Information);
                while(true)
                {
                    NetworkStream nets = tcp.GetStream();
                    Console.WriteLine();
                    Console.ForegroundColor = ConsoleColor.Gray;
                    Console.Write("> ");
                    Console.ForegroundColor = ConsoleColor.White;
                    string cmdlinetext = Console.ReadLine();
                    Console.ForegroundColor = ConsoleColor.Gray;
                    if(string.IsNullOrEmpty(cmdlinetext)) continue;
                    byte[] cmdlinebytes = Encoding.ASCII.GetBytes(cmdlinetext);
                    nets.Write(cmdlinebytes, 0, cmdlinebytes.Length);
                    while(!nets.DataAvailable);
                    byte[] rbuf = new byte[4096];
                    StringBuilder sb = new StringBuilder();
                    int read = 0;
                    do
                    {
                        read = nets.Read(rbuf, 0, rbuf.Length);
                        sb.AppendFormat("{0}", Encoding.UTF8.GetString(rbuf, 0, read));
                    } while(nets.DataAvailable);
                    string data = sb.ToString();
                    EvaluationResult ev = EvaluationResultParser.Parse(data);
                    Console.WriteLine();
                    Console.WriteLine(ev.ToString());
                    if(!ev.IsOK)
                    {

                    }
                    else
                    {
                        Console.WriteLine();
                        Console.WriteLine(ev.ToString());
                    };
                }
            }
            catch(Exception ex)
            {
                Console.Clear();
                Log("An error happened (" + ex.GetType().ToString() + ")", LogType.Error);
                Console.ForegroundColor = ConsoleColor.Gray;
                Console.Write(" - ");
                Console.ForegroundColor = ConsoleColor.White;
                Console.WriteLine("Press any key to exit.");
                Console.ReadKey();
            }
        }
    }
}
