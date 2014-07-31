#include "klog.h"
#include <QApplication>
#include <QTextStream>
#include <QDateTime>

KLog::KLog()
{
    Init();
}

KLog::~KLog()
{
    Release();
}

void KLog::Init()
{
    errorlog = new QFile(QApplication::applicationDirPath()+"/errorlog.txt");
    apilog = new QFile(QApplication::applicationDirPath()+"/apilog.txt");
    battleresultlog = new QFile(QApplication::applicationDirPath()+"/battleresultlog.txt");
	battledetaillog = new QFile(QApplication::applicationDirPath() + "/battledetaillog.txt");

    errorlog->open(QIODevice::ReadWrite|QIODevice::Append|QIODevice::Text);
	apilog->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
	battleresultlog->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
	battledetaillog->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
}

void KLog::Release()
{
    if (errorlog)
    {
        errorlog->close();
        errorlog = 0;
    }
    if (apilog)
    {
        apilog->close();
        apilog = 0;
    }
    if (battleresultlog)
    {
        battleresultlog->close();
        battleresultlog = 0;
    }
	if (battledetaillog)
	{
		battledetaillog->close();
		battledetaillog = 0;
	}
}

void KLog::AppendLog(QFile *f, QString log)
{
    if (f)
    {
        if (f->isWritable())
        {
            QString str = QDateTime::currentDateTime().toString("[yyyy/MM/dd HH:mm:ss]\t");
            str += log;
            str += "\n";
            f->write(str.toLocal8Bit());
            f->flush();
        }
    }
}

void KLog::LogError(QString log)
{
    AppendLog(errorlog, log);
}

void KLog::LogAPI(QString path, QString request, QString response)
{
    QString str = path+"\t"+request+"\t"+response;
    AppendLog(apilog, str);
}

void KLog::LogBattleResult(QString log)
{
    AppendLog(battleresultlog, log);
}

void KLog::LogBattleDetail(QString log)
{
	AppendLog(battledetaillog, log);
}


void KLog::RecordLog(QString filename, QString log)
{
	QFile * file = new QFile(QApplication::applicationDirPath() + "/" + filename);
	if (file)
	{
		file->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);

		AppendLog(file, log);

		file->close();
	}
}
