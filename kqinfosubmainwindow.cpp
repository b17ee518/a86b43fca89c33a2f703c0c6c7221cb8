#include "kqinfosubmainwindow.h"
#include "ui_kqinfosubmainwindow.h"

KQInfoSubMainWindow::KQInfoSubMainWindow(QWidget *parent) :
    KQSubMainWindowBase(parent),
    ui(new Ui::KQInfoSubMainWindow)
{
    ui->setupUi(this);

    ui->titleFrame->setHandlingWidget(this);

    mwbPostInit();

    connect(ui->contentFrame, SIGNAL(sigContentFrameSizeChanged(KQContentFrameBase*)), this, SLOT(slotHandleContentFrameSizeChange(KQContentFrameBase*)));

    ui->overviewTable->setRowCount(3);
    ui->overviewTable->setColumnCount(2);
    ui->overviewTable->setColumnWidth(0, this->width()/2);
    ui->overviewTable->setColumnWidth(1, this->width()/2);
    ui->overviewTable->adjustSize();

    QTableWidgetItem twi("AAA");
    ui->overviewTable->setItem(1, 1, &twi);
}

KQInfoSubMainWindow::~KQInfoSubMainWindow()
{
    delete ui;
}
