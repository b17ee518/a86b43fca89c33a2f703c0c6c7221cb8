#ifndef KLOG_H
#define KLOG_H

#include <QFile>

#define ERRLOG(log) KLog::getInstance().LogError(log);
#define APILOG(p, req, res) KLog::getInstance().LogAPI(p, req, res);
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

    void AppendLog(QFile * f, QString log);

    void LogError(QString log);
    void LogAPI(QString path, QString request, QString response);

	void RecordLog(QString filename, QString log);

    QFile * errorlog;
    QFile * apilog;

};

#endif // KLOG_H
