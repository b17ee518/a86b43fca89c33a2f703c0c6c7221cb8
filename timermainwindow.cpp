#include "timermainwindow.h"
#include "ui_timermainwindow.h"

TimerMainWindow::TimerMainWindow(QWidget *parent) :
    SubMainWindow(parent),
    ui(new Ui::TimerMainWindow)
{
    ui->setupUi(this);
    mwbPostInit();
    ui->titleFrame->setHandlingWidget(this);
}

TimerMainWindow::~TimerMainWindow()
{
    delete ui;
}

void TimerMainWindow::on_pbClose_clicked()
{
    close();
}

void TimerMainWindow::on_pbMinimize_clicked()
{
    setWindowState(Qt::WindowMinimized);
}
