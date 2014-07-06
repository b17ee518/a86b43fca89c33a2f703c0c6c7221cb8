#ifndef KQMAINWINDOW_H
#define KQMAINWINDOW_H

#include <QMainWindow>
#include "kqmainwindowbase.h"
#include "kqinfosubmainwindow.h"
#include "kqtimersubmainwindow.h"
#include <QEvent>
#include <QCloseEvent>

namespace Ui {
class KQMainWindow;
}

class KQMainWindow : public KQMainWindowBase
{
    Q_OBJECT

public:
    explicit KQMainWindow(QWidget *parent = 0);
    ~KQMainWindow();

    KQInfoSubMainWindow * infoSubMainWindow(){return m_pInfoWindow;}
    KQTimerSubMainWindow * timerSubMainWindow(){return m_pTimerWindow;}

    static KQMainWindow * mainWindow(){return s_pMainWindow;}
    static void setMainWindow(KQMainWindow * pWindow){s_pMainWindow = pWindow;}
    void postInit(KQInfoSubMainWindow * pInfo, KQTimerSubMainWindow * pTimer);

    void onSubMainWindowShowHide(bool bShow, KQMainWindowBase * pWindow);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void changeEvent(QEvent * e);
    virtual void closeEvent(QCloseEvent * e);


private:
    Ui::KQMainWindow *ui;

    KQInfoSubMainWindow * m_pInfoWindow;
    KQTimerSubMainWindow * m_pTimerWindow;

    static KQMainWindow * s_pMainWindow;
};


#endif // KQMAINWINDOW_H
