#ifndef TIMERMAINWINDOW_H
#define TIMERMAINWINDOW_H

#include "submainwindow.h"
#include <QList>
#include <QTableWidget>
#include "kqtime.h"
#include <QTimer>
#include <QColor>
#include <QProgressBar>
#include <QMediaPlayer>
#include <QMediaPlaylist>

namespace Ui {
	class TimerMainWindow;
}

class TimeRecorder
{
public:
	TimeRecorder(){}

	void setValue(qint64 dtime, qint64 ttime, QString cmt, bool bAlarm = false)
	{
		desttime = dtime;
		totaltime = ttime;
		comment = cmt; alarmed = bAlarm;
	}
	void clear(){ desttime = -1; totaltime = 0; alarmed = false; }


	qint64 desttime = -1;
	qint64 totaltime = 0;
	QString comment;
	bool alarmed = false;
};

class TimerMainWindow : public SubMainWindow
{
	Q_OBJECT

public:

	enum class SoundIndex
	{
		Expedition = 0,
		Repair,
		Build,
		Terminated,
		Action,
		GoodNight,
	};

	explicit TimerMainWindow(QWidget *parent = 0);
	~TimerMainWindow();

	void setExpeditionTime(int n, qint64 destms = -1, qint64 totalms = -1,
		const QString& comment = QString(), const QColor& col = QColor(0xff, 0xff, 0xff));
	void setRepairTime(int n, qint64 destms = -1, qint64 totalms = -1,
		const QString& comment = QString(), const QColor& col = QColor(0xff, 0xff, 0xff));
	void setBuildTime(int n, qint64 destms = -1, qint64 totalms = -1,
		const QString& comment = QString(), const QColor& col = QColor(0xff, 0xff, 0xff));

	void setAutoRepairTime(bool bOn, bool bResetIfOver = false);

	void setTitle(const QString& title);
	QString getTitle();
	void playSound(SoundIndex i, bool bSilent = false);

	qint64 getMinExpeditionMS(int& team, QList<int>excludes = QList<int>());

	static qint64 currentMS();

	private slots:
	void on_pbClose_clicked();
	void on_pbMinimize_clicked();
	void slotUpdateTimer();
	void onToggleSleepMode(bool bSleep);

private:

	QString getRemainTimeStr(qint64 ct, qint64 dt);
	QString getExpectedTimeStr(qint64 ct, qint64 dt);
	QString getPassedTimeStr(qint64 ct, qint64 st);

	int getPercentage(qint64 tdiff, qint64 totalt);
	bool setPercentageFlag(QTableWidgetItem * pItem, int percentage, bool bYellow);
	bool isYellow(qint64 difftime);
	void setProgressColor(QProgressBar * pBar, qint64 tdiff, bool bYellow);

	bool updateDisplay(int &mintdiff, qint64 ct, qint64 dt, qint64 tt,
		QTableWidgetItem * pRemainItem, QTableWidgetItem * pExpectedItem = 0,
		QProgressBar * pProgress = 0, bool bMinusOne = false);

	void initTableItem();

	Ui::TimerMainWindow *ui;
	QList<QTableWidget *> _lstTables;

	TimeRecorder _exptimerecord[3];
	TimeRecorder _repairtimerecord[4];
	TimeRecorder _buildtimerecord[2];

	QTimer * _pUpdateTimer = NULL;

	QProgressBar * _progressExpUs[3];
	QProgressBar * _progressRepairUs[4];

	QMediaPlayer * _pPlayer = NULL;
	QMediaPlaylist * _pPlaylist = NULL;

	qint64 _autoRepairTimeBegin = 0;
	bool _autoRepairOn = false;
};

#endif // TIMERMAINWINDOW_H
