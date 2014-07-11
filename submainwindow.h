#ifndef SUBMAINWINDOW_H
#define SUBMAINWINDOW_H

#include "mainwindowbase.h"
#include <QShowEvent>
#include <QHideEvent>
#include <QTimer>

class SubMainWindow : public MainWindowBase
{
    Q_OBJECT
public:
    explicit SubMainWindow(QWidget *parent = 0);

    bool isDockingOnTop(){return m_bDockingOnTop;}

protected:

    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *event);

    virtual void moveEvent(QMoveEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent * event);

signals:

public slots:

private:
    bool m_bDockingOnTop;
    QTimer * m_pDockingCallbackTimer;
protected slots:
    virtual void slotDockingTimerCallback();

};

#endif // SUBMAINWINDOW_H
