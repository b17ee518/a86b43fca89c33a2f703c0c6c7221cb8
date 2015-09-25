#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QNetworkProxy>
#include "cookiejar.h"
#include <QNetworkDiskCache>
#include <QStandardPaths>
#include "qnetworkproxyfactoryset.h"
#include "kqwebpage.h"
#include "kandataconnector.h"
#include <QWinTaskbarProgress>
#include <QMessageBox>
#include "kqnetworkaccessmanager.h"
#include <QNetworkRequest>
#include <QPixmap>
#include <QWebFrame>
#include <QShortcut>

#include <Audiopolicy.h>
#include <Mmdeviceapi.h>
#include <QHttpPart>

#include <QAxWidget>
#include <QSettings>
#include <QSysInfo>
#include "ControlManager.h"

#define SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; } 

MainWindow * MainWindow::s_pMainWindow = NULL;

MainWindow::MainWindow(QWidget *parent) :
	MainWindowBase(parent),
	ui(new Ui::MainWindow)
{
	loadSettings();

	_pScreenshotTimer = new QTimer(this);
	connect(_pScreenshotTimer, SIGNAL(timeout()), this, SLOT(slotScreenshotTimeout()));
	_panicTimer = new QTimer(this);
	connect(_panicTimer, SIGNAL(timeout()), this, SLOT(onPanic()));
	
	setMainWindow(this);
	ui->setupUi(this);
	ui->retranslateUi(this);

	if (_bUseIE)
	{
		setAttribute(Qt::WA_TranslucentBackground, false);

		rebuildIE(false);
		
		HWND hWnd = (HWND)this->winId();
		LONG lStyle = ::GetWindowLong(hWnd, GWL_STYLE);
		lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
		::SetWindowLong(hWnd, GWL_STYLE, lStyle);
		LONG lExStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
		::SetWindowLong(hWnd, GWL_EXSTYLE, lExStyle | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT);
		typedef int (WINAPI* LPFUNC)(HWND, COLORREF, BYTE, DWORD);
		HINSTANCE hins = ::LoadLibraryW(L"User32.DLL");
		if (hins)
		{
			LPFUNC funcSetLayeredWindowAttributes = (LPFUNC)GetProcAddress(hins, "SetLayeredWindowAttributes");
			if (funcSetLayeredWindowAttributes)
			{
				COLORREF clrMask = RGB(255, 255, 255);
				funcSetLayeredWindowAttributes(hWnd, clrMask, 0, LWA_COLORKEY);
			}
			FreeLibrary(hins);
		}
	}
	else
	{
		_webView = new QWebView(ui->webFrame);
		_webView->setObjectName(QStringLiteral("webView"));
		_webView->setUrl(QUrl(QStringLiteral("about:blank")));
		ui->webFrame_layout->addWidget(_webView);
	}
	
	mwbPostInit();

	ui->titleFrame->setHandlingWidget(this);

	connect(this,
		SIGNAL(sigParse(const QString &, const QString &, const QString &)),
		this,
		SLOT(slotParse(const QString &, const QString &, const QString &)));

	QShortcut *shortcut = new QShortcut(QKeySequence("Escape"), this);
	connect(shortcut, SIGNAL(activated()), this, SLOT(onPanic()));

	QShortcut *shortcutDoJob = new QShortcut(QKeySequence("Ctrl+F9"), this);
	connect(shortcutDoJob, SIGNAL(activated()), this, SLOT(onDoJobFuel()));

	QShortcut *shortcutDoJobKira = new QShortcut(QKeySequence("Ctrl+F10"), this);
	connect(shortcutDoJobKira, SIGNAL(activated()), this, SLOT(onDoJobKira()));

	QShortcut *shortcutExport = new QShortcut(QKeySequence("Ctrl+F6"), this);
	connect(shortcutExport, SIGNAL(activated()), this, SLOT(onExportAllList()));

	QShortcut * shortcutPauseResume = new QShortcut(QKeySequence("F8"), this);
	connect(shortcutPauseResume, SIGNAL(activated()), this, SLOT(onJobPauseResume()));
	QShortcut * shortcutPauseNext = new QShortcut(QKeySequence("F7"), this);
	connect(shortcutPauseNext, SIGNAL(activated()), this, SLOT(onJobPauseNext()));

	QShortcut * shortcutTerminateJob = new QShortcut(QKeySequence("F12"), this);
	connect(shortcutTerminateJob, SIGNAL(activated()), this, SLOT(onTerminateJob()));

	_jobTimer = new QTimer(this);
	connect(_jobTimer, SIGNAL(timeout()), this, SLOT(slotJobtTimeout()));
	_jobTimer->start(200);


	_bMoveSubTogether = true;

	setWebSettings();
	setupCss();

	navigateTo(_gameUrl);
	
//	navigateTo("http://www.google.com");
//	navigateTo("http://www.dmm.com/netgame/social/-/gadgets/=/app_id=854854/?f1=1&p1=0");
//	navigateTo("https://www.dmm.com/my/-/login/auth/");

	QApplication::instance()->installNativeEventFilter(&_windowsEventfilter);

}

MainWindow::~MainWindow()
{
	if (_pFiddler)
	{
		delete _pFiddler;
	}
	delete ui;
}

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

	AdjustVolume(-1);

	HWND hwnd = (HWND)effectiveWinId();
	RegisterPowerSettingNotification(hwnd, &GUID_CONSOLE_DISPLAY_STATE, DEVICE_NOTIFY_WINDOW_HANDLE);
	RegisterPowerSettingNotification(hwnd, &GUID_SYSTEM_AWAYMODE, DEVICE_NOTIFY_WINDOW_HANDLE);
}

void MainWindow::onSubMainWindowShowHide(bool bShow, MainWindowBase *pWindow)
{
	QPushButton * pButton = 0;
	if (pWindow == _pInfoWindow)
	{
		pButton = ui->pbCheckInfo;
	}
	else if (pWindow == _pTimerWindow)
	{
		pButton = ui->pbCheckTimer;
	}
	else if (pWindow == _pWeaponWindow)
	{
		pButton = ui->pbCheckWeapon;
	}
	else if (pWindow == _pShipWindow)
	{
		pButton = ui->pbCheckShip;
	}
	if (!pButton)
	{
		return;
	}

	pButton->setChecked(bShow);
}


void MainWindow::changeEvent(QEvent *e)
{
	MainWindowBase::changeEvent(e);

	if (e->type() == QEvent::WindowStateChange)
	{
		QWindowStateChangeEvent* event = static_cast<QWindowStateChangeEvent*>(e);
		if(event->oldState() & Qt::WindowMinimized)
		{
//            if (ui->pbCheckInfo->isChecked())
            {
                _pInfoWindow->setWindowState(Qt::WindowNoState);
                _pInfoWindow->show();
                _pInfoWindow->stackUnder(this);
            }

//            if (ui->pbCheckTimer->isChecked())
            {
                _pTimerWindow->setWindowState(Qt::WindowNoState);
                _pTimerWindow->show();
                _pTimerWindow->stackUnder(_pInfoWindow);
            }

//            if (ui->pbCheckWeapon->isChecked())
//            {
//                m_pWeaponWindow->setWindowState(Qt::WindowNoState);
//                m_pWeaponWindow->show();
//                m_pWeaponWindow->stackUnder(m_pWeaponWindow);
//            }
			this->raise();
			this->activateWindow();
			this->setSleepMode(false);
		}
		else if (isMinimized())
		{
			if (!_pInfoWindow->isDockingOnTop())
			{
				_pInfoWindow->hide();
			}
			if (!_pTimerWindow->isDockingOnTop())
			{
				_pTimerWindow->hide();
			}
			if (!_pWeaponWindow->isDockingOnTop())
			{
				_pWeaponWindow->hide();
			}
			if (!_pShipWindow->isDockingOnTop())
			{
				_pShipWindow->hide();
			}
//			this->setSleepMode(true);
		}
	}

}

void MainWindow::closeEvent(QCloseEvent *e)
{
	if (!ControlManager::getInstance().isRunning())
	{
		QMessageBox * pMessageBox = new QMessageBox(
			QMessageBox::NoIcon
			, QString::fromLocal8Bit("")
			, QString::fromLocal8Bit("本当に終了しますか？")
			, QMessageBox::Yes | QMessageBox::No
			, this
			, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
		pMessageBox->setDefaultButton(QMessageBox::No);
		pMessageBox->setAttribute(Qt::WA_DeleteOnClose, true);
		int reply = pMessageBox->exec();

		if (reply == QMessageBox::No)
		{
			e->ignore();
			return;
		}
	}

	if (_bUseFiddler)
	{
		_pFiddler->Shutdown();
	}

	ControlManager::getInstance().Terminate();

	MainWindowBase::closeEvent(e);

	_pInfoWindow->close();
	_pTimerWindow->close();
    _pWeaponWindow->close();
	_pShipWindow->close();
}

void MainWindow::moveEvent(QMoveEvent *e)
{
	MainWindowBase::moveEvent(e);

	QPoint diffPos = e->pos()-e->oldPos();

	if (_bMoveSubTogether)
	{
		if (!_pInfoWindow->isDockingOnTop())
		{
			_pInfoWindow->move(_pInfoWindow->pos() + diffPos);
		}
		if (!_pTimerWindow->isDockingOnTop())
		{
			_pTimerWindow->move(_pTimerWindow->pos() + diffPos);
		}
		if (!_pWeaponWindow->isDockingOnTop())
		{
			_pWeaponWindow->move(_pWeaponWindow->pos() + diffPos);
		}
		if (!_pShipWindow->isDockingOnTop())
		{
			_pShipWindow->move(_pShipWindow->pos() + diffPos);
		}
	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
	QRect rc = QApplication::desktop()->availableGeometry(QApplication::desktop()->primaryScreen());
	if (x() < 20 && x() > -20 && x()!=0)
	{
		move(0, y());
	}
	else if (x()+width() > rc.width()-20 && x()+width() < rc.width()+20 && x()!=rc.width()-width())
	{
		move(rc.width()-width(), y());
	}
	if (y() < 20 && y()!=0)
	{
		move(x(), 0);
	}
	MainWindowBase::mouseReleaseEvent(event);
}

void MainWindow::slotActivate(QWidget *w, bool bActivate)
{
	Q_UNUSED(bActivate);
	this->raise();
	w->stackUnder(this);
}

void MainWindow::slotToggleRestoreMinimize(bool bRestore)
{
	if (bRestore)
	{
		if (isMinimized())
		{
			this->setWindowState(Qt::WindowNoState);
		}
	}
	if (!bRestore)
	{
		if (!isMinimized())
		{
			this->setWindowState(Qt::WindowMinimized);
		}
	}
}

void MainWindow::slotSelfToggleRestoreMinimize(bool bRestore)
{
	if (!bRestore)
	{
		this->AdjustVolume(0);
	}
	else
	{
		this->AdjustVolume(-1);
	}
}

void MainWindow::slotParse(const QString &PathAndQuery, const QString &requestBody, const QString &responseBody)
{
	KanDataConnector::getInstance().Parse(PathAndQuery, requestBody, responseBody);
}

void MainWindow::on_pbClose_clicked()
{
	close();
}

void MainWindow::on_pbMinimize_clicked()
{
	setWindowState(Qt::WindowMinimized);
}

void MainWindow::on_pbCheckTransparent_toggled(bool checked)
{
	static QList<QPair<QString, QWidget*>> s_listpair;
	if (checked)
	{
		if (!_bUseIE)
		{
			s_listpair.clear();

			QList<QWidget *> lstWidget = findChildren<QWidget *>();
			QWidget * w;
			foreach(w, lstWidget)
			{
				QPair<QString, QWidget*> pair;
				pair.first = w->styleSheet();
				pair.second = w;
				s_listpair.append(pair);
			}

			lstWidget = _pInfoWindow->findChildren<QWidget *>();
			foreach(w, lstWidget)
			{
				QPair<QString, QWidget*> pair;
				pair.first = w->styleSheet();
				pair.second = w;
				s_listpair.append(pair);
			}
			lstWidget = _pTimerWindow->findChildren<QWidget *>();
			foreach(w, lstWidget)
			{
				QPair<QString, QWidget*> pair;
				pair.first = w->styleSheet();
				pair.second = w;
				s_listpair.append(pair);
			}
			lstWidget = _pWeaponWindow->findChildren<QWidget *>();
			foreach(w, lstWidget)
			{
				QPair<QString, QWidget*> pair;
				pair.first = w->styleSheet();
				pair.second = w;
				s_listpair.append(pair);
			}
			lstWidget = _pShipWindow->findChildren<QWidget *>();
			foreach(w, lstWidget)
			{
				QPair<QString, QWidget*> pair;
				pair.first = w->styleSheet();
				pair.second = w;
				s_listpair.append(pair);
			}

			QPair<QString, QWidget*> p;
			foreach(p, s_listpair)
			{
				w = p.second;
				QString stylestr = w->styleSheet();
				stylestr += "background-color: rgba(45, 45, 48, 30%);";
				w->setStyleSheet(stylestr);
			}
		}

		applyCss(QWebViewCSSIndex::Transparent);
	}
	else
	{
		QPair<QString, QWidget*> p;
		if (!_bUseIE)
		{
			foreach(p, s_listpair)
			{
				if (isAncestorOf(p.second) || _pInfoWindow->isAncestorOf(p.second) || _pTimerWindow->isAncestorOf(p.second))
				{
					p.second->setStyleSheet(p.first);
				}
			}
			s_listpair.clear();
		}

		applyCss(QWebViewCSSIndex::Normal);
	}

}

void MainWindow::slotWebViewException(int code, const QString &source, const QString &desc, const QString &help)
{
	qDebug("Exception: %d", code);
	qDebug(source.toUtf8());
	qDebug(desc.toUtf8());
	qDebug(help.toUtf8());
	qDebug("Put FiddlerCore.dll to exe folder.");
}

void MainWindow::BeforeRequestFunc(int sessionID, char *fullURL, char *requestBody)
{
//	emit mainWindow()->sigTogglePanicTimer(4000);
	// comment out for tablet

	Q_UNUSED(sessionID);
	Q_UNUSED(fullURL);
	Q_UNUSED(requestBody);

}

void MainWindow::AfterSessionCompleteFunc(int sessionID, char *mimeType, int responseCode, char *PathAndQuery, char *requestBody, char *responseBody)
{
//	emit mainWindow()->sigTogglePanicTimer(-1);
	// comment out for tablet

	Q_UNUSED(sessionID);
	Q_UNUSED(responseCode);
	QString strPathAndQuery = PathAndQuery;

	if (strPathAndQuery.startsWith("/kcsapi"))
	{
		if (0 == QString::compare(mimeType, "text/plain"))
		{
			emit MainWindow::mainWindow()->sigParse(PathAndQuery, requestBody, responseBody);
//            KanDataConnector::getInstance().Parse(PathAndQuery, requestBody, responseBody);
			/*
			QString str = QDateTime::currentDateTime().toString("[yyyy/MM/dd HH:mm:ss]\t");
			str += PathAndQuery;
			str += "\t";
			str += QUrl::fromPercentEncoding(requestBody);
			str += "\t";
			str += responseBody;
			str += "\n";

			pLogFile->write(str.toLocal8Bit());
			*/
		}
	}
}

void MainWindow::onGetNetworkReply(QNetworkReply * reply)
{
	QString mimetype = reply->header(QNetworkRequest::ContentTypeHeader).toString();
	if (mimetype.contains("text/plain"))
	{
		QUrl url = reply->request().url();
		QString PathAndQuery = url.path();
		if (PathAndQuery.startsWith("/kcsapi"))
		{
			QString requestBody = reply->property("requestBody").toString();
			QByteArray bytes = reply->readAll();
			QString responseBody = QString::fromUtf8(bytes.constData(), bytes.size());
			/*
			qDebug(QString::number(reply->bytesAvailable()).toLocal8Bit());
			qDebug(QString::number(reply->readBufferSize()).toLocal8Bit());
			qDebug(QString::number(reply->size()).toLocal8Bit());
			*/
			emit MainWindow::mainWindow()->sigParse(PathAndQuery, requestBody, responseBody);
		}
	}
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

QWidget* MainWindow::getBrowserWidget()
{
	if (_bUseIE)
	{
		return _axWidget;
	}
	return _webView;
}

void MainWindow::navigateTo(const QString& urlString)
{
	if (_bUseIE)
	{
		_axWidget->Navigate(urlString);
	}
	else
	{
		_webView->load(QUrl(urlString));
	}
}

void MainWindow::navigateReload()
{
	if (_bUseIE)
	{
		rebuildIE(true);
//		_axWidget->Refresh();
		// refresh won't work with css
//		navigateTo(_gameUrl);
	}
	else
	{
		_webView->reload();
	}
}

bool MainWindow::isUsingIE()
{
	return _bUseIE;
}

void MainWindow::slotTogglePanicTimer(int timeVal)
{
	if (timeVal > 0)
	{
		_panicTimer->start(timeVal);
	}
	else
	{
		_panicTimer->stop();
	}
}


void MainWindow::setWebSettings()
{
	if (_bUseFiddler)
	{
		_pFiddler = new FiddlerCOM::FiddlerCOMClass(this);

		loadCertKey();
		_pFiddler->SetupCertificate(_certStr, _keyStr);
		_pFiddler->InstallCertificate(_certStr, _keyStr);
		saveCertKey();
		
		connect(
			_pFiddler,
			SIGNAL(exception(int, const QString &, const QString &, const QString &)),
			this,
			SLOT(slotWebViewException(int, const QString &, const QString &, const QString &)));


		_pFiddler->SetBeforeRequest((int)&MainWindow::BeforeRequestFunc);
		_pFiddler->SetAfterSessionComplete((int)&MainWindow::AfterSessionCompleteFunc);

		_pFiddler->Startup(_useport, false, true);

		QNetworkProxyFactorySet * proxies = new QNetworkProxyFactorySet();
		proxies->init(_useport);
		QNetworkProxyFactory::setApplicationProxyFactory(proxies);
	}

	if (!_bUseIE)
	{
		KQWebPage * page = new KQWebPage();
		_webView->setPage(page);

		_webView->setContextMenuPolicy(Qt::PreventContextMenu);

		if (!_bUseFiddler)
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

		QWebSettings *websetting = QWebSettings::globalSettings();
		//JavaScript関連設定
		websetting->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
		websetting->setAttribute(QWebSettings::JavascriptCanCloseWindows, true);
		websetting->setAttribute(QWebSettings::PluginsEnabled, true);
		websetting->setAttribute(QWebSettings::JavascriptEnabled, true);
		//フォント設定
#if defined(Q_OS_WIN32)
		//    websetting->setFontFamily(QWebSettings::StandardFont, "ＭＳ Ｐゴシック");
		//    websetting->setFontFamily(QWebSettings::StandardFont, "MS PGothic");
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

		//    QNetworkProxyFactory::setUseSystemConfiguration(false);
		//    updateProxyConfiguration();
	}
}

void MainWindow::AdjustVolume(int vol)
{
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
}

void MainWindow::slotSoundEnded()
{
	if (isMinimized())
	{
		AdjustVolume(0);
	}
	else
	{
		AdjustVolume(-1);
	}
}

void MainWindow::showEvent(QShowEvent *event)
{
	MainWindowBase::showEvent(event);
	if (!_pTaskbarButton)
	{
		_pTaskbarButton = new QWinTaskbarButton(this);
		_pTaskbarButton->setWindow(windowHandle());
		QWinTaskbarProgress * pTaskbarProgress = _pTaskbarButton->progress();
		pTaskbarProgress->setVisible(true);

		pTaskbarProgress->setMinimum(0);
		pTaskbarProgress->setMaximum(10000);
		pTaskbarProgress->setValue(0);
		pTaskbarProgress->show();
	}
}

void MainWindow::SetProgressBarPos(int pos, ProgressBarState state)
{
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
}

void MainWindow::on_pbMoveTogether_toggled(bool checked)
{
	_bMoveSubTogether = checked;
}

void MainWindow::on_pbRefresh_clicked()
{
	QMessageBox * pMessageBox = new QMessageBox(
		QMessageBox::NoIcon
		, QString::fromLocal8Bit("")
		, QString::fromLocal8Bit("リロードしますか？")
		, QMessageBox::Yes | QMessageBox::No
		, this
		, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
	pMessageBox->setDefaultButton(QMessageBox::No);
	pMessageBox->setAttribute(Qt::WA_DeleteOnClose, true);
	int reply = pMessageBox->exec();

	if (reply == QMessageBox::Yes)
	{
		navigateReload();
	}

}

void MainWindow::on_pbScreenshot_clicked()
{
	shootScreen();
}

void MainWindow::onPanic()
{
	QString jsstr = QString::fromLocal8Bit("window.alert('%1'); ").arg(QString::fromLocal8Bit("パニック！"));
	if (_bUseIE)
	{
		// TODO
		QMessageBox* pMessageBox = new QMessageBox(
			QMessageBox::NoIcon
			, QString::fromLocal8Bit("")
			, QString::fromLocal8Bit("パニック！")
			, QMessageBox::Ok
			, this
			, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
		pMessageBox->setDefaultButton(QMessageBox::NoButton);
		pMessageBox->setAttribute(Qt::WA_DeleteOnClose, true);
		pMessageBox->exec();
	}
	else
	{
		_webView->page()->mainFrame()->evaluateJavaScript(jsstr);
	}
	if (_panicTimer)
	{
		_panicTimer->stop();
	}
}

void MainWindow::onDoJobFuel()
{
	auto& cm = ControlManager::getInstance();
	cm.Terminate();

	if (QApplication::queryKeyboardModifiers()&Qt::ShiftModifier)
	{
		QMessageBox* pMessageBox = new QMessageBox(
			QMessageBox::NoIcon
			, QString::fromLocal8Bit("")
			, QString::fromLocal8Bit("終了条件")
			, QMessageBox::NoButton
			, this
			, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);

		pMessageBox->addButton(QString::fromLocal8Bit("輸送×3"), QMessageBox::YesRole);
		pMessageBox->addButton(QString::fromLocal8Bit("南西×5"), QMessageBox::NoRole);
		pMessageBox->addButton(QString::fromLocal8Bit("永久"), QMessageBox::ApplyRole);

		pMessageBox->setDefaultButton(QMessageBox::NoButton);
		pMessageBox->setAttribute(Qt::WA_DeleteOnClose, true);
		int reply = pMessageBox->exec();

		if (reply == QMessageBox::Yes)
		{
			cm.setStopWhen(ControlManager::StopWhen::Yusou3);
		}
		else if (reply == QMessageBox::No)
		{
			cm.setStopWhen(ControlManager::StopWhen::SouthEast5);
		}
	}

	if (cm.isFlagshipOnly())
	{
		cm.BuildNext_Fuel();
	}
	else
	{
		cm.BuildNext_SouthEast();
	}

	cm.StartJob();
}


void MainWindow::onDoJobKira()
{
	auto& cm = ControlManager::getInstance();
	cm.Terminate();

	cm.LoadToDoShipList_Kira();
	cm.BuildNext_Kira();
	cm.StartJob();

}

void MainWindow::onExportAllList()
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector * pkdc = &KanDataConnector::getInstance();
	struct ExportInfo
	{
		int shipno;
		int shiptype;
		int shipid;
		QString shipname;
		int lv;
		int cond;
	};
	QList<ExportInfo> infos;
	if (pksd->portdata.api_ship.size())
	{
		for (auto& ship:pksd->portdata.api_ship)
		{
			if (ship.api_lv < 10)
			{
				continue;
			}

			auto pmstship = pkdc->findMstShipFromShipid(ship.api_ship_id);
			if (pmstship)
			{
				ExportInfo info;
				info.shipno = ship.api_id;
				info.shiptype = pmstship->api_stype;
				info.shipid = ship.api_ship_id;
				info.lv = ship.api_lv;
				info.shipname = pmstship->api_name;
				info.cond = ship.api_cond;
				infos.append(info);
			}
		}
	}
	if (infos.size())
	{
		qSort(infos.begin(), infos.end(), [](const ExportInfo&left, const ExportInfo&right)
		{
			if (left.shiptype < right.shiptype)
			{
				return true;
			}
			else if (left.shiptype == right.shiptype)
			{
				/*
				if (left.cond < right.cond)
				{
					return true;
				}
				else if (left.cond == right.cond)
				{*/
					if (left.shipid < right.shipid)
					{
						return true;
					}
					else if (left.shipid == right.shipid)
					{
						if (left.shipno < right.shipno)
						{
							return true;
						}
					}
//				}
			}
			return false;
		});


		QFile * file = new QFile(QApplication::applicationDirPath() + "/action/" + "exported.table");
		if (file)
		{
			if (file->open(QIODevice::WriteOnly | QIODevice::Text))
			{
				int lastShipType = -1;
				for (auto& info:infos)
				{
					QString str;
					if (lastShipType >= 0 && lastShipType != info.shiptype)
					{
						str += "\n";
					}
					lastShipType = info.shiptype;
					str += "-\t" +
						QString::number(info.shipno) + "\t" + 
						info.shipname + "\t" + 
						QString::number(info.lv) + "\t" +
						QString::number(info.cond) + "\n";

					QTextStream outstream(file);
					outstream.setCodec("UTF-16");
					outstream.setGenerateByteOrderMark(true);
					outstream << str;
				}

				file->close();
			}


			delete file;
		}
	}
	// export master data
	KanSaveData::getInstance().logAllExport();
}

void MainWindow::onTerminateJob()
{
	ControlManager::getInstance().Terminate();
}

void MainWindow::slotSetLogAll(bool bSet)
{
	KanDataConnector::getInstance().setOutputAllLog(bSet);
}

void MainWindow::onJobPauseResume()
{
	if (ControlManager::getInstance().isPaused())
	{
		ControlManager::getInstance().Resume();
	}
	else
	{
		ControlManager::getInstance().Pause();
	}
}

void MainWindow::onJobPauseNext()
{
	if (ControlManager::getInstance().isPaused())
	{
		ControlManager::getInstance().Resume();
	}
	else
	{
		ControlManager::getInstance().togglePauseNext();
	}
	ui->pbPauseNext->setChecked(ControlManager::getInstance().getPauseNextVal());
}

void MainWindow::on_pbSwitchScreenshot_toggled(bool checked)
{
	if (checked)
	{
		shootScreen();
		_pScreenshotTimer->start(250);
	}
	else
	{
		_pScreenshotTimer->stop();
	}
}

void MainWindow::shootScreen()
{
	if (ui->pbCheckTransparent->isChecked())
	{
		applyCss(QWebViewCSSIndex::Normal);
	}
	QPixmap pixmap = QPixmap::grabWidget(getBrowserWidget());

	QString filename = QApplication::applicationDirPath();
	filename += "/screenshot/";

	filename += QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss_zzz.png");

	pixmap.save(filename, "PNG");
	if (ui->pbCheckTransparent->isChecked())
	{
		applyCss(QWebViewCSSIndex::Transparent);
	}
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
	_ieCsses[(int)QWebViewCSSIndex::Normal] = "\
								body {\
									margin:0;\
									overflow:hidden;\
								}\
								#game_frame{\
									position:fixed;\
									top:-16px;\
									left:-50px;\
									z-index:1\
								 }";
	_webViewCsses[(int)QWebViewCSSIndex::Normal] = 
		(QUrl("data:text/css;charset=utf-8;base64,Ym9keSB7DQoJbWFyZ2luOjA7DQoJb3ZlcmZsb3c6aGlkZGVuDQp9DQoNCiNnYW1lX2ZyYW1lIHsNCglwb3NpdGlvbjpmaXhlZDsNCgl0b3A6LTE2cHg7DQoJbGVmdDotNTBweDsNCgl6LWluZGV4OjENCn0="));
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
	_ieCsses[(int)QWebViewCSSIndex::Transparent] = "\
										body {\
											margin:0;\
											overflow:visible;\
										}\
										#game_frame{\
											position:fixed;\
											top:-16px;\
											left:-50px;\
											z - index:1;\
										}\
										";
	
	applyCss(QWebViewCSSIndex::Normal);
}

void MainWindow::loadSettings()
{
	QString filename = QApplication::applicationDirPath();
	filename += "/settings.ini";
	QSettings* setting = new QSettings(filename, QSettings::IniFormat);
	setting->setIniCodec("UTF-8");

	const QString itemUseIE = "UseIE";
	const QString itemGameUrl = "GameUrl";
	const QString itemUrlId = "UrlId";
	const QString itemUsePort = "UsePort";
	
	setting->beginGroup("Settings");
	if (!setting->contains(itemUseIE))
	{
		if (QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS8)
		{
			setting->setValue(itemUseIE, true);
		}
		else
		{
			setting->setValue(itemUseIE, false);
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

	_bUseIE = setting->value(itemUseIE).toBool();
	_gameUrl = setting->value(itemGameUrl).toString();
	_gameUrlId = setting->value(itemUrlId).toString();
	_useport = setting->value(itemUsePort).toInt();

	setting->endGroup();

	delete setting;
}

void MainWindow::rebuildIE(bool bNavigate)
{
	if (_axWidget)
	{
		WebBrowser* toDelete = _axWidget;
		QTimer::singleShot(1, [toDelete, this](){
			QObject::disconnect(toDelete,
				SIGNAL(NavigateComplete2(IDispatch*, QVariant&)),
				this,
				SLOT(slotNavigateComplete2(IDispatch*, QVariant&)));
			toDelete->clear();
			delete toDelete; 
		});
	}

	_axWidget = new WebBrowser(ui->webFrame);
	_axWidget->Navigate("about:blank");

	QObject::connect(_axWidget,
		SIGNAL(NavigateComplete2(IDispatch*, QVariant&)),
		this,
		SLOT(slotNavigateComplete2(IDispatch*, QVariant&)));
	ui->webFrame_layout->addWidget(_axWidget);
	if (bNavigate)
	{
		navigateTo(_gameUrl);
	}
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
	if (_bUseIE)
	{
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
	}
	else
	{
		_webView->page()->settings()->setUserStyleSheetUrl(_webViewCsses[(int)css]);
	}
}

void MainWindow::slotScreenshotTimeout()
{
	shootScreen();
}

void MainWindow::slotJobtTimeout()
{
	ControlManager::getInstance().Run();
}

void MainWindow::on_pbCheckLowVol_toggled(bool checked)
{
	_bLowVol = checked;
	AdjustVolume(-1);
}

void MainWindow::slotNavigateComplete2(IDispatch*, QVariant& url)
{
	_bIEPageLoaded = true;
	QString urlStr = url.toString();
	if (urlStr.contains(_gameUrlId))
	{
		applyCss(QWebViewCSSIndex::Transparent);
		applyCss(QWebViewCSSIndex::Normal);
	}
	if (_applyCssWhenLoaded > QWebViewCSSIndex::Invalid)
	{
		applyCss(_applyCssWhenLoaded);
	}
}

void MainWindow::setPauseNextChanged(bool bVal)
{
	ui->pbPauseNext->setChecked(bVal);
}

QWindowsEventFilter::QWindowsEventFilter()
{

}

bool QWindowsEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
	Q_UNUSED(result);
	if (eventType == "windows_generic_MSG")
	{
		MSG *msg = static_cast<MSG *>(message);
		if (msg->message == WM_POWERBROADCAST)
		{
			auto mainWindow = MainWindow::mainWindow();
			if (!mainWindow)
			{
				return false;
			}
			if (msg->wParam == PBT_APMSUSPEND)
			{
				// sleep
				mainWindow->setSleepMode(true);
				return false;
			}
			if (msg->wParam == PBT_APMRESUMEAUTOMATIC)
			{
				// resume
				mainWindow->setSleepMode(false);
				return false;
			}
			if (msg->wParam == PBT_POWERSETTINGCHANGE)
			{
				POWERBROADCAST_SETTING *ps = (POWERBROADCAST_SETTING*)(msg->lParam);
				if (ps->PowerSetting == GUID_CONSOLE_DISPLAY_STATE)
				{
					DWORD dw = *(DWORD*)(ps->Data);
					if (dw == 0x0)
					{
						// off
//						mainWindow->setSleepMode(true);
						return false;
					}
					if (dw == 0x1)
					{
						// on
//						mainWindow->setSleepMode(false);
						return false;
					}
				}
			}
			return false;
		}
	}
	return false;
}
