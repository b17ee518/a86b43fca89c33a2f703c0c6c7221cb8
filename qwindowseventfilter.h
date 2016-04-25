#ifndef QWINDOWSEVENTFILTER_H
#define QWINDOWSEVENTFILTER_H

#include <QAbstractNativeEventFilter>

class QWindowsEventFilter : public QAbstractNativeEventFilter
{
public:
	QWindowsEventFilter();
	virtual ~QWindowsEventFilter();
	virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
};

#endif // QWINDOWSEVENTFILTER_H
