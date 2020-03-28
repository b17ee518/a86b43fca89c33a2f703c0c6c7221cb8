#pragma once
#include <QString>

class QTimer;
class QProcess;

class RemoteNotifyHandler
{
public:
	static RemoteNotifyHandler &getInstance()
	{
		static RemoteNotifyHandler instance;
		return instance;
	}

private:
	RemoteNotifyHandler();
	virtual ~RemoteNotifyHandler();
	RemoteNotifyHandler(RemoteNotifyHandler const &);
	void operator=(RemoteNotifyHandler const &);

public:
	enum class Level
	{
		Invalid = -1,
		Debug = 0,
		Info = 10,
		Low = 100,
		High = 1000,

		Fatal = 1000000,
	};

	Level notifyLevel = Level::High;

	void Notify(QString text, Level level);

	static void RunNotify();

	void NotifyNeko();

	void LoadSettings();

	void RunKillMITMNetStat();

	void ToggleReport(bool report);

private:
	void RunInstanceNotify();

	QString senderEmailAddress;
	QString senderPassword;
	QString receiverEmailAddress;
	QString anotherReceiverEmailAddress;

#if defined Q_OS_MAC
	QProcess *process = NULL;
#endif

	QString text;
	Level level;
	QTimer *reportTimer = NULL;
};
