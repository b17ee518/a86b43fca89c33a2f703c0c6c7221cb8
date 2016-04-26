#include "qwebenginemouseeventfilter.h"
#include <QEvent>
#include <QMouseEvent>

#define MS_IGNORE_ZONE	50

QWebEngineMouseEventFilter::QWebEngineMouseEventFilter(QObject *parent)
	: QObject(parent)
{
	auto curTime = QTime::currentTime();
	_pressedTime = _releasedTime = _dblClickedTime = curTime;
}

QWebEngineMouseEventFilter::~QWebEngineMouseEventFilter()
{

}

bool QWebEngineMouseEventFilter::eventFilter(QObject * obj, QEvent * event)
{
	Q_UNUSED(obj);
	if (event->type() == QEvent::MouseButtonDblClick ||
		event->type() == QEvent::MouseButtonPress ||
		event->type() == QEvent::MouseButtonRelease ||
		event->type() == QEvent::Wheel)
	{
		QTime curTime = QTime::currentTime();
		if (event->type() == QEvent::MouseButtonDblClick)
		{
			if (abs(_dblClickedTime.msecsTo(curTime)) > MS_IGNORE_ZONE)
			{
				_dblClickedTime = curTime;
				return false;
			}
		}
		else if (event->type() == QEvent::MouseButtonPress)
		{
			if (abs(_pressedTime.msecsTo(curTime)) > MS_IGNORE_ZONE)
			{
				_pressedTime = curTime;
				return false;
			}
		}
		else if (event->type() == QEvent::MouseButtonRelease)
		{
			if (abs(_releasedTime.msecsTo(curTime)) > MS_IGNORE_ZONE)
			{
				_releasedTime = curTime;
				return false;
			}
		}
		/*
		QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(event);
		if ((pMouseEvent != NULL) && (pMouseEvent->source() == Qt::MouseEventSource::MouseEventSynthesizedBySystem))
		{
		*/
			event->ignore();
			return true;
//		}
	}
	return false;
}