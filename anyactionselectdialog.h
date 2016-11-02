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
	bool isCheckAirBaseCond();

private:
    Ui::AnyActionSelectDialog *ui;

	private slots:
	void slotOnReset();
};

#endif // ANYACTIONSELECTDIALOG_H
