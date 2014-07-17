#include "mainwindowbase.h"
#include <QSettings>

MainWindowBase::MainWindowBase(QWidget *parent) :
    QMainWindow(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
}

void MainWindowBase::changeEvent( QEvent* e )
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
}

void MainWindowBase::slotActivate( QWidget* w, bool bActivate )
{
    this->raise();
    this->stackUnder( w );
}

void MainWindowBase::slotToggleRestoreMinimize(bool bRestore)
{

}

void MainWindowBase::mwbPostInit()
{

    QSettings settings("h5nc", "KanPlay");
    restoreGeometry(settings.value(objectName()+"/geometry").toByteArray());
    restoreState(settings.value(objectName()+"/windowstate").toByteArray());
}

void MainWindowBase::closeEvent(QCloseEvent *event)
{
    QSettings settings("h5nc", "KanPlay");
    settings.setValue(objectName()+"/geometry", saveGeometry());
    settings.setValue(objectName()+"/windowState", saveState());
    QMainWindow::closeEvent(event);
}
