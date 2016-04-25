#ifndef QWEBENGINEMOUSEEVENTFILTER_H
#define QWEBENGINEMOUSEEVENTFILTER_H

#include <QObject>

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
	
};

#endif // QWEBENGINEMOUSEEVENTFILTER_H
