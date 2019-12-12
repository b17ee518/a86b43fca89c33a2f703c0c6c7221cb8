#ifndef ANYACTIONSELECTDIALOG_H
#define ANYACTIONSELECTDIALOG_H

#include <QDialog>

namespace Ui {
	class AnyActionSelectDialog;
}

class AnyActionSelectDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AnyActionSelectDialog(QWidget *parent = 0);
	~AnyActionSelectDialog();

	void getMapAndArea(int&area, int& map);
	void setSelections(int area, int map);
	void setPositionTo(int x, int y);

	int getCountSet();
	int getOnlySSTeamSize();

	QString getRestoreSetting();

	bool isAutoFastRepair();
	bool isSwapLowCond();
	bool isCheckAirBaseCond();
	bool isCheckCond();
	bool isAllowMiddle();
	bool isPauseStartMap();

	bool isRestoreMode();

private:
	Ui::AnyActionSelectDialog *ui;

	bool isRestore = false;


private slots:
	void slotDisplayMapInfo();
	void slotOnReset();
	void slotOnRestoreToDefault();
	void slotUncheckAllAreaButtons();
	void slotClearLEAreaAndMap();
	void slotResetExtraSettings();

};

#endif // ANYACTIONSELECTDIALOG_H
