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
