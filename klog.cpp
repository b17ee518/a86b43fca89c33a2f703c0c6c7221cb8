#include "klog.h"
#include <QApplication>
#include <QTextStream>
#include <QDateTime>

KLog::KLog()
{
}


void KLog::AppendLog(QFile *f, const QString& log, bool bWithTimeStamp/*=true*/)
{
	if (f)
	{
		if (f->isWritable())
		{
			QString str = (bWithTimeStamp?QDateTime::currentDateTime().toString("[yyyy/MM/dd HH:mm:ss]\t"):"");
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

void KLog::LogError(const QString& log)
{
	RecordLog("error", log);
//	AppendLog(errorlog, log);
}

void KLog::LogAPI(const QString& path, const QString& request, const QString& response, bool bDebug/*=false*/)
{
	QString str = path+"\t"+request+"\t"+response;
	RecordLog(QString("apilog")+(bDebug?"_debug":""), str);
//	AppendLog(apilog, str);
}

void KLog::DeleteLog(const QString& filename)
{
	auto fullname = QApplication::applicationDirPath() + "/log/" + filename + ".table";
	QFile::remove(fullname);
}

void KLog::RecordLog(const QString& filename, const QString& log, bool bWithTimeStamp/*=true*/)
{
	QFile * file = new QFile(QApplication::applicationDirPath() + "/log/" + filename + ".table");
	if (file)
	{
		file->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);

		AppendLog(file, log, bWithTimeStamp);

		file->close();

		delete file;
	}
}


