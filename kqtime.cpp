#include "kqtime.h"
#include <QTime>
#include <QLocale>

enum {
//    SECS_PER_DAY = 86400,
//    MSECS_PER_DAY = 86400000,
    SECS_PER_HOUR = 3600,
    MSECS_PER_HOUR = 3600000,
    SECS_PER_MIN = 60,
    MSECS_PER_MIN = 60000,
    TIME_T_MAX = 2145916799,  // int maximum 2037-12-31T23:59:59 UTC
    JULIAN_DAY_FOR_EPOCH = 2440588 // result of julianDayFromDate(1970, 1, 1)
};

KQTime::KQTime(int h, int m, int s, int ms)
{
    setHMS(h, m, s, ms);
}


bool KQTime::isValid() const
{
    return mds > NullTime;
}


int KQTime::hour() const
{
    if (!isValid())
        return -1;

    return ds() / MSECS_PER_HOUR;
}

int KQTime::minute() const
{
    if (!isValid())
        return -1;

    return (ds() % MSECS_PER_HOUR) / MSECS_PER_MIN;
}

int KQTime::second() const
{
    if (!isValid())
        return -1;

    return (ds() / 1000)%SECS_PER_MIN;
}

int KQTime::msec() const
{
    if (!isValid())
        return -1;

    return ds() % 1000;
}

QString KQTime::toString() const
{
    QString strtime;
    strtime.sprintf("%02d : %02d: %02d", hour(), minute(), second());
    return strtime;
}

bool KQTime::setHMS(int h, int m, int s, int ms)
{
#if defined(Q_OS_WINCE)
    startTick = NullTime;
#endif
    if (!isValid(h,m,s,ms)) {
        mds = NullTime;                // make this invalid
        return false;
    }
    mds = (h*SECS_PER_HOUR + m*SECS_PER_MIN + s)*1000 + ms;
    return true;
}

bool KQTime::setTotalMS(qint64 ms)
{
    mds = ms;
    if (!isValid())
    {
        mds = NullTime;
        return false;
    }
    return true;
}

KQTime KQTime::addMSecs(qint64 ms) const
{
    KQTime t;
    if (isValid()) {
        t.mds = (ds() + ms);
    }
#if defined(Q_OS_WINCE)
    if (startTick > NullTime)
        t.startTick = (startTick + ms);
#endif
    return t;
}

qint64 KQTime::msecsTo(const KQTime &t) const
{
    if (!isValid() || !t.isValid())
        return 0;
#if defined(Q_OS_WINCE)
    // GetLocalTime() for Windows CE has no milliseconds resolution
    if (t.startTick > NullTime && startTick > NullTime)
        return t.startTick - startTick;
    else
#endif
        return t.ds() - ds();
}

bool KQTime::isValid(int h, int m, int s, int ms)
{
    return (uint)m < 60 && (uint)s < 60 && (uint)ms < 1000;
}
