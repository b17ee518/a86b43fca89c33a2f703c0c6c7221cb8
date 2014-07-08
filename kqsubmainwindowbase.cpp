#include "kqsubmainwindowbase.h"
#include "kqmainwindow.h"
#include <QApplication>

KQSubMainWindowBase::KQSubMainWindowBase(QWidget *parent) :
    KQMainWindowBase(parent)
{
    m_bDockingOnTop = false;

    m_pDockingCallbackTimer = new QTimer(this);
    connect(m_pDockingCallbackTimer, SIGNAL(timeout()), this, SLOT(slotDockingTimerCallback()));
}

void KQSubMainWindowBase::mwiInit()
{
    KQMainWindowBase::mwbInit();
}

void KQSubMainWindowBase::showEvent(QShowEvent *event)
{
    KQMainWindow::mainWindow()->onSubMainWindowShowHide(true, this);
}

void KQSubMainWindowBase::hideEvent(QHideEvent *event)
{
    KQMainWindow::mainWindow()->onSubMainWindowShowHide(false, this);
}

void KQSubMainWindowBase::moveEvent(QMoveEvent *event)
{
    if (y() == 0)
    {
        if (!m_bDockingOnTop)
        {
            m_bDockingOnTop = true;
            m_pDockingCallbackTimer->start(16);
            setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
            show();

        }
    }
    else if (y() > 20)
    {
        if (m_bDockingOnTop)
        {
            m_bDockingOnTop = false;
            m_pDockingCallbackTimer->stop();
            setWindowFlags((windowFlags() & ~Qt::WindowStaysOnTopHint) | Qt::WindowStaysOnBottomHint);
            show();
            setWindowFlags(windowFlags() & ~Qt::WindowStaysOnBottomHint);
            show();
        }
    }
}

void KQSubMainWindowBase::mouseReleaseEvent(QMouseEvent *event)
{
    if (y() < 20 && y()!=0)
    {
        move(x(), 0);
    }

    KQMainWindow * pMain = KQMainWindow::mainWindow();
    if (x()+width() <= pMain->x()+pMain->width()
            && x()>= pMain->x()
            && y()>=pMain->y()+pMain->height()-20
            && y()<=pMain->y()+pMain->height()+20)
    {
        move(x(), pMain->y()+pMain->height());
    }

    KQMainWindowBase::mouseReleaseEvent(event);
}

void KQSubMainWindowBase::slotDockingTimerCallback()
{
    QPoint pt = QCursor::pos();
    QRect rc(x(), 0, size().width(), 20);
    QRect rcwhole(x()-10, y()-10, size().width()+20, size().height()+20);

    if (y() == 0)
    {
        if (rcwhole.contains(pt))
        {
            // already expanded
            return;
        }
        else
        {
            // to collapse
            move(x(), -size().height()+5);
        }
    }
    else if (rc.contains(pt))
    {
        // to expand
        move(x(), 0);
    }
}

void KQSubMainWindowBase::slotHandleContentFrameSizeChange(KQContentFrameBase *pFrame)
{
    int height = pFrame->height()+20;
    resize(pFrame->width(), height);
    updateGeometry();
}

