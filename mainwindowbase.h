#ifndef MAINWINDOWBASE_H
#define MAINWINDOWBASE_H

#include <QMainWindow>
#include <QEvent>
#include <QCloseEvent>
#include <QResizeEvent>

class MainWindowBase : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindowBase(QWidget *parent = 0);

	void mwbPostInit();

	virtual void minimizeWindow();
	virtual void restoreWindow();

	void clickAtGlobalPos(const QPoint& pt);

protected:
	virtual void closeEvent(QCloseEvent *event);
	virtual void changeEvent(QEvent *event);

	public slots:
	virtual void slotActivate(QWidget* w, bool bActivate);
	virtual void slotToggleRestoreMinimize(bool bRestore);

signals:
	void sigActivated(QWidget* w, bool bActivate);
	void sigRestoreMinimizeToggled(bool bRestore);
};

#endif // MAINWINDOWBASE_H
