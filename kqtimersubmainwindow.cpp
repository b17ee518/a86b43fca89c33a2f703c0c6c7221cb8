#include "kqtimersubmainwindow.h"
#include "ui_kqtimersubmainwindow.h"

KQTimerSubMainWindow::KQTimerSubMainWindow(QWidget *parent) :
    KQSubMainWindowBase(parent),
    ui(new Ui::KQTimerSubMainWindow)
{
    ui->setupUi(this);

    ui->titleFrame->setHandlingWidget(this);

    mwbPostInit();
    connect(ui->contentFrame, SIGNAL(sigContentFrameSizeChanged(KQContentFrameBase*)), this, SLOT(slotHandleContentFrameSizeChange(KQContentFrameBase*)));
}

KQTimerSubMainWindow::~KQTimerSubMainWindow()
{
    delete ui;
}
