#ifndef MAINWINDOWBASE_H
#define MAINWINDOWBASE_H

#include <QMainWindow>
#include <QEvent>
#include <QCloseEvent>
#include <QResizeEvent>
#include <QShowEvent>
#include <QWinTaskbarButton>

class MainWindowBase : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindowBase(QWidget *parent = 0);

	void mwbPostInit();
	void SetProgressBarPos(int pos);

protected:
	virtual void closeEvent(QCloseEvent *event);
	virtual void changeEvent(QEvent *event);
	virtual void showEvent(QShowEvent *event);


public slots:
	virtual void slotActivate(QWidget* w , bool bActivate);
	virtual void slotToggleRestoreMinimize(bool bRestore);

signals:
	void sigActivated( QWidget* w, bool bActivate );
	void sigRestoreMinimizeToggled(bool bRestore);

protected:
	QWinTaskbarButton * m_pTaskbarButton;

};

#endif // MAINWINDOWBASE_H
