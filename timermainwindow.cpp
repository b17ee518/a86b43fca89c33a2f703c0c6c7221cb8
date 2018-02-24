#include "timermainwindow.h"
#include "ui_timermainwindow.h"
#include "kqtime.h"
#include "customtabledelegate.h"
#include <QDateTime>
#include "mainwindow.h"
#include <QSettings>
#include "ControlManager.h"

#define TIMER_UPDATETIMER_INTERVAL	250

TimerMainWindow::TimerMainWindow(QWidget *parent)
	: SubMainWindow(parent)
	, ui(new Ui::TimerMainWindow)
{
	ui->setupUi(this);
	mwbPostInit();
	ui->titleFrame->setHandlingWidget(this);

	_pPlayer = new QMediaPlayer();
	_pPlaylist = new QMediaPlaylist(_pPlayer);

	QString filename = QApplication::applicationDirPath();
	filename += "/se";
	_pPlaylist->addMedia(QUrl::fromLocalFile(filename + "/expedition.mp3"));
	_pPlaylist->addMedia(QUrl::fromLocalFile(filename + "/repair.mp3"));
	_pPlaylist->addMedia(QUrl::fromLocalFile(filename + "/build.mp3"));
	_pPlaylist->addMedia(QUrl::fromLocalFile(filename + "/terminated.mp3"));
	_pPlaylist->addMedia(QUrl::fromLocalFile(filename + "/action.mp3"));
	_pPlaylist->addMedia(QUrl::fromLocalFile(filename + "/goodnight.mp3"));
	_pPlaylist->setCurrentIndex(0);
	_pPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
	_pPlayer->setPlaylist(_pPlaylist);

	_pPlayer->setVolume(100);

	_progressExpUs[0] = ui->progressExp1u;
	_progressExpUs[1] = ui->progressExp2u;
	_progressExpUs[2] = ui->progressExp3u;

	_progressRepairUs[0] = ui->progressRepair1u;
	_progressRepairUs[1] = ui->progressRepair2u;
	_progressRepairUs[2] = ui->progressRepair3u;
	_progressRepairUs[3] = ui->progressRepair4u;

	_lstTables.append(ui->expeditionTable);
	_lstTables.append(ui->repairTable);
	_lstTables.append(ui->buildTable);
	_lstTables.append(ui->dropTable);

	initTableItem();

	_pUpdateTimer = new QTimer(this);
	_pUpdateTimer->setTimerType(Qt::PreciseTimer);
	connect(_pUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
	_pUpdateTimer->start(TIMER_UPDATETIMER_INTERVAL);
	connect(MainWindow::mainWindow(), SIGNAL(sigToggleSleepMode(bool)), this, SLOT(onToggleSleepMode(bool)));

	this->adjustSize();


}

TimerMainWindow::~TimerMainWindow()
{
	delete ui;
}

void TimerMainWindow::setExpeditionTime(int n, qint64 destms/*=-1*/, qint64 totalms/*=-1*/, const QString& comment/*=QString()*/, const QColor& col/*=QColor(0xff, 0xff, 0xff)*/)
{
	_exptimerecord[n].setValue(destms, totalms, comment, destms - currentMS() < 60000);
	QTableWidgetItem * pItem = ui->expeditionTable->item(n * 2 + 1, 1);
	if (destms < 0)
	{
		pItem->setText("");
	}
	else
	{
		pItem->setText(comment);
		pItem->setTextColor(col);
	}
	auto platformType = MainWindow::mainWindow()->getPlatformType();
	if (platformType == PlatformType::FastTablet || platformType == PlatformType::SlowTablet)
	{
		// output ini
		QSettings * setting = new QSettings("C:/KanAlarm/KanAlarm/bin/Release/AppX/KanPlayExpeditionTime.ini", QSettings::IniFormat);
		setting->setIniCodec("UTF-8");
		setting->setValue(QString::number(n), destms);

		delete setting;
	}
}

void TimerMainWindow::setRepairTime(int n, qint64 destms/*=-1*/, qint64 totalms/*=-1*/, const QString& comment/*=QString()*/, const QColor& col/*=QColor(0xff, 0xff, 0xff)*/)
{
	_repairtimerecord[n].setValue(destms, totalms, comment, destms - currentMS() < 60000);
	QTableWidgetItem * pItem = ui->repairTable->item(n * 2 + 1, 1);
	if (destms < 0)
	{
		pItem->setText("");
	}
	else
	{
		pItem->setText(comment);
		pItem->setTextColor(col);
	}
}

void TimerMainWindow::setBuildTime(int n, qint64 destms/*=-1*/, qint64 totalms/*=-1*/, const QString& comment/*=QString()*/, const QColor& col/*=QColor(0xff, 0xff, 0xff)*/)
{
	_buildtimerecord[n].setValue(destms, totalms, comment);
	QTableWidgetItem * pNameItem = ui->buildTable->item(n, 2);
	if (destms < 0)
	{
		pNameItem->setText("");
	}
	else
	{
		pNameItem->setText(comment);
		pNameItem->setTextColor(col);
	}

}

void TimerMainWindow::setLastDrop(const QString& displayName, const QColor& col/* = QColor(0xff, 0xff, 0xff)*/)
{
	QTableWidgetItem * pNameItem = ui->dropTable->item(0, 2);
	pNameItem->setText(displayName);
	pNameItem->setTextColor(col);
}


void TimerMainWindow::setAutoRepairTime(bool bOn, bool bResetIfOver/*=false*/)
{
	if (bOn)
	{
		auto ct = currentMS();
		_autoRepairOn = true;

		if (bResetIfOver)
		{
			if (ct - _autoRepairTimeBegin >= 20 * 60 * 1000)
			{
				_autoRepairTimeBegin = ct;
			}
		}
		else
		{
			_autoRepairTimeBegin = ct;
			_autoRepairOn = true;
			// hensei changed or started
		}
	}
	else
	{
		_autoRepairOn = false;
		auto title = getTitle();
		int index = title.indexOf("AR: ");
		if (index >= 0)
		{
			title = title.left(index).trimmed();
		}
		setTitle(title);
	}
}

void TimerMainWindow::setTitle(const QString& title)
{
	ui->lineEditTitle->setText(title);
}

QString TimerMainWindow::getTitle()
{
	return ui->lineEditTitle->text();
}

void TimerMainWindow::on_pbClose_clicked()
{
	close();
}

void TimerMainWindow::on_pbMinimize_clicked()
{
	minimizeWindow();
}

void TimerMainWindow::slotUpdateTimer()
{
	bool bRepaint = false;
	qint64 ct = currentMS();

	int mintdiff = 3600000;
	ProgressBarState progressbarstate = ProgressBarState::Normal;

	// exp
	for (int i = 0; i < 3; i++)
	{
		bool bUpdated = false;
		if (_exptimerecord[i].desttime >= 0)
		{
			qint64 dt = _exptimerecord[i].desttime;
			if (!_exptimerecord[i].alarmed && dt - ct < 60000)
			{
				playSound(SoundIndex::Expedition);
				_exptimerecord[i].alarmed = true;
			}
			bUpdated = updateDisplay(mintdiff
				, ct
				, dt
				, _exptimerecord[i].totaltime
				, ui->expeditionTable->item(i * 2, 1)
				, ui->expeditionTable->item(i * 2, 2)
				, _progressExpUs[i]
				, true
				);
			if (mintdiff <= 0)
			{
				progressbarstate = ProgressBarState::Stopped;
			}


		}
		else if (_exptimerecord[i].desttime == -1)
		{
			_exptimerecord[i].desttime = -2;
			bUpdated = true;
			updateDisplay(mintdiff
				, -1
				, 0
				, 0
				, ui->expeditionTable->item(i * 2, 1)
				, ui->expeditionTable->item(i * 2, 2)
				, _progressExpUs[i]
				, true
				);
		}
		if (!bRepaint && bUpdated)
		{
			bRepaint = true;
		}
	}
	if (bRepaint)
	{
		ui->expeditionTable->repaint();
		bRepaint = false;
	}

	// repair
	for (int i = 0; i < 4; i++)
	{
		bool bUpdated = false;
		if (_repairtimerecord[i].desttime >= 0)
		{
			qint64 dt = _repairtimerecord[i].desttime;
			if (!_repairtimerecord[i].alarmed && dt - ct < 60000)
			{
				playSound(SoundIndex::Repair);
				_repairtimerecord[i].alarmed = true;
			}
			bUpdated = updateDisplay(mintdiff
				, ct
				, dt
				, _repairtimerecord[i].totaltime
				, ui->repairTable->item(i * 2, 1)
				, ui->repairTable->item(i * 2, 2)
				, _progressRepairUs[i]
				, true
				);
			if (mintdiff <= 0 && progressbarstate == ProgressBarState::Normal)
			{
				progressbarstate = ProgressBarState::Paused;
			}
		}
		else if (_repairtimerecord[i].desttime == -1)
		{
			_repairtimerecord[i].desttime = -2;
			bUpdated = true;
			updateDisplay(mintdiff
				, -1
				, 0
				, 0
				, ui->repairTable->item(i * 2, 1)
				, ui->repairTable->item(i * 2, 2)
				, _progressRepairUs[i]
				, true
				);
		}
		if (!bRepaint && bUpdated)
		{
			bRepaint = true;
		}
	}
	if (bRepaint)
	{
		ui->repairTable->repaint();
		bRepaint = false;
	}

	int nomintdiff = 0;
	// build
	for (int i = 0; i < 2; i++)
	{
		bool bUpdated = false;
		if (_buildtimerecord[i].desttime >= 0)
		{
			qint64 dt = _buildtimerecord[i].desttime;
			if (!_buildtimerecord[i].alarmed && dt <= ct)
			{
				playSound(SoundIndex::Build);
				_buildtimerecord[i].alarmed = true;
			}
			bUpdated = updateDisplay(nomintdiff
				, ct
				, dt
				, _buildtimerecord[i].totaltime
				, ui->buildTable->item(i, 1)
				);
		}
		else if (_buildtimerecord[i].desttime == -1)
		{
			_buildtimerecord[i].desttime = -2;
			bUpdated = true;
			updateDisplay(nomintdiff
				, -1
				, 0
				, 0
				, ui->buildTable->item(i, 1)
				);
		}
		if (!bRepaint && bUpdated)
		{
			bRepaint = true;
		}
	}
	if (bRepaint)
	{
		ui->buildTable->repaint();
		bRepaint = false;
	}

	if (mintdiff < 0)
	{
		mintdiff = 0;
	}
	int processpercentage = getPercentage(mintdiff, 3600000);
	MainWindow::mainWindow()->SetProgressBarPos(processpercentage, progressbarstate);

	// auto repair timer
	QString titlepre = "";
	if (ControlManager::getInstance().isRunning())
	{
		titlepre = getTitle();
		int index = titlepre.indexOf("AR: ");
		if (index >= 0)
		{
			titlepre = titlepre.left(index);
			if (!titlepre.isEmpty() && !titlepre.endsWith(" "))
			{
				titlepre += " ";
			}
		}
	}
	if (_autoRepairOn)
	{
		setTitle(titlepre + "AR: " + getPassedTimeStr(ct, _autoRepairTimeBegin));
	}
}

qint64 TimerMainWindow::currentMS()
{
	QDateTime dt = QDateTime::currentDateTimeUtc();

	qint64 ct = dt.toTime_t();
	ct *= 1000;
	int ms = dt.time().msec();
	ct += ms;
	return ct;
}

QString TimerMainWindow::getRemainTimeStr(qint64 ct, qint64 dt)
{
	if (ct >= dt)
	{
		dt = ct;
	}

	KQTime difftime;
	difftime.setTotalMS(dt - ct);
	return difftime.toString();
}

QString TimerMainWindow::getExpectedTimeStr(qint64 ct, qint64 dt)
{
	uint utc = dt / 1000;
	QDateTime expectedtime = QDateTime::fromTime_t(utc).toLocalTime();
	QString expectedstr = expectedtime.toString("HH : mm : ss");

	int utcnow = ct / 1000;
	QDateTime nowtime = QDateTime::fromTime_t(utcnow).toLocalTime();
	int daydiff = nowtime.daysTo(expectedtime);
	if (daydiff > 0)
	{
		expectedstr.sprintf("%s (+%d)", expectedstr.toLocal8Bit().constData(), daydiff);
	}
	else if (daydiff < 0)
	{
		expectedstr.sprintf("%s (%d)", expectedstr.toLocal8Bit().constData(), daydiff);
	}
	return expectedstr;
}

QString TimerMainWindow::getPassedTimeStr(qint64 ct, qint64 st)
{
	QTime passedtime = QTime::fromMSecsSinceStartOfDay(ct - st);
	QString passedstr = passedtime.toString("HH : mm : ss");

	return passedstr;
}

int TimerMainWindow::getPercentage(qint64 tdiff, qint64 totalt)
{
	if (tdiff < 0)
	{
		return 10000;
	}
	if (tdiff > totalt)
	{
		return 0;
	}
	int percentage = (totalt - tdiff) * 10000 / totalt;
	return percentage;
}

bool TimerMainWindow::setPercentageFlag(QTableWidgetItem *pItem, int percentage, bool bYellow)
{
	int d = pItem->data(Qt::UserRole).toInt();
	int oldd = d;
	int flag = d % 100;

	d = flag + percentage * 100 + (bYellow ? 10000000 : 0);

	if (oldd != d)
	{
		pItem->setData(Qt::UserRole, d);
		if (oldd / 10 != d / 10)
		{
			return true;
		}
	}
	return false;
}

bool TimerMainWindow::isYellow(qint64 difftime)
{
	if (difftime <= 3600000)
	{
		return true;
	}
	return false;
}

void TimerMainWindow::setProgressColor(QProgressBar *pBar, qint64 tdiff, bool bYellow)
{
	static QString stylesheet[3] =
	{
		"QProgressBar { background-color: grey; border: 0px; border-left: 1px solid black; border-radius: 0px; } QProgressBar::chunk { background-color: rgb(255, 153, 0); }",
		"QProgressBar { background-color: grey; border: 0px; border-left: 1px solid black; border-radius: 0px; } QProgressBar::chunk { background-color: rgb(255, 255, 0); }",
		"QProgressBar { background-color: grey; border: 0px; border-left: 1px solid black; border-radius: 0px; } QProgressBar::chunk { background-color: rgb(0, 255, 0); }"
	};
	int stylei;
	if (bYellow)
	{
		if (tdiff <= 0)
		{
			stylei = 2;
		}
		else
		{
			stylei = 1;
		}
	}
	else
	{
		stylei = 0;
	}

	int i = (uintptr_t)pBar->userData(Qt::UserRole);
	if (i != stylei)
	{
		pBar->setUserData(Qt::UserRole, (QObjectUserData*)i);
		pBar->setStyleSheet(stylesheet[stylei]);
	}
}

bool TimerMainWindow::updateDisplay(int &mintdiff, qint64 ct, qint64 dt, qint64 tt, QTableWidgetItem *pRemainItem, QTableWidgetItem *pExpectedItem, QProgressBar *pProgress, bool bMinusOne)
{
	auto mainWindow = MainWindow::mainWindow();
	if (mainWindow)
	{
		if (mainWindow->isSleepMode())
		{
			return false;
		}
	}

	if (ct < 0)
	{
		pRemainItem->setText("-- : -- : --");
		setPercentageFlag(pRemainItem, 0, false);
		if (pExpectedItem)
		{
			pExpectedItem->setText("-- : -- : --");
		}
		if (pProgress)
		{
			pProgress->setValue(0);
		}
		return true;
	}

	QString strRemain = getRemainTimeStr(ct, dt);
	pRemainItem->setText(strRemain);

	if (pExpectedItem)
	{
		QString strExpected = getExpectedTimeStr(ct, dt);
		pExpectedItem->setText(strExpected);
	}

	if (bMinusOne)
	{
		dt -= 60000;
	}
	qint64 tdiff = dt - ct;
	int percentage = getPercentage(tdiff, tt);
	bool bYellow = isYellow(tdiff);
	if (bYellow && tdiff < mintdiff)
	{
		if (tdiff < 0)
		{
			mintdiff = 0;
		}
		else
		{
			mintdiff = tdiff;
		}
	}

	bool bUpdated = setPercentageFlag(pRemainItem, percentage, bYellow);

	if (pProgress)
	{
		pProgress->setValue(percentage / 100);
		setProgressColor(pProgress, tdiff, bYellow);
	}

	return bUpdated;
}

void TimerMainWindow::playSound(SoundIndex i, bool bSilent/*=false*/)
{
	if (!((ControlManager::getInstance().isExpeditionMode() || ControlManager::getInstance().isInactiveWaiting()) && i == SoundIndex::Expedition) && !bSilent)
	{
		MainWindow::mainWindow()->AdjustVolume(75);
	}
	_pPlaylist->setCurrentIndex((int)i);
	_pPlayer->play();
	//	qint64 dur = pPlayer->duration();
	int dur = 3000;
	QTimer::singleShot(dur, MainWindow::mainWindow(), SLOT(slotSoundEnded()));
}

qint64 TimerMainWindow::getMinExpeditionMS(int& team, QList<int>excludes/*=QList<int>()*/)
{
	qint64 minTime = std::numeric_limits<qint64>::max();
	qint64 ct = currentMS();
	int minTimeTeam = -1;

	for (int i = 0; i < 3; i++)
	{
		if (excludes.contains(i + 1))
		{
			continue;
		}

		qint64 testTime = _exptimerecord[i].desttime - 1000 * 60 - ct;
		if (minTime > 0)
		{
			if (testTime < minTime)
			{
				minTime = testTime;
				minTimeTeam = i + 1;
			}
		}
		else
		{
			if (testTime < 0 && testTime > minTime)
			{
				minTime = testTime;
				minTimeTeam = i + 1;
			}
		}
	}
	if (minTime < 0)
	{
		minTime = 0;
	}
	team = minTimeTeam;
	return minTime;
}

void TimerMainWindow::initTableItem()
{
	QTableWidget * pTable;
	foreach(pTable, _lstTables)
	{
		pTable->setFont(QApplication::font());
		pTable->horizontalHeader()->hide();
		pTable->horizontalHeader()->setDefaultSectionSize(20);
		pTable->verticalHeader()->hide();
		pTable->verticalHeader()->setDefaultSectionSize(20);
		pTable->setFocusPolicy(Qt::NoFocus);
		for (int i = 0; i < pTable->rowCount(); i++)
		{
			for (int j = 0; j < pTable->columnCount(); j++)
			{
				QTableWidgetItem * pItem = new QTableWidgetItem();
				pTable->setItem(i, j, pItem);
				pTable->item(i, j)->setData(Qt::UserRole, 0);
			}
		}
		pTable->setColumnWidth(0, 24);
		pTable->setColumnWidth(1, 120);
		pTable->setColumnWidth(2, 120);
		pTable->setItemDelegate(new CustomTableDelegate(pTable));

	}

	for (int i = 0; i < 3; i++)
	{
		_progressExpUs[i]->setUserData(Qt::UserRole, (QObjectUserData*)-1);
		_progressRepairUs[i]->setUserData(Qt::UserRole, (QObjectUserData*)-1);
	}
	_progressRepairUs[3]->setUserData(Qt::UserRole, (QObjectUserData*)-1);


	ui->expeditionTable->setSpan(0, 0, 2, 1);
	ui->expeditionTable->setSpan(1, 1, 1, 2);
	ui->expeditionTable->setSpan(2, 0, 2, 1);
	ui->expeditionTable->setSpan(3, 1, 1, 2);
	ui->expeditionTable->setSpan(4, 0, 2, 1);
	ui->expeditionTable->setSpan(5, 1, 1, 2);

	ui->repairTable->setSpan(0, 0, 2, 1);
	ui->repairTable->setSpan(1, 1, 1, 2);
	ui->repairTable->setSpan(2, 0, 2, 1);
	ui->repairTable->setSpan(3, 1, 1, 2);
	ui->repairTable->setSpan(4, 0, 2, 1);
	ui->repairTable->setSpan(5, 1, 1, 2);
	ui->repairTable->setSpan(6, 0, 2, 1);
	ui->repairTable->setSpan(7, 1, 1, 2);

	ui->expeditionTable->item(0, 0)->setData(Qt::UserRole, CustomTableDelegate_Right | CustomTableDelegate_Bottom);
	ui->expeditionTable->item(2, 0)->setData(Qt::UserRole, CustomTableDelegate_Right | CustomTableDelegate_Bottom);
	ui->expeditionTable->item(4, 0)->setData(Qt::UserRole, CustomTableDelegate_Right);

	ui->expeditionTable->item(1, 1)->setData(Qt::UserRole, CustomTableDelegate_Bottom);
	ui->expeditionTable->item(3, 1)->setData(Qt::UserRole, CustomTableDelegate_Bottom);
	ui->expeditionTable->item(5, 1)->setData(Qt::UserRole, CustomTableDelegate_Bottom);


	ui->repairTable->item(0, 0)->setData(Qt::UserRole, CustomTableDelegate_Right | CustomTableDelegate_Bottom);
	ui->repairTable->item(2, 0)->setData(Qt::UserRole, CustomTableDelegate_Right | CustomTableDelegate_Bottom);
	ui->repairTable->item(4, 0)->setData(Qt::UserRole, CustomTableDelegate_Right | CustomTableDelegate_Bottom);
	ui->repairTable->item(6, 0)->setData(Qt::UserRole, CustomTableDelegate_Right);

	ui->repairTable->item(1, 1)->setData(Qt::UserRole, CustomTableDelegate_Bottom);
	ui->repairTable->item(3, 1)->setData(Qt::UserRole, CustomTableDelegate_Bottom);
	ui->repairTable->item(5, 1)->setData(Qt::UserRole, CustomTableDelegate_Bottom);

	ui->buildTable->item(0, 0)->setData(Qt::UserRole, CustomTableDelegate_Right | CustomTableDelegate_Bottom);
	ui->buildTable->item(1, 0)->setData(Qt::UserRole, CustomTableDelegate_Right);

	ui->buildTable->item(0, 1)->setData(Qt::UserRole, CustomTableDelegate_Bottom);
	ui->buildTable->item(0, 2)->setData(Qt::UserRole, CustomTableDelegate_Bottom);

	ui->dropTable->item(0, 0)->setData(Qt::UserRole, CustomTableDelegate_Right | CustomTableDelegate_Bottom);
	ui->dropTable->item(0, 1)->setData(Qt::UserRole, CustomTableDelegate_Bottom);


	ui->expeditionTable->item(0, 0)->setText(QString::fromLocal8Bit("②"));
	ui->expeditionTable->item(2, 0)->setText(QString::fromLocal8Bit("③"));
	ui->expeditionTable->item(4, 0)->setText(QString::fromLocal8Bit("④"));

	ui->repairTable->item(0, 0)->setText(QString::fromLocal8Bit("①"));
	ui->repairTable->item(2, 0)->setText(QString::fromLocal8Bit("②"));
	ui->repairTable->item(4, 0)->setText(QString::fromLocal8Bit("③"));
	ui->repairTable->item(6, 0)->setText(QString::fromLocal8Bit("④"));

	ui->buildTable->item(0, 0)->setText(QString::fromLocal8Bit("①"));
	ui->buildTable->item(1, 0)->setText(QString::fromLocal8Bit("②"));

	ui->dropTable->item(0, 0)->setText(QString::fromLocal8Bit("D"));

}

void TimerMainWindow::onToggleSleepMode(bool bSleep)
{
	if (bSleep)
	{
		_pUpdateTimer->stop();
	}
	else
	{
		_pUpdateTimer->start(TIMER_UPDATETIMER_INTERVAL);
	}
}
