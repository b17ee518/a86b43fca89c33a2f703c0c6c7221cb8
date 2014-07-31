#ifndef KLOG_H
#define KLOG_H

#include <QFile>

#define ERRLOG(log) KLog::getInstance().LogError(log);
#define APILOG(p, req, res) KLog::getInstance().LogAPI(p, req, res);
#define BRESLOG(log) KLog::getInstance().LogBattleResult(log);
#define DBATLOG(log) KLog::getInstance().LogBattleDetail(log);
#define RECLOG(filename, log) KLog::getInstance().RecordLog(filename, log);

class KLog
{
private:
    KLog(void);
public:
    static KLog& getInstance(void) {
        static KLog singleton;
        return singleton;
    }
    ~KLog();

    void Init();
    void Release();

    void AppendLog(QFile * f, QString log);

    void LogError(QString log);
    void LogAPI(QString path, QString request, QString response);
    void LogBattleResult(QString log);
	void LogBattleDetail(QString log);

	void RecordLog(QString filename, QString log);

    QFile * errorlog;
    QFile * apilog;
    QFile * battleresultlog;
	QFile * battledetaillog;

};

#endif // KLOG_H
