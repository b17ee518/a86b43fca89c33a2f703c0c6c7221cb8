#include "kqtimersubmainwindow.h"
#include "ui_kqtimersubmainwindow.h"

KQTimerSubMainWindow::KQTimerSubMainWindow(QWidget *parent) :
    KQSubMainWindowBase(parent),
    ui(new Ui::KQTimerSubMainWindow)
{
    ui->setupUi(this);

    ui->titleFrame->setHandlingWidget(this);

    mwbPostInit();
}

KQTimerSubMainWindow::~KQTimerSubMainWindow()
{
    delete ui;
}
