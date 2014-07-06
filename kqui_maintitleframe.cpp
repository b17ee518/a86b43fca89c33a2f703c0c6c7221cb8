#include "kqui_maintitleframe.h"
#include "ui_kqui_maintitleframe.h"

KQUI_MainTitleFrame::KQUI_MainTitleFrame(QWidget *parent) :
    KQMainTitleFrame(parent),
    ui(new Ui::KQUI_MainTitleFrame)
{
    ui->setupUi(this);

    ui->comboBoxZoom->insertSeparator(1);
}

KQUI_MainTitleFrame::~KQUI_MainTitleFrame()
{
    delete ui;
}

void KQUI_MainTitleFrame::postInit()
{
    m_pMainWindow = KQMainWindow::mainWindow();
    m_pInfoWindow = m_pMainWindow->infoSubMainWindow();
    m_pTimerWindow = m_pMainWindow->timerSubMainWindow();

    setHandlingWidget(m_pMainWindow);

    connect(ui->pbCheckInfo, SIGNAL(toggled(bool)), m_pInfoWindow, SLOT(setVisible(bool)));
    connect(ui->pbCheckTimer, SIGNAL(toggled(bool)), m_pTimerWindow, SLOT(setVisible(bool)));
}

void KQUI_MainTitleFrame::onSubMainWindowShowHide(bool bShow, KQMainWindowBase *pWindow)
{
    KQPushButton * pButton = 0;
    if (pWindow == m_pInfoWindow)
    {
        pButton = ui->pbCheckInfo;
    }
    else if (pWindow == m_pTimerWindow)
    {
        pButton = ui->pbCheckTimer;
    }
    if (!pButton)
    {
        return;
    }

    pButton->setChecked(bShow);
}

void KQUI_MainTitleFrame::on_pbClose_clicked()
{
    m_pMainWindow->close();
}

void KQUI_MainTitleFrame::on_pbMinimize_clicked()
{
    m_pMainWindow->setWindowState(Qt::WindowMinimized);
/*
    if (m_pInfoWindow->isVisible() && !m_pInfoWindow->isDockingOnTop())
    {
        m_pInfoWindow->hide();
//        m_pInfoWindow->setWindowState(Qt::WindowMinimized);
    }
    if (m_pTimerWindow->isVisible() && !m_pTimerWindow->isDockingOnTop())
    {
        m_pTimerWindow->hide();
//        m_pTimerWindow->setWindowState(Qt::WindowMinimized);
    }
    */
}

void KQUI_MainTitleFrame::on_pbMinimizeSelf_clicked()
{
    m_pMainWindow->setWindowState(Qt::WindowMinimized);
}

void KQUI_MainTitleFrame::on_pbTrasparent_clicked()
{

}

void KQUI_MainTitleFrame::on_comboBoxZoom_currentIndexChanged(const QString &arg1)
{

}

void KQUI_MainTitleFrame::on_pbCheckTimer_toggled(bool checked)
{
    /*
    if (checked && !m_pTimerWindow->isVisible())
    {
        m_pTimerWindow->show();
    }
    else if (!checked && m_pTimerWindow->isVisible())
    {
        m_pTimerWindow->hide();
    }
    */
}

void KQUI_MainTitleFrame::on_pbCheckInfo_toggled(bool checked)
{
    /*
    if (checked && !m_pInfoWindow->isVisible())
    {
        m_pInfoWindow->show();
    }
    else if (!checked && m_pInfoWindow->isVisible())
    {
        m_pInfoWindow->hide();
    }
    */
}
