#ifndef DEVELOPACTIONSELECTDIALOG_H
#define DEVELOPACTIONSELECTDIALOG_H

#include <QDialog>

namespace Ui {
	class DevelopActionSelectDialog;
}

class DevelopActionSelectDialog : public QDialog
{
	Q_OBJECT

public:
	explicit DevelopActionSelectDialog(QWidget *parent = 0);
	~DevelopActionSelectDialog();

	void getIdAndCount(int& slotitemid, int& count);

	void setPositionTo(int x, int y);

private:
	Ui::DevelopActionSelectDialog *ui;

	void setNameByID();

	private slots:
	void slotOnIDEdited();
};

#endif // ANYACTIONSELECTDIALOG_H
