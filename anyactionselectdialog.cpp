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
	ui->cbAirBaseCond->setChecked(ControlManager::getInstance().getAnySetting().checkAirBaseCond);
	ui->cbCond->setChecked(ControlManager::getInstance().getAnySetting().checkCond);
	ui->cbMiddleDamage->setChecked(ControlManager::getInstance().getAnySetting().allowMiddleDamageSortie);

	connect(ui->leArea, SIGNAL(textEdited()), this, SLOT(slotUncheckAllAreaButtons()));
	connect(ui->leMap, SIGNAL(textEdited()), this, SLOT(slotUncheckAllAreaButtons()));

	for (int i = 0; i < 6; i++)
	{
		auto button = ui->bgArea->button(i);
		if (button)
		{
			connect(button, SIGNAL(clicked()), this, SLOT(slotClearLEAreaAndMap()));
		}
	}
}

AnyActionSelectDialog::~AnyActionSelectDialog()
{
    delete ui;
}

void AnyActionSelectDialog::getMapAndArea(int&area, int&map)
{
	if (!ui->leArea->text().isEmpty())
	{
		area = ui->leArea->text().toInt();
		map = ui->leMap->text().toInt();
	}
	else
	{
		area = ui->bgArea->checkedId();
		map = ui->bgMap->checkedId();
	}
}

void AnyActionSelectDialog::setSelections(int area, int map)
{
	QAbstractButton* button;
	if (area >0 && area <= 6)
	{
		button = ui->bgArea->button(area);
		if (button)
		{
			button->setChecked(true);
		}
	}
	else
	{
		slotUncheckAllAreaButtons();
		ui->leArea->setText(QString::number(area));
		ui->leMap->setText(QString::number(map));
	}

	if (map > 0 && map <= 6)
	{
		button = ui->bgMap->button(map);
		if (button)
		{
			button->setChecked(true);
		}
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
	return ui->cbAirBaseCond->isChecked();
}

bool AnyActionSelectDialog::isCheckCond()
{
	return ui->cbCond->isChecked();
}

bool AnyActionSelectDialog::isAllowMiddle()
{
	return ui->cbMiddleDamage->isChecked();
}

void AnyActionSelectDialog::slotUncheckAllAreaButtons()
{
	slotResetExtraSettings();
	ui->bgArea->setExclusive(false);
	ui->bgMap->setExclusive(false);
	for (int i = 0; i < 6; i++)
	{
		auto button = ui->bgArea->button(i);
		if (button)
		{
			button->setChecked(false);
		}
	}
	// omit map
	/*
	for (int i = 0; i < 6; i++)
	{
		auto button = ui->bgMap->button(i);
		if (button)
		{
			button->setChecked(false);
		}
	}
	*/
	ui->bgArea->setExclusive(true);
	ui->bgMap->setExclusive(true);
}

void AnyActionSelectDialog::slotClearLEAreaAndMap()
{
	slotResetExtraSettings();
	ui->leArea->setText("");
	ui->leMap->setText("");
}

void AnyActionSelectDialog::slotResetExtraSettings()
{
	ui->cbAirBaseCond->setChecked(false);
	ui->cbCond->setChecked(true);
	ui->cbMiddleDamage->setChecked(false);
}