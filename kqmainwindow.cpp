#include "kqmainwindow.h"
#include "ui_kqmainwindow.h"

KQMainWindow * KQMainWindow::s_pMainWindow = 0;

KQMainWindow::KQMainWindow(QWidget *parent) :
    KQMainWindowBase(parent),
    ui(new Ui::KQMainWindow)
{
    setMainWindow(this);

    ui->setupUi(this);

    mwbPostInit();
}

KQMainWindow::~KQMainWindow()
{
    delete ui;
}

void KQMainWindow::postInit(KQInfoSubMainWindow *pInfo, KQTimerSubMainWindow *pTimer)
{
    m_pInfoWindow = pInfo;
    m_pTimerWindow = pTimer;

    ui->titleFrame->postInit();
}

void KQMainWindow::onSubMainWindowShowHide(bool bShow, KQMainWindowBase *pWindow)
{
    ui->titleFrame->onSubMainWindowShowHide(bShow, pWindow);
}

bool KQMainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Move)
    {
        QMoveEvent *moveEvent = static_cast<QMoveEvent*>(event);
        QPoint diffPos = moveEvent->pos()-moveEvent->oldPos();

        m_pInfoWindow->move(m_pInfoWindow->pos()+diffPos);
        m_pTimerWindow->move(m_pTimerWindow->pos()+diffPos);
    }
    return QWidget::eventFilter(obj, event);
}

void KQMainWindow::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::WindowStateChange)
    {
        QWindowStateChangeEvent* event = static_cast<QWindowStateChangeEvent*>(e);
        if(event->oldState() & Qt::WindowMinimized)
        {
            m_pInfoWindow->setWindowState(Qt::WindowNoState);
            m_pTimerWindow->setWindowState(Qt::WindowNoState);

            m_pInfoWindow->show();
            m_pTimerWindow->show();
        }
    }
}

void KQMainWindow::closeEvent(QCloseEvent *e)
{
//    m_pFiddler->Shutdown();
    KQMainWindowBase::closeEvent(e);

    m_pInfoWindow->close();
    m_pTimerWindow->close();
}
