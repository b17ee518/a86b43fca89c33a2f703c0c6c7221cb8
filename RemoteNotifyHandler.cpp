﻿#include "RemoteNotifyHandler.h"

#include "./SmtpClientForQt/src/smtpmime"
#include <QTimer>
#include <QApplication>
#include <QSettings>
#include <QtConcurrent/QtConcurrent>

#include "mainwindow.h"
#include <QProcess>
#include "ControlManager.h"

#if defined Q_OS_WIN
#include <windows.h>
#endif

RemoteNotifyHandler::RemoteNotifyHandler()
{
#if defined Q_OS_MAC
	process = new QProcess();

	QObject::connect(process, &QProcess::readyReadStandardOutput, [this]() {
		qDebug() << process->readAllStandardOutput();
	});
	QObject::connect(process, &QProcess::readyReadStandardError, [this]() {
		qDebug() << process->readAllStandardError();
	});

	QString sh = qgetenv("SHELL");
	process->setProgram(sh);
	process->setArguments(QStringList() << "-i");
	process->start();
	process->write("pwd\n");
#endif
}


RemoteNotifyHandler::~RemoteNotifyHandler()
{
#if defined Q_OS_MAC
	if (process != NULL)
	{
		process->terminate();
		delete process;
	}
#endif
	if (reportTimer != NULL)
	{
		reportTimer->stop();
		delete reportTimer;
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

void RemoteNotifyHandler::RunKillMITMNetStat()
{
#if defined Q_OS_MAC
	if (process)
	{
		if (MainWindow::mainWindow() == NULL)
		{
			return;
		}
        process->write((QString("netstat -vanp tcp | grep ") + QString::number(MainWindow::mainWindow()->getUsePort()) + " | grep tcp46 | awk '{print \"kill -9 \", $9}' | zsh\n").toLocal8Bit());
		process->closeWriteChannel();

		QEventLoop loop;
		QTimer timer;
		timer.setSingleShot(true);
		QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
		timer.start(100);
		loop.exec();
	}
#endif
}

void RemoteNotifyHandler::Notify(QString text, Level level)
{
	this->text = text;
	this->level = level;

#if defined Q_OS_WIN
	QtConcurrent::run(RemoteNotifyHandler::RunNotify);
#elif defined Q_OS_MAC
	RemoteNotifyHandler::RunNotify();
#endif
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
		SimpleMail::MimeMessage message;

        SimpleMail::EmailAddress senderEmail(senderEmailAddress, "KanplayNotify");
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
#if defined Q_OS_WIN
			+ "\""
#endif
			+ filename
#if defined Q_OS_WIN
			+ "\""
#elif defined Q_OS_MAC
			+ "\n"
#endif
			;

#if defined Q_OS_WIN
		WinExec(command.toLocal8Bit(), SW_HIDE);
#elif defined Q_OS_MAC
		process->write(command.toLocal8Bit());
#endif

	}
}

void RemoteNotifyHandler::NotifyNeko()
{
	Notify(QString::fromLocal8Bit("猫ったぞ！"), Level::Fatal);
}

void RemoteNotifyHandler::ToggleReport(bool report)
{
	std::function<void(void)> hourlyReport = [this]() {
		if (ControlManager::getInstance().isRunning())
		{
			Notify(QString::fromLocal8Bit("Running!"), Level::High);
		}
		else
		{
			Notify(QString::fromLocal8Bit("Stopped!"), Level::High);
		}
		reportTimer->setInterval(60 * 60 * 1000);
		reportTimer->setSingleShot(true);
		reportTimer->start();
	};

	if (report)
	{
		if (reportTimer == NULL)
		{
			QDateTime dt = QDateTime::currentDateTime();
			QDateTime nextHourDT(dt);
			QTime newTime(dt.time().hour(), 0, 0, 0);
			nextHourDT.setTime(newTime);
			nextHourDT = nextHourDT.addSecs(60 * 60);
			int diffTime = dt.msecsTo(nextHourDT);

			reportTimer = new QTimer();
			reportTimer->setInterval(diffTime);
			reportTimer->setSingleShot(true);
			QObject::connect(reportTimer, &QTimer::timeout, hourlyReport);
			reportTimer->start();
		}
	}
	else
	{
		if (reportTimer != NULL)
		{
			reportTimer->stop();
			delete reportTimer;
			reportTimer = NULL;
		}
	}
}

