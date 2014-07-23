#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainwindowbase.h"
#include "infomainwindow.h"
#include "timermainwindow.h"
#include "FiddlerCOM.h"

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
    void postInit(InfoMainWindow * pInfo, TimerMainWindow * pTimer);
    static InfoMainWindow * infoWindow(){return s_pMainWindow->m_pInfoWindow;}
    static TimerMainWindow * timerWindow(){return s_pMainWindow->m_pTimerWindow;}

    void onSubMainWindowShowHide(bool bShow, MainWindowBase * pWindow);
signals:
    void sigParse(const QString &PathAndQuery, const QString &requestBody, const QString &responseBody);

public slots:
    virtual void slotActivate(QWidget* w , bool bActivate);
    virtual void slotToggleRestoreMinimize(bool bRestore);
    void slotParse(const QString &PathAndQuery, const QString &requestBody, const QString &responseBody);

protected:
    virtual void changeEvent(QEvent * e);
    virtual void closeEvent(QCloseEvent * e);
    virtual void moveEvent(QMoveEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent * event);

private slots:
    void on_pbClose_clicked();

    void on_pbMinimize_clicked();

    void on_pbCheckTrasparent_toggled(bool checked);

    void slotWebViewException(int code, const QString & source, const QString & desc, const QString & help);

private:
    Ui::MainWindow *ui;

    int useport;
    FiddlerCOM::FiddlerCOMClass * pFiddler;
    static void __stdcall BeforeRequestFunc(int sessionID, char * fullURL, char * requestBody);
    static void __stdcall AfterSessionCompleteFunc(int sessionID, char * mimeType, int responseCode, char * PathAndQuery, char * requestBody, char * responseBody);
    void SetWebSettings();

    InfoMainWindow * m_pInfoWindow;
    TimerMainWindow * m_pTimerWindow;

    static MainWindow * s_pMainWindow;
};

#endif // MAINWINDOW_H
