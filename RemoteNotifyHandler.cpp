#include "RemoteNotifyHandler.h"

#include "./SmtpClientForQt/src/smtpmime"
#include <QTimer>
#include <QApplication>
#include <QSettings>
#include <QtConcurrent/QtConcurrent>

#include "mainwindow.h"
#include <QProcess>

#if defined Q_OS_WIN
#include <windows.h>
#endif

RemoteNotifyHandler::RemoteNotifyHandler()
{
}


RemoteNotifyHandler::~RemoteNotifyHandler()
{
    if (process != NULL)
    {
        process->terminate();
        delete process;
    }
}

void RemoteNotifyHandler::LoadSettings()
{
    QString filename = MainWindow::getAbsoluteResourcePath();
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
#if defined Q_OS_MAC
        if (process == NULL)
        {
            process = new QProcess();
            QObject::connect(process, &QProcess::readyReadStandardOutput, [this](){
                qDebug()<<process->readAllStandardOutput();
            });
            process->setProgram(qgetenv("SHELL"));
            process->setArguments(QStringList() << "-i");
            process->start();
            process->waitForStarted();
        }
#endif

		SimpleMail::MimeMessage message;

		SimpleMail::EmailAddress senderEmail(senderEmailAddress, "KN: " + text);
		SimpleMail::EmailAddress recipentEmail(receiverEmailAddress);
		SimpleMail::EmailAddress anotherRecipentEmail(anotherReceiverEmailAddress);

		message.setSender(senderEmail);
		message.addTo(recipentEmail);
		if (anotherReceiverEmailAddress != "None")
		{
			message.addTo(anotherRecipentEmail);
		}
		message.setSubject(text);

        QString filename = MainWindow::getAbsoluteResourcePath();
		filename += "/emailContents.txt";
		QFile file(filename);
		if (file.open(QIODevice::WriteOnly)) {
			message.write(&file);
		}
		file.close();

		QString command = "curl smtp://smtp.gmail.com:587 -v --mail-from \""
			+ senderEmailAddress +
			"\" --mail-rcpt \""
			+ receiverEmailAddress +
			"\" --ssl -u " + senderEmailAddress +
            ":" + senderPassword + " -k --anyauth -T "
        #if Q_OS_WIN
                + "\""
        #endif
                + filename
        #if Q_OS_WIN
                + "\""
        #endif
                ;

#if defined Q_OS_WIN
        WinExec(command.toLocal8Bit(), SW_HIDE);
#elif defined Q_OS_MAC
        qDebug() << command;
        process->write(command.toLocal8Bit() + "\n");
#endif

	}
}

void RemoteNotifyHandler::NotifyNeko()
{
	Notify(QString::fromLocal8Bit("猫ったぞ！"), Level::Fatal);
}
