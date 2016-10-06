#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#ifdef Q_OS_WIN
#include <QWinTaskbarProgress>
#endif
#include <QMessageBox>
#if (QT_VERSION < QT_VERSION_CHECK(5, 6, 0))
#include <QWebFrame>
#endif

#include "ControlManager.h"
#include "ExpeditionManager.h"

void MainWindow::onSubMainWindowShowHide(bool bShow, MainWindowBase *pWindow)
{
	QPushButton * pButton = 0;
	if (pWindow == _pInfoWindow)
	{
		pButton = ui->pbCheckInfo;
	}
	else if (pWindow == _pTimerWindow)
	{
		pButton = ui->pbCheckTimer;
	}
	else if (pWindow == _pWeaponWindow)
	{
		pButton = ui->pbCheckWeapon;
	}
	else if (pWindow == _pShipWindow)
	{
		pButton = ui->pbCheckShip;
	}
	if (!pButton)
	{
		return;
	}

	pButton->setChecked(bShow);
}


void MainWindow::changeEvent(QEvent *e)
{
	MainWindowBase::changeEvent(e);

	if (e->type() == QEvent::WindowStateChange)
	{
		QWindowStateChangeEvent* event = static_cast<QWindowStateChangeEvent*>(e);
		if(event->oldState() & Qt::WindowMinimized)
		{
//            if (ui->pbCheckInfo->isChecked())
            {
                _pInfoWindow->setWindowState(Qt::WindowNoState);
                _pInfoWindow->show();
                _pInfoWindow->stackUnder(this);
            }

//            if (ui->pbCheckTimer->isChecked())
            {
                _pTimerWindow->setWindowState(Qt::WindowNoState);
                _pTimerWindow->show();
                _pTimerWindow->stackUnder(_pInfoWindow);
            }

//            if (ui->pbCheckWeapon->isChecked())
//            {
//                m_pWeaponWindow->setWindowState(Qt::WindowNoState);
//                m_pWeaponWindow->show();
//                m_pWeaponWindow->stackUnder(m_pWeaponWindow);
//            }
			this->raise();
			this->activateWindow();
			this->setSleepMode(false);
		}
		else if (isMinimized())
		{
			if (!_pInfoWindow->isDockingOnTop())
			{
				_pInfoWindow->hide();
			}
			if (!_pTimerWindow->isDockingOnTop())
			{
				_pTimerWindow->hide();
			}
			if (!_pWeaponWindow->isDockingOnTop())
			{
				_pWeaponWindow->hide();
			}
			if (!_pShipWindow->isDockingOnTop())
			{
				_pShipWindow->hide();
			}
//			this->setSleepMode(true);
		}
	}

}

void MainWindow::closeEvent(QCloseEvent *e)
{
	if (!ControlManager::getInstance().isActiveRunning())
	{
		QMessageBox * pMessageBox = new QMessageBox(
			QMessageBox::NoIcon
			, QString::fromLocal8Bit("")
			, QString::fromLocal8Bit("本当に終了しますか？")
			, QMessageBox::Yes | QMessageBox::No
			, this
			, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
		pMessageBox->setDefaultButton(QMessageBox::No);
		pMessageBox->setAttribute(Qt::WA_DeleteOnClose, true);
		int reply = pMessageBox->exec();

		if (reply == QMessageBox::No)
		{
			e->ignore();
			return;
		}
	}

#ifdef Q_OS_WIN
	if (_proxyMode == ProxyMode::Fid && _pFid)
	{
		_pFid->Shutdown();
	}
	else if (_proxyMode == ProxyMode::Nekoxy && _pNekoxy)
	{
		_pNekoxy->Shutdown();
	}
	else if (_proxyMode == ProxyMode::Titanium && _pTitanium)
	{
		_pTitanium->Shutdown();
	}
#endif
	ControlManager::getInstance().Terminate();

	MainWindowBase::closeEvent(e);

	_pInfoWindow->close();
	_pTimerWindow->close();
    _pWeaponWindow->close();
	_pShipWindow->close();
}

void MainWindow::moveEvent(QMoveEvent *e)
{
	MainWindowBase::moveEvent(e);

	QPoint diffPos = e->pos()-e->oldPos();

	if (_bMoveSubTogether)
	{
		if (!_pInfoWindow->isDockingOnTop())
		{
			_pInfoWindow->move(_pInfoWindow->pos() + diffPos);
		}
		if (!_pTimerWindow->isDockingOnTop())
		{
			_pTimerWindow->move(_pTimerWindow->pos() + diffPos);
		}
		if (!_pWeaponWindow->isDockingOnTop())
		{
			_pWeaponWindow->move(_pWeaponWindow->pos() + diffPos);
		}
		if (!_pShipWindow->isDockingOnTop())
		{
			_pShipWindow->move(_pShipWindow->pos() + diffPos);
		}
	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
	QRect rc = QApplication::desktop()->availableGeometry(QApplication::desktop()->primaryScreen());
	if (x() < 20 && x() > -20 && x()!=0)
	{
		move(0, y());
	}
	else if (x()+width() > rc.width()-20 && x()+width() < rc.width()+20 && x()!=rc.width()-width())
	{
		move(rc.width()-width(), y());
	}
	if (y() < 20 && y()!=0)
	{
		move(x(), 0);
	}
	MainWindowBase::mouseReleaseEvent(event);
}

void MainWindow::slotToggleRestoreMinimize(bool bRestore)
{
	if (bRestore)
	{
		if (isMinimized())
		{
            restoreWindow();
		}
	}
	if (!bRestore)
	{
		if (!isMinimized())
        {
            minimizeWindow();
		}
	}
}

void MainWindow::slotSelfToggleRestoreMinimize(bool bRestore)
{
	if (!bRestore)
	{
		this->AdjustVolume(0);
	}
	else
	{
		this->AdjustVolume(-1);
	}
}

void MainWindow::on_pbClose_clicked()
{
	close();
}

void MainWindow::on_pbMinimize_clicked()
{
    minimizeWindow();
}

void MainWindow::on_pbCheckTransparent_toggled(bool checked)
{
	static QList<QPair<QString, QWidget*> > s_listpair;
	if (checked)
	{
		if (_webWidgetType != WebWidgetType::IE)
		{
			s_listpair.clear();

			QList<QWidget *> lstWidget = findChildren<QWidget *>();
			QWidget * w;
			foreach(w, lstWidget)
			{
				QPair<QString, QWidget*> pair;
				pair.first = w->styleSheet();
				pair.second = w;
				s_listpair.append(pair);
			}

			lstWidget = _pInfoWindow->findChildren<QWidget *>();
			foreach(w, lstWidget)
			{
				QPair<QString, QWidget*> pair;
				pair.first = w->styleSheet();
				pair.second = w;
				s_listpair.append(pair);
			}
			lstWidget = _pTimerWindow->findChildren<QWidget *>();
			foreach(w, lstWidget)
			{
				QPair<QString, QWidget*> pair;
				pair.first = w->styleSheet();
				pair.second = w;
				s_listpair.append(pair);
			}
			lstWidget = _pWeaponWindow->findChildren<QWidget *>();
			foreach(w, lstWidget)
			{
				QPair<QString, QWidget*> pair;
				pair.first = w->styleSheet();
				pair.second = w;
				s_listpair.append(pair);
			}
			lstWidget = _pShipWindow->findChildren<QWidget *>();
			foreach(w, lstWidget)
			{
				QPair<QString, QWidget*> pair;
				pair.first = w->styleSheet();
				pair.second = w;
				s_listpair.append(pair);
			}

			QPair<QString, QWidget*> p;
			foreach(p, s_listpair)
			{
				w = p.second;
				QString stylestr = w->styleSheet();
				stylestr += "background-color: rgba(45, 45, 48, 30%);";
				w->setStyleSheet(stylestr);
			}
		}

		applyCss(QWebViewCSSIndex::Transparent);
	}
	else
	{
		QPair<QString, QWidget*> p;
		if (_webWidgetType != WebWidgetType::IE)
		{
			foreach(p, s_listpair)
			{
				if (isAncestorOf(p.second) || _pInfoWindow->isAncestorOf(p.second) || _pTimerWindow->isAncestorOf(p.second))
				{
					p.second->setStyleSheet(p.first);
				}
			}
			s_listpair.clear();
		}

		applyCss(QWebViewCSSIndex::Normal);
	}
	/*
	if (checked)
	{
		applyCss(QWebViewCSSIndex::Compact);
	}
	else
	{
		applyCss(QWebViewCSSIndex::Normal);
	}
	*/
}

void MainWindow::slotSoundEnded()
{
	if (isMinimized())
	{
		AdjustVolume(0);
	}
	else
	{
		AdjustVolume(-1);
	}
}

void MainWindow::showEvent(QShowEvent *event)
{
	MainWindowBase::showEvent(event);

#ifdef Q_OS_WIN
	if (!_pTaskbarButton)
	{
		_pTaskbarButton = new QWinTaskbarButton(this);
		_pTaskbarButton->setWindow(windowHandle());
		QWinTaskbarProgress * pTaskbarProgress = _pTaskbarButton->progress();
		pTaskbarProgress->setVisible(true);

		pTaskbarProgress->setMinimum(0);
		pTaskbarProgress->setMaximum(10000);
		pTaskbarProgress->setValue(0);
		pTaskbarProgress->show();
	}
#endif
}

void MainWindow::on_pbMoveTogether_toggled(bool checked)
{
	_bMoveSubTogether = checked;
}

void MainWindow::on_pbRefresh_clicked()
{
	QMessageBox * pMessageBox = new QMessageBox(
		QMessageBox::NoIcon
		, QString::fromLocal8Bit("")
		, QString::fromLocal8Bit("リロードしますか？")
		, QMessageBox::Yes | QMessageBox::No
		, this
		, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
	pMessageBox->setDefaultButton(QMessageBox::No);
	pMessageBox->setAttribute(Qt::WA_DeleteOnClose, true);
	int reply = pMessageBox->exec();

	if (reply == QMessageBox::Yes)
	{
		if (_webWidgetType == WebWidgetType::WebEngine)
		{
			_applyCssToGameFlag = true;
		}
		navigateReload();
	}

}

void MainWindow::on_pbScreenshot_clicked()
{
	shootScreen();
}

void MainWindow::slotTogglePanicTimer(int timeVal)
{
	if (timeVal > 0)
	{
		_panicTimer->setTimerType(Qt::PreciseTimer);
		_panicTimer->start(timeVal);
	}
	else
	{
		_panicTimer->stop();
	}
}

void MainWindow::onPanic()
{
	QString jsstr = QString::fromLocal8Bit("window.alert('%1'); ").arg(QString::fromLocal8Bit("パニック！"));
	if (_webWidgetType == WebWidgetType::IE)
	{
		// TODO
		QMessageBox* pMessageBox = new QMessageBox(
			QMessageBox::NoIcon
			, QString::fromLocal8Bit("")
			, QString::fromLocal8Bit("パニック！")
			, QMessageBox::Ok
			, this
			, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
		pMessageBox->setDefaultButton(QMessageBox::NoButton);
		pMessageBox->setAttribute(Qt::WA_DeleteOnClose, true);
		pMessageBox->exec();
	}
	else
	{

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
		_webView->page()->runJavaScript(jsstr);
#else
		_webView->page()->mainFrame()->evaluateJavaScript(jsstr);
#endif
	}
	if (_panicTimer)
	{
		_panicTimer->stop();
	}
}

void MainWindow::on_pbSwitchScreenshot_toggled(bool checked)
{
	if (checked)
	{
		shootScreen();
		_pScreenshotTimer->setTimerType(Qt::PreciseTimer);
		_pScreenshotTimer->start(250);
	}
	else
	{
		_pScreenshotTimer->stop();
	}
}

void MainWindow::shootScreen()
{
	if (ui->pbCheckTransparent->isChecked())
	{
		applyCss(QWebViewCSSIndex::Normal);
	}
	QPixmap pixmap = QPixmap::grabWidget(getBrowserWidget());

	QString filename = QApplication::applicationDirPath();
	filename += "/screenshot/";

	filename += QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss_zzz.png");

	pixmap.save(filename, "PNG");
	if (ui->pbCheckTransparent->isChecked())
	{
		applyCss(QWebViewCSSIndex::Transparent);
	}
}

void MainWindow::slotScreenshotTimeout()
{
	shootScreen();
}

void MainWindow::on_pbCheckLowVol_toggled(bool checked)
{
	_bLowVol = checked;
	AdjustVolume(-1);
}
