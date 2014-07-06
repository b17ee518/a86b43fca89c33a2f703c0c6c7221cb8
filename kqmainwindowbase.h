#ifndef KQMAINWINDOWBASE_H
#define KQMAINWINDOWBASE_H

#include <QMainWindow>
#include "kqwidgetinterface.h"
#include <QCloseEvent>

class KQMainWindowBase : public QMainWindow, public KQWidgetInterface
{
    Q_OBJECT
public:
    explicit KQMainWindowBase(QWidget *parent = 0);

protected:
    virtual void mwbInit();
    virtual void mwbPostInit();

    virtual void closeEvent(QCloseEvent *event);
    virtual void changeEvent(QEvent *event);

public slots:
    virtual void slotActivate(QWidget* w , bool bActivate);
    virtual void slotToggleRestoreMinimize(bool bRestore);

signals:
    void sigActivated( QWidget* w, bool bActivate );
    void sigRestoreMinimizeToggled(bool bRestore);

};

#endif // KQMAINWINDOWBASE_H
