#ifndef KQSUBMAINWINDOWBASE_H
#define KQSUBMAINWINDOWBASE_H

#include "kqmainwindowbase.h"
#include <QShowEvent>
#include <QHideEvent>
#include <QTimer>

class KQSubMainWindowBase : public KQMainWindowBase
{
    Q_OBJECT
public:
    explicit KQSubMainWindowBase(QWidget *parent = 0);

    bool isDockingOnTop(){return m_bDockingOnTop;}

protected:
    virtual void mwiInit();

    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *event);

    virtual void moveEvent(QMoveEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent * event);

signals:

public slots:

private:
    bool m_bDockingOnTop;
    QTimer * m_pDockingCallbackTimer;
private slots:
    void slotDockingTimerCallback();

};

#endif // KQSUBMAINWINDOWBASE_H
