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

public:
	bool allowJavaPopup = false;
	/*
protected:
virtual void javaScriptAlert(QWebFrame *originatingFrame, const QString& msg)
{
if (!allowJavaPopup)
{
return;
}
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
QWebEnginePage::javaScriptAlert(originatingFrame, msg);
#else
QWebPage::javaScriptAlert(originatingFrame, msg);
#endif
}
virtual bool javaScriptConfirm(QWebFrame *originatingFrame, const QString& msg)
{
if (!allowJavaPopup)
{
return false;
}
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
return QWebEnginePage::javaScriptConfirm(originatingFrame, msg);
#else
return QWebPage::javaScriptConfirm(originatingFrame, msg);
#endif
}
virtual bool javaScriptPrompt(QWebFrame *originatingFrame, const QString& msg, const QString& defaultValue, QString* result)
{
if (!allowJavaPopup)
{
return false;
}
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
return QWebEnginePage::javaScriptPrompt(originatingFrame, msg, defaultValue, result);
#else
return QWebPage::javaScriptPrompt(originatingFrame, msg, defaultValue, result);
#endif
}
*/
};

#endif // KQWebPage_H
