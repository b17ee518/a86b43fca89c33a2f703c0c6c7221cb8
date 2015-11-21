using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Security;
using System.Net.Sockets;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using Titanium.Web.Proxy.EventArguments;
using Titanium.Web.Proxy.Helpers;
using System.Runtime.InteropServices;

namespace Titanium.Web.Proxy
{
    [ComVisible(true)]
    public interface ITitaniumClass
    {
        [ComVisible(true)]
        void SetOnNotification(int f);
        [ComVisible(true)]
        void SetBeforeRequest(int f);
        [ComVisible(true)]
        void SetBeforeResponse(int f);
        [ComVisible(true)]
        void SetOnLogString(int f);
        [ComVisible(true)]
        void SetAfterSessionComplete(int f);
        [ComVisible(true)]
        void Startup(int listeningPort);
        [ComVisible(true)]
        void Shutdown();
        [ComVisible(true)]
        void SetMakecertPath(string path);
    }
    /// <summary>
    ///     Proxy Server Main class
    /// </summary>
    [ComVisible(true)]
    [ClassInterface(ClassInterfaceType.None)]
    public partial class ProxyServer : ITitaniumClass
    {
        [ComVisible(false)]
        public delegate void OnNotificationFunc(string notifystring);
        [ComVisible(false)]
        public delegate void BeforeRequestFunc(int sessionID, string fullURL, string requestBody);
        [ComVisible(false)]
        public delegate void BeforeResponseFunc(int sessionID, string mimeType, int responseCode, string PathAndQuery, string requestBody, string responseBody);
        [ComVisible(false)]
        public delegate void OnLogStringFunc(string logstr);
        [ComVisible(false)]
        public delegate void AfterSessionCompleteFunc(int sessionID, string mimeType, int responseCode, string PathAndQuery, string requestBody, string responseBody);

        private OnNotificationFunc _onNotificationFunc;
        private BeforeRequestFunc _beforeRequestFunc;
        private BeforeResponseFunc _beforeResponseFunc;
        private OnLogStringFunc _onLogStringFunc;
        private AfterSessionCompleteFunc _afterSessionCompleteFunc;

        [System.Runtime.InteropServices.ComVisible(true)]
        public void SetOnNotification(int f)
        {
            _onNotificationFunc = (OnNotificationFunc)Marshal.GetDelegateForFunctionPointer((IntPtr)f, typeof(OnNotificationFunc));
        }
        [System.Runtime.InteropServices.ComVisible(true)]
        public void SetBeforeResponse(int f)
        {
            _beforeResponseFunc = (BeforeResponseFunc)Marshal.GetDelegateForFunctionPointer((IntPtr)f, typeof(BeforeResponseFunc));
        }
        [System.Runtime.InteropServices.ComVisible(true)]
        public void SetBeforeRequest(int f)
        {
            _beforeRequestFunc = (BeforeRequestFunc)Marshal.GetDelegateForFunctionPointer((IntPtr)f, typeof(BeforeRequestFunc));
        }

        [System.Runtime.InteropServices.ComVisible(true)]
        public void SetOnLogString(int f)
        {
            _onLogStringFunc = (OnLogStringFunc)Marshal.GetDelegateForFunctionPointer((IntPtr)f, typeof(OnLogStringFunc));
        }
        [System.Runtime.InteropServices.ComVisible(true)]
        public void SetAfterSessionComplete(int f)
        {
            _afterSessionCompleteFunc = (AfterSessionCompleteFunc)Marshal.GetDelegateForFunctionPointer((IntPtr)f, typeof(AfterSessionCompleteFunc));
            AfterSessionComplete += ProxyServer_AfterSessionComplete;
        }

        void ProxyServer_AfterSessionComplete(object sender, SessionEventArgs e)
        {
            if (_afterSessionCompleteFunc != null)
            {
                _afterSessionCompleteFunc(0, 
                    e.ResponseContentType?? "", 
                    (int)e.ResponseStatusCode, 
                    e.PathAndQuery?? "",
                     e.RequestBodyString??"", 
                     e.ResponseBodyString??"");
            }
        }

        public static string MakecertPath { get; set; }
        [ComVisible(true)]
        public void SetMakecertPath(string path)
        {
            MakecertPath = path;
        }

        [StructLayout(LayoutKind.Sequential)]
        private class INTERNET_PROXY_INFO
        {
            public uint dwAccessType;
            [MarshalAs(UnmanagedType.LPStr)]
            public string lpszProxy;
            [MarshalAs(UnmanagedType.LPStr)]
            public string lpszProxyBypass;
        }

        [DllImport("urlmon.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern int UrlMkSetSessionOption(uint dwOption, INTERNET_PROXY_INFO structNewProxy, int dwLen, uint dwZero);

        private const uint INTERNET_OPTION_REFRESH = 37;
        private const uint INTERNET_OPTION_PROXY = 38;

        [ComVisible(true)]
        public void Startup(int listeningPort)
        {
            EnableSsl = true;
            SetAsSystemProxy = false;
            ListeningPort = listeningPort;
            Start();
            
            var proxyInfo = new INTERNET_PROXY_INFO
            {
                dwAccessType = 0x3,
                lpszProxy = string.Format("http=127.0.0.1:{0}",
                        ListeningPort),
                lpszProxyBypass = "<local>"
            };
            int structSize = Marshal.SizeOf(proxyInfo);
            const uint SetProxy = INTERNET_OPTION_PROXY;

            UrlMkSetSessionOption(SetProxy, proxyInfo, structSize, 0);
        }
        [ComVisible(true)]
        public void Shutdown()
        {
            UrlMkSetSessionOption(INTERNET_OPTION_REFRESH, null, 0, 0);
            Stop();
        }

        private static readonly int BUFFER_SIZE = 8192;
        private static readonly char[] SemiSplit = { ';' };

        private static readonly string[] ColonSpaceSplit = { ": " };
        private static readonly char[] SpaceSplit = { ' ' };

        private static readonly Regex CookieSplitRegEx = new Regex(@",(?! )");

        private static readonly byte[] ChunkTrail = Encoding.ASCII.GetBytes(Environment.NewLine);

        private static readonly byte[] ChunkEnd =
            Encoding.ASCII.GetBytes(0.ToString("x2") + Environment.NewLine + Environment.NewLine);

        private static TcpListener _listener;

        public static List<string> ExcludedHttpsHostNameRegex = new List<string>();

        static ProxyServer()
        {
            CertManager = new CertificateManager("Titanium",
                "Titanium Root Certificate Authority");

            ListeningIpAddress = IPAddress.Any;
            ListeningPort = 0;

            Initialize();
        }

        private static CertificateManager CertManager { get; set; }

        public static string RootCertificateName { get; set; }
        public static bool EnableSsl { get; set; }
        public static bool SetAsSystemProxy { get; set; }

        public static int ListeningPort { get; set; }
        public static IPAddress ListeningIpAddress { get; set; }

        public static event EventHandler<SessionEventArgs> BeforeRequest;
        public static event EventHandler<SessionEventArgs> BeforeResponse;
        public static event EventHandler<SessionEventArgs> AfterSessionComplete;

        public static void Initialize()
        {
            ServicePointManager.Expect100Continue = false;
            WebRequest.DefaultWebProxy = null;
            ServicePointManager.DefaultConnectionLimit = 10;
            ServicePointManager.DnsRefreshTimeout = 3 * 60 * 1000; //3 minutes
            ServicePointManager.MaxServicePointIdleTime = 3 * 60 * 1000;

            //HttpWebRequest certificate validation callback
            ServicePointManager.ServerCertificateValidationCallback =
                delegate(object s, X509Certificate certificate, X509Chain chain, SslPolicyErrors sslPolicyErrors)
                {
                    if (sslPolicyErrors == SslPolicyErrors.None) return true;
                    return false;
                };

            //Fix a bug in .NET 4.0
            NetFrameworkHelper.UrlPeriodFix();
            //useUnsafeHeaderParsing 
            NetFrameworkHelper.ToggleAllowUnsafeHeaderParsing(true);
        }


        public static bool Start()
        {
            _listener = new TcpListener(ListeningIpAddress, ListeningPort);
            _listener.Start();

            ListeningPort = ((IPEndPoint)_listener.LocalEndpoint).Port;
            // accept clients asynchronously
            _listener.BeginAcceptTcpClient(OnAcceptConnection, _listener);

            var certTrusted = false;

            if (EnableSsl)
                certTrusted = CertManager.CreateTrustedRootCertificate();

            if (SetAsSystemProxy)
            {
                SystemProxyHelper.EnableProxyHttp(
                    Equals(ListeningIpAddress, IPAddress.Any) ? "127.0.0.1" : ListeningIpAddress.ToString(), ListeningPort);
                FireFoxHelper.AddFirefox();


                if (EnableSsl)
                {
                    RootCertificateName = RootCertificateName ?? "Titanium_Proxy_Test_Root";

                    //If certificate was trusted by the machine
                    if (certTrusted)
                    {
                        SystemProxyHelper.EnableProxyHttps(
                            Equals(ListeningIpAddress, IPAddress.Any) ? "127.0.0.1" : ListeningIpAddress.ToString(),
                            ListeningPort);
                    }
                }
            }

            return true;
        }

        private static void OnAcceptConnection(IAsyncResult asyn)
        {
            try
            {
                // Get the listener that handles the client request.
                _listener.BeginAcceptTcpClient(OnAcceptConnection, _listener);

                var client = _listener.EndAcceptTcpClient(asyn);
                Task.Factory.StartNew(() => HandleClient(client));
            }
            catch
            {
                // ignored
            }
        }


        public static void Stop()
        {
            if (SetAsSystemProxy)
            {
                SystemProxyHelper.DisableAllProxy();
                FireFoxHelper.RemoveFirefox();
            }

            _listener.Stop();
            CertManager.Dispose();
        }
    }
}