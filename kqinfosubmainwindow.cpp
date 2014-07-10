#include "kqinfosubmainwindow.h"
#include "ui_kqinfosubmainwindow.h"
#include "kqrowdata.h"
#include <QTextCodec>

KQInfoSubMainWindow::KQInfoSubMainWindow(QWidget *parent) :
    KQSubMainWindowBase(parent),
    ui(new Ui::KQInfoSubMainWindow)
{
    ui->setupUi(this);

    ui->titleFrame->setHandlingWidget(this);

    mwbPostInit();

    ui->titleFrame->resize(this->width(), ui->titleFrame->height());

    setMissionColumnFormat();
    setFleetColumnFormat();
    setRepairColumnFormat();

    connect(ui->missionFrame->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_1->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->missionFrame->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_2->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->missionFrame->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_3->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->missionFrame->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_4->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->missionFrame->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->repairFrame->tableWidget(), SLOT(slotExpandOnSeparator(int)));

    connect(ui->fleetFrame_1->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->missionFrame->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_1->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_2->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_1->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_3->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_1->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_4->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_1->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->repairFrame->tableWidget(), SLOT(slotExpandOnSeparator(int)));

    connect(ui->fleetFrame_2->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->missionFrame->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_2->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_1->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_2->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_3->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_2->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_4->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_2->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->repairFrame->tableWidget(), SLOT(slotExpandOnSeparator(int)));

    connect(ui->fleetFrame_3->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->missionFrame->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_3->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_1->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_3->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_2->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_3->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_4->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_3->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->repairFrame->tableWidget(), SLOT(slotExpandOnSeparator(int)));

    connect(ui->fleetFrame_4->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->missionFrame->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_4->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_1->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_4->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_2->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_4->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_3->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->fleetFrame_4->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->repairFrame->tableWidget(), SLOT(slotExpandOnSeparator(int)));

    connect(ui->repairFrame->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->missionFrame->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->repairFrame->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_1->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->repairFrame->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_2->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->repairFrame->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_3->tableWidget(), SLOT(slotExpandOnSeparator(int)));
    connect(ui->repairFrame->tableWidget(), SIGNAL(sigExpandOnSeparator(int)), ui->fleetFrame_4->tableWidget(), SLOT(slotExpandOnSeparator(int)));

    QTimer::singleShot(2000, this, SLOT(slotDebugAfterInit()));

    this->resize(0, 0);

}

KQInfoSubMainWindow::~KQInfoSubMainWindow()
{
    delete ui;
}

void KQInfoSubMainWindow::setOverviewColumnFormat()
{
    ui->overviewTable->setRowCount(3);
    ui->overviewTable->setColumnCount(2);
    ui->overviewTable->setColumnWidth(0, 150);
    ui->overviewTable->setColumnWidth(1, 130);
}

void KQInfoSubMainWindow::setMissionColumnFormat()
{
    KQTableWidget * pTableWidget = ui->missionFrame->tableWidget();
    pTableWidget->setRowCount(0);
    pTableWidget->setColumnCount(3);
    pTableWidget->setSeparatorColumn(1);
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

void KQInfoSubMainWindow::setRepairColumnFormat()
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

void KQInfoSubMainWindow::slotDebugAfterInit()
{
    /*
    QTextCodec * pCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(pCodec);
    */

    QList<KQRowData> rows;
    KQRowData rd;
    rd.appendCell(KQRowCellData(" - "));
    rd.appendCell(KQRowCellData(QString::fromLocal8Bit("敵潜水艦を制圧せよ！")));
    rd.appendCell(KQRowCellData("(50%)"));
    rows.clear();
    rows.append(rd);

    ui->missionFrame->updateRow(rows);

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

    ui->fleetFrame_1->updateRow(rows);
//    this->adjustSize();

    QTimer::singleShot(2000, this, SLOT(slotDebugAfterInit2()));
}

void KQInfoSubMainWindow::slotDebugAfterInit2()
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
    ui->fleetFrame_1->updateRow(rows);
    /*
    qDebug("%d", ui->missionFrame->tableWidget()->calculateColumnsWidth());
    qDebug("%d", ui->fleetFrame_1->tableWidget()->calculateColumnsWidth());
    qDebug("%d", ui->fleetFrame_2->tableWidget()->calculateColumnsWidth());
    qDebug("%d", ui->fleetFrame_3->tableWidget()->calculateColumnsWidth());
    qDebug("%d", ui->fleetFrame_4->tableWidget()->calculateColumnsWidth());
    qDebug("%d", ui->repairFrame->tableWidget()->calculateColumnsWidth());


    qDebug("%d", ui->missionFrame->collapseButton()->width());
    qDebug("%d", ui->fleetFrame_1->collapseButton()->width());
    qDebug("%d", ui->fleetFrame_2->collapseButton()->width());
    qDebug("%d", ui->fleetFrame_3->collapseButton()->width());
    qDebug("%d", ui->fleetFrame_4->collapseButton()->width());
    qDebug("%d", ui->repairFrame->collapseButton()->width());

    ui->overviewTable->resize(0, 0);
    qDebug("%d", ui->overviewTable->width());
    ui->contentFrame->resize(0, 0);
    qDebug("%d", ui->contentFrame->width());
    */
/*
    ui->repairFrame->setMaximumWidth(280);
    ui->fleetFrame_1->setMaximumWidth(280);
    ui->fleetFrame_2->setMaximumWidth(280);
    ui->fleetFrame_3->setMaximumWidth(280);
    ui->fleetFrame_4->setMaximumWidth(280);
    ui->missionFrame->setMaximumWidth(280);
*/



    this->adjustSize();

}
