﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "kandataconnector.h"

#include "ControlManager.h"

//#include <QWebFrame>
#include <QShortcut>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
#include <QWebEngineSettings>
#include "qwebenginemouseeventfilter.h"
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#endif


MainWindow * MainWindow::s_pMainWindow = NULL;

MainWindow::MainWindow(QWidget *parent)
	: MainWindowBase(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->retranslateUi(this);

	loadSettings();

	_pScreenshotTimer = new QTimer(this);
	connect(_pScreenshotTimer, SIGNAL(timeout()), this, SLOT(slotScreenshotTimeout()));
	_panicTimer = new QTimer(this);
	connect(_panicTimer, SIGNAL(timeout()), this, SLOT(onPanic()));

	setMainWindow(this);

	if (_webWidgetType == WebWidgetType::IE)
	{
		setAttribute(Qt::WA_TranslucentBackground, false);

		rebuildIE(false);

#ifdef Q_OS_WIN
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
#endif
	}
	else
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
		_webView = new QWebEngineView(ui->webFrame);
		_webView->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
#else
		_webView = new QWebView(ui->webFrame);
		_webView->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
#endif
		_webView->setObjectName(QStringLiteral("webView"));
		//		_webView->setUrl(QUrl(QStringLiteral("about:blank")));
		ui->webFrame_layout->addWidget(_webView);
	}

	// WARNING!!! comment out this line when changing any layout
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

	QShortcut *shortcutDoJobExpedition = new QShortcut(QKeySequence("Ctrl+F11"), this);
	connect(shortcutDoJobExpedition, SIGNAL(activated()), this, SLOT(onToggleJobExpedition()));

	QShortcut *shortcutExport = new QShortcut(QKeySequence("Ctrl+F6"), this);
	connect(shortcutExport, SIGNAL(activated()), this, SLOT(onExportAllList()));

	QShortcut * shortcutPauseResume = new QShortcut(QKeySequence("F8"), this);
	connect(shortcutPauseResume, SIGNAL(activated()), this, SLOT(onJobPauseResume()));
	QShortcut * shortcutPauseNext = new QShortcut(QKeySequence("F7"), this);
	connect(shortcutPauseNext, SIGNAL(activated()), this, SLOT(onJobPauseNext()));

	QShortcut * shortcutTerminateJob = new QShortcut(QKeySequence("F12"), this);
	connect(shortcutTerminateJob, SIGNAL(activated()), this, SLOT(onTerminateJob()));

	QShortcut * shortcutIncreaseSouthEast = new QShortcut(QKeySequence("Ctrl+PgUp"), this);
	connect(shortcutIncreaseSouthEast, SIGNAL(activated()), this, SLOT(onIncreaseSouthEast()));

	QShortcut * shortcutDecreaseSouthEast = new QShortcut(QKeySequence("Ctrl+PgDown"), this);
	connect(shortcutDecreaseSouthEast, SIGNAL(activated()), this, SLOT(onDecreaseSouthEast()));

	auto shortcutIncreseTimeShift = new QShortcut(QKeySequence("Ctrl+Shift+Up"), this);
	connect(shortcutIncreseTimeShift,
		SIGNAL(activated()),
		this,
		SLOT(onIncreaseTimeShift()));
	auto shortcutDecreseTimeShift = new QShortcut(QKeySequence("Ctrl+Shift+Down"), this);
	connect(shortcutDecreseTimeShift,
		SIGNAL(activated()),
		this,
		SLOT(onDecreaseTimeShift()));

	_jobTimer = new QTimer(this);
	connect(_jobTimer, SIGNAL(timeout()), this, SLOT(slotJobtTimeout()));
	_jobTimer->setTimerType(Qt::PreciseTimer);
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

#ifdef Q_OS_WIN
	if (_pFid)
	{
		delete _pFid;
	}
	if (_pNekoxy)
	{
		delete _pNekoxy;
	}
	if (_pTitanium)
	{
		delete _pTitanium;
	}
#endif
	if (_pSharkProcess)
	{
		_pSharkProcess->close();
		delete _pSharkProcess;
	}
	delete ui;
}

void MainWindow::slotParse(const QString &PathAndQuery, const QString &requestBody, const QString &responseBody)
{
	KanDataConnector::getInstance().Parse(PathAndQuery, requestBody, responseBody);
}

void MainWindow::slotSharkProcessReadyReadError()
{
	QString receivedError = QString(_pSharkProcess->readAllStandardOutput()).trimmed();
	Q_UNUSED(receivedError);
}


void MainWindow::slotSharkProcessReadyRead()
{
	auto byteArray = _pSharkProcess->readAllStandardOutput();

	qDebug() << "Buffer: " << byteArray.left(80) << "..." << byteArray.right(80) << (byteArray.endsWith('\n') ? "... EndWithReturn" : "");

	_sharkReadingBuffer += QString::fromUtf8(byteArray.constData(), byteArray.size());
	if (!byteArray.endsWith('\n'))
	{
		return;
	}

	auto splitedPackage = _sharkReadingBuffer.split("\t", QString::SplitBehavior::SkipEmptyParts);

	_sharkReadingBuffer = "";

	int length = splitedPackage.length();
	for (int i = 0; i < length; i++)
	{
		if (splitedPackage[i].startsWith("/kcsapi/"))
		{

			SharkRequestResponseRecord record;
			record.pathAndQuery = splitedPackage[i];
			i++;
			if (i < length)
			{
				record.requestBody = splitedPackage[i];
				i++;
				if (i < length)
				{
					record.requestFrame = splitedPackage[i].toInt();
					if (record.requestFrame == 0)
					{
						continue;
					}
					bool matched = false;
					// try match
					for (int j = 0; j < _sharkResponseStack.length(); j++)
					{
						if (record.requestFrame == _sharkResponseStack[j].requestFrame)
						{
							matched = true;
							emit sigParse(record.pathAndQuery, record.requestBody, _sharkResponseStack[j].responseBody);
							_sharkResponseStack.removeAt(j);
							break;
						}
					}
					if (!matched)
					{
						// add new
						while (_sharkRequestStack.length() >= _sharkStackMax)
						{
							_sharkRequestStack.removeFirst();
						}
						_sharkRequestStack.append(record);
					}
				}
			}
		}
		else if (splitedPackage[i].startsWith("svdata="))
		{
			SharkRequestResponseRecord record;
			record.responseBody = splitedPackage[i];
			i++;
			if (i < length)
			{
				record.requestFrame = splitedPackage[i].toInt();
				if (record.requestFrame == 0)
				{
					// fatal response error
					onFatalSharkResponseError(false);
				}
				// try match
				bool matched = false;
				for (int j = _sharkRequestStack.length() - 1; j >= 0; j--)
				{
					if (_sharkRequestStack[j].requestFrame == record.requestFrame)
					{
						matched = true;
						emit sigParse(_sharkRequestStack[j].pathAndQuery, _sharkRequestStack[j].requestBody, record.responseBody);
						_sharkRequestStack.removeAt(j);
						break;
					}
				}
				if (!matched)
				{
					// fatal response error?
					onFatalSharkResponseError(false);

					// add new
					while (_sharkResponseStack.length() >= _sharkStackMax)
					{
						_sharkResponseStack.removeFirst();
					}
					_sharkResponseStack.append(record);
				}
			}
		}
	}
}

void MainWindow::onFatalSharkResponseError(bool fatalOnProxy)
{
	if (_sharkShouldRaiseFatalOnMismatchResponse || fatalOnProxy)
	{
		ControlManager::getInstance().setToTerminate("FatalSharkResponse", true);
		close();
	}
}

void MainWindow::slotWebViewException(int code, const QString &source, const QString &desc, const QString &help)
{
	qDebug("Exception: %d", code);
	qDebug(source.toUtf8());
	qDebug(desc.toUtf8());
	qDebug(help.toUtf8());
	qDebug("Put dll to exe folder.");
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

	if (strPathAndQuery.startsWith("/kcsapi") && responseCode == 200)
	{
		auto mainWindow = MainWindow::mainWindow();
		if (mainWindow->_applyCssToGameFlag && mainWindow->_webWidgetType == WebWidgetType::WebEngine)
		{
			mainWindow->installWebEngineMouseEventFilter();

			mainWindow->applyCss(QWebViewCSSIndex::Normal);
			mainWindow->_applyCssToGameFlag = false;
		}

		if (0 == QString::compare(mimeType, "text/plain"))
		{
			emit mainWindow->sigParse(PathAndQuery, requestBody, responseBody);
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

void MainWindow::onGetNetworkReply(const QString& PathAndQuery, const QString& requestBody, const QString& responseBody)
{
	emit MainWindow::mainWindow()->sigParse(PathAndQuery, requestBody, responseBody);
}

QWidget* MainWindow::getBrowserWidget()
{
	if (_webWidgetType == WebWidgetType::IE)
	{
#ifdef Q_OS_WIN
		return _axWidget;
#endif
	}
	return _webView;
}

void MainWindow::navigateTo(const QString& urlString)
{
	if (_webWidgetType == WebWidgetType::IE)
	{
#ifdef Q_OS_WIN
		_axWidget->Navigate(urlString);
#endif
	}
	else
	{
		_webView->load(QUrl(urlString));
	}
}

void MainWindow::navigateReload()
{
	if (_webWidgetType == WebWidgetType::IE)
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

void MainWindow::rebuildIE(bool bNavigate)
{

#ifdef Q_OS_WIN
	if (_axWidget)
	{
		WebBrowser* toDelete = _axWidget;
		QTimer::singleShot(1, Qt::PreciseTimer, [toDelete, this](){
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
#endif
}

void MainWindow::installWebEngineMouseEventFilter()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
	if (_webEngineMouseEventFilter)
	{
		Q_FOREACH(QObject* child, _webView->children())
		{
			child->removeEventFilter(_webEngineMouseEventFilter);
		}
		Q_FOREACH(QObject* child, _webView->page()->children())
		{
			child->removeEventFilter(_webEngineMouseEventFilter);
		}
		Q_FOREACH(QObject* child, _webView->page()->view()->children())
		{
			child->removeEventFilter(_webEngineMouseEventFilter);
		}
		delete _webEngineMouseEventFilter;
	}

	_webEngineMouseEventFilter = new QWebEngineMouseEventFilter;
	// install mouse event filter
	_webView->installEventFilter(_webEngineMouseEventFilter);
	Q_FOREACH(QObject* child, _webView->children())
	{
		child->installEventFilter(_webEngineMouseEventFilter);
	}
	Q_FOREACH(QObject* child, _webView->page()->children())
	{
		child->installEventFilter(_webEngineMouseEventFilter);
	}
	Q_FOREACH(QObject* child, _webView->page()->view()->children())
	{
		child->installEventFilter(_webEngineMouseEventFilter);
	}
#endif
}

#ifdef Q_OS_WIN
void MainWindow::slotNavigateComplete2(IDispatch*, QVariant& url)
{
	// warning : does not work with 5.6
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
#endif
