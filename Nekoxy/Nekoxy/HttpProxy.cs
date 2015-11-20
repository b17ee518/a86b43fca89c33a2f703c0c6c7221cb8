using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TrotiNet;
using System.Runtime.InteropServices;

namespace Nekoxy
{
    [ComVisible(true)]
    public interface INekoxyClass
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
        void Startup(int listeningPort, bool useIpV6 = false, bool isSetProxyInProcess = true);
        [ComVisible(true)]
        void Shutdown();

        [ComVisible(true)]
        void SetupCertificate(string certStr, string keyStr);
        [ComVisible(true)]
        bool InstallCertificate(ref string certStr, ref string keyStr);
        [ComVisible(true)]
        bool UninstallCertificate(ref string certStr, ref string keyStr);
    }

    /// <summary>
    /// HTTPプロキシサーバー。
    /// HTTPプロトコルにのみ対応し、HTTPS等はサポートしない。
    /// </summary>
    [ComVisible(true)]
    [ClassInterface(ClassInterfaceType.None)]
    public class HttpProxy : INekoxyClass
    {        
        public HttpProxy()
        {
        }
        ~HttpProxy()
        {
        }

        private static TcpServer server;

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
        }

        /*
        [ComVisible(true)]
        void Startup(int iListenPort, bool bRegisterAsSystemProxy, bool bDecryptSSL) { }
        [ComVisible(true)]
        void Shutdown() { }
        */
        [ComVisible(true)]
        public void SetupCertificate(string certStr, string keyStr) { }
        [ComVisible(true)]
        public bool InstallCertificate(ref string certStr, ref string keyStr) { return true; }
        [ComVisible(true)]
        public bool UninstallCertificate(ref string certStr, ref string keyStr) { return true; }


        /// <summary>
        /// HTTPレスポンスをプロキシ クライアントに送信完了した際に発生。
        /// </summary>
        public event Action<Session> AfterSessionComplete;

        /// <summary>
        /// リクエストヘッダを読み込み完了した際に発生。
        /// ボディは受信前。
        /// </summary>
        public event Action<HttpRequest> AfterReadRequestHeaders;

        /// <summary>
        /// レスポンスヘッダを読み込み完了した際に発生。
        /// ボディは受信前。
        /// </summary>
        public event Action<HttpResponse> AfterReadResponseHeaders;

        /// <summary>
        /// 上流プロキシ設定。
        /// </summary>
        public ProxyConfig UpstreamProxyConfig
        {
            get { return TransparentProxyLogic.UpstreamProxyConfig; }
            set { TransparentProxyLogic.UpstreamProxyConfig = value; }
        }

        /// <summary>
        /// プロキシサーバーが Listening 中かどうかを取得。
        /// </summary>
        public bool IsInListening
        {
            get {return server != null && server.IsListening;}
            set {IsInListening = value;}
        }

        /// <summary>
        /// 指定ポートで Listening を開始する。
        /// Shutdown() を呼び出さずに2回目の Startup() を呼び出した場合、InvalidOperationException が発生する。
        /// </summary>
        /// <param name="listeningPort">Listeningするポート。</param>
        /// <param name="useIpV6">falseの場合、127.0.0.1で待ち受ける。trueの場合、::1で待ち受ける。既定false。</param>
        /// <param name="isSetProxyInProcess">trueの場合、プロセス内IEプロキシの設定を実施し、HTTP通信をNekoxyに向ける。既定true。</param>
        [ComVisible(true)]
        public void Startup(int listeningPort, bool useIpV6 = false, bool isSetProxyInProcess = true)
        {
            if (server != null) throw new InvalidOperationException("Calling Startup() twice without calling Shutdown() is not permitted.");

            TransparentProxyLogic.AfterSessionComplete += InvokeAfterSessionComplete;
            TransparentProxyLogic.AfterReadRequestHeaders += InvokeAfterReadRequestHeaders;
            TransparentProxyLogic.AfterReadResponseHeaders += InvokeAfterReadResponseHeaders;
            ListeningPort = listeningPort;
            try
            {
                if (isSetProxyInProcess)
                    WinInetUtil.SetProxyInProcessForNekoxy(listeningPort);

                server = new TcpServer(listeningPort, useIpV6);
                server.Start(TransparentProxyLogic.CreateProxy);
                server.InitListenFinished.WaitOne();
                if (server.InitListenException != null) throw server.InitListenException;
            }
            catch (Exception)
            {
                Shutdown();
                throw;
            }
        }

        /// <summary>
        /// Listening しているスレッドを終了し、ソケットを閉じる。
        /// </summary>
        [ComVisible(true)]
        public void Shutdown()
        {
            TransparentProxyLogic.AfterSessionComplete -= InvokeAfterSessionComplete;
            TransparentProxyLogic.AfterReadRequestHeaders -= InvokeAfterReadRequestHeaders;
            TransparentProxyLogic.AfterReadResponseHeaders -= InvokeAfterReadResponseHeaders;
            if (server != null)
            {
                server.Stop();
            }
            server = null;
        }

        internal static int ListeningPort { get; set; }

        private void InvokeAfterSessionComplete(Session session)
        {
            if (AfterSessionComplete != null)
            {
                AfterSessionComplete.Invoke(session);
            }
            else if (_afterSessionCompleteFunc != null)
            {
                _afterSessionCompleteFunc(0, session.Response.MimeType.ToString()??"", session.Response.StatusLine.StatusCode, session.Request.PathAndQuery??"", session.Request.BodyAsString??"", session.Response.BodyAsString??"");
            }
        }

        private void InvokeAfterReadRequestHeaders(HttpRequest request)
        {
            if (AfterReadRequestHeaders != null)
            {
                AfterReadRequestHeaders.Invoke(request);
            }
        }

        private void InvokeAfterReadResponseHeaders(HttpResponse response)
        {
            if (AfterReadResponseHeaders != null)
            {
                AfterReadResponseHeaders.Invoke(response);
            }
        }
    }
}
