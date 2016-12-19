#include "developactionselectdialog.h"
#include "ui_developactionselectdialog.h"

#include <QDialogButtonBox>
#include "kansavedata.h"
#include "ControlManager.h"
#include "kandataconnector.h"

DevelopActionSelectDialog::DevelopActionSelectDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::DevelopActionSelectDialog)
{
	ui->setupUi(this);

	this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);

	const auto& developSetting = ControlManager::getInstance().getDevelopSetting();
	int id = 0;
	int count = 0;
	if (!developSetting.toDevelopSlotItemList.empty())
	{
		id = developSetting.toDevelopSlotItemList.firstKey();
		count = developSetting.toDevelopSlotItemList.first();
	}

	ui->leId->setText(QString::number(id));
	ui->leCount->setText(QString::number(count));

	setNameByID();

	connect(ui->leId, SIGNAL(textEdited()), this, SLOT(slotOnIDEdited()));
}

DevelopActionSelectDialog::~DevelopActionSelectDialog()
{
	delete ui;
}

void DevelopActionSelectDialog::setPositionTo(int x, int y)
{
	move(x - this->geometry().width() / 2, y - this->geometry().height() / 2);
}

void DevelopActionSelectDialog::getIdAndCount(int& slotitemid, int& count)
{
	slotitemid = ui->leId->text().toInt();
	count = ui->leCount->text().toInt();
}

void DevelopActionSelectDialog::setNameByID()
{
	int id = ui->leId->text().toInt();
	if (id <= 0)
	{
		ui->leName->setText("");
		return;
	}

	auto* pmstslotitem = KanDataConnector::getInstance().findMstSlotItemFromSlotitemid(id);
	if (!pmstslotitem)
	{
		ui->leName->setText("");
		return;
	}
	int ownedCount = ControlManager::getInstance().getTotalSlotItemCountForID(id);
	ui->leName->setText(QString::fromLocal8Bit("%1 (%2)")
		.arg(pmstslotitem->api_name)
		.arg(ownedCount));
}

void DevelopActionSelectDialog::slotOnIDEdited()
{
	setNameByID();
}
