#include "anyactionselectdialog.h"
#include "ui_anyactionselectdialog.h"

#include <QDialogButtonBox>
#include "kansavedata.h"
#include "ControlManager.h"
#include "kandataconnector.h"

AnyActionSelectDialog::AnyActionSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnyActionSelectDialog)
{
    ui->setupUi(this);

	ui->bgArea->setId(ui->pbArea01, 1);
	ui->bgArea->setId(ui->pbArea02, 2);
	ui->bgArea->setId(ui->pbArea03, 3);
	ui->bgArea->setId(ui->pbArea04, 4);
	ui->bgArea->setId(ui->pbArea05, 5);
	ui->bgArea->setId(ui->pbArea06, 6);

	ui->bgMap->setId(ui->pbMap01, 1);
	ui->bgMap->setId(ui->pbMap02, 2);
	ui->bgMap->setId(ui->pbMap03, 3);
	ui->bgMap->setId(ui->pbMap04, 4);
	ui->bgMap->setId(ui->pbMap05, 5);
	ui->bgMap->setId(ui->pbMap06, 6);

	this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);

	auto resetButton = ui->buttonBox->button(QDialogButtonBox::StandardButton::Reset);
	connect(resetButton, SIGNAL(clicked()), this, SLOT(slotOnReset()));

	ui->leCount->setText(QString::number(ControlManager::getInstance().getAnySetting().count));
	ui->cbAirBaseCount->setChecked(ControlManager::getInstance().getAnySetting().checkAirBaseCond);
}

AnyActionSelectDialog::~AnyActionSelectDialog()
{
    delete ui;
}

void AnyActionSelectDialog::getMapAndArea(int&area, int&map)
{
	area = ui->bgArea->checkedId();
	map = ui->bgMap->checkedId();
}

void AnyActionSelectDialog::setSelections(int area, int map)
{
	auto button = ui->bgArea->button(area);
	if (button)
	{
		button->setChecked(true);
	}
	button = ui->bgMap->button(map);
	if (button)
	{
		button->setChecked(true);
	}
}

void AnyActionSelectDialog::setPositionTo(int x, int y)
{
	move(x-this->geometry().width()/2, y-this->geometry().height()/2);
}

int AnyActionSelectDialog::getCountSet()
{
	QString countStr = ui->leCount->text();
	bool bOk = false;
	int count = countStr.toInt(&bOk);
	return count;
}

void AnyActionSelectDialog::slotOnReset()
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	pksd->totalAnyCount = 0;
	KanDataConnector::getInstance().callUpdateOverviewTable();
}

bool AnyActionSelectDialog::isCheckAirBaseCond()
{
	return ui->cbAirBaseCount->isChecked();
}
