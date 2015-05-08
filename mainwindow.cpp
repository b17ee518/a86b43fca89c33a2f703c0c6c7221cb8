﻿#include "mainwindow.h"
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

#define SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; } 


enum{
	QWEBVIEWCSS_NORMAL,
	QWEBVIEWCSS_TRANSPARENT,
//	QWEBVIEWCSS_CLEAR,
	QWEBVIEWCSS_END,
};
QUrl csses[QWEBVIEWCSS_END];


MainWindow * MainWindow::s_pMainWindow = NULL;

MainWindow::MainWindow(QWidget *parent) :
	MainWindowBase(parent),
	ui(new Ui::MainWindow)
{
	m_pTaskbarButton = NULL;

	m_pScreenshotTimer = new QTimer(this);
	connect(m_pScreenshotTimer, SIGNAL(timeout()), this, SLOT(slotScreenshotTimeout()));
	m_panicTimer = new QTimer(this);
	connect(m_panicTimer, SIGNAL(timeout()), this, SLOT(onPanic()));

	bUseFiddler = true;
//	bUseFiddler = false;

	setMainWindow(this);
	ui->setupUi(this);
	ui->retranslateUi(this);

	ui->comboBoxZoom->insertSeparator(1);

	mwbPostInit();

	ui->titleFrame->setHandlingWidget(this);

	connect(this,
		SIGNAL(sigParse(const QString &, const QString &, const QString &)),
		this,
		SLOT(slotParse(const QString &, const QString &, const QString &)));

	QShortcut *shortcut = new QShortcut(QKeySequence("Escape"), this);
	connect(shortcut, SIGNAL(activated()), this, SLOT(onPanic()));

	bMoveSubTogether = true;

	SetWebSettings();

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
	csses[QWEBVIEWCSS_NORMAL] = (QUrl("data:text/css;charset=utf-8;base64,Ym9keSB7DQoJbWFyZ2luOjA7DQoJb3ZlcmZsb3c6aGlkZGVuDQp9DQoNCiNnYW1lX2ZyYW1lIHsNCglwb3NpdGlvbjpmaXhlZDsNCgl0b3A6LTE2cHg7DQoJbGVmdDotNTBweDsNCgl6LWluZGV4OjENCn0="));
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
//	csses[QWEBVIEWCSS_TRANSPARENT] = QUrl("data:text/css;charset=utf-8;base64,Ym9keSB7DQoJbWFyZ2luOjA7DQoJb3ZlcmZsb3c6aGlkZGVuOw0KCW9wYWNpdHk6IDAuNDsNCn0NCg0KI2dhbWVfZnJhbWUgew0KCXBvc2l0aW9uOmZpeGVkOw0KCXRvcDotMTZweDsNCglsZWZ0Oi01MHB4Ow0KCXotaW5kZXg6MTsNCn0=");

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
	csses[QWEBVIEWCSS_TRANSPARENT] = QUrl("data:text/css;charset=utf-8;base64,Ym9keSB7DQoJbWFyZ2luOjA7DQoJb3ZlcmZsb3c6aGlkZGVuOw0KCW9wYWNpdHk6IDAuMjsNCn0NCg0KI2dhbWVfZnJhbWUgew0KCXBvc2l0aW9uOmZpeGVkOw0KCXRvcDotMTZweDsNCglsZWZ0Oi01MHB4Ow0KCXotaW5kZXg6MTsNCn0=");
	
	ui->webView->page()->settings()->setUserStyleSheetUrl(csses[QWEBVIEWCSS_NORMAL]);
	
//  ui->webView->load(QUrl("http://www.google.com"));
	ui->webView->load(QUrl("http://www.dmm.com/netgame/social/-/gadgets/=/app_id=854854/?f1=1&p1=0"));
//	ui->webView->load(QUrl("https://www.dmm.com/my/-/login/auth/"));

	QApplication::instance()->installNativeEventFilter(&m_windowsEventfilter);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::postInit(InfoMainWindow *pInfo, TimerMainWindow *pTimer, WeaponMainWindow* pWeapon)
{
	m_pInfoWindow = pInfo;
	m_pTimerWindow = pTimer;
    m_pWeaponWindow = pWeapon;

    QObject::connect( this, SIGNAL( sigActivated( QWidget*, bool ) ), m_pInfoWindow, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( this, SIGNAL( sigActivated( QWidget*, bool ) ), m_pTimerWindow, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( this, SIGNAL( sigActivated( QWidget*, bool ) ), m_pWeaponWindow, SLOT( slotActivate( QWidget*, bool ) ) );

    QObject::connect( m_pInfoWindow, SIGNAL( sigActivated( QWidget*, bool ) ), this, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( m_pInfoWindow, SIGNAL( sigActivated( QWidget*, bool ) ), m_pTimerWindow, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( m_pInfoWindow, SIGNAL( sigActivated( QWidget*, bool ) ), m_pWeaponWindow, SLOT( slotActivate( QWidget*, bool ) ) );

    QObject::connect( m_pTimerWindow, SIGNAL( sigActivated( QWidget*, bool ) ), m_pInfoWindow, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( m_pTimerWindow, SIGNAL( sigActivated( QWidget*, bool ) ), m_pWeaponWindow, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( m_pTimerWindow, SIGNAL( sigActivated( QWidget*, bool ) ), this, SLOT( slotActivate( QWidget*, bool ) ) );

    QObject::connect( m_pWeaponWindow, SIGNAL( sigActivated( QWidget*, bool ) ), m_pInfoWindow, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( m_pWeaponWindow, SIGNAL( sigActivated( QWidget*, bool ) ), m_pTimerWindow, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( m_pWeaponWindow, SIGNAL( sigActivated( QWidget*, bool ) ), this, SLOT( slotActivate( QWidget*, bool ) ) );

    connect(m_pInfoWindow, SIGNAL(sigRestoreMinimizeToggled(bool)), this, SLOT(slotToggleRestoreMinimize(bool)));
    connect(m_pTimerWindow, SIGNAL(sigRestoreMinimizeToggled(bool)), this, SLOT(slotToggleRestoreMinimize(bool)));
    connect(m_pWeaponWindow, SIGNAL(sigRestoreMinimizeToggled(bool)), this, SLOT(slotToggleRestoreMinimize(bool)));
	connect(this, SIGNAL(sigRestoreMinimizeToggled(bool)), this, SLOT(slotSelfToggleRestoreMinimize(bool)));


	connect(ui->pbCheckInfo, SIGNAL(toggled(bool)), m_pInfoWindow, SLOT(setVisible(bool)));
	connect(ui->pbCheckTimer, SIGNAL(toggled(bool)), m_pTimerWindow, SLOT(setVisible(bool)));
    connect(ui->pbCheckWeapon, SIGNAL(toggled(bool)), m_pWeaponWindow, SLOT(slotSetVisible(bool)));

	HWND hwnd = (HWND)winId();
	RegisterPowerSettingNotification(hwnd, &GUID_CONSOLE_DISPLAY_STATE, DEVICE_NOTIFY_WINDOW_HANDLE);
	RegisterPowerSettingNotification(hwnd, &GUID_SYSTEM_AWAYMODE, DEVICE_NOTIFY_WINDOW_HANDLE);

	//RegisterPowerSettingNotification(hwnd, &GUID_MONITOR_POWER_ON, DEVICE_NOTIFY_WINDOW_HANDLE); // win7
}

void MainWindow::onSubMainWindowShowHide(bool bShow, MainWindowBase *pWindow)
{
	QPushButton * pButton = 0;
	if (pWindow == m_pInfoWindow)
	{
		pButton = ui->pbCheckInfo;
	}
	else if (pWindow == m_pTimerWindow)
	{
		pButton = ui->pbCheckTimer;
	}
    else if (pWindow == m_pWeaponWindow)
    {
        pButton = ui->pbCheckWeapon;
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
                m_pInfoWindow->setWindowState(Qt::WindowNoState);
                m_pInfoWindow->show();
                m_pInfoWindow->stackUnder(this);
            }

//            if (ui->pbCheckTimer->isChecked())
            {
                m_pTimerWindow->setWindowState(Qt::WindowNoState);
                m_pTimerWindow->show();
                m_pTimerWindow->stackUnder(m_pInfoWindow);
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
			if (!m_pInfoWindow->isDockingOnTop())
			{
				m_pInfoWindow->hide();
			}
			if (!m_pTimerWindow->isDockingOnTop())
			{
				m_pTimerWindow->hide();
			}
            if (!m_pWeaponWindow->isDockingOnTop())
            {
                m_pWeaponWindow->hide();
            }
			this->setSleepMode(true);
		}
	}

}

void MainWindow::closeEvent(QCloseEvent *e)
{
	QMessageBox * pMessageBox = new QMessageBox(
		QMessageBox::NoIcon
		, QString::fromLocal8Bit("")
		, QString::fromLocal8Bit("本当に終了しますか？")
		, QMessageBox::Yes | QMessageBox::No
		, this
		, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
	pMessageBox->setDefaultButton(QMessageBox::No);
	int reply = pMessageBox->exec();

	if (reply == QMessageBox::No)
	{
		e->ignore();
		return;
	}

	if (bUseFiddler)
	{
		pFiddler->Shutdown();
	}

	MainWindowBase::closeEvent(e);

	m_pInfoWindow->close();
	m_pTimerWindow->close();
    m_pWeaponWindow->close();
}

void MainWindow::moveEvent(QMoveEvent *e)
{
	MainWindowBase::moveEvent(e);

	QPoint diffPos = e->pos()-e->oldPos();

	if (bMoveSubTogether)
	{
		if (!m_pInfoWindow->isDockingOnTop())
		{
			m_pInfoWindow->move(m_pInfoWindow->pos() + diffPos);
		}
		if (!m_pTimerWindow->isDockingOnTop())
		{
			m_pTimerWindow->move(m_pTimerWindow->pos() + diffPos);
		}
        if (!m_pWeaponWindow->isDockingOnTop())
        {
            m_pWeaponWindow->move(m_pWeaponWindow->pos() + diffPos);
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

void MainWindow::on_pbCheckTrasparent_toggled(bool checked)
{
	static QList<QPair<QString, QWidget*>> s_listpair;
	if (checked)
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

		lstWidget = m_pInfoWindow->findChildren<QWidget *>();
		foreach(w, lstWidget)
		{
			QPair<QString, QWidget*> pair;
			pair.first = w->styleSheet();
			pair.second = w;
			s_listpair.append(pair);
        }
        lstWidget = m_pTimerWindow->findChildren<QWidget *>();
        foreach(w, lstWidget)
        {
            QPair<QString, QWidget*> pair;
            pair.first = w->styleSheet();
            pair.second = w;
            s_listpair.append(pair);
        }
        lstWidget = m_pWeaponWindow->findChildren<QWidget *>();
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

		ui->webView->page()->settings()->setUserStyleSheetUrl(csses[QWEBVIEWCSS_TRANSPARENT]);
	}
	else
	{
		QPair<QString, QWidget*> p;
		foreach(p, s_listpair)
		{
			if (isAncestorOf(p.second) || m_pInfoWindow->isAncestorOf(p.second) || m_pTimerWindow->isAncestorOf(p.second))
			{
				p.second->setStyleSheet(p.first);
			}
		}
		s_listpair.clear();

		ui->webView->page()->settings()->setUserStyleSheetUrl(csses[QWEBVIEWCSS_NORMAL]);
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
	emit mainWindow()->sigTogglePanicTimer(4000);
//	mainWindow()->m_panicTimer->start(4000);

	Q_UNUSED(sessionID);
	Q_UNUSED(fullURL);
	Q_UNUSED(requestBody);

}

void MainWindow::AfterSessionCompleteFunc(int sessionID, char *mimeType, int responseCode, char *PathAndQuery, char *requestBody, char *responseBody)
{
	emit mainWindow()->sigTogglePanicTimer(-1);
//	mainWindow()->m_panicTimer->stop();

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
	m_bSleep = val;
//	qDebug("state changed");
}

bool MainWindow::isSleepMode()
{
	return m_bSleep;
}

void MainWindow::slotTogglePanicTimer(int timeVal)
{
	if (timeVal > 0)
	{
		m_panicTimer->start(timeVal);
	}
	else
	{
		m_panicTimer->stop();
	}
}


void MainWindow::SetWebSettings()
{
	useport = 13347;
	if (bUseFiddler)
	{
		pFiddler = new FiddlerCOM::FiddlerCOMClass(this);

		connect(
			pFiddler,
			SIGNAL(exception(int, const QString &, const QString &, const QString &)),
			this,
			SLOT(slotWebViewException(int, const QString &, const QString &, const QString &)));


		pFiddler->SetBeforeRequest((int)&MainWindow::BeforeRequestFunc);
		pFiddler->SetAfterSessionComplete((int)&MainWindow::AfterSessionCompleteFunc);

		pFiddler->Startup(useport, false, true);

		QNetworkProxyFactorySet * proxies = new QNetworkProxyFactorySet();
		proxies->init(useport);
		QNetworkProxyFactory::setApplicationProxyFactory(proxies);
	}

	KQWebPage * page = new KQWebPage();
	ui->webView->setPage(page);

	ui->webView->setContextMenuPolicy(Qt::PreventContextMenu);

	if (!bUseFiddler)
	{
		KQNetworkAccessManager * manager = new KQNetworkAccessManager(this);
		ui->webView->page()->setNetworkAccessManager(manager);
	}
	CookieJar* jar = new CookieJar(this);
	ui->webView->page()->networkAccessManager()->setCookieJar(jar);
	//WebView
	QNetworkDiskCache *cache = new QNetworkDiskCache(this);
	cache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
	cache->setMaximumCacheSize(1073741824); //about 1024MB
	ui->webView->page()->networkAccessManager()->setCache(cache);
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
		if (m_bLowVol)
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
	if (!m_pTaskbarButton)
	{
		m_pTaskbarButton = new QWinTaskbarButton(this);
		m_pTaskbarButton->setWindow(windowHandle());
		QWinTaskbarProgress * pTaskbarProgress = m_pTaskbarButton->progress();
		pTaskbarProgress->setVisible(true);

		pTaskbarProgress->setMinimum(0);
		pTaskbarProgress->setMaximum(10000);
		pTaskbarProgress->setValue(0);
		pTaskbarProgress->show();
	}
}

void MainWindow::SetProgressBarPos(int pos, int state)
{
	if (m_pTaskbarButton)
	{
		QWinTaskbarProgress * pProgress = m_pTaskbarButton->progress();
		m_pTaskbarButton->progress()->setValue(pos);
		switch (state)
		{
		case PROGRESSBARSTATE_NORMAL:
			if (pProgress->isPaused() || pProgress->isStopped())
			{
				pProgress->resume();
			}
			break;
		case PROGRESSBARSTATE_PAUSED:
			if (pProgress->isStopped())
			{
				pProgress->resume();
			}
			if (!pProgress->isPaused())
			{
				pProgress->setPaused(true);
			}
			break;
		case PROGRESSBARSTATE_STOPPED:
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
	bMoveSubTogether = checked;
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
	int reply = pMessageBox->exec();

	if (reply == QMessageBox::Yes)
	{
		ui->webView->reload();
	}

}

void MainWindow::on_pbScreenshot_clicked()
{
	ShootScreen();
}

void MainWindow::onPanic()
{
	QString jsstr = QString::fromLocal8Bit("window.alert('%1'); ").arg(QString::fromLocal8Bit("パニック！"));
	ui->webView->page()->mainFrame()->evaluateJavaScript(jsstr);
	if (m_panicTimer)
	{
		m_panicTimer->stop();
	}
}

void MainWindow::on_pbSwitchScreenshot_toggled(bool checked)
{
	if (checked)
	{
		ShootScreen();
		m_pScreenshotTimer->start(250);
	}
	else
	{
		m_pScreenshotTimer->stop();
	}
}

void MainWindow::ShootScreen()
{
	if (ui->pbCheckTrasparent->isChecked())
	{
		ui->webView->page()->settings()->setUserStyleSheetUrl(csses[QWEBVIEWCSS_NORMAL]);
	}
	QPixmap pixmap = QPixmap::grabWidget(ui->webView);

	QString filename = QApplication::applicationDirPath();
	filename += "/screenshot/";

	filename += QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss_zzz.png");

	pixmap.save(filename, "PNG");
	if (ui->pbCheckTrasparent->isChecked())
	{
		ui->webView->page()->settings()->setUserStyleSheetUrl(csses[QWEBVIEWCSS_TRANSPARENT]);
	}
}

void MainWindow::slotScreenshotTimeout()
{
	ShootScreen();
}

void MainWindow::on_pbCheckLowVol_toggled(bool checked)
{
	m_bLowVol = checked;
	AdjustVolume(-1);
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
						mainWindow->setSleepMode(true);
						return false;
					}
					if (dw == 0x1)
					{
						// on
						mainWindow->setSleepMode(false);
						return false;
					}
				}
			}
			return false;
		}
	}
	return false;
}
