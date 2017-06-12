#include "kqtitleframe.h"
#include <QApplication>

KQTitleFrame::KQTitleFrame(QWidget *parent)
	: QFrame(parent)
{
	_bMoving = false;
	_pHandlingWidget = 0;

	_pUpdateTimer = new QTimer(this);
	connect(_pUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));

	//    this->setStyleSheet("background-color: rgb(45, 45, 48);");
}

void KQTitleFrame::setHandlingWidget(QWidget *pw)
{
	_pHandlingWidget = pw;
}

void KQTitleFrame::stopMoving()
{
	_bMoving = false;
}

void KQTitleFrame::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		_bMoving = true;
		_lastMovingPos = QCursor::pos();
		_pUpdateTimer->setTimerType(Qt::PreciseTimer);
		_pUpdateTimer->start(16);
	}
	QFrame::mousePressEvent(event);

}

void KQTitleFrame::mouseReleaseEvent(QMouseEvent *event)
{
	if (_bMoving && event->button() == Qt::LeftButton)
	{
		_bMoving = false;
	}
	QFrame::mouseReleaseEvent(event);
}

void KQTitleFrame::slotUpdateTimer()
{
	if (!QApplication::mouseButtons().testFlag(Qt::LeftButton))
	{
		_bMoving = false;
	}
	if (!_bMoving)
	{
		_pUpdateTimer->stop();
		return;
	}
	QPoint newpos = QCursor::pos();
	_pHandlingWidget->move(_pHandlingWidget->pos() + newpos - _lastMovingPos);
	_lastMovingPos = newpos;
	QCursor::pos();
}
