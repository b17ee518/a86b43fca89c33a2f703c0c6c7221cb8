using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Nekoxy;

namespace NekoxyExample
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            HttpProxy proxy = new HttpProxy();
            proxy.Shutdown();
            //            proxy.UpstreamProxyConfig = new ProxyConfig(ProxyConfigType.SpecificProxy, "127.0.0.1", 8888);
            proxy.Startup(12345, false, false);
            proxy.AfterReadRequestHeaders += r => Task.Run(() => Console.WriteLine(r));
            proxy.AfterReadResponseHeaders += r => Task.Run(() => Console.WriteLine(r));
            proxy.AfterSessionComplete += s => Task.Run(() => Debug.WriteLine(s));
            //proxy.AfterSessionComplete += s => Task.Run(() => Debug.WriteLine(s.Response.BodyAsString));
            while (true) System.Threading.Thread.Sleep(1000);
        }
    }
}
