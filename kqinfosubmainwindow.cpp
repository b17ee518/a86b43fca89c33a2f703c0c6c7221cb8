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
    ui->overviewTable->resize(this->width(), 48);
    ui->overviewTable->updateGeometry();
/*
    QTableWidgetItem * pwi = new QTableWidgetItem("AAAA");
    ui->overviewTable->setItem(0, 0, pwi);
    ui->overviewTable->item(0, 0)->setTextColor(QColor(255, 255, 255));
*/
    ui->titleFrame->resize(this->width(), ui->titleFrame->height());

    QTimer::singleShot(2000, this, SLOT(slotDebugAfterInit()));

}

KQInfoSubMainWindow::~KQInfoSubMainWindow()
{
    delete ui;
}

void KQInfoSubMainWindow::slotHandleSubContentFrameSizeChange(KQContentFrameBase *pFrame)
{
    QSize size;
    size.setWidth(this->width());
    size.setHeight(
            ui->missionFrame->height()
            + ui->fleetFrame_1->height()
            + ui->fleetFrame_2->height()
            + ui->fleetFrame_3->height()
            + ui->fleetFrame_4->height()
            + ui->repairFrame->height()
            );
    ui->contentFrame->resize(size);
}

void KQInfoSubMainWindow::slotDebugAfterInit()
{
/*
    QTableWidgetItem * pwi = new QTableWidgetItem("AAAA");
    ui->overviewTable->setItem(0, 0, pwi);
    ui->overviewTable->item(0, 0)->setTextColor(QColor(255, 255, 255));
*/

    QList<KQRowData> rows;
    KQRowData rd;
    rd.appendCell(KQRowCellData("AAA"));
    rd.appendCell(KQRowCellData("BBB"));
    rd.appendCell(KQRowCellData("CCC"));
    rows.append(rd);
    ui->missionFrame->tableWidget()->setColumnCount(3);
    ui->missionFrame->updateRow(rows);

}
