﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QNetworkProxy>
#include "cookiejar.h"
#include <QNetworkDiskCache>
#include <QStandardPaths>
#include "qnetworkproxyfactoryset.h"
#include "kqwebpage.h"
#include <QWinTaskbarProgress>
#include "kandataconnector.h"
#include <QWinTaskbarButton>

QFile * MainWindow::pLogFile = 0;

MainWindow * MainWindow::s_pMainWindow = 0;

MainWindow::MainWindow(QWidget *parent) :
    MainWindowBase(parent),
    ui(new Ui::MainWindow)
{
    setMainWindow(this);
    ui->setupUi(this);
    ui->retranslateUi(this);

    pLogFile = new QFile(QApplication::applicationDirPath()+"/log.txt");
    pLogFile->open(QIODevice::ReadWrite|QIODevice::Text);

    ui->comboBoxZoom->insertSeparator(1);

//    QWinTaskbarProgress * pTaskbarProgress = new QWinTaskbarProgress(this);
	QWinTaskbarButton * pTaskbarButton = new QWinTaskbarButton(this);
	pTaskbarButton->setWindow(windowHandle());
	QWinTaskbarProgress * pTaskbarProgress = pTaskbarButton->progress();

    pTaskbarProgress->setMinimum(0);
    pTaskbarProgress->setMaximum(100);
    pTaskbarProgress->setValue(50);
    pTaskbarProgress->show();

    mwbPostInit();

    ui->titleFrame->setHandlingWidget(this);

    pFiddler = new FiddlerCOM::FiddlerCOMClass(this);

    connect(
            pFiddler,
            SIGNAL(exception(int, const QString &, const QString &, const QString &)),
            this,
            SLOT(slotWebViewException(int, const QString &, const QString &, const QString &)));

    connect(this,
            SIGNAL(sigParse(const QString &, const QString &, const QString &)),
            this,
            SLOT(slotParse(const QString &, const QString &, const QString &)));

    pFiddler->SetBeforeRequest((int)&MainWindow::BeforeRequestFunc);
    pFiddler->SetAfterSessionComplete((int)&MainWindow::AfterSessionCompleteFunc);

    useport = 13347;
    pFiddler->Startup(useport, false, true);

    SetWebSettings();
    /*
     *
body {
    margin:0;
    overflow:hidden
}

#game_frame {
    position:fixed;
    left:0%;
    top:-16px;
    left:-50px;
    z-index:1
}
    */
    QUrl css("data:text/css;charset=utf-8;base64,Ym9keSB7DQogICAgbWFyZ2luOjA7DQogICAgb3ZlcmZsb3c6aGlkZGVuDQp9DQoNCiNnYW1lX2ZyYW1lIHsNCiAgICBwb3NpdGlvbjpmaXhlZDsNCiAgICBsZWZ0OjAlOw0KICAgIHRvcDotMTZweDsNCiAgICBsZWZ0Oi01MHB4Ow0KICAgIHotaW5kZXg6MQ0KfQ==");

    ui->webView->page()->settings()->setUserStyleSheetUrl(css);

//    ui->webView->load(QUrl("http://www.google.com"));
    ui->webView->load(QUrl("http://www.dmm.com/netgame/social/application/-/detail/=/app_id=854854/"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::postInit(InfoMainWindow *pInfo, TimerMainWindow *pTimer)
{
    m_pInfoWindow = pInfo;
    m_pTimerWindow = pTimer;

    QObject::connect( this, SIGNAL( sigActivated( QWidget*, bool ) ), m_pInfoWindow, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( this, SIGNAL( sigActivated( QWidget*, bool ) ), m_pTimerWindow, SLOT( slotActivate( QWidget*, bool ) ) );

    QObject::connect( m_pInfoWindow, SIGNAL( sigActivated( QWidget*, bool ) ), this, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( m_pInfoWindow, SIGNAL( sigActivated( QWidget*, bool ) ), m_pTimerWindow, SLOT( slotActivate( QWidget*, bool ) ) );

    QObject::connect( m_pTimerWindow, SIGNAL( sigActivated( QWidget*, bool ) ), m_pInfoWindow, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( m_pTimerWindow, SIGNAL( sigActivated( QWidget*, bool ) ), this, SLOT( slotActivate( QWidget*, bool ) ) );

    connect(m_pInfoWindow, SIGNAL(sigRestoreMinimizeToggled(bool)), this, SLOT(slotToggleRestoreMinimize(bool)));
    connect(m_pTimerWindow, SIGNAL(sigRestoreMinimizeToggled(bool)), this, SLOT(slotToggleRestoreMinimize(bool)));


    connect(ui->pbCheckInfo, SIGNAL(toggled(bool)), m_pInfoWindow, SLOT(setVisible(bool)));
    connect(ui->pbCheckTimer, SIGNAL(toggled(bool)), m_pTimerWindow, SLOT(setVisible(bool)));
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
            m_pInfoWindow->setWindowState(Qt::WindowNoState);
            m_pTimerWindow->setWindowState(Qt::WindowNoState);

            m_pInfoWindow->show();
            m_pTimerWindow->show();
            m_pInfoWindow->stackUnder(this);
            m_pTimerWindow->stackUnder(m_pInfoWindow);
            this->raise();
            this->activateWindow();
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
        }
    }

}

void MainWindow::closeEvent(QCloseEvent *e)
{
//    m_pFiddler->Shutdown();
    MainWindowBase::closeEvent(e);

    m_pInfoWindow->close();
    m_pTimerWindow->close();
}

void MainWindow::moveEvent(QMoveEvent *e)
{
    MainWindowBase::moveEvent(e);

    QPoint diffPos = e->pos()-e->oldPos();

    if (!m_pInfoWindow->isDockingOnTop())
    {
        m_pInfoWindow->move(m_pInfoWindow->pos()+diffPos);
    }
    if (!m_pTimerWindow->isDockingOnTop())
    {
        m_pTimerWindow->move(m_pTimerWindow->pos()+diffPos);
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

        QPair<QString, QWidget*> p;
        foreach(p, s_listpair)
        {
            w = p.second;
            QString stylestr = w->styleSheet();
            stylestr += "background-color: rgba(45, 45, 48, 30%);";
            w->setStyleSheet(stylestr);
        }

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

}

void MainWindow::AfterSessionCompleteFunc(int sessionID, char *mimeType, int responseCode, char *PathAndQuery, char *requestBody, char *responseBody)
{
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

void MainWindow::SetWebSettings()
{
    KQWebPage * page = new KQWebPage();
    ui->webView->setPage(page);
    QNetworkProxyFactorySet * proxies = new QNetworkProxyFactorySet();
    proxies->init(useport);

    QNetworkProxyFactory::setApplicationProxyFactory(proxies);

    CookieJar* jar = new CookieJar(this);
    ui->webView->page()->networkAccessManager()->setCookieJar(jar);

    //WebView
    QNetworkDiskCache *cache = new QNetworkDiskCache(this);
    cache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)+"2");
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
