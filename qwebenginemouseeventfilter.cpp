#include "qwebenginemouseeventfilter.h"
#include <QEvent>
#include <QMouseEvent>

QWebEngineMouseEventFilter::QWebEngineMouseEventFilter(QObject *parent)
	: QObject(parent)
{

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