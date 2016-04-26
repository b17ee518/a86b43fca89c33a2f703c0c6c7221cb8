#ifndef QWEBENGINEMOUSEEVENTFILTER_H
#define QWEBENGINEMOUSEEVENTFILTER_H

#include <QObject>
#include <QTime>

class QWebEngineMouseEventFilter : public QObject
{
	Q_OBJECT

public:
	QWebEngineMouseEventFilter() {}
	QWebEngineMouseEventFilter(QObject *parent);
	~QWebEngineMouseEventFilter();

protected:
	bool eventFilter(QObject * obj, QEvent * event) override;

private:
	QTime _pressedTime;
	QTime _releasedTime;
	QTime _dblClickedTime;	
};

#endif // QWEBENGINEMOUSEEVENTFILTER_H
