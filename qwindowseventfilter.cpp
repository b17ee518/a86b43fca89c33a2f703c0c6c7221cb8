#include "qwindowseventfilter.h"
#include "MainWindow.h"

#include <windows.h>

QWindowsEventFilter::QWindowsEventFilter()
{

}

bool QWindowsEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
	Q_UNUSED(result);
	if (eventType == "windows_generic_MSG")
	{
		MSG *msg = static_cast<MSG *>(message);
		if (msg->message == WM_POWERBROADCAST)
		{
			auto mainWindow = MainWindow::mainWindow();
			if (!mainWindow)
			{
				return false;
			}
			if (msg->wParam == PBT_APMSUSPEND)
			{
				// sleep
				mainWindow->setWindowState(Qt::WindowMinimized);
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
				if (ps->PowerSetting == GUID_CONSOLE_DISPLAY_STATE)
				{
					DWORD dw = *(DWORD*)(ps->Data);
					if (dw == 0x0)
					{
						mainWindow->setWindowState(Qt::WindowMinimized);
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
	return false;
}