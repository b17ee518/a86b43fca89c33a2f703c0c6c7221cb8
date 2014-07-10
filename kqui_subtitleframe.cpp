#include "kqui_subtitleframe.h"
#include "ui_kqui_subtitleframe.h"

KQUI_SubTitleFrame::KQUI_SubTitleFrame(QWidget *parent) :
    KQTitleFrame(parent),
    ui(new Ui::KQUI_SubTitleFrame)
{
    ui->setupUi(this);
}

KQUI_SubTitleFrame::~KQUI_SubTitleFrame()
{
    delete ui;
}

void KQUI_SubTitleFrame::postInit(QWidget *pHandling)
{
    setHandlingWidget(pHandling);
}

void KQUI_SubTitleFrame::on_pbClose_clicked()
{
    handlingWidget()->close();
}

void KQUI_SubTitleFrame::on_pbMinimize_clicked()
{
    handlingWidget()->setWindowState(Qt::WindowMinimized);
}
