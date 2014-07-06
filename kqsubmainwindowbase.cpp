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
    if (pos().y() == 0)
    {
        if (!m_bDockingOnTop)
        {
            m_bDockingOnTop = true;
            m_pDockingCallbackTimer->start(16);
            setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
            show();

        }
    }
    else if (pos().y() > 20)
    {
        if (m_bDockingOnTop)
        {
            m_bDockingOnTop = false;
            m_pDockingCallbackTimer->stop();
            setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
            show();
        }
    }
}

void KQSubMainWindowBase::mouseReleaseEvent(QMouseEvent *event)
{
    if (pos().y() < 20 && pos().y()!=0)
    {
        move(pos().x(), 0);
    }
    KQMainWindowBase::mouseReleaseEvent(event);
}

void KQSubMainWindowBase::slotDockingTimerCallback()
{
    QPoint pt = QCursor::pos();
    QRect rc(pos().x(), 0, size().width(), 20);
    QRect rcwhole(pos().x()-10, pos().y()-10, size().width()+20, size().height()+20);

    if (pos().y() == 0)
    {
        if (rcwhole.contains(pt))
        {
            // already expanded
            return;
        }
        else
        {
            // to collapse
            move(pos().x(), -size().height()+5);
        }
    }
    else if (rc.contains(pt))
    {
        // to expand
        move(pos().x(), 0);
    }
}
