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
    resultlog = new QFile(QApplication::applicationDirPath()+"/resultlog.txt");

    errorlog->open(QIODevice::ReadWrite|QIODevice::Append|QIODevice::Text);
    apilog->open(QIODevice::ReadWrite|QIODevice::Append|QIODevice::Text);
    resultlog->open(QIODevice::ReadWrite|QIODevice::Append|QIODevice::Text);
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
    if (resultlog)
    {
        resultlog->close();
        resultlog = 0;
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

void KLog::LogResult(QString log)
{
    AppendLog(resultlog, log);
}
