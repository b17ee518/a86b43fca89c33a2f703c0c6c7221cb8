#ifndef KQWebPage_H
#define KQWebPage_H

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
#include <QWebEnginePage>
#else
#include <QWebPage>
#endif
#include <QApplication>

class KQWebPage
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
	: public QWebEnginePage
#else
	: public QWebPage
#endif
{
	Q_OBJECT
public:
	explicit KQWebPage(QWidget *parent = 0);

signals:

	public slots :
		virtual bool shouldInterruptJavaScript()
#if (QT_VERSION < QT_VERSION_CHECK(5, 6, 0))
		override
#endif
	{
		// QApplication::processEvents(QEventLoop::AllEvents, 42);
		// Ignore the error
		return false;
	}

};

#endif // KQWebPage_H
