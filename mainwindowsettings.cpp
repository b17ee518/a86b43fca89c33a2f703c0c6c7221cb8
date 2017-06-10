#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QNetworkProxy>
#include "cookiejar.h"
#include <QNetworkDiskCache>
#include <QStandardPaths>
#include "qnetworkproxyfactoryset.h"
#include "KQWebPage.h"

#ifdef Q_OS_WIN
#include <QWinTaskbarProgress>
#endif
#include <QWebEngineSettings>

#ifdef Q_OS_WIN
#include <Audiopolicy.h>
#include <Mmdeviceapi.h>

#include <QAxWidget>
#endif
#include <QSettings>
#include "ControlManager.h"
#include "ExpeditionManager.h"

#include "kqnetworkaccessmanager.h"

#define SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; } 

void MainWindow::postInit(InfoMainWindow *pInfo, TimerMainWindow *pTimer, WeaponMainWindow* pWeapon, ShipMainWindow* pShip)
{
	_pInfoWindow = pInfo;
	_pTimerWindow = pTimer;
	_pWeaponWindow = pWeapon;
	_pShipWindow = pShip;

	QObject::connect(this, SIGNAL(sigActivated(QWidget*, bool)), _pInfoWindow, SLOT(slotActivate(QWidget*, bool)));
	QObject::connect(this, SIGNAL(sigActivated(QWidget*, bool)), _pTimerWindow, SLOT(slotActivate(QWidget*, bool)));
	QObject::connect(this, SIGNAL(sigActivated(QWidget*, bool)), _pWeaponWindow, SLOT(slotActivate(QWidget*, bool)));
	QObject::connect(this, SIGNAL(sigActivated(QWidget*, bool)), _pShipWindow, SLOT(slotActivate(QWidget*, bool)));

	QObject::connect(_pInfoWindow, SIGNAL(sigActivated(QWidget*, bool)), this, SLOT(slotActivate(QWidget*, bool)));
	QObject::connect(_pInfoWindow, SIGNAL(sigActivated(QWidget*, bool)), _pTimerWindow, SLOT(slotActivate(QWidget*, bool)));
	QObject::connect(_pInfoWindow, SIGNAL(sigActivated(QWidget*, bool)), _pWeaponWindow, SLOT(slotActivate(QWidget*, bool)));
	QObject::connect(_pInfoWindow, SIGNAL(sigActivated(QWidget*, bool)), _pShipWindow, SLOT(slotActivate(QWidget*, bool)));

	QObject::connect(_pTimerWindow, SIGNAL(sigActivated(QWidget*, bool)), _pInfoWindow, SLOT(slotActivate(QWidget*, bool)));
	QObject::connect(_pTimerWindow, SIGNAL(sigActivated(QWidget*, bool)), _pWeaponWindow, SLOT(slotActivate(QWidget*, bool)));
	QObject::connect(_pTimerWindow, SIGNAL(sigActivated(QWidget*, bool)), _pShipWindow, SLOT(slotActivate(QWidget*, bool)));
	QObject::connect(_pTimerWindow, SIGNAL(sigActivated(QWidget*, bool)), this, SLOT(slotActivate(QWidget*, bool)));

	QObject::connect(_pWeaponWindow, SIGNAL(sigActivated(QWidget*, bool)), _pInfoWindow, SLOT(slotActivate(QWidget*, bool)));
	QObject::connect(_pWeaponWindow, SIGNAL(sigActivated(QWidget*, bool)), _pTimerWindow, SLOT(slotActivate(QWidget*, bool)));
	QObject::connect(_pWeaponWindow, SIGNAL(sigActivated(QWidget*, bool)), this, SLOT(slotActivate(QWidget*, bool)));
	QObject::connect(_pWeaponWindow, SIGNAL(sigActivated(QWidget*, bool)), _pShipWindow, SLOT(slotActivate(QWidget*, bool)));

	QObject::connect(_pShipWindow, SIGNAL(sigActivated(QWidget*, bool)), _pTimerWindow, SLOT(slotActivate(QWidget*, bool)));
	QObject::connect(_pShipWindow, SIGNAL(sigActivated(QWidget*, bool)), this, SLOT(slotActivate(QWidget*, bool)));
	QObject::connect(_pShipWindow, SIGNAL(sigActivated(QWidget*, bool)), _pWeaponWindow, SLOT(slotActivate(QWidget*, bool)));
	QObject::connect(_pShipWindow, SIGNAL(sigActivated(QWidget*, bool)), _pInfoWindow, SLOT(slotActivate(QWidget*, bool)));

	connect(_pInfoWindow, SIGNAL(sigRestoreMinimizeToggled(bool)), this, SLOT(slotToggleRestoreMinimize(bool)));
	connect(_pTimerWindow, SIGNAL(sigRestoreMinimizeToggled(bool)), this, SLOT(slotToggleRestoreMinimize(bool)));
	connect(_pWeaponWindow, SIGNAL(sigRestoreMinimizeToggled(bool)), this, SLOT(slotToggleRestoreMinimize(bool)));
	connect(_pShipWindow, SIGNAL(sigRestoreMinimizeToggled(bool)), this, SLOT(slotToggleRestoreMinimize(bool)));
	connect(this, SIGNAL(sigRestoreMinimizeToggled(bool)), this, SLOT(slotSelfToggleRestoreMinimize(bool)));


	connect(ui->pbCheckInfo, SIGNAL(toggled(bool)), _pInfoWindow, SLOT(setVisible(bool)));
	connect(ui->pbCheckTimer, SIGNAL(toggled(bool)), _pTimerWindow, SLOT(setVisible(bool)));
	connect(ui->pbCheckWeapon, SIGNAL(toggled(bool)), _pWeaponWindow, SLOT(slotSetVisible(bool)));
	connect(ui->pbCheckShip, SIGNAL(toggled(bool)), _pShipWindow, SLOT(slotSetVisible(bool)));

	connect(ui->pbCheckTransparent, SIGNAL(toggled(bool)), this, SLOT(on_pbCheckTransparent_toggled(bool)));

	AdjustVolume(-1);

#ifdef Q_OS_WIN
	HWND hwnd = (HWND)effectiveWinId();
	RegisterPowerSettingNotification(hwnd, &GUID_CONSOLE_DISPLAY_STATE, DEVICE_NOTIFY_WINDOW_HANDLE);
	RegisterPowerSettingNotification(hwnd, &GUID_SYSTEM_AWAYMODE, DEVICE_NOTIFY_WINDOW_HANDLE);
#endif
}

void MainWindow::setSleepMode(bool val)
{
	if (_bSleep != val)
	{
		_bSleep = val;

		emit this->sigToggleSleepMode(_bSleep);
	}
	//	qDebug("state changed");
}

bool MainWindow::isSleepMode()
{
	return _bSleep;
}

void MainWindow::setWebSettings()
{
	QObject * exceptionSender = NULL;
	if (_proxyMode == ProxyMode::Fid)
	{
#ifdef Q_OS_WIN
		_pFid = new FidCOM::FidCOMClass(this);

		loadCertKey();
		_pFid->SetupCertificate(_certStr, _keyStr);
		_pFid->InstallCertificate(_certStr, _keyStr);
		saveCertKey();
		exceptionSender = _pFid;

		_pFid->SetBeforeRequest((int)&MainWindow::BeforeRequestFunc);
		_pFid->SetAfterSessionComplete((int)&MainWindow::AfterSessionCompleteFunc);

		_pFid->Startup(_useport, false, true);
#endif
	}
	else if (_proxyMode == ProxyMode::Nekoxy)
	{
#ifdef Q_OS_WIN
		_pNekoxy = new Nekoxy::HttpProxy(this);
		exceptionSender = _pNekoxy;

		_pNekoxy->SetAfterSessionComplete((int)&MainWindow::AfterSessionCompleteFunc);

		_pNekoxy->Startup(_useport, false, true);
#endif
	}
	else if (_proxyMode == ProxyMode::Titanium)
	{
#ifdef Q_OS_WIN
		_pTitanium = new Titanium_Web_Proxy::ProxyServer(this);
		exceptionSender = _pTitanium;
		_pTitanium->SetMakecertPath(QApplication::applicationDirPath() + "/makecert.exe");

		_pTitanium->SetAfterSessionComplete((int)&MainWindow::AfterSessionCompleteFunc);
		_pTitanium->Startup(_useport);
#endif
	}

	if (_proxyMode != ProxyMode::NoProxy && _proxyMode != ProxyMode::QtProxy)
	{
		if (exceptionSender)
		{
			connect(
				exceptionSender,
				SIGNAL(exception(int, const QString &, const QString &, const QString &)),
				this,
				SLOT(slotWebViewException(int, const QString &, const QString &, const QString &)));
		}

		_proxyFactory.init(_useport);

		// TODO: check why
		if (_webWidgetType == WebWidgetType::WebEngine)
		{
			QNetworkProxy::setApplicationProxy(_proxyFactory.getHttpProxy());
		}
		else
		{
			QNetworkProxyFactory::setApplicationProxyFactory(&_proxyFactory);
		}
	}

	if (_webWidgetType != WebWidgetType::IE)
	{
		KQWebPage * page = new KQWebPage();
		_webView->setPage(page);
		_webView->setContextMenuPolicy(Qt::PreventContextMenu);

#if (QT_VERSION < QT_VERSION_CHECK(5, 6, 0))
		if (_proxyMode == ProxyMode::QtProxy)
		{
			KQNetworkAccessManager * manager = new KQNetworkAccessManager(this);
			_webView->page()->setNetworkAccessManager(manager);
		}
		CookieJar* jar = new CookieJar(this);
		_webView->page()->networkAccessManager()->setCookieJar(jar);

		//WebView
		QNetworkDiskCache *cache = new QNetworkDiskCache(this);
		cache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
		cache->setMaximumCacheSize(1073741824); //about 1024MB
		_webView->page()->networkAccessManager()->setCache(cache);

		/*
		QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
		if(dir.exists()){
		dir.removeRecursively();
		}
		*/
#endif

	}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
	auto webSettingFunc = [](QWebEngineSettings* websetting)
	{
		//JavaScript関連設定
		websetting->setAttribute(QWebEngineSettings::PluginsEnabled, true);
		websetting->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
		websetting->setAttribute(QWebEngineSettings::PluginsEnabled, true);
		websetting->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
		//フォント設定
#if defined(Q_OS_WIN32)
		websetting->setFontFamily(QWebEngineSettings::StandardFont, "Meiryo UI");
		websetting->setFontFamily(QWebEngineSettings::SerifFont, "MS PMincho");
		websetting->setFontFamily(QWebEngineSettings::SansSerifFont, "MS PGothic");
		websetting->setFontFamily(QWebEngineSettings::FixedFont, "MS Gothic");
#elif defined(Q_OS_LINUX)
#elif defined(Q_OS_MAC)
		websetting->setFontFamily(QWebEngineSettings::StandardFont, "ヒラギノ角ゴPro");
		websetting->setFontFamily(QWebEngineSettings::SerifFont, "ヒラギノ明朝Pro");
		websetting->setFontFamily(QWebEngineSettings::SansSerifFont, "ヒラギノ角ゴPro");
		websetting->setFontFamily(QWebEngineSettings::FixedFont, "Osaka");
#else
#endif
	};

	webSettingFunc(QWebEngineSettings::defaultSettings());
#else
	QWebSettings *websetting = QWebSettings::globalSettings();
	//JavaScript関連設定
	websetting->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
	websetting->setAttribute(QWebSettings::JavascriptCanCloseWindows, true);
	websetting->setAttribute(QWebSettings::PluginsEnabled, true);
	websetting->setAttribute(QWebSettings::JavascriptEnabled, true);
	//フォント設定
#if defined(Q_OS_WIN32)
	websetting->setFontFamily(QWebSettings::StandardFont, "Meiryo UI");
	websetting->setFontFamily(QWebSettings::SerifFont, "MS PMincho");
	websetting->setFontFamily(QWebSettings::SansSerifFont, "MS PGothic");
	websetting->setFontFamily(QWebSettings::FixedFont, "MS Gothic");
#elif defined(Q_OS_LINUX)
#elif defined(Q_OS_MAC)
	websetting->setFontFamily(QWebSettings::StandardFont, "ヒラギノ角ゴPro");
	websetting->setFontFamily(QWebSettings::SerifFont, "ヒラギノ明朝Pro");
	websetting->setFontFamily(QWebSettings::SansSerifFont, "ヒラギノ角ゴPro");
	websetting->setFontFamily(QWebSettings::FixedFont, "Osaka");
#else
#endif
#endif
}

void MainWindow::AdjustVolume(int vol)
{

#ifdef Q_OS_WIN
	HRESULT hr = S_OK;

	IMMDevice* pDevice = NULL;
	IMMDeviceEnumerator* pEnumerator = NULL;
	IAudioSessionManager2* pSessionManager = NULL;


	// Create the device enumerator.
	(hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator),
		NULL, CLSCTX_ALL,
		__uuidof(IMMDeviceEnumerator),
		(void**)&pEnumerator));

	// Get the default audio device.
	(hr = pEnumerator->GetDefaultAudioEndpoint(
		eRender, eConsole, &pDevice));

	// Get the session manager.
	(hr = pDevice->Activate(
		__uuidof(IAudioSessionManager2), CLSCTX_ALL,
		NULL, (void**)&pSessionManager));

	IAudioSessionControl *pControl;
	pSessionManager->GetAudioSessionControl(NULL, 0, &pControl);
	ISimpleAudioVolume *pSimpleVolume;
	pSessionManager->GetSimpleAudioVolume(NULL, 0, &pSimpleVolume);
	GUID nid = GUID_NULL;

	if (vol < 0)
	{
		if (_bLowVol)
		{
			vol = 12;
		}
		else
		{
			vol = 100;
		}
	}

	pSimpleVolume->SetMasterVolume(vol / 100.0f, &nid);

	// Clean up.
	SAFE_RELEASE(pSessionManager);
	SAFE_RELEASE(pEnumerator);
	SAFE_RELEASE(pDevice);
#endif
}

void MainWindow::SetProgressBarPos(int pos, ProgressBarState state)
{

#ifdef Q_OS_WIN
	if (_pTaskbarButton)
	{
		QWinTaskbarProgress * pProgress = _pTaskbarButton->progress();
		_pTaskbarButton->progress()->setValue(pos);
		switch (state)
		{
		case ProgressBarState::Normal:
			if (pProgress->isPaused() || pProgress->isStopped())
			{
				pProgress->resume();
			}
			break;
		case ProgressBarState::Paused:
			if (pProgress->isStopped())
			{
				pProgress->resume();
			}
			if (!pProgress->isPaused())
			{
				pProgress->setPaused(true);
			}
			break;
		case ProgressBarState::Stopped:
			if (pProgress->isPaused())
			{
				pProgress->resume();
			}
			if (!pProgress->isStopped())
			{
				pProgress->stop();
			}
			break;
		}
	}
#endif
}
void MainWindow::setupCss()
{
	/*
	 *
	 body {
	 margin:0;
	 overflow:hidden
	 }

	 #game_frame {
	 position:fixed;
	 top:-16px;
	 left:-50px;
	 z-index:1
	 }
	 */
	_ieCsses[(int)QWebViewCSSIndex::Normal] = "body {margin:0;overflow:hidden;} #game_frame{position:fixed;top:-16px;left:-50px;z-index:1}";
	_webViewCsses[(int)QWebViewCSSIndex::Normal] = QUrl(QString("data:text/css;charset=utf-8;base64,")
		+ _ieCsses[(int)QWebViewCSSIndex::Normal].toUtf8().toBase64());
	//		(QUrl("data:text/css;charset=utf-8;base64,Ym9keSB7DQoJbWFyZ2luOjA7DQoJb3ZlcmZsb3c6aGlkZGVuDQp9DQoNCiNnYW1lX2ZyYW1lIHsNCglwb3NpdGlvbjpmaXhlZDsNCgl0b3A6LTE2cHg7DQoJbGVmdDotNTBweDsNCgl6LWluZGV4OjENCn0="));


	/*
	*
	body {
	margin:0;
	overflow:hidden
	}

	#game_frame {
	position:fixed;
	top:-16px;
	left:-50px;
	z-index:1
	zoom:0.5
	}
	*/

	_ieCsses[(int)QWebViewCSSIndex::Compact] = "body {margin:0;overflow:hidden;} #game_frame{position:fixed;top:-16px;left:-50px;z-index:1}";
	_webViewCsses[(int)QWebViewCSSIndex::Compact] = QUrl(QString("data:text/css;charset=utf-8;base64,")
		+ _ieCsses[(int)QWebViewCSSIndex::Compact].toUtf8().toBase64());
	/*
body {
margin:0;
overflow:hidden;
opacity: 0.4;
}

#game_frame {
position:fixed;
top:-16px;
left:-50px;
z-index:1;
}
*/
	//	csses[QWebViewCSS::TRANSPARENT] = QUrl("data:text/css;charset=utf-8;base64,Ym9keSB7DQoJbWFyZ2luOjA7DQoJb3ZlcmZsb3c6aGlkZGVuOw0KCW9wYWNpdHk6IDAuNDsNCn0NCg0KI2dhbWVfZnJhbWUgew0KCXBvc2l0aW9uOmZpeGVkOw0KCXRvcDotMTZweDsNCglsZWZ0Oi01MHB4Ow0KCXotaW5kZXg6MTsNCn0=");

	/*
	body {
	margin:0;
	overflow:hidden;
	opacity: 0.2;
	}

	#game_frame {
	position:fixed;
	top:-16px;
	left:-50px;
	z-index:1;
	}
	*/
	_webViewCsses[(int)QWebViewCSSIndex::Transparent] =
		QUrl("data:text/css;charset=utf-8;base64,Ym9keSB7DQoJbWFyZ2luOjA7DQoJb3ZlcmZsb3c6aGlkZGVuOw0KCW9wYWNpdHk6IDAuMjsNCn0NCg0KI2dhbWVfZnJhbWUgew0KCXBvc2l0aW9uOmZpeGVkOw0KCXRvcDotMTZweDsNCglsZWZ0Oi01MHB4Ow0KCXotaW5kZXg6MTsNCn0=");

	// do not set opacity
	_ieCsses[(int)QWebViewCSSIndex::Transparent] = "body {margin:0;overflow:hidden;} #game_frame{position:fixed;top:-16px;left:-50px;z-index:1}";

	applyCss(QWebViewCSSIndex::Normal);
}

void MainWindow::loadSettings()
{
	// platform
#ifdef Q_OS_WIN
	if (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS8
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
		&& QSysInfo::WindowsVersion < QSysInfo::WV_WINDOWS10
#endif
		)
	{
		_platformType = PlatformType::SlowTablet;
	}
	else

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
		if (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS10)
#else 
		if (false)
#endif
		{
			_platformType = PlatformType::FastTablet;
		}
		else
		{
			_platformType = PlatformType::Windows7;
		}
#else
	_platformType = PlatformType::Mac;
#endif

	QString filename = QApplication::applicationDirPath();
	filename += "/settings.ini";
	QSettings* setting = new QSettings(filename, QSettings::IniFormat);
	setting->setIniCodec("UTF-8");

	const QString itemWebWidgetType = "WebWidgetType";
	const QString itemGameUrl = "GameUrl";
	const QString itemUrlId = "UrlId";
	const QString itemUsePort = "UsePort";
	const QString itemIntervalMul = "IntervalMul";
	const QString itemProxyMode = "ProxyMode";
	//	const QString itemExpeditionMode = "ExpeditionMode";
	const QString itemTimeShift = "TimeShift";

	setting->beginGroup("Settings");
	if (!setting->contains(itemWebWidgetType))
	{
		if (_platformType == PlatformType::SlowTablet)
		{
			setting->setValue(itemWebWidgetType, "IE");
		}
		else
		{
			setting->setValue(itemWebWidgetType, "WebKit");
		}
	}
	if (!setting->contains(itemGameUrl))
	{
		setting->setValue(itemGameUrl, "http://www.dmm.com/netgame/social/-/gadgets/=/app_id=854854/?f1=1&p1=0");
	}
	if (!setting->contains(itemUrlId))
	{
		setting->setValue(itemUrlId, "app_id=854854");
	}
	if (!setting->contains(itemUsePort))
	{
		setting->setValue(itemUsePort, 13347);
	}
	if (!setting->contains(itemIntervalMul))
	{
		if (_platformType == PlatformType::SlowTablet)
		{
			setting->setValue(itemIntervalMul, 150);
		}
		else
		{
			setting->setValue(itemIntervalMul, 100);
		}
	}
	if (!setting->contains(itemProxyMode))
	{
		if (_platformType == PlatformType::Mac)
		{
			setting->setValue(itemProxyMode, "QtProxy");
		}
		else
		{
			setting->setValue(itemProxyMode, "Nekoxy");
		}
	}
	/*
	if (!setting->contains(itemExpeditionMode))
	{
	setting->setValue(itemExpeditionMode, "General");
	}
	*/
	if (!setting->contains(itemTimeShift))
	{
		setting->setValue(itemTimeShift, 0);
	}

	QString webWidgetType = setting->value(itemWebWidgetType).toString();
	if (!webWidgetType.compare("IE", Qt::CaseInsensitive))
	{
		_webWidgetType = WebWidgetType::IE;
	}
	else if (!webWidgetType.compare("WebEngine", Qt::CaseInsensitive))
	{
		_webWidgetType = WebWidgetType::WebEngine;
	}
	else if (!webWidgetType.compare("WebKit", Qt::CaseInsensitive))
	{
		_webWidgetType = WebWidgetType::Webkit;
	}

	if (_webWidgetType == WebWidgetType::WebEngine || _webWidgetType == WebWidgetType::Webkit)
	{
		if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
		{
			_webWidgetType = WebWidgetType::WebEngine;
		}
		else
		{
			_webWidgetType = WebWidgetType::Webkit;
			_applyCssToGameFlag = false;	// close
		}
	}

	_gameUrl = setting->value(itemGameUrl).toString();
	_gameUrlId = setting->value(itemUrlId).toString();
	_useport = setting->value(itemUsePort).toInt();
	ControlManager::getInstance().setIntervalMul(setting->value(itemIntervalMul).toInt() / 100.0);

	QString proxymode = setting->value(itemProxyMode).toString();
	if (!proxymode.compare("NoProxy", Qt::CaseInsensitive))
	{
		_proxyMode = ProxyMode::NoProxy;
	}
	else if (!proxymode.compare("QtProxy", Qt::CaseInsensitive))
	{
		_proxyMode = ProxyMode::QtProxy;
	}
	else if (!proxymode.compare("Fid", Qt::CaseInsensitive))
	{
		_proxyMode = ProxyMode::Fid;
	}
	else if (!proxymode.compare("Nekoxy", Qt::CaseInsensitive))
	{
		_proxyMode = ProxyMode::Nekoxy;
	}
	else if (!proxymode.compare("Titanium", Qt::CaseInsensitive))
	{
		_proxyMode = ProxyMode::Titanium;
	}

	// no longer use ini file for expedition
	ExpeditionManager::getInstance().BuildByPreset("Default");

	setTimeShift(setting->value(itemTimeShift).toInt());

	setting->endGroup();

	delete setting;
}

void MainWindow::loadCertKey()
{
	QSettings settings("h5nc", "KanPlay");
	_certStr = settings.value("CertStr", "").toString();
	_keyStr = settings.value("KeyStr", "").toString();
}

void MainWindow::saveCertKey()
{
	QSettings settings("h5nc", "KanPlay");
	settings.setValue("CertStr", _certStr);
	settings.setValue("KeyStr", _keyStr);
}

void MainWindow::applyCss(QWebViewCSSIndex css)
{
	if (css < QWebViewCSSIndex::Normal || css >= QWebViewCSSIndex::Max)
	{
		return;
	}
	if (_webWidgetType == WebWidgetType::IE)
	{
#ifdef Q_OS_WIN
		_applyCssWhenLoaded = css;
		if (!_bIEPageLoaded)
		{
			return;
		}

		IDispatch* htmlDoc = _axWidget->Document();
		if (!htmlDoc)
		{
			return;
		}
		QAxObject * htmlDocObj = new QAxObject(htmlDoc);

		if (!htmlDocObj)
		{
			return;
		}

		QAxObject * styleSheetObj = htmlDocObj->querySubObject("createStyleSheet()");
		if (!styleSheetObj)
		{
			return;
		}

		styleSheetObj->dynamicCall("setCssText(QString)", _ieCsses[(int)css]);
		_applyCssWhenLoaded = QWebViewCSSIndex::Invalid;
		delete htmlDocObj;
#endif
	}
	else
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
		QString jsStr = 
			"function addStyleString(str) {\
										var node = document.createElement('style');\
																					node.innerHTML = str;\
																																				document.body.appendChild(node);\
																																																						}\
																																																																											addStyleString('" + _ieCsses[(int)css] + "'); \
																																																																																																			addStyleString('body { background: silver }'); ";

		_webView->page()->runJavaScript(jsStr);
#else
		_webView->page()->settings()->setUserStyleSheetUrl(_webViewCsses[(int)css]);
#endif
	}
}
