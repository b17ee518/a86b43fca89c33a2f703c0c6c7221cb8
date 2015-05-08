#include "WeaponMainWindow.h"
#include "ui_WeaponMainWindow.h"

#include "mainwindow.h"

#define QPROPERTY_SLOTITEMID	"slotitem_id"

#define COLINDEX_NORMAL			0
#define COLINDEX_SPARENOLOCK	1
#define COLINDEX_ALLLOCKED		2
#define COLINDEX_ONNOLOCK		3
#define COLINDEX_NOON			4

WeaponMainWindow::WeaponMainWindow(QWidget *parent)
: SubMainWindow(parent)
, ui(new Ui::WeaponMainWindow)
, needRebuildTable(true)
, lastToggledId(-1)
, doNotRecordLast(false)
{
	ui->setupUi(this);
	mwbPostInit();
	ui->titleFrame->setHandlingWidget(this);

	connect(ui->lineEditTitle, SIGNAL(textChanged(QString)), this, SLOT(slotTextChanged(QString)));

	pUpdateTimer = new QTimer(this);
	connect(pUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
	pUpdateTimer->start(50);

}

WeaponMainWindow::~WeaponMainWindow()
{
    delete ui;
}

void WeaponMainWindow::clearWeaponData()
{
    weaponGroupList.clear();
	needRebuildTable = true;
}

void WeaponMainWindow::addWeaponData(int slotitemId, QString itemname, int rare, bool bLocked, int level, QString shipname, int shiplv)
{
    UIWeaponData data;
    data.setData(bLocked, level, shipname, shiplv);
    bool bDone = false;
    for (QList<UIWeaponGroupData>::iterator it=weaponGroupList.begin(); it!=weaponGroupList.end(); ++it)
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
        group.setMetaData(slotitemId, itemname, rare);
        group.weapons.push_back(data);
        weaponGroupList.push_back(group);
	}
	needRebuildTable = true;
}

bool UIWeaponGroupData::higherThan(const UIWeaponGroupData& left, const UIWeaponGroupData& right)
{
	if (left.rare > right.rare)
	{
		return true;
	}
	if (left.id > right.id)
	{
		return true;
	}
	return false;
}

void WeaponMainWindow::buildTable()
{
	doNotRecordLast = true;
	qSort(weaponGroupList.begin(), weaponGroupList.end(), UIWeaponGroupData::higherThan);
	if (!isVisible() || !needRebuildTable)
	{
		return;
	}
	for (QList<KQUI_CollapsibleFrame*>::iterator it = lstCollapsibleFrames.begin(); it != lstCollapsibleFrames.end(); ++it)
	{
		delete *it;
	}
	lstCollapsibleFrames.clear();

    int count = 0;
    foreach (auto item, weaponGroupList)
    {
        if (item.weapons.size())
        {
            buildSingleTable(item);
            count++;
        }
	}
	needRebuildTable = false;
	doNotRecordLast = false;

	if (!ui->lineEditTitle->text().isEmpty())
	{
		slotTextChanged(ui->lineEditTitle->text());
	}

	if (lastToggledId >= 0)
	{
		foreach(auto pFrame, lstCollapsibleFrames)
		{
			auto prop = pFrame->pushButton()->property(QPROPERTY_SLOTITEMID);
			if (prop.isValid())
			{
				bool bOk;
				int id = prop.toInt(&bOk);
				if (bOk && id == lastToggledId)
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
	if (bValue && needRebuildTable)
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
	connect(pFrame->pushButton(), SIGNAL(toggled(bool)), pFrame, SLOT(slotResize(bool)));
	connect(pFrame->pushButton(), SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));


    ui->verticalLayout->addWidget(pFrame);
    lstCollapsibleFrames.push_back(pFrame);

    setWeaponColumnFormat(pFrame);

    int sparecount = 0;
    int sparenolock = 0;
	int maxsparenolocklevel = 0;
    QList<KQRowData> rows;

    QColor colWhite = QColor(255, 255, 255);
    QColor colYellow = QColor(255, 255, 0);
    QColor colRed = QColor(255, 0, 0);

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
        else
        {
            KQRowData rd;
            rd.appendCell(KQRowCellData(QString::fromLocal8Bit("★%1").arg(it.level), it.level>0?colYellow:colWhite));
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
		}
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

	if (sparenolock > 0)
	{
		KQRowData rd;
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("★%1").arg(maxsparenolocklevel), maxsparenolocklevel > 0 ? colYellow : colWhite));
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("-")));
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("%1").arg(sparenolock)));
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("空"), colRed));
		rows.push_back(rd);
	}

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

	if (checkedIdList.contains(groupData.id))
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
    pTableWidget->setColumnCount(4);
    pTableWidget->setColumnWidth(0, 40);
    pTableWidget->setColumnWidth(1, 120);
    pTableWidget->setColumnWidth(2, 60);
    pTableWidget->setColumnWidth(3, 18);
    pTableWidget->setSeparatorColumn(1);
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

	if (isVisible())
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

void WeaponMainWindow::on_pbClose_clicked()
{
	close();
}

void WeaponMainWindow::on_pbMinimize_clicked()
{
	setWindowState(Qt::WindowMinimized);
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
			if (!doNotRecordLast)
			{
				lastToggledId = id;
			}
			if (bValue)
			{
				if (!checkedIdList.contains(id))
				{
					checkedIdList.push_back(id);
				}
			}
			else
			{ 
				checkedIdList.removeAll(id);
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

void WeaponMainWindow::slotTextChanged(QString text)
{
	if (text.isEmpty())
	{
		foreach(auto pFrame, lstCollapsibleFrames)
		{
			pFrame->setVisible(true);
		}
	}
	else
	{
		foreach(auto pFrame, lstCollapsibleFrames)
		{
			if (pFrame->pushButton()->text().contains(text))
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