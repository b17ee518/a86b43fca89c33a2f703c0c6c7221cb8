#include "infomainwindow.h"
#include "ui_infomainwindow.h"

InfoMainWindow::InfoMainWindow(QWidget *parent) :
    SubMainWindow(parent),
    ui(new Ui::InfoMainWindow)
{
    ui->setupUi(this);
    mwbPostInit();
    ui->titleFrame->setHandlingWidget(this);

    ui->overviewTable->horizontalHeader()->hide();
    ui->overviewTable->verticalHeader()->hide();

    lstCollapsibleFrames.append(ui->missionFrame);
    lstCollapsibleFrames.append(ui->fleetFrame_1);
    lstCollapsibleFrames.append(ui->fleetFrame_2);
    lstCollapsibleFrames.append(ui->fleetFrame_3);
    lstCollapsibleFrames.append(ui->fleetFrame_4);
    lstCollapsibleFrames.append(ui->repairFrame);

    setOverviewColumnFormat();
    setMissionColumnFormat();
    setFleetColumnFormat();
    setRepairColumnFormat();

    pUpdateTimer = new QTimer(this);
    connect(pUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
    pUpdateTimer->start(16);

    KQUI_CollapsibleFrame * pFrame;
    foreach(pFrame, lstCollapsibleFrames)
    {
        pFrame->tableWidget()->hide();
        connect(pFrame->tableWidget(), SIGNAL(sigTableSizeChanged()), this, SLOT(slotOnTableSizeChanged()));
        connect(pFrame->pushButton(), SIGNAL(toggled(bool)), pFrame->tableWidget(), SLOT(setVisible(bool)));
    }
    ui->missionFrame->pushButton()->setChecked(true);
    ui->fleetFrame_1->pushButton()->setChecked(true);
    ui->repairFrame->pushButton()->setChecked(true);
}

InfoMainWindow::~InfoMainWindow()
{
    delete ui;
}

void InfoMainWindow::setOverviewColumnFormat()
{
    ui->overviewTable->setRowCount(3);
    ui->overviewTable->setColumnCount(2);
    ui->overviewTable->setColumnWidth(0, 150);
    ui->overviewTable->setColumnWidth(1, 130);
}

void InfoMainWindow::setMissionColumnFormat()
{
    KQTableWidget * pTableWidget = ui->missionFrame->tableWidget();
    pTableWidget->setRowCount(0);
    pTableWidget->setColumnCount(3);
    pTableWidget->setSeparatorColumn(1);
    pTableWidget->setColumnWidth(0, 24);
    pTableWidget->setColumnWidth(1, 252);
    pTableWidget->setColumnWidth(2, 64);
}

void InfoMainWindow::setFleetColumnFormat()
{
    setFleetColumnFormat(ui->fleetFrame_1->tableWidget());
    setFleetColumnFormat(ui->fleetFrame_2->tableWidget());
    setFleetColumnFormat(ui->fleetFrame_3->tableWidget());
    setFleetColumnFormat(ui->fleetFrame_4->tableWidget());
}

void InfoMainWindow::setFleetColumnFormat(KQTableWidget *pTableWidget)
{
    pTableWidget->setRowCount(0);
    pTableWidget->setColumnCount(9);
    pTableWidget->setSeparatorColumn(3);
    /*
    pTableWidget->setColumnWidth(0, 12);    // No.
    pTableWidget->setColumnWidth(1, 80);    // Name
    pTableWidget->setColumnWidth(2, 40);    // Lv
    pTableWidget->setColumnWidth(3, 32);    // Cond
    pTableWidget->setColumnWidth(4, 66);    // Exp
    pTableWidget->setColumnWidth(5, 18);    // Fuel
    pTableWidget->setColumnWidth(6, 18);    // Bullet
    pTableWidget->setColumnWidth(7, 56);    // Hp
    pTableWidget->setColumnWidth(8, 18);    // State
    */
}

void InfoMainWindow::setRepairColumnFormat()
{
    KQTableWidget * pTableWidget = ui->repairFrame->tableWidget();

    pTableWidget->setRowCount(0);
    pTableWidget->setColumnCount(6);
    pTableWidget->setSeparatorColumn(3);
    /*
    pTableWidget->setColumnWidth(0, 132);    // Name
    pTableWidget->setColumnWidth(1, 40);    // Lv
    pTableWidget->setColumnWidth(2, 32);    // Cond
    pTableWidget->setColumnWidth(3, 64);    // Time
    pTableWidget->setColumnWidth(4, 56);    // HP
    pTableWidget->setColumnWidth(5, 16);    // State
    */

}

void InfoMainWindow::slotOnTableSizeChanged()
{
    int minw=280;
    KQUI_CollapsibleFrame * pFrame;
    foreach (pFrame, lstCollapsibleFrames)
    {
        pFrame->tableWidget()->resizeColumnsToContents();
        int w = pFrame->tableWidget()->minimumWidth();
        if (w > minw)
        {
            minw = w;
        }
    }
    foreach (pFrame, lstCollapsibleFrames)
    {
        pFrame->tableWidget()->adjustSizeToWidth(minw);
    }
    this->adjustSize();

}
void InfoMainWindow::slotUpdateTimer()
{
    static int count = 0;
    count++;

    if (count == 125)
    {
        QList<KQRowData> rows;
        KQRowData rd;
        rd.appendCell(KQRowCellData(" - "));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("敵潜水艦を制圧せよ！")));
        rd.appendCell(KQRowCellData("(50%)"));
        rows.clear();
        rows.append(rd);

        ui->missionFrame->tableWidget()->updateFullTable(rows);

        rd.clearCells();
        rd.appendCell(KQRowCellData("1"));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("浦風AAAAAAAAAAAAAAAAAAAAA")));
        rd.appendCell(KQRowCellData("Lv.27"));
        rd.appendCell(KQRowCellData("(58)"));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("次:548")));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("燃料")));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("弾薬")));
        rd.appendCell(KQRowCellData("(1/16)"));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("大")));
        rows.clear();
        rows.append(rd);

        rd.appendCell(KQRowCellData("1"));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("浦風")));
        rd.appendCell(KQRowCellData("Lv"));
        rd.appendCell(KQRowCellData(" "));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("")));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("")));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("")));
        rd.appendCell(KQRowCellData("()"));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("大")));
        rows.append(rd);
        rows.append(rd);
        rows.append(rd);
        rows.append(rd);
        rows.append(rd);
        rows.append(rd);
        rows.append(rd);

        ui->fleetFrame_1->tableWidget()->updateFullTable(rows);
    }
    else if (count == 250)
    {

        QList<KQRowData> rows;
        KQRowData rd;
        rd.clearCells();
        rd.appendCell(KQRowCellData("1"));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("浦風")));
        rd.appendCell(KQRowCellData("Lv"));
        rd.appendCell(KQRowCellData(" "));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("")));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("")));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("")));
        rd.appendCell(KQRowCellData("()"));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("大")));
        rows.clear();
        rows.append(rd);
        ui->fleetFrame_1->tableWidget()->updateFullTable(rows);

    }
    this->adjustSize();
}

void InfoMainWindow::on_pbClose_clicked()
{
    close();
}

void InfoMainWindow::on_pbMinimize_clicked()
{
    setWindowState(Qt::WindowMinimized);
}
