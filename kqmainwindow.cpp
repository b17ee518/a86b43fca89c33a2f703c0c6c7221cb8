#include "kqmainwindow.h"
#include "ui_kqmainwindow.h"
#include <QDesktopWidget>

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

    QObject::connect( this, SIGNAL( sigActivated( QWidget*, bool ) ), m_pInfoWindow, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( this, SIGNAL( sigActivated( QWidget*, bool ) ), m_pTimerWindow, SLOT( slotActivate( QWidget*, bool ) ) );

    QObject::connect( m_pInfoWindow, SIGNAL( sigActivated( QWidget*, bool ) ), this, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( m_pInfoWindow, SIGNAL( sigActivated( QWidget*, bool ) ), m_pTimerWindow, SLOT( slotActivate( QWidget*, bool ) ) );

    QObject::connect( m_pTimerWindow, SIGNAL( sigActivated( QWidget*, bool ) ), m_pInfoWindow, SLOT( slotActivate( QWidget*, bool ) ) );
    QObject::connect( m_pTimerWindow, SIGNAL( sigActivated( QWidget*, bool ) ), this, SLOT( slotActivate( QWidget*, bool ) ) );

    connect(m_pInfoWindow, SIGNAL(sigRestoreMinimizeToggled(bool)), this, SLOT(slotToggleRestoreMinimize(bool)));
    connect(m_pTimerWindow, SIGNAL(sigRestoreMinimizeToggled(bool)), this, SLOT(slotToggleRestoreMinimize(bool)));
}

void KQMainWindow::onSubMainWindowShowHide(bool bShow, KQMainWindowBase *pWindow)
{
    ui->titleFrame->onSubMainWindowShowHide(bShow, pWindow);
}

void KQMainWindow::changeEvent(QEvent *e)
{
    KQMainWindowBase::changeEvent(e);

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

void KQMainWindow::closeEvent(QCloseEvent *e)
{
//    m_pFiddler->Shutdown();
    KQMainWindowBase::closeEvent(e);

    m_pInfoWindow->close();
    m_pTimerWindow->close();
}

void KQMainWindow::moveEvent(QMoveEvent *e)
{
    KQMainWindowBase::moveEvent(e);

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

void KQMainWindow::mouseReleaseEvent(QMouseEvent *event)
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
    KQMainWindowBase::mouseReleaseEvent(event);
}

void KQMainWindow::slotActivate(QWidget *w, bool bActivate)
{
    this->raise();
    w->stackUnder(this);
}

void KQMainWindow::slotToggleRestoreMinimize(bool bRestore)
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
