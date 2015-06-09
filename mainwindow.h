void on_pbCheckLowVol_toggled(bool checked);
void on_pbScreenshot_clicked();
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainwindowbase.h"
#include "infomainwindow.h"
#include "timermainwindow.h"
#include "weaponmainwindow.h"
#include "shipmainwindow.h"
#include "FiddlerCOM.h"
#include <QShowEvent>
#include <QWinTaskbarButton>
#include <QNetworkReply>
#include <QWebView>
#include <QAxWidget>
#include "shdocvw.h"

using namespace SHDocVw;

#include <QAbstractNativeEventFilter>

class QWindowsEventFilter : public QAbstractNativeEventFilter
{
public:
	QWindowsEventFilter();
	virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
};
enum
{
	PROGRESSBARSTATE_NORMAL,
	PROGRESSBARSTATE_PAUSED,
	PROGRESSBARSTATE_STOPPED,
};

enum {
	QWEBVIEWCSS_NORMAL,
	QWEBVIEWCSS_TRANSPARENT,
	//	QWEBVIEWCSS_CLEAR,
	QWEBVIEWCSS_END,
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

	static MainWindow * mainWindow(){return s_pMainWindow;}
	static void setMainWindow(MainWindow * pWindow){s_pMainWindow = pWindow;}
    void postInit(InfoMainWindow * pInfo, TimerMainWindow * pTimer, WeaponMainWindow *pWeapon, ShipMainWindow* pShip);
	static InfoMainWindow * infoWindow(){return s_pMainWindow->_pInfoWindow;}
	static TimerMainWindow * timerWindow(){return s_pMainWindow->_pTimerWindow;}
    static WeaponMainWindow * weaponWindow(){return s_pMainWindow->_pWeaponWindow;}
	static ShipMainWindow * shipWindow(){ return s_pMainWindow->_pShipWindow; }

	void AdjustVolume(int vol);
	void onSubMainWindowShowHide(bool bShow, MainWindowBase * pWindow);
	void SetProgressBarPos(int pos, int state);

	void onGetNetworkReply(QNetworkReply * reply);

	void setSleepMode(bool val);
	bool isSleepMode();

	QWidget* getBrowserWidget();
	void navigateTo(QString urlString);
	void navigateReload();

	bool isUsingIE();

signals:
	void sigParse(const QString &PathAndQuery, const QString &requestBody, const QString &responseBody);
	void sigTogglePanicTimer(int timeVal);
	void sigToggleSleepMode(bool bSleep);

public slots:
	virtual void slotActivate(QWidget* w, bool bActivate);
	virtual void slotToggleRestoreMinimize(bool bRestore);
	virtual void slotSelfToggleRestoreMinimize(bool bRestore);
	void slotParse(const QString &PathAndQuery, const QString &requestBody, const QString &responseBody);
	void slotSoundEnded();
	void slotTogglePanicTimer(int timeVal);

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
	void on_pbCheckLowVol_toggled(bool checked);

	void slotNavigateComplete2(IDispatch*, QVariant&);
	void slotWebViewException(int code, const QString & source, const QString & desc, const QString & help);
	void onPanic();

private:
	// Fiddler
	static void __stdcall BeforeRequestFunc(int sessionID, char * fullURL, char * requestBody);
	static void __stdcall AfterSessionCompleteFunc(int sessionID, char * mimeType, int responseCode, char * PathAndQuery, char * requestBody, char * responseBody);

private:
	void setWebSettings();
	void applyCss(int css);
	void shootScreen();
	void setupCss();
	void loadSettings();
	void rebuildIE(bool bNavigate);

private:
	Ui::MainWindow *ui;

	bool _bUseFiddler = true;
	int _useport = 0;
	FiddlerCOM::FiddlerCOMClass * _pFiddler = NULL;

	bool _bIEPageLoaded = false;
	int _applyCssWhenLoaded = -1;
	QString _ieCsses[QWEBVIEWCSS_END];
	QUrl _webViewCsses[QWEBVIEWCSS_END];
	
	InfoMainWindow * _pInfoWindow = NULL;
	TimerMainWindow * _pTimerWindow = NULL;
    WeaponMainWindow * _pWeaponWindow = NULL;
	ShipMainWindow* _pShipWindow = NULL;
	
	static MainWindow * s_pMainWindow;

	QWinTaskbarButton * _pTaskbarButton = NULL;

	bool _bMoveSubTogether = true;

	QTimer * _pScreenshotTimer = NULL;
	QTimer* _panicTimer = NULL;

	bool _bLowVol = true;
	bool _bSleep = false;

	QWindowsEventFilter _windowsEventfilter;

	QWebView* _webView = NULL;
	WebBrowser* _axWidget = NULL;

	// settings
	bool _bUseIE = false;
	QString _gameUrl;
	QString _gameUrlId;
};



#endif // MAINWINDOW_H
