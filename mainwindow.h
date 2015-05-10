void on_pbCheckLowVol_toggled(bool checked);
void on_pbScreenshot_clicked();
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainwindowbase.h"
#include "infomainwindow.h"
#include "timermainwindow.h"
#include "weaponmainwindow.h"
#include "FiddlerCOM.h"
#include <QShowEvent>
#include <QWinTaskbarButton>
#include <QNetworkReply>

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
    void postInit(InfoMainWindow * pInfo, TimerMainWindow * pTimer, WeaponMainWindow *pWeapon);
	static InfoMainWindow * infoWindow(){return s_pMainWindow->m_pInfoWindow;}
	static TimerMainWindow * timerWindow(){return s_pMainWindow->m_pTimerWindow;}
    static WeaponMainWindow * weaponWindow(){return s_pMainWindow->m_pWeaponWindow;}

	void AdjustVolume(int vol);
	void onSubMainWindowShowHide(bool bShow, MainWindowBase * pWindow);
	void SetProgressBarPos(int pos, int state);

	void onGetNetworkReply(QNetworkReply * reply);

	void setSleepMode(bool val);
	bool isSleepMode();

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

	void on_pbCheckTrasparent_toggled(bool checked);

	void slotWebViewException(int code, const QString & source, const QString & desc, const QString & help);

	void on_pbMoveTogether_toggled(bool checked);

	void on_pbRefresh_clicked();

	void on_pbScreenshot_clicked();

	void on_pbSwitchScreenshot_toggled(bool checked);

	void slotScreenshotTimeout();

	void on_pbCheckLowVol_toggled(bool checked);

	void onPanic();

private:
	Ui::MainWindow *ui;

	bool bUseFiddler;
	int useport;
	FiddlerCOM::FiddlerCOMClass * pFiddler;
	static void __stdcall BeforeRequestFunc(int sessionID, char * fullURL, char * requestBody);
	static void __stdcall AfterSessionCompleteFunc(int sessionID, char * mimeType, int responseCode, char * PathAndQuery, char * requestBody, char * responseBody);
	void SetWebSettings();

	void ShootScreen();

	InfoMainWindow * m_pInfoWindow;
	TimerMainWindow * m_pTimerWindow;
    WeaponMainWindow * m_pWeaponWindow;
	
	static MainWindow * s_pMainWindow;

	QWinTaskbarButton * m_pTaskbarButton;

	bool bMoveSubTogether;

	QTimer * m_pScreenshotTimer;
	QTimer* m_panicTimer;

	bool m_bLowVol = true;
	bool m_bSleep = false;

	QWindowsEventFilter m_windowsEventfilter;
};



#endif // MAINWINDOW_H
