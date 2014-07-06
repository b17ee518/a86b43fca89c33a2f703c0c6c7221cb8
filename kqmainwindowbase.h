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

    virtual void mwbInit();
    virtual void mwbPostInit();

    virtual void closeEvent(QCloseEvent *event);

signals:

public slots:

};

#endif // KQMAINWINDOWBASE_H
