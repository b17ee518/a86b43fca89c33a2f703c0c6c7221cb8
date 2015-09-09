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

    inline bool isDockingOnTop(){return _bDockingOnTop;}

protected:

    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *event);

    virtual void moveEvent(QMoveEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent * event);

signals:

public slots:

private:
    bool _bDockingOnTop;
    QTimer * _pDockingCallbackTimer;
protected slots:
    virtual void slotDockingTimerCallback();

};

#endif // SUBMAINWINDOW_H
