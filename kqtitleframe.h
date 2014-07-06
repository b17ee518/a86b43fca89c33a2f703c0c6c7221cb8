#ifndef KQTITLEFRAME_H
#define KQTITLEFRAME_H

#include "kqframebase.h"
#include "kqpushbutton.h"
#include <QMouseEvent>
#include <QTimer>

class KQTitleFrame : public KQFrameBase
{
    Q_OBJECT
public:
    explicit KQTitleFrame(QWidget *parent = 0);

    void setHandlingWidget(QWidget * pw);
    QWidget * handlingWidget(){return m_pHandlingWidget;}

protected:
    void stopMoving();

private:
    QWidget * m_pHandlingWidget;

    bool m_bMoving;
    QPoint m_lastMovingPos;

    QTimer * m_pUpdateTimer;

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);


signals:

public slots:

private slots:
    void slotUpdateTimer();

};

#endif // KQTITLEFRAME_H
