#include "WeaponMainWindow.h"
#include "ui_WeaponMainWindow.h"

#include "mainwindow.h"
#include "kandataconnector.h"
#include "kansavedata.h"

#define QPROPERTY_SLOTITEMID	"slotitem_id"

#define COLINDEX_NORMAL			0
#define COLINDEX_SPARENOLOCK	1
#define COLINDEX_ALLLOCKED		2
#define COLINDEX_ONNOLOCK		3
#define COLINDEX_NOON			4

#define WEAPON_UPDATETIMER_INTERVAL	50

WeaponMainWindow::WeaponMainWindow(QWidget *parent)
: SubMainWindow(parent)
, ui(new Ui::WeaponMainWindow)
, _needRebuildTable(true)
, _lastToggledId(-1)
, _doNotRecordLast(false)
{
	ui->setupUi(this);
	mwbPostInit();
	ui->titleFrame->setHandlingWidget(this);

	connect(ui->lineEditTitle, SIGNAL(textChanged(QString)), this, SLOT(slotTextChanged(QString)));

	_pUpdateTimer = new QTimer(this);
	_pUpdateTimer->setTimerType(Qt::PreciseTimer);
	connect(_pUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
	_pUpdateTimer->start(WEAPON_UPDATETIMER_INTERVAL);
	connect(MainWindow::mainWindow(), SIGNAL(sigToggleSleepMode(bool)), this, SLOT(onToggleSleepMode(bool)));

}

WeaponMainWindow::~WeaponMainWindow()
{
    delete ui;
}

void WeaponMainWindow::clearWeaponData()
{
    _weaponGroupList.clear();
	_needRebuildTable = true;
}

void WeaponMainWindow::addWeaponData(int slotitemId, const QString& itemname, int type, int rare, bool bLocked, int level, int alv, const QString& shipname, int shiplv, int distance)
{
    UIWeaponData data;
    data.setData(bLocked, level, shipname, shiplv, alv, distance);
    bool bDone = false;
    for (QList<UIWeaponGroupData>::iterator it=_weaponGroupList.begin(); it!=_weaponGroupList.end(); ++it)
    {
        if (it->id == slotitemId)
        {
            it->weapons.push_back(data);
            bDone = true;
            break;
        }
    }
    if (!bDone)
    {
        UIWeaponGroupData group;
        group.setMetaData(slotitemId, itemname, type, rare);
        group.weapons.push_back(data);
        _weaponGroupList.push_back(group);
	}
	_needRebuildTable = true;
}

bool UIWeaponGroupData::higherThan(const UIWeaponGroupData& left, const UIWeaponGroupData& right)
{
	if (left.type < right.type)
	{
		return true;
	}
	if (left.type == right.type)
	{
		if (left.rare > right.rare)
		{
			return true;
		}
		if (left.rare == right.rare)
		{
			if (left.id > right.id)
			{
				return true;
			}
		}
	}
	return false;
}

void WeaponMainWindow::buildTable()
{
	_doNotRecordLast = true;
	if (!isVisible() || !_needRebuildTable)
	{
		return;
	}

	//
	// build here

	KanDataConnector * pkdc = &KanDataConnector::getInstance();
	KanSaveData * pksd = &KanSaveData::getInstance();
	foreach(const kcsapi_slotitem& v, pksd->slotitemdata)
	{
		bool bLocked = v.api_locked > 0;
		int level = v.api_level;
		int alv = v.api_alv;
		int slotitemId = v.api_slotitem_id;
		int distance = 0;

		auto pslotitem = pkdc->findMstSlotItemFromSlotitemid(slotitemId);
		QString itemname;
		int rare = 0;
		int type = 0;
		if (pslotitem)
		{
			itemname = pslotitem->api_name;
			rare = pslotitem->api_rare;
			auto pmstslotitem = pkdc->findMstSlotItemFromSlotitemid(slotitemId);
			if (pmstslotitem)
			{
				if (pmstslotitem->api_type.count() > 2)
				{
					type = pmstslotitem->api_type[2];
				}
				distance = pmstslotitem->api_distance;
			}
		}
		QString shipname;
		int shipid = -1;
		int shiplv = 0;
		foreach(const kcsapi_ship2& ship, pksd->portdata.api_ship)
		{
			foreach(int itemid, ship.api_slot)
			{
				if (itemid == v.api_id)
				{
					shipid = ship.api_ship_id;
					shiplv = ship.api_lv;
					break;
				}
			}
			if (shipid >= 0)
			{
				break;
			}
		}
		if (shipid >= 0)
		{
			auto pmstship = pkdc->findMstShipFromShipid(shipid);
			if (pmstship)
			{
				shipname = pmstship->api_name;
			}
		}

		MainWindow::weaponWindow()->addWeaponData(slotitemId, itemname, type, rare, bLocked, level, alv, shipname, shiplv, distance);
	}
	//

	qSort(_weaponGroupList.begin(), _weaponGroupList.end(), UIWeaponGroupData::higherThan);

	for (QList<KQUI_CollapsibleFrame*>::iterator it = _lstCollapsibleFrames.begin(); it != _lstCollapsibleFrames.end(); ++it)
	{
		delete *it;
	}
	_lstCollapsibleFrames.clear();

    int count = 0;
    foreach (auto& item, _weaponGroupList)
    {
        if (item.weapons.size())
        {
            buildSingleTable(item);
            count++;
        }
	}
	_needRebuildTable = false;
	_doNotRecordLast = false;

	if (!ui->lineEditTitle->text().isEmpty())
	{
		slotTextChanged(ui->lineEditTitle->text());
	}

	if (_lastToggledId >= 0)
	{
		foreach(auto pFrame, _lstCollapsibleFrames)
		{
			auto prop = pFrame->pushButton()->property(QPROPERTY_SLOTITEMID);
			if (prop.isValid())
			{
				bool bOk;
				int id = prop.toInt(&bOk);
				if (bOk && id == _lastToggledId)
				{
					QApplication::processEvents();
					ui->scrollArea->ensureWidgetVisible(pFrame);
				}
			}
		}
	}
}

void WeaponMainWindow::slotSetVisible(bool bValue)
{
	setVisible(bValue);
	if (bValue && _needRebuildTable)
	{
		buildTable();
	}
}

void WeaponMainWindow::buildSingleTable(const UIWeaponGroupData& groupData)
{
    auto pFrame = new KQUI_CollapsibleFrame(ui->scrollAreaWidgetContents);
    pFrame->setObjectName(QStringLiteral("Frame"));
    QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(pFrame->sizePolicy().hasHeightForWidth());
    pFrame->setSizePolicy(sizePolicy4);
    pFrame->setMinimumSize(QSize(0, 25));
    pFrame->setFrameShape(QFrame::StyledPanel);
    pFrame->setFrameShadow(QFrame::Raised);

    pFrame->tableWidget()->hide();
    pFrame->pushButton()->setStyleSheet("text-align: left;");
    connect(pFrame->tableWidget(), SIGNAL(sigTableSizeChanged()), this, SLOT(slotOnTableSizeChanged()));
	connect(pFrame->pushButton(), SIGNAL(toggled(bool)), pFrame->tableWidget(), SLOT(setVisible(bool)));
	connect(pFrame->pushButton(), SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));
	connect(pFrame->pushButton(), SIGNAL(toggled(bool)), pFrame, SLOT(slotResize(bool)));


    ui->verticalLayout->addWidget(pFrame);
    _lstCollapsibleFrames.push_back(pFrame);

    setWeaponColumnFormat(pFrame);

    int sparecount = 0;
    int sparenolock = 0;
	int maxsparenolocklevel = 0;
    QList<KQRowData> rows;

    QColor colWhite = QColor(255, 255, 255);
    QColor colYellow = QColor(255, 255, 0);
    QColor colRed = QColor(255, 0, 0);
	QColor colAqua = QColor(153, 255, 255);

	int colindex = COLINDEX_NORMAL;
	bool bRed = false;
	int lockedcount = 0;
    foreach (const UIWeaponData& it, groupData.weapons)
    {
		if (it.shipname.isEmpty())
		{
			sparecount++;
			if (!it.bLocked)
			{
				sparenolock++;
				if (colindex < COLINDEX_SPARENOLOCK)
				{
					colindex = COLINDEX_SPARENOLOCK;
				}
				if (it.level > maxsparenolocklevel)
				{
					maxsparenolocklevel = it.level;
				}
			}
		}
		KQRowData rd;
		QColor colLevel = colWhite;
		if (it.level >= 4)
		{
			colLevel = colYellow;
		}
		else if (it.level > 0)
		{
			colLevel = colAqua;
		}
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("星%1").arg(it.level), colLevel));
		QColor colAlv = colWhite;
		if (it.alv >= 7)
		{
			colAlv = colYellow;
		}
		else if (it.alv > 0)
		{
			colAlv = colAqua;
		}
		QColor colDist = colWhite;
		if (it.distance > 6)
		{
			colDist = colYellow;
		}
		else if (it.distance > 4)
		{
			colDist = colAqua;
		}
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("熟%1").arg(it.alv), colAlv));
		rd.appendCell(KQRowCellData(it.distance > 0? QString::fromLocal8Bit("距%1").arg(it.distance) : "", colDist));
        rd.appendCell(KQRowCellData(it.shipname));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("Lv.%1").arg(it.shiplv)));
        if (it.bLocked)
        {
            rd.appendCell(KQRowCellData(QString::fromLocal8Bit("鎖"), colYellow));
        }
        else
        {
            rd.appendCell(KQRowCellData(QString::fromLocal8Bit("空"), colRed));
			colindex = COLINDEX_ONNOLOCK;
        }
        rows.push_back(rd);
		if (it.level && !it.bLocked)
		{
			bRed = true;
		}
		else if (it.bLocked)
		{
			lockedcount++;
		}
    }
	if (sparecount == groupData.weapons.size() && !colindex)
	{
		colindex = COLINDEX_NOON;
	}
	if (lockedcount == groupData.weapons.size() && !colindex)
	{
		colindex = COLINDEX_ALLLOCKED;
	}
	/*
	if (sparenolock > 0)
	{
		KQRowData rd;
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("★%1").arg(maxsparenolocklevel), maxsparenolocklevel > 0 ? colYellow : colWhite));
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("-")));
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("%1").arg(sparenolock)));
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("空"), colRed));
		rows.push_back(rd);
	}
	*/
    QString title = QString::fromLocal8Bit("%2 ★%1 (空:%3/廃:%4/総:%5)")
            .arg(groupData.rare)
            .arg(groupData.name)
            .arg(sparecount)
            .arg(sparenolock)
            .arg(groupData.weapons.size());
    pFrame->pushButton()->setText(title);
	setButtonColor(pFrame, colindex, bRed);

	pFrame->pushButton()->setProperty(QPROPERTY_SLOTITEMID, groupData.id);

    pFrame->tableWidget()->updateFullTable(rows);

	if (_checkedIdList.contains(groupData.id))
	{
		pFrame->pushButton()->setChecked(true);
	}
	else
	{
		pFrame->pushButton()->setChecked(false);
	}

//	pFrame->pushButton()->setChecked(false);
}

void WeaponMainWindow::setWeaponColumnFormat(KQUI_CollapsibleFrame *pFrame)
{
    auto pTableWidget = pFrame->tableWidget();
    pTableWidget->setRowCount(0);
	pTableWidget->setColumnCount(6);
	pTableWidget->setColumnWidth(0, 40);
	pTableWidget->setColumnWidth(1, 40);
	pTableWidget->setColumnWidth(2, 40);
    pTableWidget->setColumnWidth(3, 120);
    pTableWidget->setColumnWidth(4, 60);
    pTableWidget->setColumnWidth(5, 18);
    pTableWidget->setSeparatorColumn(3);
}

void WeaponMainWindow::slotOnTableSizeChanged()
{
}

void WeaponMainWindow::slotUpdateTimer()
{
	/*
	static int count = 0;
	count++;
	
	if (count == 50)
	{
		needRebuildTable = true;
		buildTable();
	}
	*/

	if (isVisible() && !isMinimized())
	{
		auto mainWindow = MainWindow::mainWindow();
		if (mainWindow)
		{
			if (mainWindow->isSleepMode())
			{
				return;
			}
		}
//		this->adjustSize();
	}
}

void WeaponMainWindow::on_pbClose_clicked()
{
	close();
}

void WeaponMainWindow::on_pbMinimize_clicked()
{
    minimizeWindow();
}

void WeaponMainWindow::slotToggled(bool bValue)
{
	Q_UNUSED(bValue);

	auto senderObj = QObject::sender();
	auto propid = senderObj->property(QPROPERTY_SLOTITEMID);
	if (propid.isValid())
	{
		bool bOk;
		int id = propid.toInt(&bOk);
		if (bOk)
		{
			if (!_doNotRecordLast)
			{
				_lastToggledId = id;
			}
			if (bValue)
			{
				if (!_checkedIdList.contains(id))
				{
					_checkedIdList.push_back(id);
				}
			}
			else
			{ 
				_checkedIdList.removeAll(id);
			}
		}
	}
}

void WeaponMainWindow::setButtonColor(KQUI_CollapsibleFrame* pFrame, int colindex, bool bRed)
{
	
	static QString stylesheet_a[] =
	{
		"\
		QPushButton {   \
			color:white;    \
			text-align: left;\
		}   \
		",  // normal
		"\
			QPushButton {   \
			color:rgb(153, 255, 255);    \
			text-align: left;\
		}   \
		",  // have spare no lock
		"\
		QPushButton {   \
			color:rgb(255, 255, 0);    \
			text-align: left;\
		}   \
		",   // all locked
		"\
			QPushButton {   \
			color:rgb(255, 153, 0);    \
			text-align: left;\
		}   \
		",   // on but no lock
		"\
			QPushButton {   \
			color:rgb(180, 180, 180);    \
			text-align: left;\
		}   \
		"   // no on
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

	pFrame->pushButton()->setAutoFillBackground(true);
	pFrame->pushButton()->setStyleSheet(stylesheet_a[colindex] + stylesheet_b[bRed ? 1 : 0]);
}

void WeaponMainWindow::slotTextChanged(const QString& text)
{
	if (text.isEmpty())
	{
		foreach(auto pFrame, _lstCollapsibleFrames)
		{
			pFrame->setVisible(true);
		}
	}
	else
	{
		foreach(auto pFrame, _lstCollapsibleFrames)
		{
			if (pFrame->pushButton()->text().contains(text, Qt::CaseInsensitive))
			{
				pFrame->setVisible(true);
			}
			else
			{
				pFrame->setVisible(false);
			}
		}
	}
}

void WeaponMainWindow::onToggleSleepMode(bool bSleep)
{
	if (bSleep)
	{
		_pUpdateTimer->stop();
	}
	else
	{
		_pUpdateTimer->start(WEAPON_UPDATETIMER_INTERVAL);
	}
}
