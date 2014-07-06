#include "kqinfosubmainwindow.h"
#include "ui_kqinfosubmainwindow.h"

KQInfoSubMainWindow::KQInfoSubMainWindow(QWidget *parent) :
    KQSubMainWindowBase(parent),
    ui(new Ui::KQInfoSubMainWindow)
{
    ui->setupUi(this);

    ui->titleFrame->setHandlingWidget(this);

    mwbPostInit();
}

KQInfoSubMainWindow::~KQInfoSubMainWindow()
{
    delete ui;
}
