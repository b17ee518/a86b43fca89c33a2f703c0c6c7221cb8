#ifndef KQTITLEFRAME_H
#define KQTITLEFRAME_H

#include <QFrame>
#include <QTimer>
#include <QMouseEvent>

class KQTitleFrame : public QFrame
{
	Q_OBJECT
public:
	explicit KQTitleFrame(QWidget *parent = 0);

	void setHandlingWidget(QWidget * pw);
	inline QWidget * handlingWidget(){ return _pHandlingWidget; }

protected:
	void stopMoving();

private:
	QWidget * _pHandlingWidget = NULL;

	bool _bMoving = false;
	QPoint _lastMovingPos;

	QTimer * _pUpdateTimer = NULL;

protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

signals:

	public slots :

		private slots :
		void slotUpdateTimer();

};

#endif // KQTITLEFRAME_H
