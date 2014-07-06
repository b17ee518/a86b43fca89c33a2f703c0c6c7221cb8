using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using Fiddler;
/*
internal class NativeMethods
{
    [DllImport("wininet.dll", SetLastError = true)]
    public static extern bool InternetSetOption(IntPtr hInternet, int dwOption, IntPtr lpBuffer, int lpdwBufferLength);
}
*/
namespace FiddlerCOMNS
{
    [ComVisible(true)]
    public interface IFiddlerComClass
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
        void Startup(int iListenPort, bool bRegisterAsSystemProxy, bool bDecryptSSL);
        [ComVisible(true)]
        void Shutdown();
    }

    [ComVisible(true)]
    [ClassInterface(ClassInterfaceType.None)]
    public class FiddlerCOMClass : IFiddlerComClass
    {
        public FiddlerCOMClass()
        {
        }

        private OnNotificationFunc m_OnNotificationFunc;
        private BeforeRequestFunc m_BeforeRequestFunc;
        private BeforeResponseFunc m_BeforeResponseFunc;
        private OnLogStringFunc m_OnLogStringFunc;
        private AfterSessionCompleteFunc m_AfterSessionCompleteFunc;


        [System.Runtime.InteropServices.ComVisible(true)]
        public void Startup(int iListenPort, bool bRegisterAsSystemProxy, bool bDecryptSSL)
        {
            Fiddler.FiddlerApplication.Startup(iListenPort, bRegisterAsSystemProxy, bDecryptSSL);
            Fiddler.URLMonInterop.SetProxyInProcess("127.0.0.1:"+iListenPort, null);

//            SetIESettings("localhost:" + iListenPort);
        }
        /*
        internal struct INTERNET_PROXY_INFO
        {
            public int dwAccessType;
            public IntPtr proxy;
            public IntPtr proxyBypass;
        }
        */
        /*
        [System.Runtime.InteropServices.ComVisible(false)]
        private static void SetIESettings(string proxyUri)
        {
            // ReSharper disable InconsistentNaming
            const int INTERNET_OPTION_PROXY = 38;
            const int INTERNET_OPEN_TYPE_PROXY = 3;
            // ReSharper restore InconsistentNaming

            INTERNET_PROXY_INFO proxyInfo;
            proxyInfo.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
            proxyInfo.proxy = Marshal.StringToHGlobalAnsi(proxyUri);
            proxyInfo.proxyBypass = Marshal.StringToHGlobalAnsi("local");

            var proxyInfoSize = Marshal.SizeOf(proxyInfo);
            var proxyInfoPtr = Marshal.AllocCoTaskMem(proxyInfoSize);
            Marshal.StructureToPtr(proxyInfo, proxyInfoPtr, true);

            NativeMethods.InternetSetOption(IntPtr.Zero, INTERNET_OPTION_PROXY, proxyInfoPtr, proxyInfoSize);
        }
        */
        [System.Runtime.InteropServices.ComVisible(true)]
        public void Shutdown()
        {
            Fiddler.FiddlerApplication.Shutdown();
        }
        [System.Runtime.InteropServices.ComVisible(true)]
        public void SetOnNotification(int f)
        {
            Fiddler.FiddlerApplication.OnNotification += FiddlerApplication_OnNotification;
            m_OnNotificationFunc = (OnNotificationFunc)Marshal.GetDelegateForFunctionPointer((IntPtr)f, typeof(OnNotificationFunc));
        }
        [System.Runtime.InteropServices.ComVisible(true)]
        public void SetBeforeResponse(int f)
        {
            Fiddler.FiddlerApplication.BeforeResponse += FiddlerApplication_BeforeResponse;
            m_BeforeResponseFunc = (BeforeResponseFunc)Marshal.GetDelegateForFunctionPointer((IntPtr)f, typeof(BeforeResponseFunc));
        }
        [System.Runtime.InteropServices.ComVisible(true)]
        public void SetBeforeRequest(int f)
        {
            Fiddler.FiddlerApplication.BeforeRequest += FiddlerApplication_BeforeRequest;
            m_BeforeRequestFunc = (BeforeRequestFunc)Marshal.GetDelegateForFunctionPointer((IntPtr)f, typeof(BeforeRequestFunc));
        }
        [System.Runtime.InteropServices.ComVisible(true)]
        public void SetOnLogString(int f)
        {
            Fiddler.FiddlerApplication.Log.OnLogString += FiddlerApplication_OnLogString;
            m_OnLogStringFunc = (OnLogStringFunc)Marshal.GetDelegateForFunctionPointer((IntPtr)f, typeof(OnLogStringFunc));
        }
        [System.Runtime.InteropServices.ComVisible(true)]
        public void SetAfterSessionComplete(int f)
        {
            Fiddler.FiddlerApplication.AfterSessionComplete += FiddlerApplication_AfterSessionComplete;
            m_AfterSessionCompleteFunc = (AfterSessionCompleteFunc)Marshal.GetDelegateForFunctionPointer((IntPtr)f, typeof(AfterSessionCompleteFunc));
        }

        [System.Runtime.InteropServices.ComVisible(false)]
        private void FiddlerApplication_BeforeRequest(Fiddler.Session oSession)
        {
            m_BeforeRequestFunc(
                oSession.id,
                oSession.fullUrl,
                oSession.GetRequestBodyAsString()
                );
            oSession.bBufferResponse = true;
        }
        [System.Runtime.InteropServices.ComVisible(false)]
        private void FiddlerApplication_BeforeResponse(Fiddler.Session oSession)
        {
            m_BeforeResponseFunc(
                oSession.id,
                oSession.oResponse.MIMEType,
                oSession.responseCode,
                oSession.PathAndQuery,
                oSession.GetRequestBodyAsString(),
                oSession.GetResponseBodyAsString()
                );
        }
        [System.Runtime.InteropServices.ComVisible(false)]
        private void FiddlerApplication_OnLogString(object sender, Fiddler.LogEventArgs e)
        {
            m_OnLogStringFunc(
                e.LogString
                );
        }
        [System.Runtime.InteropServices.ComVisible(false)]

        private void FiddlerApplication_OnNotification(object sender, Fiddler.NotificationEventArgs e)
        {
            m_OnNotificationFunc(
                e.NotifyString
                );
        }
        [System.Runtime.InteropServices.ComVisible(false)]
        private void FiddlerApplication_AfterSessionComplete(Fiddler.Session oSession)
        {
            m_AfterSessionCompleteFunc(
                oSession.id,
                oSession.oResponse.MIMEType,
                oSession.responseCode,
                oSession.PathAndQuery,
                oSession.GetRequestBodyAsString(),
                oSession.GetResponseBodyAsString()
                );
        }
    }
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
}
