#ifndef KLOG_H
#define KLOG_H

#include <QFile>

#define ERRLOG(log) KLog::getInstance().LogError(log);
#define APILOG(p, req, res) KLog::getInstance().LogAPI(p, req, res);
#define APILOGDEBUG(p, req, res) KLog::getInstance().LogAPI(p, req, res, true);
#define RECLOG(filename, log) KLog::getInstance().RecordLog(filename, log);
#define RECLINE(filename, log) KLog::getInstance().RecordLog(filename, log, false);

class KLog
{
private:
    KLog(void);
public:
    static KLog& getInstance(void) {
        static KLog singleton;
        return singleton;
    }

	void AppendLog(QFile * f, const QString& log, bool bWithTimeStamp=true);

	void LogError(const QString& log);
	void LogAPI(const QString& path, const QString& request, const QString& response, bool bDebug=false);

	void DeleteLog(const QString& filename);
	void RecordLog(const QString& filename, const QString& log, bool bWithTimeStamp=true);
};

#endif // KLOG_H
