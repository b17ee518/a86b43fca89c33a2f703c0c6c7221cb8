#include "kqinfosubmainwindow.h"
#include "ui_kqinfosubmainwindow.h"
#include "kqrowdata.h"

KQInfoSubMainWindow::KQInfoSubMainWindow(QWidget *parent) :
    KQSubMainWindowBase(parent),
    ui(new Ui::KQInfoSubMainWindow)
{
    ui->setupUi(this);

    ui->titleFrame->setHandlingWidget(this);

    mwbPostInit();

    connect(ui->contentFrame, SIGNAL(sigContentFrameSizeChanged(KQContentFrameBase*)), this, SLOT(slotHandleContentFrameSizeChange(KQContentFrameBase*)));

    ui->titleFrame->resize(this->width(), ui->titleFrame->height());

    setMissionColumnFormat();
    setFleetColumnFormat();
    setRepairColumnFormat();

    QTimer::singleShot(2000, this, SLOT(slotDebugAfterInit()));

}

KQInfoSubMainWindow::~KQInfoSubMainWindow()
{
    delete ui;
}

void KQInfoSubMainWindow::setOverviewColumnFormat()
{
    ui->overviewTable->setRowCount(3);
    ui->overviewTable->setColumnCount(2);
    ui->overviewTable->setColumnWidth(0, 170);
    ui->overviewTable->setColumnWidth(1, 170);
}

void KQInfoSubMainWindow::setMissionColumnFormat()
{
    KQTableWidget * pTableWidget = ui->missionFrame->tableWidget();
    pTableWidget->setRowCount(0);
    pTableWidget->setColumnCount(3);
    pTableWidget->setColumnWidth(0, 24);
    pTableWidget->setColumnWidth(1, 252);
    pTableWidget->setColumnWidth(2, 64);
}

void KQInfoSubMainWindow::setFleetColumnFormat()
{
    setFleetColumnFormat(ui->fleetFrame_1->tableWidget());
    setFleetColumnFormat(ui->fleetFrame_2->tableWidget());
    setFleetColumnFormat(ui->fleetFrame_3->tableWidget());
    setFleetColumnFormat(ui->fleetFrame_4->tableWidget());
}

void KQInfoSubMainWindow::setFleetColumnFormat(KQTableWidget *pTableWidget)
{
    pTableWidget->setRowCount(0);
    pTableWidget->setColumnCount(9);
    pTableWidget->setColumnWidth(0, 12);    // No.
    pTableWidget->setColumnWidth(1, 80);    // Name
    pTableWidget->setColumnWidth(2, 40);    // Lv
    pTableWidget->setColumnWidth(3, 32);    // Cond
    pTableWidget->setColumnWidth(4, 66);    // Exp
    pTableWidget->setColumnWidth(5, 18);    // Fuel
    pTableWidget->setColumnWidth(6, 18);    // Bullet
    pTableWidget->setColumnWidth(7, 56);    // Hp
    pTableWidget->setColumnWidth(8, 18);    // State
}

void KQInfoSubMainWindow::setRepairColumnFormat()
{
    KQTableWidget * pTableWidget = ui->repairFrame->tableWidget();

    pTableWidget->setRowCount(0);
    pTableWidget->setColumnCount(6);
    pTableWidget->setColumnWidth(0, 132);    // Name
    pTableWidget->setColumnWidth(1, 40);    // Lv
    pTableWidget->setColumnWidth(2, 32);    // Cond
    pTableWidget->setColumnWidth(3, 64);    // Time
    pTableWidget->setColumnWidth(4, 56);    // HP
    pTableWidget->setColumnWidth(5, 16);    // State

}

void KQInfoSubMainWindow::slotDebugAfterInit()
{
    QList<KQRowData> rows;
    KQRowData rd;
    rd.appendCell(KQRowCellData(" - "));
    rd.appendCell(KQRowCellData("敵潜水艦を制圧せよ！"));
    rd.appendCell(KQRowCellData("(50%)"));
    rows.append(rd);
    ui->missionFrame->updateRow(rows);

    rd.clearCells();
    rd.appendCell(KQRowCellData("1"));
    rd.appendCell(KQRowCellData("Urakaze"));
    rd.appendCell(KQRowCellData("Lv.27"));
    rd.appendCell(KQRowCellData("(58)"));
    rd.appendCell(KQRowCellData("tugi:548"));
    rd.appendCell(KQRowCellData("ra"));
    rd.appendCell(KQRowCellData("dn"));
    rd.appendCell(KQRowCellData("(1/16)"));
    rd.appendCell(KQRowCellData("da"));
    rows.clear();
    rows.append(rd);
    ui->fleetFrame_1->updateRow(rows);


//    QTimer::singleShot(2000, this, SLOT(slotDebugAfterInit2()));
}

void KQInfoSubMainWindow::slotDebugAfterInit2()
{
    QList<KQRowData> rows;
    ui->missionFrame->updateRow(rows);

}
