#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainwindowbase.h"
#include "infomainwindow.h"
#include "timermainwindow.h"

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

    void onSubMainWindowShowHide(bool bShow, MainWindowBase * pWindow);

public slots:
    virtual void slotActivate(QWidget* w , bool bActivate);
    virtual void slotToggleRestoreMinimize(bool bRestore);

protected:
    virtual void changeEvent(QEvent * e);
    virtual void closeEvent(QCloseEvent * e);
    virtual void moveEvent(QMoveEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent * event);

private slots:
    void on_pbClose_clicked();

    void on_pbMinimize_clicked();

    void on_pbCheckTrasparent_toggled(bool checked);

private:
    Ui::MainWindow *ui;

    InfoMainWindow * m_pInfoWindow;
    TimerMainWindow * m_pTimerWindow;

    static MainWindow * s_pMainWindow;
};

#endif // MAINWINDOW_H
