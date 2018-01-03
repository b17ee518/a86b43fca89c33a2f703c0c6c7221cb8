#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGlobal>
#include <QMainWindow>
#include <QProcess>
#include "mainwindowbase.h"
#include "infomainwindow.h"
#include "timermainwindow.h"
#include "weaponmainwindow.h"
#include "shipmainwindow.h"

#include "QNetworkProxyFactorySet.h"
#include <QShowEvent>

#if defined Q_OS_WIN && !defined NO_WIN_EXTRA
#include <QWinTaskbarButton>
#endif
#include <QNetworkReply>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
#include <QWebEngineView>
#else
#include <QWebView>
#endif

#if defined Q_OS_WIN && !defined NO_WIN_EXTRA
#include <QAxWidget>
#include "shdocvw.h"

#include "fidcom.h"
#include "nekoxy.h"
#include "titanium_web_proxy.h"
using namespace SHDocVw;
#endif

#include "qwindowseventfilter.h"
#include "qwebenginemouseeventfilter.h"


enum class ProgressBarState
{
	Normal,
	Paused,
	Stopped,
};

enum class QWebViewCSSIndex
{
	Invalid = -1,
	Normal = 0,
	Compact,
	Transparent,
	//	CLEAR,
	Max,
};

enum class ProxyMode
{
	NoProxy,
	QtProxy,
	Fid,
	Nekoxy,
	Titanium,
	Shark,
};

enum class WebWidgetType
{
	Webkit,
	WebEngine,	// Qt5.6
	IE,
};

enum class PlatformType
{
	Windows7,
	SlowTablet,		// windows8
	FastTablet,		// surface
	Mac,
};

namespace Ui {
	class MainWindow;
}

class MainWindow : public MainWindowBase
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	static inline MainWindow * mainWindow(){ return s_pMainWindow; }
	static inline void setMainWindow(MainWindow * pWindow){ s_pMainWindow = pWindow; }
	void postInit(InfoMainWindow * pInfo, TimerMainWindow * pTimer, WeaponMainWindow *pWeapon, ShipMainWindow* pShip);
	static inline InfoMainWindow * infoWindow(){ return s_pMainWindow->_pInfoWindow; }
	static inline TimerMainWindow * timerWindow(){ return s_pMainWindow->_pTimerWindow; }
	static inline WeaponMainWindow * weaponWindow(){ return s_pMainWindow->_pWeaponWindow; }
	static inline ShipMainWindow * shipWindow(){ return s_pMainWindow->_pShipWindow; }

	void AdjustVolume(int vol);
	void onSubMainWindowShowHide(bool bShow, MainWindowBase * pWindow);
	void SetProgressBarPos(int pos, ProgressBarState state);

	void onGetNetworkReply(QNetworkReply * reply);
	void onGetNetworkReply(const QString& PathAndQuery, const QString& requestBody, const QString& responseBody);

	void setSleepMode(bool val);
	bool isSleepMode();

	QWidget* getBrowserWidget();
	void navigateTo(const QString& urlString);
	void navigateReload();

	void setPauseNextChanged(bool bVal);
	void setJobTerminated();

	bool switchToExpeditionWait();

	WebWidgetType getWebWidgetType() { return _webWidgetType; }
	PlatformType getPlatformType() { return _platformType; }

signals:
	void sigParse(const QString &PathAndQuery, const QString &requestBody, const QString &responseBody);
	void sigTogglePanicTimer(int timeVal);
	void sigToggleSleepMode(bool bSleep);

	public slots:
	virtual void slotToggleRestoreMinimize(bool bRestore);
	virtual void slotSelfToggleRestoreMinimize(bool bRestore);
	void slotParse(const QString &PathAndQuery, const QString &requestBody, const QString &responseBody);
	void slotSoundEnded();
	void slotTogglePanicTimer(int timeVal);

	void slotSharkProcessReadyRead();
	void slotSharkProcessReadyReadError();

	void onFatalSharkResponseError(bool fatalOnProxy);

protected:
	virtual void changeEvent(QEvent * e);
	virtual void closeEvent(QCloseEvent * e);
	virtual void moveEvent(QMoveEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent * event);
	virtual void showEvent(QShowEvent *event);

	private slots:
	void on_pbClose_clicked();
	void on_pbMinimize_clicked();
	void on_pbCheckTransparent_toggled(bool checked);

	void on_pbMoveTogether_toggled(bool checked);
	void on_pbRefresh_clicked();
	void on_pbScreenshot_clicked();
	void on_pbSwitchScreenshot_toggled(bool checked);
	void slotScreenshotTimeout();
	void slotJobtTimeout();
	void on_pbCheckLowVol_toggled(bool checked);

#if defined Q_OS_WIN && !defined NO_WIN_EXTRA
	void slotNavigateComplete2(IDispatch*, QVariant&);
#endif
	void slotWebViewException(int code, const QString & source, const QString & desc, const QString & help);
	void onPanic();
	void onDoJobFuel();
	void onDoJobKira();
	void onDoJobLevel();
	void onDoJobRank();
	void onDoJobAny();
	void onDoJobExpedition(bool bDo);
	void onDoJobDestroy();
	void onDoJobRepair();
	void onDoJobDevelop();
	void onDoJobMorning();
	void onToggleJobExpedition();
	void onExportAllList();
	void onTerminateJob();
	void onIncreaseSouthEast();
	void onDecreaseSouthEast();
	void onIncreaseTimeShift();
	void onDecreaseTimeShift();
	void setTimeShift(int min);

	void slotSetLogAll(bool bSet);

	void onJobPauseResume();
	void onJobPauseNext();

	void onGetColorOnScreen();

private:
	static void __stdcall BeforeRequestFunc(int sessionID, char * fullURL, char * requestBody);
	static void __stdcall AfterSessionCompleteFunc(int sessionID, char * mimeType, int responseCode, char * PathAndQuery, char * requestBody, char * responseBody);

private:
	void setWebSettings();
	void applyCss(QWebViewCSSIndex css);
	void shootScreen();
	void setupCss();
	void loadSettings();
	void rebuildIE(bool bNavigate);

	void loadCertKey();
	void saveCertKey();

	void installWebEngineMouseEventFilter();

private:
	Ui::MainWindow *ui;

	ProxyMode _proxyMode = ProxyMode::Nekoxy;

	int _useport = 0;
#if defined Q_OS_WIN && !defined NO_WIN_EXTRA
	FidCOM::FidCOMClass * _pFid = NULL;
	Nekoxy::HttpProxy * _pNekoxy = NULL;
	Titanium_Web_Proxy::ProxyServer* _pTitanium = NULL;
#endif

	class SharkRequestResponseRecord
	{
	public:
		SharkRequestResponseRecord(){}
		int requestFrame;
		QString pathAndQuery;
		QString requestBody;
		QString responseBody;
	};

	QProcess * _pSharkProcess = NULL;
	QString _sharkWorkingPath;
	QString _sharkCommand;
	QList<SharkRequestResponseRecord> _sharkRequestStack;
	QList<SharkRequestResponseRecord> _sharkResponseStack;
	const int _sharkStackMax = 10;
	bool _sharkShouldRaiseFatalOnMismatchResponse = true;
	QString _sharkReadingBuffer;
	QMutex _sharkReadingMutex;

	bool _bIEPageLoaded = false;
	QWebViewCSSIndex _applyCssWhenLoaded = QWebViewCSSIndex::Invalid;
	QString _ieCsses[(int)QWebViewCSSIndex::Max];
	QUrl _webViewCsses[(int)QWebViewCSSIndex::Max];

	InfoMainWindow * _pInfoWindow = NULL;
	TimerMainWindow * _pTimerWindow = NULL;
	WeaponMainWindow * _pWeaponWindow = NULL;
	ShipMainWindow* _pShipWindow = NULL;

	static MainWindow * s_pMainWindow;

#if defined Q_OS_WIN && !defined NO_WIN_EXTRA
	QWinTaskbarButton * _pTaskbarButton = NULL;
#endif
	bool _bMoveSubTogether = true;

	QTimer * _pScreenshotTimer = NULL;
	QTimer* _panicTimer = NULL;

	QTimer* _jobTimer = NULL;

	bool _bLowVol = true;
	bool _bSleep = false;

	QWindowsEventFilter _windowsEventfilter;
	QNetworkProxyFactorySet _proxyFactory;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
	QWebEngineView* _webView = NULL;
#else
	QWebView* _webView = NULL;
#endif

#if defined Q_OS_WIN && !defined NO_WIN_EXTRA
	WebBrowser* _axWidget = NULL;
#endif

	// settings
	QString _gameUrl;
	QString _gameUrlId;

	QString _certStr;
	QString _keyStr;

	// need to be set
	WebWidgetType _webWidgetType = WebWidgetType::Webkit;
	PlatformType _platformType = PlatformType::Windows7;

	bool _applyCssToGameFlag = true;

	QWebEngineMouseEventFilter * _webEngineMouseEventFilter = NULL;
};



#endif // MAINWINDOW_H
