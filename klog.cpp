#include "klog.h"
#include <QApplication>
#include <QTextStream>
#include <QDateTime>

KLog::KLog()
{
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
			QTextStream outstream(f);
			outstream.setCodec("UTF-16");
			outstream.setGenerateByteOrderMark(true);
			outstream << str;
			f->flush();
		}
	}
}

void KLog::LogError(QString log)
{
	RecordLog("error", log);
//	AppendLog(errorlog, log);
}

void KLog::LogAPI(QString path, QString request, QString response)
{
	QString str = path+"\t"+request+"\t"+response;
	RecordLog("apilog", str);
//	AppendLog(apilog, str);
}

void KLog::RecordLog(QString filename, QString log)
{
	QFile * file = new QFile(QApplication::applicationDirPath() + "/log/" + filename + ".table");
	if (file)
	{
		file->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);

		AppendLog(file, log);

		file->close();
	}
}
