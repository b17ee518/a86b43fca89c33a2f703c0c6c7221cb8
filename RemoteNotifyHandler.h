#pragma once
#include <QString>


class RemoteNotifyHandler
{
public:
	static RemoteNotifyHandler& getInstance() {
		static RemoteNotifyHandler instance;
		return instance;
	}

private:
	RemoteNotifyHandler();
	virtual ~RemoteNotifyHandler();
	RemoteNotifyHandler(RemoteNotifyHandler const&);
	void operator=(RemoteNotifyHandler const&);


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

private:

	void RunInstanceNotify();

	QString senderEmailAddress;
	QString senderPassword;
	QString receiverEmailAddress;
	QString anotherReceiverEmailAddress;

	QString text;
	Level level;
};

