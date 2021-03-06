﻿#include "kqui_collapsibleframe.h"
#include "ui_kqui_collapsibleframe.h"

KQUI_CollapsibleFrame::KQUI_CollapsibleFrame(QWidget *parent)
	: QFrame(parent)
	, ui(new Ui::KQUI_CollapsibleFrame)
{
	ui->setupUi(this);
	ui->retranslateUi(this);

	ui->tableWidget->horizontalHeader()->hide();
	ui->tableWidget->verticalHeader()->hide();
}

KQUI_CollapsibleFrame::~KQUI_CollapsibleFrame()
{
	delete ui;
}

KQTableWidget *KQUI_CollapsibleFrame::tableWidget()
{
	return ui->tableWidget;
}

QPushButton *KQUI_CollapsibleFrame::pushButton()
{
	return ui->pushButton;
}

void KQUI_CollapsibleFrame::slotResize(bool bValue)
{
	QSize minsize = QSize(pushButton()->minimumWidth(),
		(bValue ? tableWidget()->minimumHeight() : 0) + pushButton()->minimumHeight());
	setMinimumSize(minsize);
	emit ui->tableWidget->sigTableSizeChanged();
}
