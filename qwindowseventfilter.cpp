#include "qwindowseventfilter.h"
#include "MainWindow.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QMessageLogger>
#include <QFile>
#include <QApplication>

QWindowsEventFilter::QWindowsEventFilter()
{
	/*
    _logFile = new QFile(MainWindow::getAbsoluteResourcePath() + "/WindowsEvent.log");
	_logFile->open(QIODevice::WriteOnly | QIODevice::Text);
	_logStream = new QTextStream(_logFile);
	_logStream->setCodec("UTF-16");
	_logStream->setGenerateByteOrderMark(true);
	*/
}


QWindowsEventFilter::~QWindowsEventFilter()
{
	/*
	if (_logFile)
	{
	_logFile->close();
	delete _logFile;
	}
	if (_logStream)
	{
	delete _logStream;
	}
	*/
}

bool QWindowsEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
	Q_UNUSED(result);

#ifdef Q_OS_WIN
	if (eventType == "windows_generic_MSG")
	{
		MSG *msg = static_cast<MSG *>(message);

		if (msg->message == WM_POWERBROADCAST)
		{
			/*
			if (_logStream)
			{
			QString str = QString("\nMessage: ") + QString::number(msg->message) + " lParam: " + QString::number((int)(msg->lParam)) + " Time :" + QTime::currentTime().toString() + "\n";
			(*_logStream) << str;
			if (_logFile)
			{
			_logFile->flush();
			}
			}
			*/
			auto mainWindow = MainWindow::mainWindow();
			if (!mainWindow)
			{
				return false;
			}
			if (msg->wParam == PBT_APMSUSPEND)
			{
				// sleep
				mainWindow->minimizeWindow();
				mainWindow->setSleepMode(true);
				return false;
			}
			if (msg->wParam == PBT_APMRESUMEAUTOMATIC)
			{
				// resume
				mainWindow->setSleepMode(false);
				return false;
			}
			if (msg->wParam == PBT_POWERSETTINGCHANGE)
			{
				POWERBROADCAST_SETTING *ps = (POWERBROADCAST_SETTING*)(msg->lParam);
				/*
				if (_logStream)
				{
				QString str = QString("Power Setting: ") + QString::number(*(DWORD*)(ps->Data)) + "\n";
				(*_logStream) << str;
				if (_logFile)
				{
				_logFile->flush();
				}
				}
				*/
				if (ps->PowerSetting == GUID_CONSOLE_DISPLAY_STATE)
				{
					DWORD dw = *(DWORD*)(ps->Data);
					if (dw == 0x0)
					{
						mainWindow->minimizeWindow();
						// off
						//						mainWindow->setSleepMode(true);
						return false;
					}
					if (dw == 0x1)
					{
						// on
						//						mainWindow->setSleepMode(false);
						return false;
					}
				}
			}
			return false;
		}
	}
#endif
	return false;
}
