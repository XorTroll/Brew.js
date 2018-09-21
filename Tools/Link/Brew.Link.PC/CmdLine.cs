using System;
using System.Text;
using System.Net.Sockets;

namespace Brew.Link.PC
{
    public enum LogType
    {
        Information,
        Warning,
        Error
    }

    public static class CmdLine
    {
        public static int BrewPort = 42760;

        public static void log(string LogText, LogType Type, bool NewLine = true)
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
            if(NewLine) Console.WriteLine(LogText);
            else Console.Write(LogText);
        }

        public static void Main(string[] Arguments)
        {
            try
            {
                Console.Title = "Brew.js link - PC client";
                log("Which is the Nintendo Switch's IP address? ", LogType.Information, false);
                string ipadd = Console.ReadLine();
                TcpClient tcp = new TcpClient();
                log("Connecting to Brew.js link homebrew with IP address " + ipadd + "...", LogType.Information);
                tcp.Connect(ipadd, BrewPort);
                log("Connected to Brew.js link homebrew interpreter!", LogType.Information);
                while(true)
                {
                    Console.WriteLine();
                    Console.ForegroundColor = ConsoleColor.Gray;
                    Console.Write("> ");
                    Console.ForegroundColor = ConsoleColor.White;
                    string cmdlinetext = Console.ReadLine();
                    Console.ForegroundColor = ConsoleColor.Gray;
                    if(string.IsNullOrEmpty(cmdlinetext)) continue;
                    byte[] cmdlinebytes = Encoding.ASCII.GetBytes(cmdlinetext);
                    tcp.GetStream().Write(cmdlinebytes, 0, cmdlinebytes.Length);
                }
            }
            catch(Exception ex)
            {
                log(ex.Message, LogType.Error);
                Console.ForegroundColor = ConsoleColor.Gray;
                Console.Write(" - ");
                Console.ForegroundColor = ConsoleColor.White;
                Console.WriteLine("Press any key to exit.");
                Console.ReadKey();
            }
        }
    }
}
