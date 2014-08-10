#include "submainwindow.h"
#include "mainwindow.h"
#include <QApplication>

SubMainWindow::SubMainWindow(QWidget *parent) :
	MainWindowBase(parent)
{
	m_bDockingOnTop = false;

	m_pDockingCallbackTimer = new QTimer(this);
	connect(m_pDockingCallbackTimer, SIGNAL(timeout()), this, SLOT(slotDockingTimerCallback()));
}

void SubMainWindow::showEvent(QShowEvent *event)
{
	MainWindow::mainWindow()->onSubMainWindowShowHide(true, this);
	MainWindowBase::showEvent(event);
}

void SubMainWindow::hideEvent(QHideEvent *event)
{
	MainWindow::mainWindow()->onSubMainWindowShowHide(false, this);
	MainWindowBase::hideEvent(event);
}

void SubMainWindow::moveEvent(QMoveEvent *event)
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
	MainWindowBase::moveEvent(event);
}

void SubMainWindow::mouseReleaseEvent(QMouseEvent *event)
{
	if (y() < 20 && y()!=0)
	{
		move(x(), 0);
	}

	MainWindow * pMain = MainWindow::mainWindow();
	if (x()+width() <= pMain->x()+pMain->width()
			&& x()>= pMain->x()
			&& y()>=pMain->y()+pMain->height()-20
			&& y()<=pMain->y()+pMain->height()+20)
	{
		move(x(), pMain->y()+pMain->height());
	}

	MainWindowBase::mouseReleaseEvent(event);
}

void SubMainWindow::slotDockingTimerCallback()
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
