#include "infomainwindow.h"
#include "ui_infomainwindow.h"

#include "mainwindow.h"

#define INFO_UPDATETIMER_INTERVAL				50
#define INFO_UPDATETIMER_INTERVAL_SLOW	500

InfoMainWindow::InfoMainWindow(QWidget *parent) :
	SubMainWindow(parent),
	ui(new Ui::InfoMainWindow)
{
	ui->setupUi(this);

	mwbPostInit();
	ui->titleFrame->setHandlingWidget(this);
	ui->overviewTable->setFont(QApplication::font());

	ui->overviewTable->horizontalHeader()->hide();
	ui->overviewTable->verticalHeader()->hide();

	_pFleetFrames[0] = ui->fleetFrame_1;
	_pFleetFrames[1] = ui->fleetFrame_2;
	_pFleetFrames[2] = ui->fleetFrame_3;
	_pFleetFrames[3] = ui->fleetFrame_4;

	_lstCollapsibleFrames.append(ui->missionFrame);
	_lstCollapsibleFrames.append(ui->fleetFrame_1);
	_lstCollapsibleFrames.append(ui->fleetFrame_2);
	_lstCollapsibleFrames.append(ui->fleetFrame_3);
	_lstCollapsibleFrames.append(ui->fleetFrame_4);
	_lstCollapsibleFrames.append(ui->repairFrame);

	setOverviewColumnFormat();
	setMissionColumnFormat();
	setFleetColumnFormat();
	setRepairColumnFormat();

	_pUpdateTimer = new QTimer(this);
	_pUpdateTimer->setTimerType(Qt::PreciseTimer);
	connect(_pUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
	_pUpdateTimer->start(INFO_UPDATETIMER_INTERVAL);
	connect(MainWindow::mainWindow(), SIGNAL(sigToggleSleepMode(bool)), this, SLOT(onToggleSleepMode(bool)));

	KQUI_CollapsibleFrame * pFrame;
	foreach(pFrame, _lstCollapsibleFrames)
	{
		pFrame->tableWidget()->hide();
		pFrame->pushButton()->setStyleSheet("text-align: left;");
		connect(pFrame->tableWidget(), SIGNAL(sigTableSizeChanged()), this, SLOT(slotOnTableSizeChanged()));
		connect(pFrame->pushButton(), SIGNAL(toggled(bool)), pFrame->tableWidget(), SLOT(setVisible(bool)));
		connect(pFrame->pushButton(), SIGNAL(toggled(bool)), pFrame, SLOT(slotResize(bool)));
	}
	ui->missionFrame->pushButton()->setChecked(true);
	ui->fleetFrame_1->pushButton()->setChecked(true);
	ui->repairFrame->pushButton()->setChecked(true);
}

InfoMainWindow::~InfoMainWindow()
{
	delete ui;
}

void InfoMainWindow::updateOverviewTable(const QList<QString>& lst, const QList<QColor>& cols)
{
	if (lst.count() == 5 && cols.count() == 5)
	{
		for (int i=0; i<2; i++)
		{
			for (int j=0; j<2; j++)
			{
				QTableWidgetItem * pItem = ui->overviewTable->item(j, i);
				pItem->setText(lst[j*2+i]);
				pItem->setTextColor(cols[j*2+i]);
			}
		}
	}
	QTableWidgetItem * pItem = ui->overviewTable->item(2, 0);
	pItem->setText(lst[4]);
	pItem->setTextColor(cols[4]);
}

void InfoMainWindow::updateMissionTable(const QString& buttonTitle, const QList<KQRowData>& rows)
{
	ui->missionFrame->pushButton()->setText(buttonTitle);
	ui->missionFrame->tableWidget()->updateFullTable(rows);
}

void InfoMainWindow::updateFleetTable(int n, const QString& buttonTitle, int colindex, bool bRed, const QList<KQRowData>& rows)
{
	static QString stylesheet_a[] =
	{
		"\
		QPushButton {   \
			color:white;    \
			text-align: left;\
		}   \
		",  // normal 0
		"\
			QPushButton {   \
			color:rgb(153, 255, 255);    \
			text-align: left;\
		}   \
		",  // have kira 1
		"\
		QPushButton {   \
			color:rgb(255, 255, 0);    \
			text-align: left;\
		}   \
		",  // kira full 2
		"\
		QPushButton {   \
			color:rgb(60, 179, 113);    \
			text-align: left;\
		}   \
		",   // tokyu ok 3
		"\
		QPushButton {   \
			color:rgb(102, 153, 255);    \
			text-align: left;\
		}   \
		",   // nezumi ok 4
		"\
			QPushButton {   \
			color:rgb(255, 153, 0);    \
			text-align: left;\
		}   \
		"   // need hokyu 4
	};
	static QString stylesheet_b[] =
	{
		" \
		QPushButton{\
			background-color: rgb(80, 80, 80);\
			border: none; \
		}\
		QPushButton:checked{\
			background-color: rgb(80, 80, 80);\
			border: none; \
		}\
		QPushButton:hover{  \
			background-color: grey; \
			border-style: outset;  \
		}  \
		",
		" \
		QPushButton:checked{\
			background-color: rgb(153, 0, 0);\
			border: none; \
		}\
		QPushButton:flat{\
			background-color: rgb(153, 0, 0);\
			border: none; \
		}\
		QPushButton:{\
			background-color: rgb(153, 0, 0);\
			border: none; \
		}\
		QPushButton:hover{  \
			background-color: rgb(128, 0, 0);; \
			border-style: outset;  \
		}  \
		"
	};
	_pFleetFrames[n]->pushButton()->setText(buttonTitle);
	_pFleetFrames[n]->tableWidget()->updateFullTable(rows);

	_pFleetFrames[n]->pushButton()->setStyleSheet(stylesheet_a[colindex]+stylesheet_b[bRed?1:0]);
}

void InfoMainWindow::updateRepairTable(const QString& buttonTitle, const QList<KQRowData>& rows)
{
	ui->repairFrame->pushButton()->setText(buttonTitle);
	ui->repairFrame->tableWidget()->updateFullTable(rows);
}

void InfoMainWindow::updateTitle(const QString& title, int colindex)
{
	static QString stylesheet[] =
	{
		" \
		QLineEdit{\
			color: rgb(255, 255, 255);\
			border: none;\
		}\
		",
		" \
		QLineEdit{\
			color: rgb(255, 255, 0);\
			border: none;\
		}\
		",
		" \
		QLineEdit{\
			color: rgb(255, 153, 0);\
			border: none;\
		}\
		"
	};
	ui->lineEditTitle->setText(title);
	ui->lineEditTitle->setStyleSheet(stylesheet[colindex]);
}

void InfoMainWindow::setOverviewColumnFormat()
{
	ui->overviewTable->setRowCount(3);
	ui->overviewTable->setColumnCount(2);
	ui->overviewTable->setColumnWidth(0, 175);
	ui->overviewTable->setColumnWidth(1, 145);
	ui->overviewTable->setSpan(2, 0, 1, 2);
	for (int i=0; i<2; i++)
	{
		for (int j=0; j<2; j++)
		{
			QTableWidgetItem * pItem = new QTableWidgetItem();
			ui->overviewTable->setItem(i, j, pItem);
		}
	}
	QTableWidgetItem * pItem = new QTableWidgetItem();
	ui->overviewTable->setItem(2, 0, pItem);
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
	pTableWidget->setColumnCount(10);
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
	pTableWidget->setColumnWidth(9, 18);    // DamekonState
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
    foreach (pFrame, _lstCollapsibleFrames)
    {
        pFrame->tableWidget()->resizeColumnsToContents();
        int w = pFrame->tableWidget()->minimumWidth();
        if (w > minw)
        {
            minw = w;
        }
    }
    foreach (pFrame, _lstCollapsibleFrames)
    {
        pFrame->tableWidget()->adjustSizeToWidth(minw);
    }
	_needUpdateNext[0] = true;
//    this->adjustSize();

}
void InfoMainWindow::slotUpdateTimer()
{
	/*
	static int count = 0;
	count++;
	*/
	auto mainWindow = MainWindow::mainWindow();
	if (mainWindow)
	{
		if (mainWindow->isSleepMode())
		{
			return;
		}
	}
	if (!this->isMinimized())
	{
		int index = -1;
		for (int i = 0; i < UPDATENEXT_TRYTIME; i++)
		{
			if (_needUpdateNext[i])
			{
				index = i;
				break;
			}
		}
		if (index >= 0)
		{
			_needUpdateNext[index] = false;
			if (index < UPDATENEXT_TRYTIME)
			{
				_needUpdateNext[index + 1] = true;
			}
			this->adjustSize();
		}
	}
}

void InfoMainWindow::on_pbClose_clicked()
{
	close();
}

void InfoMainWindow::on_pbMinimize_clicked()
{
	setWindowState(Qt::WindowMinimized);
}

void InfoMainWindow::onToggleSleepMode(bool bSleep)
{
	if (bSleep)
	{
		_pUpdateTimer->stop();
	}
	else
	{
		_pUpdateTimer->start(INFO_UPDATETIMER_INTERVAL);
	}
}
