#include "kqmainwindowbase.h"
#include <QSettings>

KQMainWindowBase::KQMainWindowBase(QWidget *parent) :
    QMainWindow(parent),
    KQWidgetInterface(this)
{
    mwbInit();
}

void KQMainWindowBase::mwbInit()
{
    wiSetCommonStyleSheet();
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);

}
void KQMainWindowBase::changeEvent( QEvent* e )
{
    QMainWindow::changeEvent(e);

    if (e->type() == QEvent::WindowStateChange)
    {
        QWindowStateChangeEvent* event = static_cast<QWindowStateChangeEvent*>(e);
        if(event->oldState() & Qt::WindowMinimized)
        {
            emit this->sigRestoreMinimizeToggled(true);
        }
        else if (isMinimized())
        {
            emit this->sigRestoreMinimizeToggled(false);
        }
    }
    else if( e->type() == QEvent::ActivationChange )
    {
        if (this->isActiveWindow())
        {
            emit this->sigActivated( this, true );
        }
        else
        {
            emit this->sigActivated( this, false);
        }
    }
    QMainWindow::changeEvent( e );
}

void KQMainWindowBase::slotActivate( QWidget* w, bool bActivate )
{
    this->raise();
    this->stackUnder( w );
}

void KQMainWindowBase::slotToggleRestoreMinimize(bool bRestore)
{

}

void KQMainWindowBase::mwbPostInit()
{
    QSettings settings("h5nc", "KanPlay");
    restoreGeometry(settings.value(objectName()+"/geometry").toByteArray());
    restoreState(settings.value(objectName()+"/windowstate").toByteArray());
}

void KQMainWindowBase::closeEvent(QCloseEvent *event)
{
    QSettings settings("h5nc", "KanPlay");
    settings.setValue(objectName()+"/geometry", saveGeometry());
    settings.setValue(objectName()+"/windowState", saveState());
    QMainWindow::closeEvent(event);
}
