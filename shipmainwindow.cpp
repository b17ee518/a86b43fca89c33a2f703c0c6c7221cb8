#include "ShipMainWindow.h"
#include "ui_ShipMainWindow.h"

#include "mainwindow.h"
#include "kandatacalc.h"
#include "kandataconnector.h"
#include "kansavedata.h"

#define QPROPERTY_SLOTITEMID	"slotitem_id"

#define COLINDEX_NORMAL			0
#define COLINDEX_SOMEKIRA		1
#define COLINDEX_LOTSKIRA		2
#define COLINDEX_SELDOMKIRA	3
#define COLINDEX_NOKIRA			4

#define SHIP_UPDATETIMER_INTERVAL	50

ShipMainWindow::ShipMainWindow(QWidget *parent)
: SubMainWindow(parent)
, ui(new Ui::ShipMainWindow)
, _needRebuildTable(true)
, _lastToggledId(-1)
, _doNotRecordLast(false)
{
	ui->setupUi(this);
	mwbPostInit();
	ui->titleFrame->setHandlingWidget(this);

	connect(ui->lineEditTitle, SIGNAL(textChanged(QString)), this, SLOT(slotTextChanged(QString)));

	_pUpdateTimer = new QTimer(this);
	connect(_pUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
	_pUpdateTimer->start(SHIP_UPDATETIMER_INTERVAL);
	connect(MainWindow::mainWindow(), SIGNAL(sigToggleSleepMode(bool)), this, SLOT(onToggleSleepMode(bool)));
}

ShipMainWindow::~ShipMainWindow()
{
    delete ui;
}

void ShipMainWindow::clearShipData()
{
    _shipGroupList.clear();
	_needRebuildTable = true;
}

void ShipMainWindow::addShipData(int stype, const QString& stypename, int team, bool bExpedition, const QString& name, int lv, int cond, bool bNeedCharge, bool bNeedRepair, int drumCount, int sortnum, int shipid)
{
    UIShipData data;
    data.setData(team, bExpedition, name, lv, cond, bNeedCharge, bNeedRepair, drumCount, sortnum, shipid);
    bool bDone = false;
    for (QList<UIShipGroupData>::iterator it=_shipGroupList.begin(); it!=_shipGroupList.end(); ++it)
    {
        if (it->stype == stype)
        {
            it->ships.push_back(data);
            bDone = true;
            break;
        }
    }
    if (!bDone)
    {
        UIShipGroupData group;
        group.setMetaData(stype, stypename);
        group.ships.push_back(data);
        _shipGroupList.push_back(group);
	}
	_needRebuildTable = true;
}

bool UIShipGroupData::higherThan(const UIShipGroupData& left, const UIShipGroupData& right)
{
	if (left.stype < right.stype)
	{
		return true;
	}
	return false;
}

bool UIShipData::higherThan(const UIShipData& left, const UIShipData& right)
{
	if (left.sortnum < right.sortnum)
	{
		return true;
	}
	if (left.sortnum == right.sortnum)
	{
		if (left.shipid < right.shipid)
		{
			return true;
		}
	}
	return false;
}


void ShipMainWindow::buildTable()
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

	foreach (auto& ship, pksd->portdata.api_ship)
	{
		auto pmstship = pkdc->findMstShipFromShipid(ship.api_ship_id);
		QString shiptypename = pkdc->findMstShipTypeNameFromSType(pmstship->api_stype);
		int team = -1;
		bool bExpedition = false;
		bool bNeedCharge = false;
		bool bNeedRepair = false;
		int drumcount = 0;
		
		foreach (auto& deck, pksd->portdata.api_deck_port)
		{
			foreach (auto shipno, deck.api_ship)
			{
				if (ship.api_id == shipno)
				{
					team = deck.api_id;
					if (deck.api_mission[0] > 0)
					{
						bExpedition = true;
					}
				}
			}
		}
		if (ship.api_fuel < pmstship->api_fuel_max || ship.api_bull < pmstship->api_bull_max)
		{
			bNeedCharge = true;
		}
		if (ship.api_nowhp < ship.api_maxhp)
		{
			bNeedRepair = true;
		}

		for (int i = 0; i < ship.api_slot.count(); i++)
		{
			int slotitemid = ship.api_slot[i];
			foreach(const kcsapi_slotitem &v, pksd->slotitemdata)
			{
				if (v.api_id == slotitemid)
				{
					const kcsapi_mst_slotitem* pmstslotitem = pkdc->findMstSlotItemFromSlotitemid(v.api_slotitem_id);
					if (pmstslotitem)
					{
						if (pmstslotitem->api_type.count() > 2)
						{
							int type = pmstslotitem->api_type[2];
							
							// drum
							if (type == (int)SlotitemType::YuSou)
							{
								drumcount++;
							}
						}
					}
				}
			}
		}

		addShipData(pmstship->api_stype, shiptypename, team, bExpedition, pmstship->api_name, ship.api_lv, ship.api_cond, bNeedCharge, bNeedRepair, drumcount, pmstship->api_sortno, ship.api_id);
	}
	//

	qSort(_shipGroupList.begin(), _shipGroupList.end(), UIShipGroupData::higherThan);
	int shiptypecount = _shipGroupList.count();
	for (int i = 0; i < shiptypecount; i++)
	{
		qSort(_shipGroupList[i].ships.begin(), _shipGroupList[i].ships.end(), UIShipData::higherThan);
	}

	for (QList<KQUI_CollapsibleFrame*>::iterator it = _lstCollapsibleFrames.begin(); it != _lstCollapsibleFrames.end(); ++it)
	{
		delete *it;
	}
	_lstCollapsibleFrames.clear();

    int count = 0;
    foreach (auto& item, _shipGroupList)
    {
        if (item.ships.size())
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

void ShipMainWindow::slotSetVisible(bool bValue)
{
	setVisible(bValue);
	if (bValue && _needRebuildTable)
	{
		buildTable();
	}
}

void ShipMainWindow::buildSingleTable(const UIShipGroupData& groupData)
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
	connect(pFrame->pushButton(), SIGNAL(toggled(bool)), pFrame, SLOT(slotResize(bool)));
	connect(pFrame->pushButton(), SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));


    ui->verticalLayout->addWidget(pFrame);
    _lstCollapsibleFrames.push_back(pFrame);

    setShipColumnFormat(pFrame);

	int kiracount = 0;
	int usablekiracount = 0;
    QList<KQRowData> rows;

    QColor colWhite = QColor(255, 255, 255);
    QColor colYellow = QColor(255, 255, 0);
    QColor colRed = QColor(255, 0, 0);
	QColor colGray = QColor(180, 180, 180);

	int colIndex = COLINDEX_NORMAL;
	bool bRed = false;

    foreach (const UIShipData& it, groupData.ships)
	{
		bool bKira = false;
		QColor mainColor = getMainColor(it);
		QColor condColor = getCondColor(it, &bKira);

		KQRowData rd;
		if (it.team >= 0)
		{
			rd.appendCell(KQRowCellData(QString::fromLocal8Bit("%1").arg(it.team), it.bExpedition ? colGray : colWhite));
		}
		else
		{
			rd.appendCell(KQRowCellData(QString::fromLocal8Bit("")));
		}
		rd.appendCell(KQRowCellData(it.name, mainColor));
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("Lv.%1").arg(it.lv), condColor));
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("(%1)").arg(it.cond), condColor));
		if (it.bNeedRepair)
		{
			rd.appendCell(KQRowCellData(QString::fromLocal8Bit("修"), colRed));
		}
		else if (it.bNeedCharge)
		{
			rd.appendCell(KQRowCellData(QString::fromLocal8Bit("補"), colRed));
		}
		else
		{
			rd.appendCell(KQRowCellData(QString::fromLocal8Bit("")));
		}

		if (it.drumCount)
		{
			rd.appendCell(KQRowCellData(QString::fromLocal8Bit("缶:%1").arg(it.drumCount)));
		}
		else
		{
			rd.appendCell(KQRowCellData(QString::fromLocal8Bit("")));
		}
		rows.push_back(rd);

		if (bKira)
		{
			kiracount++;
			if (!it.bExpedition && !it.bNeedCharge && !it.bNeedRepair)
			{
				usablekiracount++;
			}
		}

		if (it.bNeedCharge && it.lv > 2)
		{
			bRed = true;
		}
    }

	int totalCount = groupData.ships.size();
	if (totalCount)
	{
		float kiraval = (float)kiracount / (float)totalCount;
		float usableval = (float)usablekiracount / (float)totalCount;
		if (!usablekiracount || !kiracount)
		{
			colIndex = COLINDEX_NOKIRA;
		}
		else if (kiraval < 1.0f/8.0f || usableval < 1.0f/10.0f)
		{
			colIndex = COLINDEX_SELDOMKIRA;
		}
		else if (kiraval >= 1.0f/2.0f  || usableval >= 1.0f/3.0f)
		{
			colIndex = COLINDEX_LOTSKIRA;
		}
		else if (kiraval >= 1.0f/3.0f || usableval >= 1.0f/4.0f)
		{
			colIndex = COLINDEX_SOMEKIRA;
		}
	}
	
    QString title = QString::fromLocal8Bit("%1 (キラ:%2/使えるキラ:%3/総:%4)")
		.arg(groupData.stypename)
		.arg(kiracount)
		.arg(usablekiracount)
		.arg(totalCount);
	pFrame->pushButton()->setText(title);
	setButtonColor(pFrame, colIndex, bRed);

	pFrame->pushButton()->setProperty(QPROPERTY_SLOTITEMID, groupData.stype);

    pFrame->tableWidget()->updateFullTable(rows);

	if (_checkedIdList.contains(groupData.stype))
	{
		pFrame->pushButton()->setChecked(true);
	}
	else
	{
		pFrame->pushButton()->setChecked(false);
	}

//	pFrame->pushButton()->setChecked(false);
}

void ShipMainWindow::setShipColumnFormat(KQUI_CollapsibleFrame *pFrame)
{
    auto pTableWidget = pFrame->tableWidget();
    pTableWidget->setRowCount(0);
    pTableWidget->setColumnCount(6);
    pTableWidget->setColumnWidth(0, 18);		// team
	pTableWidget->setColumnWidth(1, 120);	// name
	pTableWidget->setColumnWidth(2, 60);		// lv
	pTableWidget->setColumnWidth(3, 50);		// cond
	pTableWidget->setColumnWidth(4, 18);		// charge/HP
	pTableWidget->setColumnWidth(5, 80);		// drum
    pTableWidget->setSeparatorColumn(2);
}

QColor ShipMainWindow::getMainColor(const UIShipData& ship)
{
	if (ship.bNeedCharge || ship.bNeedRepair)
	{
		return QColor(255, 0, 0);
	}
	if (ship.bExpedition)
	{
		return QColor(180, 180, 180);
	}
	return getCondColor(ship);
}

QColor ShipMainWindow::getCondColor(const UIShipData& ship, bool* pbKira/*=NULL*/)
{
	CondState condstate = KanDataCalc::GetCondState(ship.cond);
	QColor col;
	if (pbKira)
	{
		*pbKira = false;
	}
	switch (condstate)
	{
	case CondState::Normal:
		col = QColor(255, 255, 255);
		break;
	case CondState::Small:
		col = QColor(192, 192, 192);
		break;
	case CondState::Middle:
		col = QColor(255, 153, 0);
		break;
	case CondState::Big:
		col = QColor(255, 0, 0);
		break;
	case CondState::Kira:
		col = QColor(255, 255, 0);
		if (pbKira)
		{
			*pbKira = true;
		}
		break;
	}
	return col;
}

void ShipMainWindow::slotOnTableSizeChanged()
{
}

void ShipMainWindow::slotUpdateTimer()
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
		this->adjustSize();
	}
}

void ShipMainWindow::on_pbClose_clicked()
{
	close();
}

void ShipMainWindow::on_pbMinimize_clicked()
{
	setWindowState(Qt::WindowMinimized);
}

void ShipMainWindow::slotToggled(bool bValue)
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


void ShipMainWindow::slotTextChanged(const QString& text)
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
			pFrame->setVisible(false);
			if (pFrame->pushButton()->text().contains(text, Qt::CaseInsensitive))
			{
				pFrame->setVisible(true);
			}
			else
			{
				auto table = pFrame->tableWidget();
				int rowcount = table->rowCount();
				for (int i = 0; i < rowcount; i++)
				{
					auto pitem = table->item(i, 1);
					if (pitem)
					{
						if (pitem->text().contains(text, Qt::CaseInsensitive))
						{
							pFrame->setVisible(true);
						}
					}
				}
			}
		}
	}
}

void ShipMainWindow::onToggleSleepMode(bool bSleep)
{
	if (bSleep)
	{
		_pUpdateTimer->stop();
	}
	else
	{
		_pUpdateTimer->start(SHIP_UPDATETIMER_INTERVAL);
	}
}

void ShipMainWindow::setButtonColor(KQUI_CollapsibleFrame* pFrame, int colindex, bool bRed)
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
		",  // 1/4 usable or 1/3 kira
		"\
		QPushButton {   \
			color:rgb(255, 255, 0);    \
			text-align: left;\
		}   \
		",   // 1/3 usable or 1/2 kira
		"\
			QPushButton {   \
			color:rgb(255, 153, 0);    \
			text-align: left;\
		}   \
		",   // lower than 1/10 usable or 1/8 kira
		"\
			QPushButton {   \
			color:rgb(180, 180, 180);    \
			text-align: left;\
		}   \
		"   // no kira
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