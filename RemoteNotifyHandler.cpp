#include "RemoteNotifyHandler.h"

#include "./SmtpClientForQt/src/smtpmime"
#include <QTimer>
#include <QApplication>
#include <QSettings>
#include <QtConcurrent/QtConcurrent>

RemoteNotifyHandler::RemoteNotifyHandler()
{
}


RemoteNotifyHandler::~RemoteNotifyHandler()
{
}

void RemoteNotifyHandler::LoadSettings()
{
	QString filename = QApplication::applicationDirPath();
	filename += "/notifysettings.ini";
	QSettings* setting = new QSettings(filename, QSettings::IniFormat);
	setting->setIniCodec("UTF-8");

	const QString itemDefaultNotifyLevel = "NotifyLevel";
	const QString itemSenderEmailAddress = "SenderEmailAddress";
	const QString itemReceiverEmailAddress = "ReceiverEmailAddress";
	const QString itemAnotherReceiverEmailAddress = "AnotherReceiverEmailAddress";
	const QString itemSenderPassword = "SenderPassword";


	setting->beginGroup("Settings");
	if (!setting->contains(itemDefaultNotifyLevel))
	{
		setting->setValue(itemDefaultNotifyLevel, "High");
	}

	if (!setting->contains(itemSenderEmailAddress))
	{
		setting->setValue(itemSenderEmailAddress, "None");
	}
	if (!setting->contains(itemReceiverEmailAddress))
	{
		setting->setValue(itemReceiverEmailAddress, "None");
	}
	if (!setting->contains(itemAnotherReceiverEmailAddress))
	{
		setting->setValue(itemAnotherReceiverEmailAddress, "None");
	}
	if (!setting->contains(itemSenderPassword))
	{
		setting->setValue(itemSenderPassword, "None");
	}

	QString notifyLevelText = setting->value(itemDefaultNotifyLevel).toString();
	if (!notifyLevelText.compare("High", Qt::CaseInsensitive))
	{
		notifyLevel = Level::High;
	}
	else if (!notifyLevelText.compare("Fatal", Qt::CaseInsensitive))
	{
		notifyLevel = Level::Fatal;
	}
	else if (!notifyLevelText.compare("Low", Qt::CaseInsensitive))
	{
		notifyLevel = Level::Low;
	}
	else if (!notifyLevelText.compare("Info", Qt::CaseInsensitive))
	{
		notifyLevel = Level::Info;
	}
	else if (!notifyLevelText.compare("Debug", Qt::CaseInsensitive))
	{
		notifyLevel = Level::Debug;
	}

	senderEmailAddress = setting->value(itemSenderEmailAddress).toString();
	receiverEmailAddress = setting->value(itemReceiverEmailAddress).toString();
	anotherReceiverEmailAddress = setting->value(itemAnotherReceiverEmailAddress).toString();
	senderPassword = setting->value(itemSenderPassword).toString();
}

void RemoteNotifyHandler::Notify(QString text, Level level)
{
	this->text = text;
	this->level = level;

	QtConcurrent::run(RemoteNotifyHandler::RunNotify);
}

void RemoteNotifyHandler::RunNotify()
{
	getInstance().RunInstanceNotify();
}

void RemoteNotifyHandler::RunInstanceNotify()
{
	LoadSettings();

	if (senderEmailAddress == "None" || receiverEmailAddress == "None" || senderPassword == "None")
	{
		return;
	}

	if (level >= notifyLevel)
	{
		SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);

		smtp.setUser(senderEmailAddress);
		smtp.setPassword(senderPassword);

		MimeMessage message;

		message.setSender(new EmailAddress(senderEmailAddress, "KN: " + text));
		message.addRecipient(new EmailAddress(receiverEmailAddress));
		if (anotherReceiverEmailAddress != "None")
		{
			message.addRecipient(new EmailAddress(anotherReceiverEmailAddress));
		}
		message.setSubject(text);

		// Now we can send the mail
		smtp.setAuthMethod(SmtpClient::AuthLogin);

		bool done = false;
		if (smtp.connectToHost())
		{
			if (smtp.login())
			{
				if (smtp.sendMail(message))
				{
					done = true;
				}
			}
		}

		smtp.quit();


	}
}

void RemoteNotifyHandler::NotifyNeko()
{
	Notify(QString::fromLocal8Bit("猫ったぞ！"), Level::Fatal);
}
