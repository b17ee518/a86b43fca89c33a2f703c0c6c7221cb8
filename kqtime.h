#ifndef KQTIME_H
#define KQTIME_H

#include <QString>

class KQTime
{
public:
	KQTime(){}
    KQTime(int h, int m, int s = 0, int ms = 0);

	inline bool isNull() const { return _mds == NullTime; }
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

    bool operator==(const KQTime &other) const { return _mds == other._mds; }
    bool operator!=(const KQTime &other) const { return _mds != other._mds; }
    bool operator<(const KQTime &other) const { return _mds < other._mds; }
    bool operator<=(const KQTime &other) const { return _mds <= other._mds; }
    bool operator>(const KQTime &other) const { return _mds > other._mds; }
    bool operator>=(const KQTime &other) const { return _mds >= other._mds; }

    static bool isValid(int h, int m, int s, int ms = 0);
private:
	enum TimeFlag{ NullTime = -1 };
	inline qint64 ds() const { return _mds == NullTime ? 0 : _mds; }
	qint64 _mds = NullTime;
#if defined(Q_OS_WINCE)
	int _startTick = NullTime;
#endif

    friend class QDateTime;
    friend class QDateTimePrivate;
};

#endif //KQTIME_H
