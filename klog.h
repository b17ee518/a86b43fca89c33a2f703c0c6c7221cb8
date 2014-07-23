#ifndef KLOG_H
#define KLOG_H

#include <QFile>

#define ERRLOG(log) KLog::getInstance().LogError(log);
#define APILOG(p, req, res) KLog::getInstance().LogAPI(p, req, res);
#define RESLOG(log) KLog::getInstance().LogResult(log);

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
    void LogResult(QString log);

    QFile * errorlog;
    QFile * apilog;
    QFile * resultlog;

};

#endif // KLOG_H
