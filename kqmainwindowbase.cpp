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
