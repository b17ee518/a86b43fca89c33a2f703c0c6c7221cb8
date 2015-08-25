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

class TimeRecord
{
public:
	TimeRecord(){}

    void setValue(qint64 dtime, qint64 ttime, QString cmt, bool bAlarm=false){desttime=dtime; totaltime=ttime; comment=cmt; alarmed=bAlarm;}
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
    explicit TimerMainWindow(QWidget *parent = 0);
    ~TimerMainWindow();

    void setExpeditionTime(int n, qint64 destms=-1, qint64 totalms=-1, QString comment=QString(), QColor col=QColor(0xff, 0xff, 0xff));
    void setRepairTime(int n, qint64 destms=-1, qint64 totalms=-1, QString comment=QString(), QColor col=QColor(0xff, 0xff, 0xff));
    void setBuildTime(int n, qint64 destms=-1, qint64 totalms=-1, QString comment=QString(), QColor col=QColor(0xff, 0xff, 0xff));

	void setTitle(const QString& title);

private slots:
    void on_pbClose_clicked();

    void on_pbMinimize_clicked();

    void slotUpdateTimer();

	void onToggleSleepMode(bool bSleep);

private:

    qint64 currentMS();
    QString getRemainTimeStr(qint64 ct, qint64 dt);
    QString getExpectedTimeStr(qint64 ct, qint64 dt);

    int getPercentage(qint64 tdiff, qint64 totalt);
    bool setPercentageFlag(QTableWidgetItem * pItem, int percentage, bool bYellow);
    bool isYellow(qint64 difftime);
    void setProgressColor(QProgressBar * pBar, qint64 tdiff, bool bYellow);

    bool updateDisplay(int &mintdiff, qint64 ct, qint64 dt, qint64 tt, QTableWidgetItem * pRemainItem, QTableWidgetItem * pExpectedItem=0, QProgressBar * pProgress=0, bool bMinusOne=false);

    void playSound(int i);

    void initTableItem();

    Ui::TimerMainWindow *ui;
    QList<QTableWidget *> lstTables;

    TimeRecord exptimerecord[3];
    TimeRecord repairtimerecord[4];
    TimeRecord buildtimerecord[2];

    QTimer * pUpdateTimer;

    QProgressBar * progressExpUs[3];
    QProgressBar * progressRepairUs[4];

    QMediaPlayer * pPlayer;
    QMediaPlaylist * pPlaylist;
};

#endif // TIMERMAINWINDOW_H
