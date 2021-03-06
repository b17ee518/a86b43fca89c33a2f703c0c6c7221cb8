#ifndef QWINDOWSEVENTFILTER_H
#define QWINDOWSEVENTFILTER_H

#include <QAbstractNativeEventFilter>

#include <QFile>
#include <QTextStream>

class QWindowsEventFilter : public QAbstractNativeEventFilter
{
public:
	QWindowsEventFilter();
	virtual ~QWindowsEventFilter();
	virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
	/*
private:
QFile *_logFile = NULL;
QTextStream * _logStream = NULL;
*/
};

#endif // QWINDOWSEVENTFILTER_H
