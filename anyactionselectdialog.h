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
	bool isCheckAirBaseCond();
	bool isCheckCond();
	bool isAllowMiddle();
	bool isPauseStartMap();

private:
	Ui::AnyActionSelectDialog *ui;

	private slots:
	void slotOnReset();
	void slotUncheckAllAreaButtons();
	void slotClearLEAreaAndMap();
	void slotResetExtraSettings();
};

#endif // ANYACTIONSELECTDIALOG_H
