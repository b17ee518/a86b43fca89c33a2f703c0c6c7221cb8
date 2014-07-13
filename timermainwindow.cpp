#include "timermainwindow.h"
#include "ui_timermainwindow.h"
#include "kqtime.h"
#include "customtabledelegate.h"
#include <QDateTime>

TimerMainWindow::TimerMainWindow(QWidget *parent) :
    SubMainWindow(parent),
    ui(new Ui::TimerMainWindow)
{
    ui->setupUi(this);
    mwbPostInit();
    ui->titleFrame->setHandlingWidget(this);

    pPlayer = new QMediaPlayer();
    pPlaylist = new QMediaPlaylist(pPlayer);

    QString filename = QApplication::applicationDirPath();
    pPlaylist->addMedia(QUrl::fromLocalFile(filename+"/expedition.mp3"));
    pPlaylist->addMedia(QUrl::fromLocalFile(filename+"/repair.mp3"));
    pPlaylist->addMedia(QUrl::fromLocalFile(filename+"/build.mp3"));
    pPlaylist->setCurrentIndex(0);
    pPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    pPlayer->setPlaylist(pPlaylist);

    pPlayer->setVolume(50);

    progressExpUs[0] = ui->progressExp1u;
    progressExpUs[1] = ui->progressExp2u;
    progressExpUs[2] = ui->progressExp3u;
    progressExpUs[3] = ui->progressExp4u;

    progressRepairUs[0] = ui->progressRepair1u;
    progressRepairUs[1] = ui->progressRepair2u;
    progressRepairUs[2] = ui->progressRepair3u;
    progressRepairUs[3] = ui->progressRepair4u;

    lstTables.append(ui->expeditionTable);
    lstTables.append(ui->repairTable);
    lstTables.append(ui->buildTable);

    initTableItem();

    pUpdateTimer = new QTimer(this);
    connect(pUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
    pUpdateTimer->start(250);

//    this->adjustSize();


}

TimerMainWindow::~TimerMainWindow()
{
    delete ui;
}

void TimerMainWindow::setExpeditionTime(int n, qint64 destms, qint64 totalms, QString comment, QColor col)
{
    exptimerecord[n].setValue(destms, totalms, comment, col, destms-currentMS()<60000);
    QTableWidgetItem * pItem = ui->expeditionTable->item(n*2+1, 1);
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

void TimerMainWindow::setRepairTime(int n, qint64 destms, qint64 totalms, QString comment, QColor col)
{
    repairtimerecord[n].setValue(destms, totalms, comment, col, destms-currentMS()<60000);
    QTableWidgetItem * pItem = ui->repairTable->item(n*2+1, 1);
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

void TimerMainWindow::setbuildTime(int n, qint64 destms, qint64 totalms, QString comment, QColor col)
{
    buildtimerecord[n].setValue(destms, totalms, comment, col);
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


void TimerMainWindow::on_pbClose_clicked()
{
    close();
}

void TimerMainWindow::on_pbMinimize_clicked()
{
    setWindowState(Qt::WindowMinimized);
}

void TimerMainWindow::slotUpdateTimer()
{
    bool bRepaint = false;
    qint64 ct = currentMS();

    // exp
    for (int i=0; i<4; i++)
    {
        if (exptimerecord[i].desttime >= 0)
        {
            qint64 dt = exptimerecord[i].desttime;
            if (!exptimerecord[i].alarmed && dt - ct < 60000)
            {
                playSound(0);
                exptimerecord[i].alarmed = true;
            }
            bool bUpdated = updateDisplay(ct
                          , dt
                          , exptimerecord[i].totaltime
                          , ui->expeditionTable->item(i*2, 1)
                          , ui->expeditionTable->item(i*2, 2)
                          , progressExpUs[i]
                          );


            if (!bRepaint && bUpdated)
            {
                bRepaint = true;
            }
        }
    }
    if (bRepaint)
    {
        ui->expeditionTable->repaint();
        bRepaint = false;
    }

    // repair
    for (int i=0; i<4; i++)
    {
        if (repairtimerecord[i].desttime >= 0)
        {
            qint64 dt = repairtimerecord[i].desttime;
            if (!repairtimerecord[i].alarmed && dt - ct < 60000)
            {
                playSound(1);
                repairtimerecord[i].alarmed = true;
            }
            bool bUpdated = updateDisplay(ct
                          , dt
                          , repairtimerecord[i].totaltime
                          , ui->repairTable->item(i*2, 1)
                          , ui->repairTable->item(i*2, 2)
                          , progressRepairUs[i]
                          );


            if (!bRepaint && bUpdated)
            {
                bRepaint = true;
            }
        }
    }
    if (bRepaint)
    {
        ui->repairTable->repaint();
        bRepaint = false;
    }

    // build
    for (int i=0; i<2; i++)
    {
        if (buildtimerecord[i].desttime >= 0)
        {
            qint64 dt = buildtimerecord[i].desttime;
            if (!buildtimerecord[i].alarmed && dt <= ct)
            {
                playSound(2);
                buildtimerecord[i].alarmed = true;
            }
            bool bUpdated = updateDisplay(ct
                          , dt
                          , buildtimerecord[i].totaltime
                          , ui->buildTable->item(i, 1)
                          );


            if (!bRepaint && bUpdated)
            {
                bRepaint = true;
            }
        }
    }
    if (bRepaint)
    {
        ui->buildTable->repaint();
        bRepaint = false;
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
    difftime.setTotalMS(dt-ct);
    return difftime.toString();
}

QString TimerMainWindow::getExpectedTimeStr(qint64 ct, qint64 dt)
{
    uint utc = dt/1000;
    QDateTime expectedtime = QDateTime::fromTime_t(utc).toLocalTime();
    QString expectedstr = expectedtime.toString("HH : mm : ss");

    int utcnow = ct/1000;
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
    int percentage = (totalt-tdiff)*10000/totalt;
    return percentage;
}

bool TimerMainWindow::setPercentageFlag(QTableWidgetItem *pItem, int percentage, bool bYellow)
{
    int d = pItem->data(Qt::UserRole).toInt();
    int oldd = d;
    int flag = d%100;

    d = flag+percentage*100+(bYellow?10000000:0);

    if (oldd != d)
    {
        pItem->setData(Qt::UserRole, d);
        if (oldd/10 != d/10)
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
        "\
        QProgressBar {  \
            background-color: grey;  \
            border: 0px; \
            border-left: 1px solid black;\
            border-radius: 0px; \
        }\
        QProgressBar::chunk {   \
            background-color: rgb(255, 153, 0);  \
        }\
        ",

        "\
        QProgressBar {  \
            background-color: grey;  \
            border: 0px; \
            border-left: 1px solid black;\
            border-radius: 0px; \
        }\
        QProgressBar::chunk {   \
            background-color: rgb(255, 255, 0);  \
        }\
        ",

        "\
        QProgressBar {  \
            background-color: grey;  \
            border: 0px; \
            border-left: 1px solid black;\
            border-radius: 0px; \
        }\
        QProgressBar::chunk {   \
            background-color: rgb(0, 255, 0);  \
        }\
        "
    };
    int stylei;
    if (bYellow)
    {
        if (tdiff  <= 0)
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

    int i = (int)pBar->userData(Qt::UserRole);
    if (i != stylei)
    {
        pBar->setUserData(Qt::UserRole, (QObjectUserData*)i);
        pBar->setStyleSheet(stylesheet[stylei]);
    }
}

bool TimerMainWindow::updateDisplay(qint64 ct, qint64 dt, qint64 tt, QTableWidgetItem *pRemainItem, QTableWidgetItem *pExpectedItem, QProgressBar *pProgress)
{
    QString strRemain = getRemainTimeStr(ct, dt);
    pRemainItem->setText(strRemain);

    if (pExpectedItem)
    {
        QString strExpected = getExpectedTimeStr(ct, dt);
        pExpectedItem->setText(strExpected);
    }

    qint64 tdiff = dt-ct;
    int percentage = getPercentage(tdiff, tt);
    bool bYellow = isYellow(tdiff);

    bool bUpdated = setPercentageFlag(pRemainItem, percentage, bYellow);

    if (pProgress)
    {
        pProgress->setValue(percentage/100);
        setProgressColor(pProgress, tdiff, bYellow);
    }

    return bUpdated;
}

void TimerMainWindow::playSound(int i)
{
    /*
    QString filename = QApplication::applicationDirPath();
    switch(i)
    {
    case 0:
        filename += "/expedition.mp3";
        break;
    case 1:
        filename += "/repair.mp3";
        break;
    case 2:
        filename += "/build.mp3";
        break;
    }
    pPlayer->setMedia(QUrl::fromLocalFile(filename));
    pPlayer->play();
    */
    pPlaylist->setCurrentIndex(i);
    pPlayer->play();
}

void TimerMainWindow::initTableItem()
{
    QTableWidget * pTable;
    foreach(pTable, lstTables)
    {
        pTable->horizontalHeader()->hide();
        pTable->horizontalHeader()->setDefaultSectionSize(20);
        pTable->verticalHeader()->hide();
        pTable->verticalHeader()->setDefaultSectionSize(20);
        pTable->setFocusPolicy(Qt::NoFocus);
        for (int i=0; i<pTable->rowCount(); i++)
        {
            for (int j=0; j<pTable->columnCount(); j++)
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

    for (int i=0; i<4; i++)
    {
        progressExpUs[i]->setUserData(Qt::UserRole, (QObjectUserData*)-1);
        progressRepairUs[i]->setUserData(Qt::UserRole, (QObjectUserData*)-1);
    }


    ui->expeditionTable->setSpan(0, 0, 2, 1);
    ui->expeditionTable->setSpan(1, 1, 1, 2);
    ui->expeditionTable->setSpan(2, 0, 2, 1);
    ui->expeditionTable->setSpan(3, 1, 1, 2);
    ui->expeditionTable->setSpan(4, 0, 2, 1);
    ui->expeditionTable->setSpan(5, 1, 1, 2);
    ui->expeditionTable->setSpan(6, 0, 2, 1);
    ui->expeditionTable->setSpan(7, 1, 1, 2);
    ui->expeditionTable->setSpan(0, 0, 2, 1);

    ui->repairTable->setSpan(0, 0, 2, 1);
    ui->repairTable->setSpan(1, 1, 1, 2);
    ui->repairTable->setSpan(2, 0, 2, 1);
    ui->repairTable->setSpan(3, 1, 1, 2);
    ui->repairTable->setSpan(4, 0, 2, 1);
    ui->repairTable->setSpan(5, 1, 1, 2);
    ui->repairTable->setSpan(6, 0, 2, 1);
    ui->repairTable->setSpan(7, 1, 1, 2);

    ui->expeditionTable->item(0, 0)->setData(Qt::UserRole, CustomTableDelegate_Right|CustomTableDelegate_Bottom);
    ui->expeditionTable->item(2, 0)->setData(Qt::UserRole, CustomTableDelegate_Right|CustomTableDelegate_Bottom);
    ui->expeditionTable->item(4, 0)->setData(Qt::UserRole, CustomTableDelegate_Right|CustomTableDelegate_Bottom);
    ui->expeditionTable->item(6, 0)->setData(Qt::UserRole, CustomTableDelegate_Right);

    ui->expeditionTable->item(1, 1)->setData(Qt::UserRole, CustomTableDelegate_Bottom);
    ui->expeditionTable->item(3, 1)->setData(Qt::UserRole, CustomTableDelegate_Bottom);
    ui->expeditionTable->item(5, 1)->setData(Qt::UserRole, CustomTableDelegate_Bottom);


    ui->repairTable->item(0, 0)->setData(Qt::UserRole, CustomTableDelegate_Right|CustomTableDelegate_Bottom);
    ui->repairTable->item(2, 0)->setData(Qt::UserRole, CustomTableDelegate_Right|CustomTableDelegate_Bottom);
    ui->repairTable->item(4, 0)->setData(Qt::UserRole, CustomTableDelegate_Right|CustomTableDelegate_Bottom);
    ui->repairTable->item(6, 0)->setData(Qt::UserRole, CustomTableDelegate_Right);

    ui->repairTable->item(1, 1)->setData(Qt::UserRole, CustomTableDelegate_Bottom);
    ui->repairTable->item(3, 1)->setData(Qt::UserRole, CustomTableDelegate_Bottom);
    ui->repairTable->item(5, 1)->setData(Qt::UserRole, CustomTableDelegate_Bottom);

    ui->buildTable->item(0, 0)->setData(Qt::UserRole, CustomTableDelegate_Right|CustomTableDelegate_Bottom);
    ui->buildTable->item(1, 0)->setData(Qt::UserRole, CustomTableDelegate_Right);

    ui->buildTable->item(0, 1)->setData(Qt::UserRole, CustomTableDelegate_Bottom);
    ui->buildTable->item(0, 2)->setData(Qt::UserRole, CustomTableDelegate_Bottom);



    ui->expeditionTable->item(0, 0)->setText(QString::fromLocal8Bit("①"));
    ui->expeditionTable->item(2, 0)->setText(QString::fromLocal8Bit("②"));
    ui->expeditionTable->item(4, 0)->setText(QString::fromLocal8Bit("③"));
    ui->expeditionTable->item(6, 0)->setText(QString::fromLocal8Bit("④"));

    ui->repairTable->item(0, 0)->setText(QString::fromLocal8Bit("①"));
    ui->repairTable->item(2, 0)->setText(QString::fromLocal8Bit("②"));
    ui->repairTable->item(4, 0)->setText(QString::fromLocal8Bit("③"));
    ui->repairTable->item(6, 0)->setText(QString::fromLocal8Bit("④"));

    ui->buildTable->item(0, 0)->setText(QString::fromLocal8Bit("①"));
    ui->buildTable->item(1, 0)->setText(QString::fromLocal8Bit("②"));


    /*

    KQTime zerotime;
    zerotime.setHMS(135, 0, 0);
    zerotime.setTotalMS(86400000);

    qDebug(zerotime.toString().toLocal8Bit().constData());
    ui->expeditionTable->item(0, 1)->setText(zerotime.toString());
    ui->expeditionTable->item(0, 1)->setData(Qt::UserRole, 600000);
    ui->expeditionTable->item(0, 2)->setText(zerotime.toString());
    ui->expeditionTable->item(1, 1)->setText(QString::fromLocal8Bit("鼠輸送作戦"));
    */

    setExpeditionTime(0, currentMS()+70000, 600000, QString::fromLocal8Bit("鼠輸送作戦"));
    setExpeditionTime(1, currentMS()+100005000, 120000000, QString::fromLocal8Bit("鼠輸送作戦"));
    setExpeditionTime(2, currentMS()+2005000, 6000000, QString::fromLocal8Bit("鼠輸送作戦"));
    setExpeditionTime(3, currentMS()+305000, 6000000, QString::fromLocal8Bit("鼠輸送作戦"));

    setRepairTime(0, currentMS()+48560485, 54086454, QString::fromLocal8Bit("AAA"));
    setRepairTime(1, currentMS()+45064, 79005560560, QString::fromLocal8Bit("BBB"));
    setRepairTime(2, currentMS()+809640530, 5048964, QString::fromLocal8Bit("CCC"));
    setRepairTime(3, currentMS()+548904156, 480645848, QString::fromLocal8Bit("DDD"));

    setbuildTime(0, currentMS()+1894908, 408904650, QString::fromLocal8Bit("AAA"));
    setbuildTime(1, currentMS()+4809848960, 40894560, QString::fromLocal8Bit("BBB"));
}
