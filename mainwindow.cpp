#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "kandataconnector.h"

#include "ControlManager.h"

//#include <QWebFrame>
#include <QShortcut>
#include <QMessageBox>
#include <QRgb>
#include <QPoint>

#include <QClipboard>
#include <QDir>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
#include <QWebEngineSettings>
#include "qwebenginemouseeventfilter.h"
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include "RemoteNotifyHandler.h"

MainWindow * MainWindow::s_pMainWindow = NULL;

QString MainWindow::s_absoluteResourcePath;

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

	QShortcut *shortcutPing = new QShortcut(QKeySequence("P"), this);
	connect(shortcutPing, SIGNAL(activated()), this, SLOT(onPingTest()));

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

	QShortcut * shortcutDisplayColor = new QShortcut(QKeySequence("Ctrl+I"), this);
	connect(shortcutDisplayColor, SIGNAL(activated()), this, SLOT(onGetColorOnScreen()));
	QShortcut * shortcutDisplayColorDelay = new QShortcut(QKeySequence("Ctrl+Shift+I"), this);
	connect(shortcutDisplayColorDelay, SIGNAL(activated()), this, SLOT(onGetColorOnScreen()));

	QShortcut * shortcutCopyInfo = new QShortcut(QKeySequence("Ctrl+C"), this);
	connect(shortcutCopyInfo, SIGNAL(activated()), this, SLOT(onCopyInfo()));
	QShortcut * shortcutCopyInfoAll = new QShortcut(QKeySequence("Ctrl+Shift+C"), this);
	connect(shortcutCopyInfoAll, SIGNAL(activated()), this, SLOT(onCopyInfo()));

	auto shortcutIncreseTimeShift = new QShortcut(QKeySequence("Shift+Up"), this);
	connect(shortcutIncreseTimeShift,
		SIGNAL(activated()),
		this,
		SLOT(onIncreaseTimeShift()));
	auto shortcutDecreseTimeShift = new QShortcut(QKeySequence("Shift+Down"), this);
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

#if defined Q_OS_WIN && !defined NO_WIN_EXTRA
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
		_pSharkProcess->kill();
		_pSharkProcess->close();
		delete _pSharkProcess;
	}
	delete ui;
}

void MainWindow::slotParse(const QString &PathAndQuery, const QString &requestBody, const QString &responseBody)
{
	if (_proxyMode == ProxyMode::Shark)
	{
		if (s_pMainWindow->_applyCssToGameFlag && s_pMainWindow->_webWidgetType == WebWidgetType::WebEngine)
		{

#if QT_OS_WIN
			s_pMainWindow->installWebEngineMouseEventFilter();
#endif

			s_pMainWindow->applyCss(QWebViewCSSIndex::Normal);
			s_pMainWindow->_applyCssToGameFlag = false;
		}
	}
	KanDataConnector::getInstance().Parse(PathAndQuery, requestBody, responseBody);
}

void MainWindow::slotSharkProcessReadyReadError()
{
	auto byteArray = _pSharkProcess->readAllStandardError();
	qDebug() << byteArray;
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

void MainWindow::onGetColorOnScreen()
{
	QPoint pt = getBrowserWidget()->mapFromGlobal(QCursor::pos());

	if (QApplication::queryKeyboardModifiers()&Qt::ShiftModifier)
	{
		QTime dieTime = QTime::currentTime().addSecs(1);
		while (QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

	}

	QRgb col = ControlManager::getInstance().getColorAtPosition(pt);

	QString titleInfo = QString::fromLocal8Bit("%1, %2, %3, %4, %5")
		.arg(pt.x())
		.arg(pt.y())
		.arg(qRed(col))
		.arg(qGreen(col))
		.arg(qBlue(col));

	QString textInfo = QString::fromLocal8Bit("X: %1, Y: %2, R: %3, G: %4, B: %5")
		.arg(pt.x())
		.arg(pt.y())
		.arg(qRed(col))
		.arg(qGreen(col))
		.arg(qBlue(col));

	QMessageBox * pMessageBox = new QMessageBox(
		QMessageBox::NoIcon
		, QString::fromLocal8Bit("")
		, textInfo
		, QMessageBox::Ok
		, this
		, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
	pMessageBox->setDefaultButton(QMessageBox::Ok);
	pMessageBox->setAttribute(Qt::WA_DeleteOnClose, true);
	pMessageBox->exec();
}

void MainWindow::onCopyInfo()
{
	bool copyAll = false;
	if (QApplication::queryKeyboardModifiers()&Qt::ShiftModifier)
	{
		copyAll = true;
	}

	QString info = "";

	KanSaveData* pksd = &KanSaveData::getInstance();
	if (pksd->portdata.api_deck_port.isEmpty())
	{
		return;
	}

	// 1_1_Ship = 10000
	// 1_1_ShipName = mutsuki
	// 1_1_SlotItem = 1,2,3,-1,-1,-1
	// 1_1_SlotItemName = tenzan+5,tenzan+10
	// 1_1_ExSlotItem = 4
	// 1_1_ExSlotItemName = tenzan+5
	KanDataConnector* pkdc = &KanDataConnector::getInstance();
	for (int i = 0; i < pksd->portdata.api_deck_port.count(); i++)
	{
		if (i > 0 && !copyAll)
		{
			break;
		}

		for (int j = 0; j < pksd->portdata.api_deck_port[i].api_ship.count(); j++)
		{
			int shipno = pksd->portdata.api_deck_port[i].api_ship[j];

			QString positionPrefix = QString::number(i + 1) + "_" + QString::number(j + 1);
			info += positionPrefix + "_Ship = " + QString::number(shipno) + "\n";

			auto pship = pkdc->findShipFromShipno(shipno);
			if (pship)
			{
				auto pmstship = pkdc->findMstShipFromShipid(pship->api_ship_id);
				if (pmstship)
				{
					info += positionPrefix + "_ShipName = " + pmstship->api_name + ", Lv:" + QString::number(pship->api_lv) + "\n";
				}

				info += positionPrefix + "_SlotItem = ";
				for (int k = 0; k < pship->api_slot.count(); k++)
				{
					if (pship->api_slot[k] > 0)
					{
						if (k > 0)
						{
							info += ",";
						}
						info += QString::number(pship->api_slot[k]);
					}
				}
				info += "\n";

				info += positionPrefix + "_SlotItemName = ";
				for (int k = 0; k < pship->api_slot.count(); k++)
				{
					auto pslotitem = pkdc->findSlotitemFromId(pship->api_slot[k]);
					if (pslotitem)
					{
						auto pmstslotitem = pkdc->findMstSlotItemFromSlotitemid(pslotitem->api_slotitem_id);
						if (pmstslotitem)
						{
							if (k > 0)
							{
								info += ", \t";
							}
							info += pmstslotitem->api_name;
							if (pslotitem->api_level > 0)
							{
								info += "+" + QString::number(pslotitem->api_level);
							}
						}
					}
				}
				info += "\n";

				if (pship->api_slot_ex > 0)
				{
					info += positionPrefix + "_ExSlotItem = " + QString::number(pship->api_slot_ex) + "\n";
					auto pslotitem = pkdc->findSlotitemFromId(pship->api_slot_ex);
					if (pslotitem)
					{
						auto pmstslotitem = pkdc->findMstSlotItemFromSlotitemid(pslotitem->api_slotitem_id);
						if (pmstslotitem)
						{
							info += positionPrefix + "_ExSlotItemName = ";
							info += pmstslotitem->api_name;
							if (pslotitem->api_level > 0)
							{
								info += "+" + QString::number(pslotitem->api_level);
							}
							info += "\n";
						}
					}
				}
			}
		}
	}

	QClipboard* clipBoard = QApplication::clipboard();
	clipBoard->setText(info);
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
		if (responseCode == 200)
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
		else
		{
			RemoteNotifyHandler::getInstance().NotifyNeko();
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

			QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
			if (!statusCode.isValid() || statusCode.toInt() != 200)
			{
				RemoteNotifyHandler::getInstance().NotifyNeko();
			}
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
#if defined Q_OS_WIN && !defined NO_WIN_EXTRA
		return _axWidget;
#endif
	}
	return _webView;
}

void MainWindow::navigateTo(const QString& urlString)
{
	if (_webWidgetType == WebWidgetType::IE)
	{
#if defined Q_OS_WIN && !defined NO_WIN_EXTRA
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

#if defined Q_OS_WIN && !defined NO_WIN_EXTRA
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
#else
	Q_UNUSED(bNavigate);
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

QString MainWindow::getAbsoluteResourcePath()
{
    if (!s_absoluteResourcePath.isEmpty())
    {
        return s_absoluteResourcePath;
    }

    QDir dir = QDir::current();
#if defined Q_OS_WIN
    s_absoluteResourcePath = dir.absolutePath();
#elif defined Q_OS_MAC
    dir.cd("../../../../KanPlayResources");
    s_absoluteResourcePath = dir.path();
#endif

    return s_absoluteResourcePath;
}

#if defined Q_OS_WIN && !defined NO_WIN_EXTRA
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
