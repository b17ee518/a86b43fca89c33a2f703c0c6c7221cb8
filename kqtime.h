#ifndef KQTIME_H
#define KQTIME_H

#include <QString>

class KQTime
{
public:
    KQTime(): mds(NullTime)
#if defined(Q_OS_WINCE)
        , startTick(NullTime)
#endif
    {}
    KQTime(int h, int m, int s = 0, int ms = 0);

    bool isNull() const { return mds == NullTime; }
    bool isValid() const;

    int hour() const;
    int minute() const;
    int second() const;
    int msec() const;

    QString toString() const;

    bool setHMS(int h, int m, int s, int ms = 0);
    bool setTotalMS(qint64 ms);

    KQTime addMSecs(qint64 ms) const;
    qint64 msecsTo(const KQTime &) const;

    bool operator==(const KQTime &other) const { return mds == other.mds; }
    bool operator!=(const KQTime &other) const { return mds != other.mds; }
    bool operator<(const KQTime &other) const { return mds < other.mds; }
    bool operator<=(const KQTime &other) const { return mds <= other.mds; }
    bool operator>(const KQTime &other) const { return mds > other.mds; }
    bool operator>=(const KQTime &other) const { return mds >= other.mds; }

    static bool isValid(int h, int m, int s, int ms = 0);
private:
    enum TimeFlag { NullTime = -1 };
    inline qint64 ds() const { return mds == -1 ? 0 : mds; }
    qint64 mds;
#if defined(Q_OS_WINCE)
    int startTick;
#endif

    friend class QDateTime;
    friend class QDateTimePrivate;
};

#endif //KQTIME_H
