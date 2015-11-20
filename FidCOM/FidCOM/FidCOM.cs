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
namespace FidCOMNS
{
    [ComVisible(true)]
    public interface IFidComClass
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

        [ComVisible(true)]
        void SetupCertificate(string certStr, string keyStr);
        [ComVisible(true)]
        bool InstallCertificate(ref string certStr, ref string keyStr);
        [ComVisible(true)]
        bool UninstallCertificate(ref string certStr, ref string keyStr);
    }

    [ComVisible(true)]
    [ClassInterface(ClassInterfaceType.None)]
    public class FidCOMClass : IFidComClass
    {
        public FidCOMClass()
        {
        }
        ~FidCOMClass()
        {
        }

        private OnNotificationFunc _onNotificationFunc;
        private BeforeRequestFunc _beforeRequestFunc;
        private BeforeResponseFunc _beforeResponseFunc;
        private OnLogStringFunc _onLogStringFunc;
        private AfterSessionCompleteFunc _afterSessionCompleteFunc;

        // first setup
        [System.Runtime.InteropServices.ComVisible(true)]
        public void SetupCertificate(string certStr, string keyStr)
        {
            if (!string.IsNullOrEmpty(certStr) && !string.IsNullOrEmpty(keyStr))
            {
                FiddlerApplication.Prefs.SetStringPref("fid.certmaker.bc.cert", certStr);
                FiddlerApplication.Prefs.SetStringPref("fid.certmaker.bc.key", keyStr);
            }
        }

        // then install
        [System.Runtime.InteropServices.ComVisible(true)]
        public bool InstallCertificate(ref string certStr, ref string keyStr)
        {
            if (!CertMaker.rootCertExists())
            {
                if (!CertMaker.createRootCert())
                    return false;

                if (!CertMaker.trustRootCert())
                    return false;
            }

            certStr = FiddlerApplication.Prefs.GetStringPref("fiddler.certmaker.bc.cert", null);
            keyStr = FiddlerApplication.Prefs.GetStringPref("fiddler.certmaker.bc.key", null);
            return true;
        }

        // should never be used?
        [System.Runtime.InteropServices.ComVisible(true)]
        public bool UninstallCertificate(ref string certStr, ref string keyStr)
        {
            if (CertMaker.rootCertExists())
            {
                if (!CertMaker.removeFiddlerGeneratedCerts(true))
                    return false;
            }
            certStr = null;
            keyStr = null;
            return true;
        }

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
            _onNotificationFunc = (OnNotificationFunc)Marshal.GetDelegateForFunctionPointer((IntPtr)f, typeof(OnNotificationFunc));
        }
        [System.Runtime.InteropServices.ComVisible(true)]
        public void SetBeforeResponse(int f)
        {
            Fiddler.FiddlerApplication.BeforeResponse += FiddlerApplication_BeforeResponse;
            _beforeResponseFunc = (BeforeResponseFunc)Marshal.GetDelegateForFunctionPointer((IntPtr)f, typeof(BeforeResponseFunc));
        }
        [System.Runtime.InteropServices.ComVisible(true)]
        public void SetBeforeRequest(int f)
        {
            Fiddler.FiddlerApplication.BeforeRequest += FiddlerApplication_BeforeRequest;
            _beforeRequestFunc = (BeforeRequestFunc)Marshal.GetDelegateForFunctionPointer((IntPtr)f, typeof(BeforeRequestFunc));
        }
        [System.Runtime.InteropServices.ComVisible(true)]
        public void SetOnLogString(int f)
        {
            Fiddler.FiddlerApplication.Log.OnLogString += FiddlerApplication_OnLogString;
            _onLogStringFunc = (OnLogStringFunc)Marshal.GetDelegateForFunctionPointer((IntPtr)f, typeof(OnLogStringFunc));
        }
        [System.Runtime.InteropServices.ComVisible(true)]
        public void SetAfterSessionComplete(int f)
        {
            Fiddler.FiddlerApplication.AfterSessionComplete += FiddlerApplication_AfterSessionComplete;
            _afterSessionCompleteFunc = (AfterSessionCompleteFunc)Marshal.GetDelegateForFunctionPointer((IntPtr)f, typeof(AfterSessionCompleteFunc));
        }

        [System.Runtime.InteropServices.ComVisible(false)]
        private void FiddlerApplication_BeforeRequest(Fiddler.Session oSession)
        {
            _beforeRequestFunc(
                oSession.id,
                oSession.fullUrl,
                oSession.GetRequestBodyAsString()
                );
            oSession.bBufferResponse = true;
        }
        [System.Runtime.InteropServices.ComVisible(false)]
        private void FiddlerApplication_BeforeResponse(Fiddler.Session oSession)
        {
            _beforeResponseFunc(
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
            _onLogStringFunc(
                e.LogString
                );
        }
        [System.Runtime.InteropServices.ComVisible(false)]

        private void FiddlerApplication_OnNotification(object sender, Fiddler.NotificationEventArgs e)
        {
            _onNotificationFunc(
                e.NotifyString
                );
        }
        [System.Runtime.InteropServices.ComVisible(false)]
        private void FiddlerApplication_AfterSessionComplete(Fiddler.Session oSession)
        {
            _afterSessionCompleteFunc(
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
